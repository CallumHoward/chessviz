// ChessBoard.hpp

#ifndef CHESSBOARD_HPP
#define CHESSBOARD_HPP


namespace chess {

enum Cell {
    EMPTY,
    W_PAWN,
    W_ROOK,
    W_KNIGHT,
    W_BISHOP,
    W_KING,
    W_QUEEN,
    B_PAWN,
    B_ROOK,
    B_KNIGHT,
    B_BISHOP,
    B_KING,
    B_QUEEN,
};

class ChessBoard {
public:
    void setup();
    void draw();
    void reset();

    void setCell(size_t row, size_t col, Cell c) { mBoard[row][col] = c; }
    Cell getCell(size_t row, size_t col) { return mBoard[row][col]; }

private:
    size_t mBoardSize = 8;
    bool mIsWhite;
    bool mIsCaptured;
    std::vector<std::vector<Cell>> mBoard;
};

} // namespace chess

#endif // CHESSBOARD_HPP
