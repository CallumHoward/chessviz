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
    mBoard[0] = {
        W_ROOK, W_KNIGHT, W_BISHOP, W_QUEEN, W_KING, W_BISHOP, W_KNIGHT, W_ROOK};
    mBoard[mBoardSize - 1] = {
        B_ROOK, B_KNIGHT, B_BISHOP, B_QUEEN, B_KING, B_BISHOP, B_KNIGHT, B_ROOK};

    for (size_t c = 0; c < mBoard.at(0).size(); ++c) {
        mBoard[1][c] = W_PAWN;
        mBoard[mBoardSize - 2][c] = B_PAWN;
    }
}

char ChessBoard::getLastPieceMoved() const {
    return mLastPlay.piece().letter();
}

} // namespace chess

