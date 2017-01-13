#ifndef DUCHESS_CPP_SERVER_H
#define DUCHESS_CPP_SERVER_H

#include <vector>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>

#include "../duchess/Piece.h"
#include "../duchess/Move.h"
#include "../player/Player.h"
#include "../player/Brain.h"

class Server {
public:
    Server(const char* const portNum);

    ~Server();

    void run();

    static void signal_handler(int sig);

    void setPlayer(Player* t_player) {
        this->m_player = t_player;
    }

    void setBrain(Brain* t_brain) {
        this->m_brain = t_brain;
    }

    Player* getPlayer() const {
        return m_player;
    }

    Brain* getBrain() const {
        return m_brain;
    }

    std::vector<std::vector<Piece>> dataToPieces(const char incomingData[]) const;
    std::vector<std::vector<Piece>> getUnplacedPieces() const;


    Position* getOffBoardPos() const {
        return this->m_offBoardPos;
    }

private:
    Player* m_player;
    Brain* m_brain;

    // A malloced pos that the pieces functions can use
    Position* m_offBoardPos;

    const static int SOCKET_QUEUE_SIZE = 10;
    const static int DATA_BUFFER_SIZE = 1000;

    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    int socketfd;
    int new_sd;

    static bool isRunning;

    struct sigaction sa;
};

#endif //DUCHESS_CPP_SERVER_H
