/*

 Copyright (C) 2002 Andrea Vinzoni <vinzo@planet.it>

     This file is part of pgnlib.

     pgnlib is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 2 of the License, or
     (at your option) any later version.

     pgnlib is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

     See the GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with pgnlib; if not, write to the Free Software Foundation,
     Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#include <PGNGameCollection.h>
#include <PGNPosition.h>
#include <fstream>
#include <iostream>

#include "AsciiBoard.hpp"

template <typename T>
bool within(T target, T lower, T upper);

//
// A very basic example in handling move object from a list of games.
// Using iterators we get each move and we just print it on stdout.
//
int main() {

    auto board = ch::AsciiBoard{};
    board.printBoard();

    try {
        // get all the games from files into game collection
        std::string fname = "./games.pgn";
        std::ifstream pgnfile(fname.c_str());
        pgn::GameCollection games;
        pgnfile >> games;

        // iterate now through the collection ...
        for (const auto& game : games) {

            // ... and get the move list from each game
            pgn::MoveList movelist = game.moves();

            std::cout << '\n'
                      << "---- new game ----" << '\n'
                      << '\n';

            auto positions = pgn::Position{};

            // now let's iterate trough the list of moves and print each one to stdout
            for (const auto& move : movelist) {
                std::cout << "move: ." << move << "." << '\n';

                {
                    auto play = move.white();
                    positions.update(play);
                    const auto from = play.fromSquare();

                    const auto fromRow = static_cast<size_t>(from.row() - '1');
                    const auto fromCol = static_cast<size_t>(from.col() - 'a');
                    if (not within(fromRow, 0ul, 8ul) || not within(fromCol, 0ul, 8ul)) {
                        std::cout << "invalid white from move: "
                            << fromRow << ", " << fromCol
                            << " <= " << from.row() << ", " << from.col() << "\n";
                    } else {
                        board.setCell(fromRow, fromCol, ch::AsciiBoard::EMPTY);
                    }

                    const auto to = move.white().toSquare();
                    const auto toRow = static_cast<size_t>(to.row() - '1');
                    const auto toCol = static_cast<size_t>(to.col() - 'a');
                    if (not within(toRow, 0ul, 8ul) || not within(toCol, 0ul, 8ul)) {
                        std::cout << "invalid white to move: "
                            << fromRow << ", " << fromCol
                            << " <= " << from.row() << ", " << from.col() << "\n";
                    } else {
                        board.setCell(toRow, toCol, ch::AsciiBoard::OCCUPIED);
                    }
                }

                board.printBoard();
                std::cout << std::endl;

                {
                    auto play = move.black();
                    if (not play.valid()) { break; }
                    positions.update(play);
                    const auto from = play.fromSquare();

                    //if (from.null()) { continue; }

                    const auto fromRow = static_cast<size_t>(from.row() - '1');
                    const auto fromCol = static_cast<size_t>(from.col() - 'a');
                    if (not within(fromRow, 0ul, 8ul) || not within(fromCol, 0ul, 8ul)) {
                        std::cout << "invalid black from move: "
                            << fromRow << ", " << fromCol
                            << " <= " << from.row() << ", " << from.col() << "\n";
                    } else {
                        board.setCell(fromRow, fromCol, ch::AsciiBoard::EMPTY);
                    }

                    const auto to = move.black().toSquare();
                    const auto toRow = static_cast<size_t>(to.row() - '1');
                    const auto toCol = static_cast<size_t>(to.col() - 'a');
                    if (not within(toRow, 0ul, 8ul) || not within(toCol, 0ul, 8ul)) {
                        std::cout << "invalid black to move: "
                            << toRow << ", " << toCol
                            << " <= " << from.row() << ", " << from.col() << "\n";
                    } else {
                        board.setCell(toRow, toCol, ch::AsciiBoard::EMPTY);
                    }
                }

                board.printBoard();
                std::cout << std::endl;
            }
        }

    } catch (std::exception &e) {
        std::cerr << "exception: " << e.what() << '\n';
        return -1;
    }

    return 0;
}

template <typename T>
bool within(T target, T lower, T upper) {
    return lower <= target and target < upper;
}
