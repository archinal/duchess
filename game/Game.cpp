#define NDEBUG

#include "Game.h"
#include "GameLog.h"

#include "../duchess/Board.h"
#include "../duchess/TeamType.h"
#include "../duchess/TeamUtils.h"
#include "../duchess/Move.h"
#include "../duchess/EvaluationMode.h"

#include "../player/Player.h"

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <ctime>
#include <cassert>
#include <fstream>
#include <deque>

Game::Game(std::vector<Player*> t_players, EvaluationMode t_evaluationMode) :
    m_players(t_players), m_evaluationMode(t_evaluationMode) {
    std::time_t curTime = std::time(nullptr);
    std::string id = std::ctime(&curTime);
    std::replace(id.begin(), id.end(), ' ', '_');
    std::replace(id.begin(), id.end(), ':', '-');
    id.erase(std::remove(id.begin(), id.end(), '\n'), id.end());
    // If the player is an AI, add a random string onto the end of the ID
    if (t_players[0]->getPlayerType() == PlayerType::AI) {
        id += '_';
        const int numRandomChars = 6;
        int i;
        for (i = 0; i < numRandomChars; ++i) {
            id += 'a' + t_players[0]->getBrain()->getRandomNumber(25);
        }
    }

    m_id = id;
    m_log.setID(m_id);
    m_logLocation = "";
}

void Game::run() {
    Board& board = this->getBoard();
    GameLog& log = this->getLog();

    log.startTiming();

    // Start by initialising the attacking and defending vectors
    board.initialiseAttDefVectors();

    short whoseTurn = 1;
    int turnNumber = 1;

    const EvaluationMode evalMode = this->getEvaluationMode();

    // Keep track of the last 24 moves
    // If the first 12 are the same as the last 12, we're in stalemate
    std::deque<Move> last24Moves;

    bool stalemate = false;

    while (!board.isTerminal() && turnNumber <= this->MAX_TURNS && !stalemate) {
        // Ask the player whose turn it is for a move
        const Move move = this->getPlayers()[whoseTurn - 1]->chooseMove(&board, turnNumber);

        log.recordMove(move);

        assert(board.isLegalMove(move));

        // Apply the move
        board.applyMove(&move, evalMode);

        last24Moves.push_back(move);

        if (last24Moves.size() == 24) {
            bool movesRepeated = true;
            // Check last 24 moves, if the first 12 are the same as the last 12 we're in stalemate
            int i;
            for (i = 0; i < 12 && movesRepeated; ++i) {
                // Loop breaks when movesRepeated becomes false
                movesRepeated = last24Moves[i] == last24Moves[i + 12];
            }

            last24Moves.pop_front();
            stalemate = movesRepeated;
        }

        // Increment whose turn it is
        whoseTurn = (whoseTurn == 6 ? 1 : whoseTurn + 1);
        ++turnNumber;
    }

    log.stopTiming();

    // Tell the players who won
    const TeamType winner = board.getWinner();

    log.setWinner(winner);

    for (auto const& player : this->getPlayers()) {
        GameResult result;
        if (winner == TeamType::NONE) {
            result = GameResult::DRAW;
        } else if (TeamUtils::getTeamForPlayer(player->getPlayerNumber()) == winner) {
            result = GameResult::WIN;
        } else {
            result = GameResult::LOSE;
        }
        player->notifyResult(result);
    }

    this->saveLog();

}

void Game::saveLog() const {
    const std::string dest = this->getLogLocation();
    if (dest != "") {
        std::ofstream outFile(dest);
        outFile << m_log.toString() << std::endl;
        outFile.close();
    }
}