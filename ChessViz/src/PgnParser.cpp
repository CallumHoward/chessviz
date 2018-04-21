// PgnParser.cpp

#include <string>
#include <stdexcept>

#include <PGNGameCollection.h>
#include <PGNPosition.h>
#include <fstream>

#include "cinder/Log.h"

#include "chUtils.hpp"
#include "ChessBoard.hpp"
#include "PgnParser.hpp"


namespace chess {

void PgnParser::parse(const std::string& pgnFilename) {
    pgn::GameCollection pgnGames;

    try {
        // get all the games from files into game collection
        std::ifstream pgnfile(pgnFilename.c_str());
        pgnfile >> pgnGames;

    } catch (std::exception& e) {
        CI_LOG_E("Couldn't read from file: " << e.what());
    }

    for (const auto& game : pgnGames) {
        mGames.emplace_back(processGame(game));
    }
}

std::vector<ChessBoard> PgnParser::processGame(const pgn::Game& game) {
    auto chessBoards = std::vector<ChessBoard>{ChessBoard{}};
    chessBoards.back().setup();

    for (const auto& move : game.moves()) {
        // take a copy of the most recent move from most recent game
        auto chessBoard = chessBoards.back();

        // apply moves to the chessBoard state
        processPlay(chessBoard, move.white());
        processPlay(chessBoard, move.black());

        // add state back on to the list of games
        mGames.back().push_back(chessBoard);
    }

    return chessBoards;
}

void PgnParser::processPlay(ChessBoard& chessBoard, pgn::Ply play) {
    auto positions = pgn::Position{};
    positions.update(play);

    auto fromCoord = getCoord(play.fromSquare());
    Cell cellContents = chessBoard.getCell(fromCoord.first, fromCoord.second);
    chessBoard.setCell(fromCoord.first, fromCoord.second, Cell::EMPTY);

    auto toCoord = getCoord(play.toSquare());
    chessBoard.setCell(toCoord.first, toCoord.second, cellContents);
}

std::pair<size_t, size_t> PgnParser::getCoord(const pgn::Square& square) {
    const auto row = square.row();
    const auto col = square.col();

    if (row == '\0') { throw std::runtime_error{"no row given"}; }
    if (col == '\0') { throw std::runtime_error{"no col given"}; }

    const auto coordRow = static_cast<size_t>(row - '1');
    const auto coordCol = static_cast<size_t>(col - 'a');

    if (not ch::within(coordRow, 0ul, 8ul) or not ch::within(coordCol, 0ul, 8ul)) {
        throw std::range_error{"coordinate not in range"};
    }

    return std::pair<int, int>{coordRow, coordCol};
}

ChessBoard& PgnParser::getBoardAt(size_t game, size_t move) {
    return mGames.at(game).at(move);
}

} // namespace chess

