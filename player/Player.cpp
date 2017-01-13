#define NDEBUG

#include "Player.h"
#include "PlayerType.h"
#include "SearchAnalytics.h"
#include "Brain.h"

#include "../game/GameResult.h"

#include "../duchess/Move.h"
#include "../duchess/Board.h"
#include "../duchess/TeamUtils.h"

#include <vector>
#include <set>
#include <iostream>
#include <cassert>
#include <string>
#include <algorithm>
#include <cmath>
#include <chrono>

Move Player::chooseMove(Board* t_board, const int t_globalTurn) {
    std::vector<Move> validMoves = t_board->getLegalMoves(this->getPlayerNumber());

    // If the player is a human, just ask them for input
    if (this->getPlayerType() == PlayerType::HUMAN) {
        return this->chooseMoveAsHuman(t_board, &validMoves);
    } else if (this->getBrain()->getHeuristic() == Heuristic::RANDOM ||
                (this->isStochastic() && t_globalTurn <= Player::AI_RANDOM_MOVE_THRESHOLD)) {
        // If we're using the RANDOM "heuristic" or if it's early game, just get a random move
        int numMoves = validMoves.size();
        int randIndex = this->getBrain()->getRandomNumber(numMoves);
        return validMoves[randIndex];
    } else {
        // Otherwise, intelligently choose a move
        return this->searchForMove(t_board, validMoves, t_globalTurn);
    }

}

// Scan for input, make a move out of it, check it it's valid
Move Player::chooseMoveAsHuman(Board* t_board, const std::vector<Move>* t_validMoves) const {
    std::string input;
    std::cout << "Player " << this->getPlayerNumber() << " > ";
    std::cin >> input;
    while (input[0] != std::char_traits<char>::eof() && input[0] != 4) {
        if (input == "h") {
            std::cout << "Usage: " << std::endl;
            std::cout << "    h: Print this list of commands" << std::endl;
            std::cout << "    m: List legal moves" << std::endl;
            std::cout << "    b: Print board state" << std::endl;
            std::cout << "    i: Print info" << std::endl;
            std::cout << "Or write a move in the format:" << std::endl;
            std::cout << "    ORIGIN_DESTINATION[_REPLACE]" << std::endl;
        } else if (input == "m") {
            std::cout << "Legal moves:" << std::endl;
            for (auto const& move : *t_validMoves) {
                std::cout << move.toShortString() << " (" << move.toString() << ")" << std::endl;
            }
        } else if (input == "b") {
            std::cout << "Not implemented" << std::endl;
        } else if (input == "i") {
            std::cout << "You are player " << this->getPlayerNumber() << std::endl;
        } else {
            for (auto const& move : *t_validMoves) {
                if (move.toShortString() == input) {
                    return move;
                }
            }
            std::cout << "Your input does not match a move. Type 'h' for help." << std::endl;
        }
        std::cout << "Player " << this->getPlayerNumber() << " > ";
        std::cin >> input;
    }

    // If the signal is terminated, return a null move
    Move m;
    return m;
}

void Player::notifyResult(const GameResult t_result) {
    if (this->getPlayerType() == PlayerType::HUMAN) {
        std::cout << "Player " << this->getPlayerNumber() << ", you are a ";

        if (t_result == GameResult::WIN) {
            std::cout << "WINNER";
        } else if (t_result == GameResult::LOSE) {
            std::cout << "LOSER";
        } else {
            std::cout << "DRAWER?";
        }
        std::cout << std::endl;
    }
}

// Run alpha-beta search for each move, choose the best one
Move Player::searchForMove(Board* t_board, std::vector<Move> t_validMoves, const int t_globalTurn) {
    // Establish two timers so we can stop our iteratively deepened search
    // Note that this approach says "once you've gone over the time limit, just finish the depth you're currently on"
    std::chrono::high_resolution_clock::time_point searchEndedTime;
    std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();

    Move bestMove = t_validMoves[0];

    const int estimatedBranchingFactor = t_validMoves.size();
    const int estimatedBranchingFactorSqrt = std::sqrt(estimatedBranchingFactor);

    // If we're training enforce that we must search to at least depth 4
    // Otherwise, just depth 2 is okay
    const int minDepth = 2;
    const int maxDepth = 20;

    int depth = 0;
    SearchAnalytics analytics(t_globalTurn, t_board->getNumPieces());
    analytics.startTiming();
    int previousSearchDuration = 0;
    int estimatedNextDepthDuration = 0;

    // Keeps searching deeper until it estimates that the next depth down will take too long
    while ((estimatedNextDepthDuration < Brain::MAX_SEARCH_TIME_MILLIS && depth <= maxDepth)
           || depth <= minDepth) {
        // Only train the weights on our deepest pass
        if (this->getBrain()->isLearning()) {
            this->getBrain()->resetWeightDelta();
        }
        ++depth;
        // Record a new set of analytics now that we're going an extra depth (i.e. invalidate old measurements)
        analytics.reset();

        // Get best move searching to depth
        bestMove = this->alphaBetaFromRoot(t_validMoves, t_board, depth, &analytics, t_globalTurn);

        // Find out how long that search took
        searchEndedTime = std::chrono::high_resolution_clock::now();
        previousSearchDuration = std::chrono::duration_cast<std::chrono::milliseconds>(searchEndedTime - currentTime).count();

        // Estimate how long the next search will take
        estimatedNextDepthDuration = previousSearchDuration * estimatedBranchingFactorSqrt;
        currentTime = std::chrono::high_resolution_clock::now();
    }
    analytics.stopTiming();

    if (this->getBrain()->isLearning()) {
        this->getBrain()->applyWeightDelta();
    }

    this->getBrain()->addLineToSearchLogs(analytics.toString());

    // Finally, if this is stochastic, run a random check to just return a random value
    if (this->isStochastic()) {
        if (this->getBrain()->getRandomNumber(100) / 100.0 < Player::RANDOM_MOVE_CHANCE) {
            const int randIndex = this->getBrain()->getRandomNumber(t_validMoves.size());
            return t_validMoves[randIndex];
        }
    }

    return bestMove;
}

Move Player::alphaBetaFromRoot(std::vector<Move> t_validMoves, Board* t_board, const int t_maxDepth, SearchAnalytics* searchAnalytics, const int t_globalTurn) const {
    const int numValidMoves = t_validMoves.size();
    const EvaluationMode evaluationMode = this->getBrain()->getEvaluationMode();
    Move bestMove = t_validMoves[0];
    double alpha = -INFINITY;
    const short nextTurn = this->getPlayerNumber() == 6 ? 1 : this->getPlayerNumber() + 1;

    // Sort the valid moves based on the heuristic
    this->getBrain()->sortMovesBasedOnHeuristic(t_validMoves);

    // Analytics
    searchAnalytics->recordNodesExpandedAtTurn(t_globalTurn, numValidMoves);
    searchAnalytics->recordNodesEvaluatedAtTurn(t_globalTurn, numValidMoves);

    if (!this->getBrain()->isLearning() && numValidMoves == 1) {
        // If we're not learning and only have one valid move (e.g. in checkmate), just return that
        // If we are learning, it's important for us to train from here, so do the full search
        return t_validMoves[0];
    } else {
        // For each legal move, get it's alpha-beta search value
        // Choose the best one
        for (auto const& move : t_validMoves) {
            t_board->applyMove(&move, evaluationMode);
            // Note we use max depth - 1 here because t_board is now one down from the root, so we're already 1 depth in
            const double val = -1 * this->alphaBetaMoveSearch(t_board, t_maxDepth - 1, alpha, INFINITY, nextTurn, searchAnalytics, t_globalTurn + 1, move, t_maxDepth);
            t_board->undoMove(&move, evaluationMode);

            if (val >= alpha) {
                alpha = val;
                bestMove = move;
            }

        }

        // If we are learning, update the weights
        if (this->getBrain()->isLearning()) {
            // t_alpha is an exact value
            this->getBrain()->updateWeightsForFeatures(this->getBrain()->boardToFeatures(*t_board, this->getPlayerNumber()), alpha, alpha, 1, this->getPlayerNumber(), *t_board);
        }
    }

    return bestMove;
}

double Player::alphaBetaMoveSearch(Board* t_board, const int t_depth, double t_alpha, double t_beta, const short t_turn, SearchAnalytics* searchAnalytics, const int t_globalTurn, const Move& t_parentMove, const int t_maxDepth) const {
    // Should we stop now?
    const bool isTerminal = t_board->isTerminal();
    if (t_depth == 0 || isTerminal) {
        // Search is complete, return evaluation of this state
        // We use -WINNING_MOVE_SCORE + depth here to prefer winning in a short number of moves
        return (isTerminal ? ((-1 * Brain::WINNING_MOVE_SCORE) + t_depth) : this->getBrain()->evaluateBoardState(*t_board, t_turn));
    } else {
        // For our analytics, we track how many moves we generated to look at, and how many we actually looked through
        int nodesExpanded = 0;
        int nodesEvaluated = 0;

        const EvaluationMode evaluationMode = this->getBrain()->getEvaluationMode();
        double bestValue = -INFINITY;
        bool foundCutoff = false;
        double originalAlpha = t_alpha;

        // Before we generate the legal moves here, depending on our heuristic we may be able to find an early cutoff
        // Here we try to fill earlyMoves with cached prior-moves that may be helpful
        const Heuristic heuristic = this->getBrain()->getHeuristic();
        std::vector<Move> earlyMoves;
        if (heuristic == Heuristic::KILLER_MOVE) {
            const std::set<Move> killerMoves = this->getBrain()->getHeuristicCache().getKillerMoves(t_turn);
            for (auto const& killerMove : killerMoves) {
                if (t_board->isLegalMove(killerMove, t_turn)) {
                    earlyMoves.push_back(killerMove);
                } else {
                    this->getBrain()->getHeuristicCache().eraseMoveAsKiller(killerMove, t_turn);
                }
            }
        } else if (heuristic == Heuristic::HISTORY) {
            const std::vector<Move> historyMoves = this->getBrain()->getHeuristicCache().getHistoryMoves(t_turn);
            for (auto const& historyMove : historyMoves) {
                if (t_board->isLegalMove(historyMove, t_turn)) {
                    earlyMoves.push_back(historyMove);
                } else {
                    this->getBrain()->getHeuristicCache().eraseMoveAsHistory(historyMove, t_turn);
                }
            }
        } else if (heuristic == Heuristic::BEST_REPLY && this->getBrain()->getHeuristicCache().hasBestReply(t_parentMove)) {
            const Move bestReply = this->getBrain()->getHeuristicCache().getBestReply(t_parentMove);
            if (t_board->isLegalMove(bestReply, t_turn)) {
                earlyMoves.push_back(bestReply);
            } else {
                this->getBrain()->getHeuristicCache().eraseMoveAsBestReply(t_parentMove);
            }
        }

        // If we found any moves to try early, try them first
        for (auto const& move : earlyMoves) {
            ++nodesEvaluated;
            // Apply (and undo) the move to effectively temporarily generate a child state, and evaluate it
            t_board->applyMove(&move, evaluationMode);
            const short nextTurn = (t_turn == 6 ? 1 : t_turn + 1);
            const double val = -alphaBetaMoveSearch(t_board, t_depth - 1, -t_beta, -t_alpha, nextTurn, searchAnalytics, t_globalTurn + 1, move, t_maxDepth);
            t_board->undoMove(&move, evaluationMode);

            bestValue = std::max(bestValue, val);
            t_alpha = std::max(t_alpha, val);

            // Note the use of FLOAT_FUDGE to stop rounding errors from preventing cutoffs
            if (t_alpha + Brain::FLOAT_FUDGE >= t_beta) {
                // We have found a cutoff
                foundCutoff = true;
                searchAnalytics->recordCutoffAtTurn(t_globalTurn);
                break;
            }
        }

        // If our heuristics didn't give us a premature cutoff, we go ahead and generate the legal moves from here
        if (!foundCutoff) {

            // Get the valid moves from this state as child nodes
            std::vector <Move> validMoves = t_board->getLegalMoves(t_turn);
            nodesExpanded += validMoves.size();
            // Arrange the child nodes based on the ordering heuristic
            this->getBrain()->sortMovesBasedOnHeuristic(validMoves);

            for (auto const &move : validMoves) {
                ++nodesEvaluated;
                // Apply (and undo) the move to effectively temporarily generate a child state, and evaluate it
                t_board->applyMove(&move, evaluationMode);
                const short nextTurn = (t_turn == 6 ? 1 : t_turn + 1);
                const double val = -alphaBetaMoveSearch(t_board, t_depth - 1, -t_beta, -t_alpha, nextTurn,
                                                        searchAnalytics, t_globalTurn + 1, move, t_maxDepth);
                t_board->undoMove(&move, evaluationMode);
                if (val > bestValue) {
                    bestValue = val;
                    if (heuristic == Heuristic::BEST_REPLY) {
                        this->getBrain()->getHeuristicCache().markMoveAsBestReply(move, t_parentMove);
                    }
                }
                t_alpha = std::max(t_alpha, val);

                // Note the use of FLOAT_FUDGE to stop rounding errors from preventing cutoffs
                if (t_alpha + Brain::FLOAT_FUDGE >= t_beta) {
                    // We have found a cutoff
                    foundCutoff = true;
                    searchAnalytics->recordCutoffAtTurn(t_globalTurn);
                    if (heuristic == Heuristic::KILLER_MOVE) {
                        this->getBrain()->getHeuristicCache().markMoveAsKiller(move, t_turn);
                    } else if (heuristic == Heuristic::HISTORY) {
                        // Note that the depth in the tree currently is t_maxDepth - t_depth
                        this->getBrain()->getHeuristicCache().markMoveAsHistory(move, t_maxDepth - t_depth, t_turn);
                    }
                    break;
                }
            }
        }

        // Mark the results in our analytics
        searchAnalytics->recordNodesExpandedAtTurn(t_globalTurn, nodesExpanded);
        searchAnalytics->recordNodesEvaluatedAtTurn(t_globalTurn, nodesEvaluated);

        // If we are learning, update the weights
        if (this->getBrain()->isLearning()) {
            if (foundCutoff) {
                // t_alpha is a lower bound
                this->getBrain()->updateWeightsForFeatures(this->getBrain()->boardToFeatures(*t_board, t_turn), t_alpha, INFINITY, t_maxDepth - t_depth, t_turn, *t_board);
            } else if (t_alpha > originalAlpha) {
                // t_alpha is an exact value
                this->getBrain()->updateWeightsForFeatures(this->getBrain()->boardToFeatures(*t_board, t_turn), t_alpha, t_alpha, t_maxDepth - t_depth, t_turn, *t_board);
            } else {
                // t_alpha is an upper bound
                this->getBrain()->updateWeightsForFeatures(this->getBrain()->boardToFeatures(*t_board, t_turn), -INFINITY, t_alpha, t_maxDepth - t_depth, t_turn, *t_board);
            }
        }

        return bestValue;
    }

}