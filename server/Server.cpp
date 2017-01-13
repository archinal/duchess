#include "Server.h"
#include "LegacyUtils.h"

#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <cassert>
#include <vector>

#include "../duchess/Piece.h"
#include "../duchess/Move.h"
#include "../duchess/Board.h"
#include "../duchess/Position.h"
#include "../duchess/PositionUtils.h"
#include "../duchess/PieceType.h"

// Unfortunately we need this to be a static variable, as the signal_handler callback needs to be able to change
// it without having a Server object explicitly passed
bool Server::isRunning = true;

Server::Server(const char *const portNum) {
    // Set up offBoardPos for parser functions to use
    this->m_offBoardPos = new Position("OB");

    isRunning = true;
    //setup the socket
    memset(&host_info, 0, sizeof host_info);

    host_info.ai_family = AF_UNSPEC;
    host_info.ai_flags = AI_PASSIVE;
    host_info.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(NULL, portNum, &host_info, &host_info_list);

    if (status != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
    }

    socketfd = socket(host_info_list->ai_family, host_info_list->ai_socktype, host_info_list->ai_protocol);

    if (socketfd == -1) {
        std::cerr << "socket error: " << strerror(errno) << std::endl;
    }

    int yes = 1;
    status = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    if (status == -1) {
        std::cerr << "setsockopt error: " << strerror(errno) << std::endl;
    }
    status = bind(socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1) {
        std::cerr << "bind error: " << strerror(errno) << std::endl;
    }

    //setup a listener for exit signals
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    sigfillset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);
}

Server::~Server() {
    freeaddrinfo(host_info_list);
    close(new_sd);
    close(socketfd);
    delete getOffBoardPos();
}

void Server::run() {
    std::cout << "Server running. Listening for connections..." << std::endl;
    int status = listen(socketfd, SOCKET_QUEUE_SIZE);
    if (status == -1) {
        std::cerr << "listen error: " << strerror(errno) << std::endl;
    }

    const int numSquares = 157;
    const int expectedInputSize = numSquares * 2 + 8;

    //listen for new connections
    while (isRunning) {
        struct sockaddr_storage their_addr;
        socklen_t addr_size = sizeof their_addr;
        new_sd = accept(socketfd, (struct sockaddr *) &their_addr, &addr_size);
        if (new_sd == -1) {
            continue;
        }

        // Track the turn number (for the use of the AI)
        int turnNumber = -1;
        int lastTurn = 1;

        std::cout << "Received connection..." << std::endl;
        //listen for new messages within the current connection
        while (isRunning) {
            char incoming_data_buffer[DATA_BUFFER_SIZE];
            ssize_t bytes_received = recv(new_sd, incoming_data_buffer, DATA_BUFFER_SIZE, 0);
            if (bytes_received == 0 || bytes_received == -1) {
                break;
            }
            incoming_data_buffer[bytes_received] = '\0';

            std::cout << "Received message. responding..." << std::endl;

            assert(bytes_received == expectedInputSize);

            std::vector <std::vector<Piece>> pieces = this->dataToPieces(incoming_data_buffer);
            Board b(pieces);
            b.initialiseAttDefVectors();

            short playerTurn = LegacyUtils::dataToPlayerTurn(incoming_data_buffer);
            // Update the turn number
            if (turnNumber == -1) {
                turnNumber = playerTurn;
            } else if (playerTurn > lastTurn) {
                turnNumber += playerTurn - lastTurn;
            } else {
                turnNumber += playerTurn - lastTurn + 6;
            }

            this->getPlayer()->setPlayerNumber(playerTurn);
            const Move move = this->getPlayer()->chooseMove(&b, turnNumber);
            const Piece* activePiece = b.getPieceAtPosition(move.getFromPosition());
            // Check that our move is legal
            std::cout << "Checking move legal: " << move.toString() << std::endl;
            assert(b.isLegalMove(move, playerTurn));
            // Apply our designated move to work out if it created a terminal board state
            b.applyMoveCumulative(&move);
            const bool isTerminal = b.isTerminal();

            std::string msg = LegacyUtils::moveToMoveData(move, activePiece, isTerminal, &b);
            send(new_sd, msg.c_str(), msg.length(), 0);

            std::cout << "Message sent." << std::endl;

            lastTurn = playerTurn;
        }

        std::cout << "Previous connection lost" << std::endl;
    }
}

void Server::signal_handler(int sig) {
    if (sig == SIGINT) {
        isRunning = false;
    }
}

std::vector <std::vector<Piece>> Server::dataToPieces(const char incomingData[]) const {
    std::vector <std::vector<Piece>> pieces = getUnplacedPieces();
    assert(pieces.size() == 6);
    assert(pieces[0].size() == 15);

    int numSquares = 157;
    int loopLimit = numSquares * 2;
    int i;
    for (i = 0; i < loopLimit; i += 2) {
        const int piece = ((int) incomingData[i] << 8) | (int) incomingData[i + 1];
        int square = i / 2;
        // If the piece is 90 it means "there is no piece on this square"
        if (piece != 90) {
            const int pieceIndex = LegacyUtils::legacyPieceNumberToPieceIndex(piece);
            const int owner = LegacyUtils::legacyPieceNumberToOwner(piece);
            Position* pos = LegacyUtils::legacySquareToPosition(square);

            pieces[owner - 1][pieceIndex].updatePosition(pos);
        }

    }

    int numPlayers = (incomingData[i] << 8) | incomingData[i + 1];
    assert(numPlayers == 6);
//    int clockTime = (incomingData[i + 3] << 24) | (incomingData[i + 4] << 16)
//                    | (incomingData[i + 5] << 8) | incomingData[i + 6];
//    int randomMove = incomingData[i + 7];

    return pieces;
}


std::vector <std::vector<Piece>> Server::getUnplacedPieces() const {
    std::vector <std::vector<Piece>> pieces;

    short player;
    short piece;
    const short numPlayers = 6;
    const short piecesPerPlayer = 15;
    const std::vector <PieceType> pieceTypes{
            PieceType::PAWN, PieceType::PAWN, PieceType::PAWN, PieceType::PAWN, PieceType::PAWN,
            PieceType::BISHOP, PieceType::KNIGHT, PieceType::QUEEN, PieceType::BISHOP, PieceType::DUCHESS,
            PieceType::ROOK, PieceType::WIZARD, PieceType::KING, PieceType::FORTRESS, PieceType::ROOK
    };
    for (player = 0; player < numPlayers; ++player) {
        // Create all pieces for this player
        std::vector <Piece> piecesForPlayer;
        for (piece = 0; piece < piecesPerPlayer; ++piece) {
            const PieceType type = pieceTypes[piece];
            const short owner = player + 1;

            Piece p(this->getOffBoardPos(), type, owner);

            piecesForPlayer.push_back(p);
        }
        pieces.push_back(piecesForPlayer);
    }

    return pieces;
}
