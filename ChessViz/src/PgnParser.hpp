// PgnParser.hpp

#ifndef PGNPARSER_HPP
#define PGNPARSER_HPP

#include <string>
#include <vector>

#include <PGNGameCollection.h>
#include <PGNPosition.h>

#include "ChessBoard.hpp"


namespace chess {

class PgnParser {
public:
    void parse(const std::string& pgnFile);
    ChessBoard& getBoardAt(size_t game, size_t move);

private:
    std::vector<ChessBoard> processGame(const pgn::Game& game);
    void processPlay(ChessBoard& chessBoard, pgn::Position& positions, pgn::Ply play);
    std::pair<size_t, size_t> getCoord(const pgn::Square& square);

    std::vector<std::vector<ChessBoard>> mGames;
};

} // namespace chess

#endif // PGNPARSER_HPP
