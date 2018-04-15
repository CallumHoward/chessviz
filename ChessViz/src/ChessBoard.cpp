// ChessBoard.cpp

#include "ChessState.hpp"
#include "ChessBoard.hpp"

namespace chess {

ChessBoard::setup() {
    mBoard = std::vector<std::vector<Cell>>{
        mBoardSize, std::vector<Cell>{mBoardSize, EMPTY}};
    reset();
}

ChessBoard::draw() {

}

ChessBoard::reset() {
    for (size_t c = 0; c < mBoard.at(0).size(); ++c) {
        mBoard[0][c] = PAWN;
        mBoard[1][c] = PAWN;
        mBoard[mBoardSize - 1][c] = PAWN;
        mBoard[mBoardSize - 2][c] = PAWN;
    }
}

} // namespace chess

