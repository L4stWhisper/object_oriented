#include "RandomAI.h"

Move RandomAI::decideMove(const Board& board, PlayerColor) {
    int n = board.getSize();
    std::vector<Move> moves;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (board.at(i, j) == PlayerColor::NONE)
                moves.push_back({i, j, false});

    if (moves.empty()) return {0, 0, true};
    return moves[rand() % moves.size()];
}
