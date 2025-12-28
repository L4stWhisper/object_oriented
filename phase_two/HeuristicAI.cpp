#include "HeuristicAI.h"

Move HeuristicAI::decideMove(const Board& board, PlayerColor) {
    int n = board.getSize();
    int bestX = -1, bestY = -1;
    int bestScore = -1;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (board.at(i, j) == PlayerColor::NONE) {
                int score = 0;
                if (i > 0 && board.at(i - 1, j) != PlayerColor::NONE) score++;
                if (j > 0 && board.at(i, j - 1) != PlayerColor::NONE) score++;
                if (score > bestScore) {
                    bestScore = score;
                    bestX = i;
                    bestY = j;
                }
            }
        }
    }
    return {bestX, bestY, false};
}
