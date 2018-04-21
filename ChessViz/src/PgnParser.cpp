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

    auto positions = pgn::Position{};

    for (const auto& move : game.moves()) {
        // take a copy of the most recent move from most recent game
        auto chessBoard = chessBoards.back();

        // apply moves to the chessBoard state
        const auto whitePlay = move.white();
        processPlay(chessBoard, positions, whitePlay);
        const auto blackPlay = move.black();
        if (not blackPlay.valid()) { break; }  // game ends in a white move
        processPlay(chessBoard, positions, blackPlay);

        // add state back on to the list of games
        chessBoards.push_back(chessBoard);
    }

    return chessBoards;
}

void PgnParser::processSquares(pgn::Square from, pgn::Square to) {
    const auto fromCoord = getCoord(from);
    chessBoard.setCell(fromCoord.first, fromCoord.second, Cell::EMPTY);

    const auto toCoord = getCoord(to);
    chessBoard.setCell(toCoord.first, toCoord.second, Cell::B_KING);
}

void PgnParser::processPlay(ChessBoard& chessBoard, pgn::Position& positions, pgn::Ply play) {
    positions.update(play);

    // castling is a special case
    if (play.isShortCastle()) {
        if (positions.sideToMove() == pgn::Color::white) {
            processSquares(pgn::Square('e', '8', pgn::Piece::BlackKing()),
                    pgn::Square('g', '8', pgn::Piece::BlackKing()));

            processSquares(pgn::Square('h', '8', pgn::Piece::BlackRook()),
                    pgn::Square('g', '8', pgn::Square('f', '8', pgn::Piece::BlackRook())));

        } else {
            processSquares(pgn::Square('e', '1', pgn::Piece::WhiteKing()),
                    pgn::Square('g', '1', pgn::Piece::WhiteKing()));

            processSquares(pgn::Square('h', '1', pgn::Piece::WhiteRook()),
                    pgn::Square('f', '1', pgn::Piece::WhiteRook());
        }

    }

    if (play.isLongCastle()) {

    }

    const auto fromCoord = getCoord(play.fromSquare());
    Cell cellContents = chessBoard.getCell(fromCoord.first, fromCoord.second);
    chessBoard.setCell(fromCoord.first, fromCoord.second, Cell::EMPTY);

    const auto toCoord = getCoord(play.toSquare());
    chessBoard.setCell(toCoord.first, toCoord.second, cellContents);
}

std::pair<size_t, size_t> PgnParser::getCoord(const pgn::Square& square) {
    const auto row = square.row();
    const auto col = square.col();

    if (row == '\0') {
        throw std::runtime_error{"no row given"};
    }
    if (col == '\0') {
        throw std::runtime_error{"no col given"};
    }

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

