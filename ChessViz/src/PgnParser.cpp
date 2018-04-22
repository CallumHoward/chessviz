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

std::vector<ChessBoard> PgnParser::processGame(const pgn::Game& game) const {
    auto chessBoards = std::vector<ChessBoard>{ChessBoard{}};
    chessBoards.back().setup();

    auto positions = pgn::Position{};

    for (const auto& move : game.moves()) {
        // take a copy of the most recent move from most recent game
        auto chessBoard = chessBoards.back();

        // apply white move to the chessBoard state
        const auto whitePlay = move.white();
        processPlay(chessBoard, positions, whitePlay);

        // add state back on to the list of games
        chessBoard.setIsWhite(true);
        chessBoards.push_back(chessBoard);

        // apply black move to the chessBoard state
        const auto blackPlay = move.black();
        if (not blackPlay.valid()) { break; }  // game ends in a white move
        processPlay(chessBoard, positions, blackPlay);

        // add state back on to the list of games
        chessBoard.setIsWhite(false);
        chessBoards.push_back(chessBoard);
    }

    return chessBoards;
}

void PgnParser::processPieceMove(ChessBoard& chessBoard,
        const std::pair<size_t, size_t>& from,
        const std::pair<size_t, size_t>& to) const {
    const auto cellContents = chessBoard.getCell(from.first, from.second);
    chessBoard.setCell(from.first, from.second, Cell::EMPTY);
    chessBoard.setCell(to.first, to.second, cellContents);
}

void PgnParser::processPlay(ChessBoard& chessBoard,
        pgn::Position& positions, pgn::Ply play) const {
    positions.update(play);

    // castling is a special case
    if (play.isShortCastle()) {
        if (positions.sideToMove() == pgn::Color::white) {  // so it was black
            processPieceMove(chessBoard,
                    getCoord(pgn::Square('e', '8', pgn::Piece::BlackKing())),
                    getCoord(pgn::Square('g', '8', pgn::Piece::BlackKing())));
            processPieceMove(chessBoard,
                    getCoord(pgn::Square('h', '8', pgn::Piece::BlackRook())),
                    getCoord(pgn::Square('f', '8', pgn::Piece::BlackRook())));

        } else {
            processPieceMove(chessBoard,
                    getCoord(pgn::Square('g', '1', pgn::Piece::WhiteKing())),
                    getCoord(pgn::Square('g', '1', pgn::Piece::WhiteKing())));
            processPieceMove(chessBoard,
                    getCoord(pgn::Square('h', '1', pgn::Piece::WhiteRook())),
                    getCoord(pgn::Square('f', '1', pgn::Piece::WhiteRook())));
        }

    } else if (play.isLongCastle()) {
        if (positions.sideToMove() == pgn::Color::white) {  // so it was black
            processPieceMove(chessBoard,
                    getCoord(pgn::Square('e', '8', pgn::Piece::BlackKing())),
                    getCoord(pgn::Square('c', '8', pgn::Piece::BlackKing())));
            processPieceMove(chessBoard,
                    getCoord(pgn::Square('a', '8', pgn::Piece::BlackRook())),
                    getCoord(pgn::Square('d', '8', pgn::Piece::BlackRook())));

        } else {
            processPieceMove(chessBoard,
                    getCoord(pgn::Square('e', '1', pgn::Piece::WhiteKing())),
                    getCoord(pgn::Square('c', '1', pgn::Piece::WhiteKing())));
            processPieceMove(chessBoard,
                    getCoord(pgn::Square('a', '1', pgn::Piece::WhiteRook())),
                    getCoord(pgn::Square('d', '1', pgn::Piece::WhiteRook())));
        }

    } else {
        const auto fromCoord = getCoord(play.fromSquare());
        const auto toCoord = getCoord(play.toSquare());
        processPieceMove(chessBoard, fromCoord, toCoord);
    }

    chessBoard.setIsCaptured(play.isCapture());
    chessBoard.setLastPlay(play);
}

std::pair<size_t, size_t> PgnParser::getCoord(const pgn::Square& square) const {
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

const ChessBoard& PgnParser::getBoardAt(size_t game, size_t move) const {
    return mGames.at(game).at(move);
}

} // namespace chess

