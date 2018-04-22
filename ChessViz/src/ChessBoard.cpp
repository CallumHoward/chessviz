// ChessBoard.cpp

#include <PGNPly.h>

#include "ChessBoard.hpp"


namespace chess {

void ChessBoard::setup() {
    mBoard = std::vector<std::vector<Cell>>{
        mBoardSize, std::vector<Cell>{mBoardSize, EMPTY}};
    reset();
}

void ChessBoard::reset() {
    for (size_t c = 0; c < mBoard.at(0).size(); ++c) {
        mBoard[0][c] = W_PAWN;
        mBoard[1][c] = W_PAWN;
        mBoard[mBoardSize - 1][c] = B_PAWN;
        mBoard[mBoardSize - 2][c] = B_PAWN;
    }
}

char ChessBoard::getLastPieceMoved() const {
    return mLastPlay.piece().letter();
}

} // namespace chess

