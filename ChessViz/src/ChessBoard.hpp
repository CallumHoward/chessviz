// ChessBoard.hpp

#ifndef CHESSBOARD_HPP
#define CHESSBOARD_HPP

#include "ChessState.hpp"

namespace chess {

enum Cell {
    EMPTY
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    KING,
    QUEEN
};

class ChessBoard {
public:
    void setup();
    void draw();
    void reset();

    void setCell(size_t row, size_t col, Cell c);
    Cell getCell(size_t row, size_t col) { return mBoard[row][col]; }

private:
    size_t mBoardSize = 8;
    bool mIsWhite;
    bool mIsCaptured;
    std::vector<std::vector<Cell>> mBoard;
};

} // namespace chess

#endif // CHESSBOARD_HPP
