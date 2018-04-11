#include <PGNGameCollection.h>
#include <fstream>
#include <iostream>
#include <exception>



int main() {
    try {
        std::ifstream pgnfile("./sample.pgn");

        pgn::GameCollection games;

        // collecting games from file
        pgnfile >> games;

        // counting the games
        std::cout << "The sample.pgn file contains " << games.size() << " games." << '\n';

    } catch (std::exception &e) {
        std::cerr << "exception: " << e.what() << '\n';
        return -1;
    }

    return 0;
}
