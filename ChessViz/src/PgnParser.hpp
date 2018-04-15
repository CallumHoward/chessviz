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
    void parse(std::string pgnFile);
    ChessBoard& getBoardAt(size_t move);
    std::vector<ChessBoard> processGame(const pgn::Game& game);
    void processPlay(const pgn::Ply& play);
    ChessBoard& getBoardAt(size_t game, size_t move);
    std::pair<int, int> getCoord(const pgn::Square& square);

private:
    std::vector<std::vector<ChessBoard>> mGames;
};

} // namespace chess

#endif // PGNPARSER_HPP
