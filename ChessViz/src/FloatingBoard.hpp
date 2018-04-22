// FloatingBoard.hpp

#ifndef FLOATINGBOARD_HPP
#define FLOATINGBOARD_HPP

#include <vector>
#include <string>

#include "cinder/gl/gl.h"

#include "ChessBoard.hpp"


namespace chess {

using namespace ci;
using namespace app;

class FloatingBoard {
public:
    void setup();
    void update(const ChessBoard& chessBoard);
    void draw();

private:
    const int mWidth = 100;
    const int mHeight = 100;
    const int mLeftMargin = 50;
    const int mTopMargin = 50;
    const int mRowOffset = mHeight;
    const int mColOffset = mWidth;
    const int mNumRows = 8;
    const int mNumCols = 8;
    const int mNumTiles = 18;

    gl::TextureRef mGlow;
    std::string mGlowFilename = "particle";
    std::vector<gl::TextureRef> mTiles;
    std::vector<gl::TextureRef> mGlyphs;
    std::vector<std::string> mTileFilenames;
    std::vector<std::string> mGlyphFilenames = {
        "Bishop", "King", "Knight", "Pawn", "Queen", "Rook"};
    std::vector<Rectf> mTargets;
    std::vector<size_t> mTileMappings;

    ChessBoard mCurrentBoard;
};

} // namespace chess

#endif
