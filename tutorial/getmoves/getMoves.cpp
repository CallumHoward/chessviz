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
#include <fstream>
#include <iostream>

//
// A very basic example in handling move object from a list of games.
// Using iterators we get each move and we just print it on stdout.
//
int main() {
    try {
        // get all the games from files into game collection
        std::string fname = "./games.pgn";
        std::ifstream pgnfile(fname.c_str());
        pgn::GameCollection games;
        pgnfile >> games;

        // iterate now trough the collection ...
        for (const auto& game : games) {

            // ... and get the move list from each game
            pgn::MoveList movelist = game.moves();

            std::cout << '\n'
                      << "---- new game ----" << '\n'
                      << '\n';

            // now let's iterate trough the list of moves and print each one to stdout
            for (const auto& move : movelist) {
                std::cout << "move: ." << move << "." << '\n';
            }
        }

    } catch (std::exception &e) {
        std::cerr << "exception: " << e.what() << '\n';
        return -1;
    }

    return 0;
}
