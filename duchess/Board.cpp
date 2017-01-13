#define NDEBUG

#include "Board.h"
#include "Position.h"
#include "PositionUtils.h"
#include "TeamUtils.h"
#include "MoveVector.h"
#include "MoveVectorUtils.h"
#include "GeneralUtils.h"

#include <vector>
#include <deque>
#include <cassert>
#include <iostream>
#include <algorithm>

Board::Board() {
    // Populate positions, squares, pieces
    BoardIndex i;
    for (i = 0; i <= 157; ++i) {
        this->m_positions.push_back(PositionUtils::boardIndexToPositionSixPlayer(i));
        this->m_squares.push_back(nullptr);
    }

    // Create pieces for all players, set their positions as appropriate and then
    // map the relevant entry in m_squares to the address of the piece
    short player;
    short piece;
    const short numPlayers = 6;
    const short piecesPerPlayer = 15;
    const std::vector<PieceType> pieceTypes {
            PieceType::PAWN, PieceType::PAWN, PieceType::PAWN, PieceType::PAWN, PieceType::PAWN,
            PieceType::BISHOP, PieceType::KNIGHT, PieceType::QUEEN, PieceType::BISHOP, PieceType::DUCHESS,
            PieceType::ROOK, PieceType::WIZARD, PieceType::KING, PieceType::FORTRESS, PieceType::ROOK
    };
    for (player = 0; player < numPlayers; ++player) {
        // Create all pieces for this player
        std::vector<Piece*> piecesForPlayer;
        for (piece = 0; piece < piecesPerPlayer; ++piece) {
            const PieceType type = pieceTypes[piece];
            const short owner = player + 1;
            const short flap = player + 1;
            const char row = 'a' + (piece % 5);
            const short column =  (-1 *(piece - 19)) / 5;

            const Position pos (flap, row, column);
            const BoardIndex posIndex = PositionUtils::positionToBoardIndexSixPlayer(&pos);

            Piece* p = new Piece(m_positions[posIndex], type, owner);

            this->m_squares[posIndex] = p;
            piecesForPlayer.push_back(p);
        }
        this->m_pieces.push_back(piecesForPlayer);
    }

    // Initialise checkmateRecords as indicating no one is in checkmate
    this->m_checkmateRecords = {false, false, false, false, false, false};

}

Board::Board(std::vector<std::vector<Piece>> t_pieces) {
    // Populate positions, squares
    BoardIndex i;
    for (i = 0; i <= 157; ++i) {
        this->m_positions.push_back(PositionUtils::boardIndexToPositionSixPlayer(i));
        this->m_squares.push_back(nullptr);
    }

    // Set up pieces
    short player;
    short piece;
    const short numPlayers = 6;
    const short piecesPerPlayer = 15;
    for (player = 0; player < numPlayers; ++player) {
        // Create all pieces for this player
        // Copy the ones provided in the constructor
        std::vector<Piece*> piecesForPlayer;
        for (piece = 0; piece < piecesPerPlayer; ++piece) {
            const short owner = player + 1;

            Piece pieceToCopy = t_pieces[player][piece];
            const Position* pos = pieceToCopy.getPosition();
            const BoardIndex posIndex = PositionUtils::positionToBoardIndexSixPlayer(pos);
            Piece* p = new Piece(m_positions[posIndex], pieceToCopy.getType(), owner);

            if (!pos->isOffBoard()) {
                // It's our job here to deallocate the memory malloced by server for positions
                // With the exception of OB which Server handles itself
                delete pos;
            }

            this->m_squares[posIndex] = p;
            piecesForPlayer.push_back(p);
        }
        this->m_pieces.push_back(piecesForPlayer);
    }

    // Initialise the checkmate records as all false
    this->m_checkmateRecords = {false, false, false, false, false, false};
}

Board::~Board() {
    // Free up the Pieces we created
    short i;
    for (i = 0; i < 6; ++i) {
        for (auto piece : m_pieces[i]) {
            delete piece;
        }
    }

    // Free up the positions we created
    for (auto pos : m_positions) {
        delete pos;
    }
}

Piece* Board::getPieceAtPosition(const Position* t_position) const {
    const BoardIndex positionIndex = PositionUtils::positionToBoardIndexSixPlayer(t_position);
    return this->getPieceAtPosition(positionIndex);
}

Piece* Board::getPieceAtPosition(const BoardIndex t_positionIndex) const {
    // This method should not be used to lookup all pieces that are off-board
    assert(t_positionIndex != 0);
    return m_squares[t_positionIndex];
}

Position* Board::getPosition(const BoardIndex t_positionIndex) const {
    return m_positions[t_positionIndex];
}

Position* Board::getPosition(const Position* t_position) const {
    return m_positions[PositionUtils::positionToBoardIndexSixPlayer(t_position)];
}

// Assumes 1 <= t_playerNumber <= 6
std::vector<Piece*> Board::getPiecesForPlayer(const short t_playerNumber) const {
    return m_pieces[t_playerNumber - 1];
}

void Board::markPlayerInCheckmate(const short t_player) {
    m_checkmateRecords[t_player - 1] = true;
}

void Board::markPlayerNotInCheckmate(const short t_player) {
    m_checkmateRecords[t_player - 1] = false;
}

bool Board::isPlayerInCheckmate(const short t_player) const {
    return m_checkmateRecords[t_player - 1];
}

bool Board::isTerminal() const {
    return this->getWinner() != TeamType::NONE;
}

TeamType Board::getWinner() const {
    if (this->isPlayerInCheckmate(1) && this->isPlayerInCheckmate(3) && this->isPlayerInCheckmate(5)) {
        return TeamType::EVENS;
    } else if (this->isPlayerInCheckmate(2) && this->isPlayerInCheckmate(4) && this->isPlayerInCheckmate(6)) {
        return TeamType::ODDS;
    } else {
        return TeamType::NONE;
    }
}

std::vector<Position*> Board::getAccessibleSquares (const Piece* t_piece) const {
    std::vector<Position*> accessiblePositions;
    const PieceType pieceType = t_piece->getType();
    const Position* piecePosition = t_piece->getPosition();

    if (pieceType == PieceType::PAWN) {
        // PAWN
        // Adjacent squares for regular movement
        const std::vector<BoardIndex>& adjacentSquares = PositionUtils::getAdjacentSquareIndices(piecePosition);
        for(auto const& adjacentSquareIndex: adjacentSquares) {
            Position* pos = this->getPosition(adjacentSquareIndex);
            if (!this->doesSquareContainPiece(pos)) {
                accessiblePositions.push_back(pos);
            }
        }

        // Taking on diagonals
        const std::vector<BoardIndex>& diagonalSquares = PositionUtils::getDiagonalSquareIndices(piecePosition);
        for (auto const& diagonalSquareIndex: diagonalSquares) {
            Position* pos = this->getPosition(diagonalSquareIndex);
            if (this->doesSquareContainEnemy(pos, t_piece)) {
                accessiblePositions.push_back(pos);
            }
        }

    } else if (pieceType == PieceType::KING) {
        // Kings and Wizards can move (and take) unit bishop or unit rook
        //
        // Note we exclude Wizards from this branch, however, as their unit moves are equivalent to "teleportations next to self", which
        // are added in the Wizard-adjacent check below
        const std::vector<BoardIndex>& allAdjacent = PositionUtils::getAdjacentAndDiagonalSquareIndices(piecePosition);
        for (auto const& adjacentSquareIndex : allAdjacent) {
            Position* pos = this->getPosition(adjacentSquareIndex);
            if (!this->doesSquareContainAlly(pos, t_piece)) {
                accessiblePositions.push_back(pos);
            }
        }
    }

    if (pieceType == PieceType::KNIGHT || pieceType == PieceType::DUCHESS || pieceType == PieceType::FORTRESS) {
        // Each of these pieces can take on knight-moves
        const std::vector<BoardIndex>& knightAdjacent = PositionUtils::getKnightMovesFromPosition(piecePosition);
        for (auto const& knightAdjacentIndex : knightAdjacent) {
            Position* pos = this->getPosition(knightAdjacentIndex);
            if (!this->doesSquareContainAlly(pos, t_piece)) {
                accessiblePositions.push_back(pos);
            }
        }
    }

    if (piecePosition->isVortex()) {
        if (pieceType == PieceType::ROOK || pieceType == PieceType::FORTRESS || pieceType == PieceType::QUEEN) {
            const std::vector<BoardIndex>& adjacentSquares = PositionUtils::getAdjacentSquareIndices(piecePosition);
            short direction = PositionUtils::DIRECTION_OUT;
            for (auto const& index : adjacentSquares) {
                short tempDirection = direction;
                Position* prev = this->getPosition(piecePosition);
                Position* cur = this->getPosition(index);
                // Can't go off board, can't go past an enemy, can't go onto ally
                while (!cur->isOffBoard() && !this->doesSquareContainEnemy(prev, t_piece) && !this->doesSquareContainAlly(cur, t_piece)) {
                    accessiblePositions.push_back(cur);
                    const BoardIndex nextInLineIndex = PositionUtils::getIndexOfNextSquareInLine(prev, cur, tempDirection);
                    const short flapChange = PositionUtils::getFlapChange(prev, cur);
                    tempDirection = PositionUtils::determineNextDirectionGivenPreviousDirectionAndFlapChange(tempDirection,
                                                                                                             flapChange);
                    prev = cur;
                    cur = this->getPosition(nextInLineIndex);
                }
            }
        }

        if (pieceType == PieceType::BISHOP || pieceType == PieceType::QUEEN || pieceType == PieceType::DUCHESS) {
            const std::vector<BoardIndex>& diagonalSquares = PositionUtils::getDiagonalSquareIndices(piecePosition);
            short direction = PositionUtils::DIRECTION_OUT_ANTICLOCKWISE;
            for (auto const& index : diagonalSquares) {
                short tempDirection = direction;
                Position* prev = this->getPosition(piecePosition);
                Position* cur = this->getPosition(index);
                // Can't go off board, can't go past an enemy, can't go onto ally
                while (!cur->isOffBoard() && !this->doesSquareContainEnemy(prev, t_piece) && !this->doesSquareContainAlly(cur, t_piece)) {
                    accessiblePositions.push_back(cur);
                    const BoardIndex nextInLineIndex = PositionUtils::getIndexOfNextSquareInLine(prev, cur, tempDirection);
                    const short flapChange = PositionUtils::getFlapChange(prev, cur);
                    tempDirection = PositionUtils::determineNextDirectionGivenPreviousDirectionAndFlapChange(tempDirection,
                                                                                                             flapChange);
                    prev = cur;
                    cur = this->getPosition(nextInLineIndex);
                }
            }
        }

    } else {
        if (pieceType == PieceType::ROOK || pieceType == PieceType::FORTRESS || pieceType == PieceType::QUEEN) {
            // Rook-style moves
            // Loop through CLOCKWISE/ANTICLOCKWISE/IN/OUT
            short startDirection;
            for (startDirection = PositionUtils::DIRECTION_CLOCKWISE;
                 startDirection <= PositionUtils::DIRECTION_OUT; ++startDirection) {
                const std::vector<Position *> positionsAlongLine = this->getAccessibleSquaresAlongDirection(t_piece,
                                                                                                            startDirection);
                // append positionsAlongLine to accessiblePositions
                accessiblePositions.insert(std::end(accessiblePositions), std::begin(positionsAlongLine),
                                           std::end(positionsAlongLine));
            }
        }

        if (pieceType == PieceType::BISHOP || pieceType == PieceType::QUEEN || pieceType == PieceType::DUCHESS) {
            // Bishop-style moves
            // Loop through IN_CLOCKWISE/IN_ANTICLOCKWISE/OUT_CLOCKWISE/OUT_ANTICLOCKWISE
            short startDirection;
            for (startDirection = PositionUtils::DIRECTION_IN_CLOCKWISE;
                 startDirection <= PositionUtils::DIRECTION_OUT_ANTICLOCKWISE; ++startDirection) {
                const std::vector<Position *> &positionsAlongLine = this->getAccessibleSquaresAlongDirection(t_piece,
                                                                                                             startDirection);
                // append positionsAlongLine to accessiblePositions
                accessiblePositions.insert(std::end(accessiblePositions), std::begin(positionsAlongLine),
                                           std::end(positionsAlongLine));
            }
        }
    }
    // If piece is adjacent to wizard, add in the teleportable squares
    if (this->isPieceWizardAdjacent(t_piece)) {
        const std::vector<BoardIndex>& teleportableSquares = this->getTeleportableSquares(t_piece);
        for (auto const& teleportableSquare : teleportableSquares) {
            Position* pos = this->getPosition(teleportableSquare);
            // Don't add if duplicate or if contains ally
            if (!doesSquareContainAlly(pos, t_piece) &&
                    std::find(accessiblePositions.begin(), accessiblePositions.end(), pos) == accessiblePositions.end()) {
                accessiblePositions.push_back(pos);
            }
        }
    }

    return accessiblePositions;
}

std::vector<Position*> Board::getAccessibleSquaresAlongDirection(const Piece* t_piece, const short t_startDirection) const {
    std::vector<Position*> accessiblePositions;

    short direction = t_startDirection;
    Position* startPosition = t_piece->getPosition();
    Position* prev = startPosition;
    const BoardIndex posIndex = PositionUtils::getIndexOfPositionInDirection(startPosition, t_startDirection);
    Position* curr = this->getPosition(posIndex);
    while (!curr->isOffBoard() && !this->doesSquareContainAlly(curr, t_piece) && !this->doesSquareContainEnemy(prev, t_piece)) {
        // Keep extending along line adding positions
        // Finish one short of an ally, or on top of an enemy, or when off board
        accessiblePositions.push_back(curr);
        const BoardIndex nextCurrIndex = PositionUtils::getIndexOfNextSquareInLine(prev, curr, direction);
        const short flapChange = PositionUtils::getFlapChange(prev, curr);
        direction = PositionUtils::determineNextDirectionGivenPreviousDirectionAndFlapChange(direction, flapChange);
        prev = curr;
        curr = this->getPosition(nextCurrIndex);
    }

    return accessiblePositions;
}

// Given that t_piece is adjacent to (or is) a friendly wizard, returns a list of positions that can therefore be accessed
// using teleportation (including self-teleportation for wizards, which is equivalent to a KING move)
std::vector<BoardIndex> Board::getTeleportableSquares(const Piece* t_piece) const {
    const short originalOwner = t_piece->getOwner();
    std::vector<BoardIndex> positions;
    // For each wizard on my team who is not Off Board
    short pieceOwner;
    for (pieceOwner = 1; pieceOwner <= 6; ++pieceOwner) {
        if (TeamUtils::isSameTeam(originalOwner, pieceOwner)) {
            // Note that wizard is indexed 11 in the set of pieces, as defined in Board.h
            const Position* wizardPosition = this->getPiecesForPlayer(pieceOwner)[11]->getPosition();
            if (!wizardPosition->isOffBoard()) {
                // Get the squares adjacent and diagonal of them
                const std::vector<BoardIndex>& adjacentSquares = PositionUtils::getAdjacentAndDiagonalSquareIndices(wizardPosition);

                // For each of those squares add it to positions
                for (auto const& adjacentSquareIndex : adjacentSquares) {
                    if (this->getPieceAtPosition(adjacentSquareIndex) != t_piece) {
                        positions.push_back(adjacentSquareIndex);
                    }
                }
            }
        }
    }

    return positions;
}

// Tells us if a piece is adjacent to a wizard (i.e. if it is elligible to teleport)
// Note that if the piece itself is a wizard, we will say it is adjacent
bool Board::isPieceWizardAdjacent(const Piece* t_piece) const {
    assert(t_piece != nullptr);

    if (t_piece->getType() == PieceType::WIZARD) {
        return true;
    } else {
        const short owner = t_piece->getOwner();

        for(auto const& adjacentSquare : PositionUtils::getAdjacentAndDiagonalSquareIndices(t_piece->getPosition())) {
            const Piece* p = this->getPieceAtPosition(adjacentSquare);
            if (p != nullptr && p->getType() == PieceType::WIZARD && TeamUtils::isSameTeam(owner, p->getOwner())) {
                return true;
            }
        }
    }

    return false;
}

std::vector<Piece*> Board::getAdjacentPieces(const Piece* t_piece) const {
    const std::vector<BoardIndex>& adjacentSquares = PositionUtils::getAdjacentAndDiagonalSquareIndices(t_piece->getPosition());
    std::vector<Piece*> adjacentPieces;

    for (auto const& adjacentIndex : adjacentSquares) {
        Piece* pieceAtPosition = this->getPieceAtPosition(adjacentIndex);
        if (pieceAtPosition != nullptr) {
            adjacentPieces.push_back(pieceAtPosition);
        }
    }

    return adjacentPieces;
}


bool Board::doesSquareContainPiece(const Position* t_position) const {
    assert(t_position != nullptr && !t_position->isOffBoard());
    return this->getPieceAtPosition(t_position) != nullptr;
}

bool Board::doesSquareContainEnemy(const Position* t_position, const Piece* t_piece) const {
    assert(t_position != nullptr && !t_position->isOffBoard() && t_piece != nullptr);
    bool result = false;

    Piece* pieceInPosition = this->getPieceAtPosition(t_position);
    if (pieceInPosition != nullptr) {
        result = !(TeamUtils::isSameTeam(t_piece->getOwner(), pieceInPosition->getOwner()));
    }

    return result;
}

bool Board::doesSquareContainAlly(const Position* t_position, const Piece* t_piece) const {
    return this->doesSquareContainPiece(t_position) && !this->doesSquareContainEnemy(t_position, t_piece);
}

std::vector<Move> Board::getLegalMoves(const short t_player) const {
    std::vector<Move> legalMoves;
    std::vector<Position*> blockableSquares;
    const Piece* myKing = this->getPiecesForPlayer(t_player)[12];

    // Determine if king is in check
    const std::vector<MoveVector*>& attackingKingVectors = myKing->getPassiveAttackingVectors();
    bool kingInCheck = attackingKingVectors.size() != 0;

    if (kingInCheck) {
        blockableSquares = MoveVectorUtils::getCommonPositions(attackingKingVectors);
    }

    std::vector<Piece*> pieces = this->getPiecesForPlayer(t_player);
    for (auto const& piece : pieces) {
        if (!piece->getPosition()->isOffBoard()) {
            // Get the legal moves for this piece
            const std::vector<Move>& movesForPiece = getLegalMovesForPiece(piece, kingInCheck, blockableSquares);

            // Append those moves to our total legal moves vector
            legalMoves.insert(std::end(legalMoves), std::begin(movesForPiece), std::end(movesForPiece));
        }
    }

    // If we don't actually have any legal moves (i.e. we're in checkmate or stalemate), we can perform the null move
    if (legalMoves.size() == 0) {
        Move nullMove;
        legalMoves.push_back(nullMove);
    }

    return legalMoves;

}

std::vector<Move> Board::getLegalMovesForPiece(const Piece* t_piece, bool t_kingInCheck, const std::vector<Position*>& t_blockableSquares) const {
    const Position* piecePosition = t_piece->getPosition();
    const PieceType pieceType = t_piece->getType();

    // Work out where we can move to
    std::vector<Move> legalMoves;
    const std::vector<Position*>& accessibleSquares = this->getAccessibleSquares(t_piece);
    const std::vector<Position*>& attackingLineOnKingThroughThisPiece = this->getAttackingLineOnKingThroughPosition(piecePosition, t_piece->getOwner());
    std::vector<Position*> legalDestinations;
    if (accessibleSquares.size() > 0) {
        if (t_kingInCheck && pieceType != PieceType::KING) {
            legalDestinations = MoveVectorUtils::getCommonPositions(accessibleSquares, t_blockableSquares);
        } else {
            legalDestinations = accessibleSquares;
        }

        // If moving this piece opens up a new attack vector, we have to only move along that vector (thereby blocking the attack)
        if (attackingLineOnKingThroughThisPiece.size() > 0) {
            legalDestinations = MoveVectorUtils::getCommonPositions(attackingLineOnKingThroughThisPiece, legalDestinations);
        }
    }

    // Construct moves
    // For each destination we know we can go to, create a move based around it
    for (auto const& dest : legalDestinations) {
        Piece* destinationPiece = this->getPieceAtPosition(dest);
        // Make sure we don't try to take an enemy king
        if (destinationPiece == nullptr || destinationPiece->getType() != PieceType::KING) {
            // If the destination is a vortex there are actually a number of moves we can make
            if (dest->isVortex() && pieceType == PieceType::PAWN) {
                // m_from(nullptr), m_to(nullptr), m_pieceTaken(nullptr), m_replacePiece(nullptr), m_isNull(true)
                for (const auto& piece : this->getPawnReplaceOptionsForPlayer(t_piece->getOwner())) {
                    Move m (piecePosition, dest, destinationPiece, piece);
                    legalMoves.push_back(m);
                }
            }
            // Not surrounding this in "else" since it's legal to move pawn to vortex and not upgrade it
            if (t_piece->getType() != PieceType::KING) {
                Move m (piecePosition, dest, destinationPiece);
                legalMoves.push_back(m);
            } else {
                // If the piece is a king, make sure it's not moving into the path of an attack
                const std::vector<std::deque<Position*>>& attacksThroughPos = this->getLinesAttackingSquare(dest);
                bool anyoneAttacking = false;
                for (auto const& positionDeque : attacksThroughPos) {
                    const Piece* attackingPiece = this->getPieceAtPosition(positionDeque[0]);
                    anyoneAttacking = anyoneAttacking || !TeamUtils::isSameTeam(t_piece->getOwner(), attackingPiece->getOwner());
                }
                if (!anyoneAttacking) {
                    Move m (piecePosition, dest, destinationPiece);
                    legalMoves.push_back(m);
                }
            }

        }

    }

    return legalMoves;
}

// When using this in apply move, be sure to update position of taken piece first
// Does not update vectors. Should really only be called by applyMove
void Board::setPiecePosition(Piece* t_piece, const BoardIndex t_positionIndex) {
    Position* pos = this->getPosition(t_positionIndex);

    // Update m_squares
    // Make sure there's nothing in the square we're moving this to (taken pieces should be handled first)
    assert(t_positionIndex == 0 || this->getPieceAtPosition(t_positionIndex) == nullptr);

    // Set piece's old position to now hold nothing
    const BoardIndex oldPositionIndex = t_piece->getPosition()->getBoardIndex();
    this->m_squares[oldPositionIndex] = nullptr;
    if (t_positionIndex != 0) {
        this->m_squares[t_positionIndex] = t_piece;
    }

    // Set the position on the piece itself
    t_piece->updatePosition(pos);

}

// Does not update vectors. Should really only be called by applyMove
void Board::setPiecePosition(Piece* t_piece, const Position* t_position) {
    const BoardIndex positionIndex = t_position->getBoardIndex();
    this->setPiecePosition(t_piece, positionIndex);
}

void Board::initialiseAttDefVectors() {
    // Go through each piece, find what it's actively doing (attacking or defending)
    // Add these vectors to both pieces
    short playerNum;

    // Clear all vectors for all pieces
    for (playerNum = 1; playerNum <= 6; ++playerNum) {
        for (auto& piece : this->getPiecesForPlayer(playerNum)) {
            piece->clearActiveMoveVectors();
        }
    }

    for (playerNum = 1; playerNum <= 6; ++playerNum) {
        for (auto& piece : this->getPiecesForPlayer(playerNum)) {
            this->initialiseActiveVectorsForPiece(piece);
        }
    }

    // Finally, update the checkmate records
    this->updateCheckmateRecords();
};

void Board::initialiseActiveVectorsForPiece(Piece* t_piece) {
    if (!t_piece->getPosition()->isOffBoard()) {
        std::vector <std::vector<Position *>> activelyTouching;
        const PieceType pieceType = t_piece->getType();
        Position *piecePosition = t_piece->getPosition();

        if (pieceType == PieceType::PAWN) {
            // PAWN
            // Taking on diagonals
            const std::vector<BoardIndex>& diagonalSquares = PositionUtils::getDiagonalSquareIndices(piecePosition);
            for (auto const &diagonalSquareIndex: diagonalSquares) {
                Position *pos = this->getPosition(diagonalSquareIndex);
                if (this->doesSquareContainPiece(pos)) {
                    activelyTouching.push_back(std::vector < Position * > {piecePosition, pos});
                }
            }

        } else if (pieceType == PieceType::KING || pieceType == PieceType::WIZARD) {
            // Kings and Wizards can move (and take) unit bishop or unit rook
            const std::vector<BoardIndex>& allAdjacent = PositionUtils::getAdjacentAndDiagonalSquareIndices(piecePosition);
            for (auto const &adjacentSquareIndex : allAdjacent) {
                Position *pos = this->getPosition(adjacentSquareIndex);
                if (this->doesSquareContainPiece(pos)) {
                    activelyTouching.push_back(std::vector < Position * > {piecePosition, pos});
                }
            }
        }

        if (pieceType == PieceType::KNIGHT || pieceType == PieceType::DUCHESS || pieceType == PieceType::FORTRESS) {
            // Each of these pieces can take on knight-moves
            const std::vector<BoardIndex>& knightAdjacent = PositionUtils::getKnightMovesFromPosition(piecePosition);
            for (auto const &knightAdjacentIndex : knightAdjacent) {
                Position *pos = this->getPosition(knightAdjacentIndex);
                if (this->doesSquareContainPiece(pos)) {
                    activelyTouching.push_back(std::vector < Position * > {piecePosition, pos});
                }
            }
        }

        if (piecePosition->isVortex()) {
            for (auto const& line : this->getActiveLinesFromVortex(t_piece)) {
                activelyTouching.push_back(line);
            }
        } else {

            if (pieceType == PieceType::ROOK || pieceType == PieceType::FORTRESS || pieceType == PieceType::QUEEN) {

                // Rook-style moves
                // Loop through CLOCKWISE/ANTICLOCKWISE/IN/OUT
                short startDirection;
                for (startDirection = PositionUtils::DIRECTION_CLOCKWISE;
                     startDirection <= PositionUtils::DIRECTION_OUT; ++startDirection) {
                    std::vector < Position * > positions = this->getAttackingOrDefendingVectorAlongLine(t_piece,
                                                                                                        startDirection);
                    if (positions.size() > 0) {
                        activelyTouching.push_back(positions);
                    }
                }


            }

            if (pieceType == PieceType::BISHOP || pieceType == PieceType::QUEEN || pieceType == PieceType::DUCHESS) {
                // Bishop-style moves
                // Loop through IN_CLOCKWISE/IN_ANTICLOCKWISE/OUT_CLOCKWISE/OUT_ANTICLOCKWISE
                short startDirection;
                for (startDirection = PositionUtils::DIRECTION_IN_CLOCKWISE;
                     startDirection <= PositionUtils::DIRECTION_OUT_ANTICLOCKWISE; ++startDirection) {
                    std::vector < Position * > positions = this->getAttackingOrDefendingVectorAlongLine(t_piece,
                                                                                                        startDirection);
                    if (positions.size() > 0) {
                        activelyTouching.push_back(positions);
                    }
                }

            }
        }

        for (auto const &attDefVector : activelyTouching) {
            Piece *passivePiece = this->getPieceAtPosition(attDefVector.back());
            MoveVector *mv = new MoveVector(t_piece, passivePiece, attDefVector);
            mv->registerSelf();
        }
    }
}

// Given a piece on the vortex, tells us the lines it's attacking on from there
// Only intended to be used by pieces who move in non-unit bishop / rook lines
std::vector<std::vector<Position*>> Board::getActiveLinesFromVortex(const Piece* t_piece) const {
    std::vector<std::vector<Position*>> attackingLines;
    Position* piecePosition = t_piece->getPosition();

    // Work out the directions we need to check for this piece
    const std::vector<short> directionsToCheck {PositionUtils::DIRECTION_OUT_ANTICLOCKWISE, PositionUtils::DIRECTION_OUT};

    // For each direction this piece can go in...
    for (auto const& direction : directionsToCheck) {
        if (PositionUtils::canPieceMoveInDirection(t_piece->getType(), direction)) {
            std::vector<BoardIndex> surroundingSquareIndices;
            if (direction == PositionUtils::DIRECTION_OUT) {
                surroundingSquareIndices = PositionUtils::getAdjacentSquareIndices(piecePosition);
            } else {
                surroundingSquareIndices = PositionUtils::getDiagonalSquareIndices(piecePosition);
            }

            // For the relevant surrounding squares (diagonal or adjacent), keep branching out until we go off board
            // or until we hit a piece
            for (auto const &squareIndex : surroundingSquareIndices) {
                bool success = false;
                short tempDirection = direction;
                Position *prev = piecePosition;
                Position *cur = this->getPosition(squareIndex);
                std::vector < Position * > line {prev, cur};

                while (!success && !cur->isOffBoard()) {
                    success = this->getPieceAtPosition(cur) != nullptr;

                    if (!success) {
                        prev = cur;
                        cur = this->getPosition(PositionUtils::getIndexOfPositionInDirection(cur, tempDirection));
                        line.push_back(cur);
                    }
                }

                if (success) {
                    attackingLines.push_back(line);
                }

            }
        }
    }

    return attackingLines;
}


void Board::initialisePassiveVectorsForPiece(Piece *t_piece) {
    const std::vector <std::deque<Position *>>& beingTouched = this->getLinesAttackingSquare(t_piece->getPosition());

    // Use the vectors in beingTouched to create MoveVectors
    for (auto const& attDefQueue : beingTouched) {
        // Convert the queue attDefQueue to a vector attDefVector
        std::vector<Position*> attDefVector;
        attDefVector.reserve(attDefQueue.size());
        auto qit = attDefQueue.begin();
        attDefVector.insert(attDefVector.begin(), qit, qit + attDefQueue.size());

        // For each active piece now, clear all of its existing move vectors that pass through this position
        Piece* activePiece = this->getPieceAtPosition(attDefVector.front());
        activePiece->clearActiveMoveVectorsThatPassThroughPosition(t_piece->getPosition());

        // Construct and register the new move vector
        MoveVector* mv = new MoveVector(activePiece, t_piece, attDefVector);
        mv->registerSelf();
    }

}

std::vector<std::deque<Position*>> Board::getLinesAttackingSquare(Position* t_position) const {
    std::vector <std::deque<Position *>> beingTouched;
    if (!t_position->isOffBoard()) {
        const std::vector<BoardIndex>& knightMovesFromPos = PositionUtils::getKnightMovesFromPosition(t_position);
        for (auto const &posIndex : knightMovesFromPos) {
            const Piece *piece = this->getPieceAtPosition(posIndex);
            if (piece != nullptr) {
                const PieceType type = piece->getType();
                if (type == PieceType::KNIGHT || type == PieceType::DUCHESS || type == PieceType::FORTRESS) {
                    std::deque<Position *> vec {this->getPosition(posIndex), t_position};
                    beingTouched.push_back(vec);
                }
            }
        }

        // Check for pawns/kings/wizards taking diagonally
        const std::vector<BoardIndex>& diagonalFromPos = PositionUtils::getDiagonalSquareIndices(t_position);
        for (auto const &posIndex : diagonalFromPos) {
            const Piece *piece = this->getPieceAtPosition(posIndex);
            if (piece != nullptr) {
                const PieceType type = piece->getType();
                if (type == PieceType::PAWN || type == PieceType::KING || type == PieceType::WIZARD) {
                    std::deque<Position*> vec {this->getPosition(posIndex), t_position};
                    beingTouched.push_back(vec);
                }

            }
        }

        // Check for kings/wizards taking horizontally/vertically
        const std::vector<BoardIndex>& adjacentToPos = PositionUtils::getAdjacentSquareIndices(t_position);
        for (auto const &posIndex : adjacentToPos) {
            const Piece *piece = this->getPieceAtPosition(posIndex);
            if (piece != nullptr) {
                const PieceType type = piece->getType();
                if (type == PieceType::KING || type == PieceType::WIZARD) {
                    std::deque < Position * > vec {this->getPosition(posIndex), t_position};
                    beingTouched.push_back(vec);
                }
            }
        }

        // Check for rook-style and bishop style attacks to here
        if (t_position->isVortex()) {
            // For each square diagonal of the vortex, go in a line OUT_ANTICLOCKWISE
            // For each square adjacent to the vortex, go in a line OUT
            const std::vector<short> directions {PositionUtils::DIRECTION_OUT_ANTICLOCKWISE, PositionUtils::DIRECTION_OUT};
            const std::vector<std::vector<BoardIndex>> adjacentSquares {diagonalFromPos, adjacentToPos};
            short i;
            for (i = 0; i < 2; ++i) {
                const std::vector<BoardIndex>& surroundingPositions = adjacentSquares[i];
                const short direction = directions[i];

                for (auto const& posIndex : surroundingPositions) {
                    short tempDirection = direction;
                    Position* pos = this->getPosition(posIndex);
                    Position* prev = t_position;
                    std::deque<Position*> line {prev};
                    Position* cur = pos;
                    bool success = false;
                    bool passComplete = false;
                    while (!passComplete && !cur->isOffBoard()) {
                        line.push_front(cur);
                        Piece* pieceHere = this->getPieceAtPosition(cur);
                        passComplete = pieceHere != nullptr;
                        if (passComplete) {
                            success = PositionUtils::canPieceMoveInDirection(pieceHere->getType(), direction);
                        } else {
                            const BoardIndex nextInLineIndex = PositionUtils::getIndexOfNextSquareInLine(prev, cur, tempDirection);
                            const short flapChange = PositionUtils::getFlapChange(prev, cur);
                            tempDirection = PositionUtils::determineNextDirectionGivenPreviousDirectionAndFlapChange(tempDirection,
                                                                                                                     flapChange);
                            prev = cur;
                            cur = this->getPosition(nextInLineIndex);
                        }

                    }

                    if (success) {
                        beingTouched.push_back(line);
                    }
                }
            }


        } else {
            short direction;
            for (direction = PositionUtils::DIRECTION_CLOCKWISE;
                 direction <= PositionUtils::DIRECTION_OUT_ANTICLOCKWISE; ++direction) {
                std::deque < Position * > positionsInLine {t_position};
                short tempDirection = direction;
                Position *prev = t_position;
                Position *cur = this->getPosition(PositionUtils::getIndexOfPositionInDirection(t_position, direction));
                // Do we know there's no point in continuing?
                bool passComplete = false;

                while (!cur->isOffBoard() && !passComplete) {
                    positionsInLine.push_front(cur);
                    const Piece *pieceHere = this->getPieceAtPosition(cur);
                    if (pieceHere != nullptr) {
                        passComplete = true;
                        if (PositionUtils::canPieceMoveInDirection(pieceHere->getType(), tempDirection)) {
                            beingTouched.push_back(positionsInLine);
                        }
                    }


                    const BoardIndex nextInLineIndex = PositionUtils::getIndexOfNextSquareInLine(prev, cur, tempDirection);
                    const short flapChange = PositionUtils::getFlapChange(prev, cur);
                    tempDirection = PositionUtils::determineNextDirectionGivenPreviousDirectionAndFlapChange(tempDirection,
                                                                                                             flapChange);
                    prev = cur;
                    cur = this->getPosition(nextInLineIndex);

                }
            }
        }

    }
    return beingTouched;
}

std::vector<Position*> Board::getAttackingOrDefendingVectorAlongLine(const Piece* t_piece, const short t_direction) const {

    short direction = t_direction;
    Position* startPosition = t_piece->getPosition();
    Position* prev = startPosition;
    const BoardIndex posIndex = PositionUtils::getIndexOfPositionInDirection(startPosition, t_direction);
    Position* curr = this->getPosition(posIndex);
    std::vector<Position*> positions {startPosition, curr};
    while (!curr->isOffBoard() && !this->doesSquareContainPiece(curr)) {
        // Keep extending along line adding positions
        // Finish one short of an ally, or on top of an enemy, or when off board
        // NB: This used to use nextPositionInDirection but ran into a bug crossing over the vortex
        // Further investigation may be required to ensure it's getting lines along vortex properly
        const BoardIndex nextCurrIndex = PositionUtils::getIndexOfNextSquareInLine(prev, curr, direction);
        const short flapChange = PositionUtils::getFlapChange(prev, curr);
        direction = PositionUtils::determineNextDirectionGivenPreviousDirectionAndFlapChange(direction, flapChange);
        prev = curr;
        curr = this->getPosition(nextCurrIndex);
        positions.push_back(curr);
    }

    // If the vector ended because we ran off the board there's obviously nothing along this line
    // Therefore, send back an empty array
    if (curr->isOffBoard()) {
        positions.clear();
    }

    return positions;
}

std::vector<Piece*> Board::getPawnReplaceOptionsForPlayer(const short t_player) const {
    std::vector<Piece*> options;
    const std::vector<Piece*>& playerPieces = this->getPiecesForPlayer(t_player);
    bool rookAdded = false;
    bool bishopAdded = false;
    for (auto const& piece : playerPieces) {
        if (piece->getPosition()->isOffBoard()) {
            if (piece->getType() == PieceType::BISHOP) {
                if (!bishopAdded) {
                    bishopAdded = true;
                    options.push_back(piece);
                }
            } else if (piece->getType() == PieceType::ROOK) {
                if (!rookAdded) {
                    rookAdded = true;
                    options.push_back(piece);
                }
            } else if (piece->getType() != PieceType::PAWN) {
                options.push_back(piece);
            }
        }
    }

    return options;
}

// If a piece moves, will it open a new
std::vector<Position*> Board::getAttackingLineOnKingThroughPosition(const Position* t_throughPosition, const short t_owner) const {
    Position* kingPosition = this->getPiecesForPlayer(t_owner)[12]->getPosition();
    std::vector<Position*> positionsInLine;

    if (kingPosition->isVortex()) {
        const std::vector<short> directions {PositionUtils::DIRECTION_OUT_ANTICLOCKWISE, PositionUtils::DIRECTION_OUT};
        const std::vector<std::vector<BoardIndex>> adjacentSquares {PositionUtils::getDiagonalSquareIndices(kingPosition),
                                                                  PositionUtils::getAdjacentSquareIndices(kingPosition)};
        short i;
        bool success = false;
        for (i = 0; i < 2 && !success; ++i) {
            const std::vector<BoardIndex>& surroundingPositions = adjacentSquares[i];
            const short& direction = directions[i];

            for (auto const& posIndex : surroundingPositions) {
                short tempDirection = direction;
                Position* pos = this->getPosition(posIndex);
                Position* prev = kingPosition;
                positionsInLine.push_back(prev);
                Position* cur = pos;
                bool passComplete = false;
                bool throughPos = false;
                while (!passComplete && !cur->isOffBoard()) {
                    bool curIsThroughPos = *cur == *t_throughPosition;
                    throughPos = throughPos || curIsThroughPos;
                    positionsInLine.push_back(cur);
                    const Piece* pieceHere = this->getPieceAtPosition(cur);
                    passComplete = !curIsThroughPos && pieceHere != nullptr;
                    if (passComplete) {
                        success = throughPos;
                    } else {
                        const BoardIndex nextInLineIndex = PositionUtils::getIndexOfNextSquareInLine(prev, cur, tempDirection);
                        const short flapChange = PositionUtils::getFlapChange(prev, cur);
                        tempDirection = PositionUtils::determineNextDirectionGivenPreviousDirectionAndFlapChange(tempDirection,
                                                                                                                 flapChange);
                        prev = cur;
                        cur = this->getPosition(nextInLineIndex);
                    }


                }

                if (!success) {
                    positionsInLine.clear();
                } else {
                    // We found a solution, break out of this for loop
                    break;
                }
            }
        }

    } else {

        short direction;
        Position *prev = kingPosition;
        bool solutionFound = false;
        bool possibleSolutionsExhausted = false;
        // Loop through each direction
        for (direction = PositionUtils::DIRECTION_CLOCKWISE;
             direction <= PositionUtils::DIRECTION_OUT_ANTICLOCKWISE && !solutionFound && !possibleSolutionsExhausted;
             ++direction) {

            short tempDirection = direction;
            Position *cur = this->getPosition(PositionUtils::getIndexOfPositionInDirection(kingPosition, direction));
            // Have we passed through the target position?
            bool passedThroughPos = false;
            // Do we know there's no point in continuing?
            bool passComplete = false;
            positionsInLine.push_back(kingPosition);
            // If we pass through t_throughPosition in this direction (with no interference beforehand) and then hit an enemy piece
            // which can travel in that direction, we know that moving a piece away from t_throughPosition will open up a new attack
            while (!cur->isOffBoard() && !solutionFound && !passComplete) {
                positionsInLine.push_back(cur);
                passedThroughPos = passedThroughPos || (*cur == *t_throughPosition);
                if (!passedThroughPos) {
                    // If there's a piece between the king and the target, we're done
                    passComplete = (this->getPieceAtPosition(cur) != nullptr);
                } else {
                    const Piece *pieceHere = this->getPieceAtPosition(cur);
                    if (pieceHere != nullptr) {
                        passComplete = true;
                        if (!TeamUtils::isSameTeam(pieceHere->getOwner(), t_owner)) {
                            solutionFound = PositionUtils::canPieceMoveInDirection(pieceHere->getType(), tempDirection);
                        }
                    }
                }
                const short flapChange = PositionUtils::getFlapChange(prev, cur);
                tempDirection = PositionUtils::determineNextDirectionGivenPreviousDirectionAndFlapChange(tempDirection,
                                                                                                         flapChange);
                const BoardIndex nextInLineIndex = PositionUtils::getIndexOfNextSquareInLine(prev, cur,
                                                                                                 tempDirection);
                prev = cur;
                cur = this->getPosition(nextInLineIndex);

            }

            if (!solutionFound) {
                positionsInLine.clear();
            }

            possibleSolutionsExhausted = passedThroughPos;

        }
    }

    return positionsInLine;
}

bool Board::isPlayerInCheck(const short t_player) const {
    const Piece* king = this->getPiecesForPlayer(t_player)[12];
    return king->getPassiveAttackingVectors().size() > 0;
}

void Board::applyMove(const Move *m, const EvaluationMode mode) {
    if (mode == EvaluationMode::CUMULATIVE) {
        this->applyMoveCumulative(m);
    } else {
        this->applyMoveDeNovo(m);
    }
}

void Board::applyMoveDeNovo(const Move *m) {
    if (!m->isNull()) {
        const BoardIndex offBoardIndex = 0;
        const Position* toPosition = m->getToPosition();
        const Position* fromPosition = m->getFromPosition();


        Piece *pieceAtOrigin = this->getPieceAtPosition(fromPosition);
        Piece *pieceAtDestination = this->getPieceAtPosition(toPosition);

        if (pieceAtDestination != nullptr) {
            // Move it to off-board
            this->setPiecePosition(pieceAtDestination, offBoardIndex);
        }

        Piece* activePiece;

        // Check vortex upgrades
        Piece* replacePiece = m->getReplacePiece();
        if (replacePiece != nullptr) {
            assert(replacePiece->getPosition()->isOffBoard());
            // Move the pawn to Off Board
            this->setPiecePosition(pieceAtOrigin, offBoardIndex);

            // Henceforth operate on the target piece
            activePiece = replacePiece;
        } else {
            activePiece = pieceAtOrigin;
        }

        this->setPiecePosition(activePiece, toPosition);

        this->initialiseAttDefVectors();

    }

    this->updateCheckmateRecords();
}

void Board::applyMoveCumulative(const Move* m) {
    if (!m->isNull()) {
        const BoardIndex offBoardIndex = 0;

        const Position* toPosition = m->getToPosition();
        const Position* fromPosition = m->getFromPosition();
        Piece *pieceAtOrigin = this->getPieceAtPosition(fromPosition);
        Piece *pieceAtDestination = this->getPieceAtPosition(toPosition);

        if (pieceAtDestination != nullptr) {
            // This piece is being taken
            // Clear its move vectors
            pieceAtDestination->clearAllMoveVectors();

            // Move it to off-board
            this->setPiecePosition(pieceAtDestination, offBoardIndex);
        }
        // For each piece this was being passively attacked/defended,
        // re-search along this square and add anything that comes up
        // (Effectively only bishop or rook style moves)
        const std::vector<Piece*>& piecesThatUsedToTouchThis = pieceAtOrigin->getPiecesActivelyTouchingThis();

        // Clear the move vectors of the piece we're moving
        pieceAtOrigin->clearAllMoveVectors();

        Piece* activePiece;

        // Check vortex upgrades
        Piece* replacePiece = m->getReplacePiece();
        if (replacePiece != nullptr) {
            assert(replacePiece->getPosition()->isOffBoard());
            // Move the pawn to Off Board
            this->setPiecePosition(pieceAtOrigin, offBoardIndex);

            // Henceforth operate on the target piece
            activePiece = replacePiece;
        } else {
            activePiece = pieceAtOrigin;
        }

        this->setPiecePosition(activePiece, toPosition);

        // Recompute the move vectors of the piece we're moving (passive and active)
        this->initialiseActiveVectorsForPiece(activePiece);
        this->initialisePassiveVectorsForPiece(activePiece);

        for (auto const& piece : piecesThatUsedToTouchThis) {
            const PieceType pieceType = piece->getType();
            if (pieceType == PieceType::ROOK || pieceType == PieceType::BISHOP || pieceType == PieceType::QUEEN ||
                pieceType == PieceType::FORTRESS || pieceType == PieceType::DUCHESS) {
                this->initialiseActiveVectorsForPieceThroughPosition(piece, fromPosition);
            }
        }

    }

    this->updateCheckmateRecords();
}

// Check for rook-style and bishop style attacks by t_piece through t_position
void Board::initialiseActiveVectorsForPieceThroughPosition(Piece *t_piece, const Position* t_position) {
    Position *piecePosition = t_piece->getPosition();

    if (piecePosition->isVortex()) {
        std::vector<std::vector<Position*>> attackingLinesFromVortex = getActiveLinesFromVortex(t_piece);
        for (auto const& attackingLine : attackingLinesFromVortex) {
            if (std::find(attackingLine.begin(), attackingLine.end(), t_position) != attackingLine.end()) {
                Piece *passivePiece = this->getPieceAtPosition(attackingLine.back());
                MoveVector *mv = new MoveVector(t_piece, passivePiece, attackingLine);
                mv->registerSelf();
            }
        }
    } else {

        std::vector < Position * > positionsInLine {piecePosition};

        // Have we found a solution yet?
        bool solutionFound = false;
        // Have we passed through pos yet?
        bool throughPos = false;

        const PieceType pieceType = t_piece->getType();

        short directionRangeStart;
        short directionRangeEnd;

        // Work out which directions piece can move in
        if (pieceType == PieceType::ROOK || pieceType == PieceType::QUEEN || pieceType == PieceType::FORTRESS) {
            directionRangeStart = PositionUtils::DIRECTION_CLOCKWISE;
        } else {
            directionRangeStart = PositionUtils::DIRECTION_IN_CLOCKWISE;
        }

        if (pieceType == PieceType::BISHOP || pieceType == PieceType::QUEEN || pieceType == PieceType::DUCHESS) {
            directionRangeEnd = PositionUtils::DIRECTION_OUT_ANTICLOCKWISE;
        } else {
            directionRangeEnd = PositionUtils::DIRECTION_OUT;
        }


        short direction;
        for (direction = directionRangeStart; direction <= directionRangeEnd && !throughPos; ++direction) {
            short tempDirection = direction;
            Position *prev = piecePosition;
            Position *cur = this->getPosition(PositionUtils::getIndexOfPositionInDirection(piecePosition, direction));
            // Do we know there's no point in continuing?
            bool passComplete = false;

            while (!cur->isOffBoard() && !passComplete) {
                positionsInLine.push_back(cur);
                const Piece *pieceHere = this->getPieceAtPosition(cur);
                throughPos = throughPos || (*cur == *t_position);
                if (pieceHere != nullptr) {
                    passComplete = true;
                    if (throughPos) {
                        solutionFound = true;
                    }
                }
                const BoardIndex nextInLineIndex = PositionUtils::getIndexOfNextSquareInLine(prev, cur,
                                                                                                 tempDirection);
                const short flapChange = PositionUtils::getFlapChange(prev, cur);
                tempDirection = PositionUtils::determineNextDirectionGivenPreviousDirectionAndFlapChange(tempDirection,
                                                                                                         flapChange);
                prev = cur;
                cur = this->getPosition(nextInLineIndex);

            }

            // If we've found a solution, make a mv out of it and register it
            if (solutionFound) {
                Piece *passivePiece = this->getPieceAtPosition(positionsInLine.back());
                MoveVector *mv = new MoveVector(t_piece, passivePiece, positionsInLine);
                mv->registerSelf();
            } else {
                // Otherwise, start again with a fresh vector
                positionsInLine = {piecePosition};
            }
        }
    }
}

void Board::updateCheckmateRecords() {
    // Update checkmate records
    // For each player whose king has someone actively attacking it (i.e. in check), check if they have any legal moves
    short player;
    for (player = 1; player <= 6; ++player) {
        bool isInCheckmate = false;
        if (this->isPlayerInCheck(player)) {
            const std::vector<Move>& moves = this->getLegalMoves(player);
            if (moves.size() == 1) {
                isInCheckmate = moves[0].isNull();
            }
        }

        if (isInCheckmate) {
            this->markPlayerInCheckmate(player);
        } else {
            this->markPlayerNotInCheckmate(player);
        }
    }
}

void Board::undoMove(const Move *m, const EvaluationMode mode) {
    if (mode == EvaluationMode::CUMULATIVE) {
        this->undoMoveCumulative(m);
    } else {
        this->undoMoveDeNovo(m);
    }
}

void Board::undoMoveDeNovo(const Move* m) {
    if (!m->isNull()) {
        const BoardIndex offBoardIndex = 0;

        // If there was a replacePiece, swap it out with a pawn
        // Fill activePiece accordingly
        Piece* activePiece = nullptr;
        Piece* replacePiece = m->getReplacePiece();
        if (replacePiece != nullptr) {
            // Move the new piece back off the board
            this->setPiecePosition(replacePiece, offBoardIndex);

            // Get a pawn off the board as our new active piece
            for (auto const& playerPiece : this->getPiecesForPlayer(replacePiece->getOwner())) {
                if (activePiece == nullptr) {
                    if (playerPiece->getType() == PieceType::PAWN && playerPiece->getPosition()->isOffBoard()) {
                        activePiece = playerPiece;
                    }
                } else {
                    break;
                }
            }
        } else {
            activePiece = this->getPieceAtPosition(m->getToPosition());
        }

        assert(activePiece != nullptr);

        // Move active piece back to "fromPosition"
        this->setPiecePosition(activePiece, m->getFromPosition());

        // If a piece was taken, put it on "toPosition"
        Piece* takenPiece = m->getPieceTaken();
        if (takenPiece != nullptr) {
            this->setPiecePosition(takenPiece, m->getToPosition());
        }

        this->initialiseAttDefVectors();

    }

    this->updateCheckmateRecords();

}

void Board::undoMoveCumulative(const Move* m) {
    if (!m->isNull()) {
        const BoardIndex offBoardIndex = 0;

        // If there was a replacePiece, swap it out with a pawn
        // Fill activePiece accordingly
        const Position* toPosition = m->getToPosition();
        const Position* fromPosition = m->getFromPosition();
        Piece* activePiece = nullptr;
        Piece* replacePiece = m->getReplacePiece();
        Piece* takenPiece = m->getPieceTaken();
        std::vector<Piece*> piecesThatUsedToTouchActive;
        if (replacePiece != nullptr) {
            // This path involved upgrading a pawn
            if (takenPiece == nullptr) {
                // We only need to force-update old touchers if we're not filling this
                // place with a new piece anyway (which would compute its own passive etc)
                piecesThatUsedToTouchActive = replacePiece->getPiecesActivelyTouchingThis();
            }
            // Move the new piece back off the board
            replacePiece->clearAllMoveVectors();
            this->setPiecePosition(replacePiece, offBoardIndex);

            // Get a pawn off the board as our new active piece
            for (auto const& playerPiece : this->getPiecesForPlayer(replacePiece->getOwner())) {
                if (activePiece == nullptr) {
                    if (playerPiece->getType() == PieceType::PAWN && playerPiece->getPosition()->isOffBoard()) {
                        activePiece = playerPiece;
                    }
                } else {
                    break;
                }
            }
        } else {
            activePiece = this->getPieceAtPosition(toPosition);
            if (takenPiece == nullptr) {
                // We only need to force-update old touchers if we're not filling this
                // place with a new piece anyway (which would compute its own passive etc)
                piecesThatUsedToTouchActive = activePiece->getPiecesActivelyTouchingThis();
            }
        }

        assert(activePiece != nullptr);

        // Move active piece back to "fromPosition"
        this->setPiecePosition(activePiece, fromPosition);
        activePiece->clearAllMoveVectors();

        // If a piece was taken, put it on "toPosition"
        if (takenPiece != nullptr) {
            this->setPiecePosition(takenPiece, toPosition);
            this->initialiseActiveVectorsForPiece(takenPiece);
            this->initialisePassiveVectorsForPiece(takenPiece);
        }

        activePiece->clearAllMoveVectors();
        this->initialiseActiveVectorsForPiece(activePiece);
        this->initialisePassiveVectorsForPiece(activePiece);

        // If the move was not used to take a piece, we need the pieces that used to touch the active piece
        // to search along the square in question for new move vectors
        if (takenPiece == nullptr) {

            for (auto const& piece : piecesThatUsedToTouchActive) {
                const PieceType pieceType = piece->getType();
                if (pieceType == PieceType::ROOK || pieceType == PieceType::BISHOP || pieceType == PieceType::QUEEN ||
                    pieceType == PieceType::FORTRESS || pieceType == PieceType::DUCHESS) {
                    this->initialiseActiveVectorsForPieceThroughPosition(piece, toPosition);
                }
            }
        }

    }

    this->updateCheckmateRecords();

}

bool Board::isLegalMove(const Move& t_move, const short t_player) const {
    // If it's a null move, check that we're in checkmate or that we're in stalemate
    if (t_move.isNull()) {
        return this->isPlayerInCheckmate(t_player) || this->getLegalMoves(t_player)[0].isNull();
    }
    const Position* fromPosition = t_move.getFromPosition();
    const Position* toPosition = t_move.getToPosition();
    const Piece* movingPiece = this->getPieceAtPosition(fromPosition);
    const Piece* takenPiece = t_move.getPieceTaken();
    const Piece* replacePiece = t_move.getReplacePiece();
    // Check some basic things about piece ownership
    if (!fromPosition->isOffBoard() &&
            (fromPosition->getBoardIndex() != toPosition->getBoardIndex()) &&
            movingPiece != nullptr &&
            (movingPiece->getOwner() == t_player) &&
            (replacePiece == nullptr ||
                    (replacePiece->getOwner() == t_player &&
                    replacePiece->getPosition()->isOffBoard() &&
                    toPosition->isVortex())) &&
            (takenPiece == nullptr ||
                    (!TeamUtils::isSameTeam(takenPiece->getOwner(), t_player) &&
                    (takenPiece->getPosition()->getBoardIndex() == toPosition->getBoardIndex())))) {

        // Now that the move seems legit, make sure it's a move that ensures we don't end in check
        std::vector<Position*> blockableSquares;
        const PieceType pieceType = movingPiece->getType();
        const Piece* myKing = this->getPiecesForPlayer(t_player)[12];

        // Determine if king is in check
        const std::vector<MoveVector*>& attackingKingVectors = myKing->getPassiveAttackingVectors();
        bool kingInCheck = attackingKingVectors.size() != 0;

        if (kingInCheck) {
            blockableSquares = MoveVectorUtils::getCommonPositions(attackingKingVectors);
        }

        const std::vector<Position*>& accessibleSquares = this->getAccessibleSquares(this->getPieceAtPosition(fromPosition));
        // If the destination isn't accessible, return false
        if (std::find(accessibleSquares.begin(), accessibleSquares.end(), toPosition) == accessibleSquares.end()) {
            return false;
        }

        // If our piece is blocking an attack on our king, the move has to be along that line
        const std::vector<Position*>& attackingLineOnKingThroughThisPiece = this->getAttackingLineOnKingThroughPosition(fromPosition, movingPiece->getOwner());
        if (attackingLineOnKingThroughThisPiece.size() > 0 &&
                std::find(attackingLineOnKingThroughThisPiece.begin(), attackingLineOnKingThroughThisPiece.end(), toPosition) == attackingLineOnKingThroughThisPiece.end()) {
            return false;
        }

        if (kingInCheck && pieceType != PieceType::KING) {
            // If the king is in check, make sure we're blocking that check AND that position is one we're allowed to go to
            return std::find(blockableSquares.begin(), blockableSquares.end(), toPosition) != blockableSquares.end();
        } else if (pieceType == PieceType::KING) {
            // If the piece is a king, make sure it's not moving into the path of an attack
            const std::vector<std::deque<Position*>>& attacksThroughPos = this->getLinesAttackingSquare(this->getPosition(toPosition));
            bool anyoneAttacking = false;
            for (auto const& positionDeque : attacksThroughPos) {
                const Piece* attackingPiece = this->getPieceAtPosition(positionDeque[0]);
                anyoneAttacking = anyoneAttacking || !TeamUtils::isSameTeam(movingPiece->getOwner(), attackingPiece->getOwner());
            }
            return !anyoneAttacking;

        } else {
            return true;
        }


    } else {
        return false;
    }
}

int Board::getNumPieces() const {
    int num = 0;
    for (auto const& playerPieces : m_pieces) {
        for (auto const& piece : playerPieces) {
            if (!piece->getPosition()->isOffBoard()) {
                ++num;
            }
        }
    }
    return num;
}