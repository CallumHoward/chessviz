// AsciiBoard.hpp
// Callum Howard, 2018

#include <vector>
#include <sstream>
#include <iostream>


namespace ch {

class AsciiBoard {
public:
    enum Cell {
        EMPTY,
        OCCUPIED
//        PAWN,
//        ROOK,
//        KNIGHT,
//        BISHOP,
//        KING,
//        QUEEN
    };

    AsciiBoard() {
        mBoard = std::vector<std::vector<Cell>>{mBoardSize, std::vector<Cell>{mBoardSize, EMPTY}};
        reset();
    }

    void printBoard() {
        std::ostringstream oss;

        for (const auto& row : mBoard) {

            for (const auto& cell : row) {

                if (cell == OCCUPIED) {
                    oss << "* ";
                } else {
                    oss << ". ";
                }

            }

            oss << '\n';
        }

        std::cout << oss.str();
    }

    void setCell(size_t row, size_t col, Cell c) { mBoard[row][col] = c; }

    Cell getCell(size_t row, size_t col) { return mBoard[row][col]; }

    void reset() {
        for (size_t c = 0; c < mBoard.at(0).size(); ++c) {
            mBoard[0][c] = OCCUPIED;
            mBoard[1][c] = OCCUPIED;
            mBoard[mBoardSize - 1][c] = OCCUPIED;
            mBoard[mBoardSize - 2][c] = OCCUPIED;
        }
    }

private:
    size_t mBoardSize = 8;
    std::vector<std::vector<Cell>> mBoard;
};

} // namespace ch
