// PgnParser.cpp

#include <string>

#include <PGNGameCollection.h>
#include <PGNPosition.h>
#include <fstream>


namespace chess {

PgnParser::parse(const std::string& pgnFilename) {
    pgn::GameCollection pgnGames;

    try {
        // get all the games from files into game collection
        std::ifstream pgnfile(pgnFilename.c_str());
        pgnfile >> pgnGames;

    } catch (std::exception& e) {
        CI_LOG_E("Couldn't read from file: " << e.what() << '\n');
    }

    for (const auto& game : games) {
        mGames.emplace_back(processGame(game));
    }
}

std::vector<ChessBoard> PgnParser::processGame(const pgn::Game& game) {
    for (const auto& move : game.moves()) {
        auto cb = mGames.back().back();
        processPlay(cb, move.white());
        processPlay(cb, move.black());
        //TODO
    }
}

void PgnParser::processPlay(const pgn::Ply& play) {
    auto positions = pgn::Position{};
    positions.update(play);
    getCoord(play.fromSquare());
}

std::pair<int, int> PgnParser::getCoord(const pgn::Square& square) {
    const auto row = square.row();
    const auto col = square.col();

    if (row == '\0') { throw std::exception{"no row given"}; }
    if (col == '\0') { throw std::exception{"no col given"}; }

    const auto coordRow = static_cast<size_t>(row - '1');
    const auto coordCol = static_cast<size_t>(col - 'a');

    if (not within(coordRow, 0ul, 8ul) or not within(coordCol, 0ul, 8ul)) {
        throw std::exception{"coordinate not in range"}
    }

    return std::pair<int, int>{coordRow, coordCol};
}

ChessBoard& PgnParser::getBoardAt(size_t game, size_t move) {
    return mBoards.at(game).at(move);
}

} // namespace chess

