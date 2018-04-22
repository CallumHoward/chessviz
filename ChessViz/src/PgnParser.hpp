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
    const ChessBoard& getBoardAt(size_t game, size_t move) const;

private:
    std::vector<ChessBoard> processGame(const pgn::Game& game) const;
    void processPlay(ChessBoard& chessBoard, pgn::Position& positions, pgn::Ply play) const;
    std::pair<size_t, size_t> getCoord(const pgn::Square& square) const;
    void processPieceMove(ChessBoard& chessBoard,
            const std::pair<size_t, size_t>& from, const std::pair<size_t, size_t>& to) const;

    std::vector<std::vector<ChessBoard>> mGames;

public:
    // iterator adapter
    decltype(mGames)::const_iterator cbegin() const { return mGames.cbegin(); }
    decltype(mGames)::const_iterator cend() const { return mGames.cend(); }
};

} // namespace chess

#endif // PGNPARSER_HPP
