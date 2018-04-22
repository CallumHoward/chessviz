// ChessBoard.hpp

#ifndef CHESSBOARD_HPP
#define CHESSBOARD_HPP

#include <PGNPly.h>


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
    void reset();

    void setCell(size_t row, size_t col, Cell c) { mBoard[row][col] = c; }
    Cell getCell(size_t row, size_t col) const { return mBoard[row][col]; }

    bool getIsWhite() const { return mIsWhite; }
    void setIsWhite(bool isWhite) { mIsWhite = isWhite; }
    bool getIsCaptured() const { return mIsCaptured; }
    void setIsCaptured(bool isCaptured) { mIsCaptured = isCaptured; }

    void setLastPlay(pgn::Ply play) { mLastPlay = play; }
    //pgn::Ply getLastPlay() const { return mLastPlay; }

    char getLastPieceMoved() const;

private:
    size_t mBoardSize = 8;
    bool mIsWhite;
    bool mIsCaptured;
    std::vector<std::vector<Cell>> mBoard;
    pgn::Ply mLastPlay;
};

} // namespace chess

#endif // CHESSBOARD_HPP
