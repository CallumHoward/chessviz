// FloatingBoard.cpp

#include <string>

#include "cinder/app/App.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"

#include "FloatingBoard.hpp"
#include "ChessBoard.hpp"


namespace chess {

void FloatingBoard::setup() {
    // load glow
    {
        const auto asset = loadAsset(mGlowFilename + ".png");
        const auto image = loadImage(asset);
        mGlow = gl::Texture::create(image);
    }

    // load tiles
    for (int i = 0; i < mNumTiles; ++i) {
        mTileFilenames.emplace_back("tile_" + std::to_string(i) + ".png");
    }

    for (const auto& filename : mTileFilenames) {
        const auto asset = loadAsset(filename);
        const auto image = loadImage(asset);
        mTiles.emplace_back(gl::Texture::create(image));
    }

    // load glyphs
    for (const auto& filename : mGlyphFilenames) {
        const auto asset = loadAsset(filename + ".png");
        const auto image = loadImage(asset);
        mGlyphs.emplace_back(gl::Texture::create(image));
    }

    // set targets
    for (int r = 0; r < mNumRows; ++r) {
        for (int c = 0; c < mNumCols; ++c) {
            mTargets.emplace_back(mLeftMargin + (c * mColOffset),
                                  mTopMargin + ((mNumRows - r - 1) * mRowOffset),
                                  mLeftMargin + (c * mColOffset) + mWidth,
                                  mTopMargin + ((mNumRows - r - 1) * mRowOffset) + mHeight);
        }
    }

    // assign a tile type to each cell
    for (int i = 0; i < mTargets.size(); ++i) {
        mTileMappings.emplace_back(randInt(0, static_cast<int>(mTiles.size())));
    }

}

void FloatingBoard::update(const ChessBoard& chessBoard) {
    mCurrentBoard = chessBoard;
}

void FloatingBoard::draw() {
    gl::ScopedModelMatrix modelMatrix;
    gl::translate((getWindowWidth() - (mWidth * mNumCols) - (2 * mLeftMargin)) / 2,
            (getWindowHeight() - (mHeight * mNumRows) - (2 * mTopMargin)) / 2);

    // draw tiles
    for (int i = 0; i < mTargets.size(); ++i) {
        gl::draw(mTiles.at(mTileMappings.at(i)), mTargets.at(i));
    }

    // draw pieces
    for (int i = 0; i < mTargets.size(); ++i) {
        gl::ScopedModelMatrix targetsModelMatrix;
        gl::translate(mTargets.at(i).getCenter());

        // convert counter into coord
        const auto row = i % mNumRows;
        const auto col = i / mNumRows;
        const auto currentCell = mCurrentBoard.getCell(row, col);

        // colors
        const auto whiteColor = ColorA{0.4f, 0.1f, 0.1f, 0.5f};
        const auto blackColor = ColorA{0.1f, 0.1f, 0.4f, 0.5f};
        const auto whiteHighlight = ColorA{0.6f, 0.3f, 0.3f, 0.8f};
        const auto blackHighlight = ColorA{0.3f, 0.3f, 0.6f, 0.8f};
        const auto whiteGlyph = ColorA{0.6f, 0.3f, 0.3f, 0.8f};
        const auto blackGlyph = ColorA{0.3f, 0.3f, 0.6f, 0.8f};

        // draw piece
        if (currentCell != Cell::EMPTY) {

            // draw glow
            //const auto dim = 55;
            const auto dim = 65;
            const auto glowTarget = Rectf{-dim, -dim, dim, dim};
            gl::ScopedColor scopedGlowColor{
                //1.0f, 1.0f, 1.0f, 1.0f};
                ChessBoard::isWhiteCell(currentCell) ? whiteHighlight : blackHighlight};
            gl::draw(mGlow, glowTarget);

            gl::ScopedColor scopedColor{
                ChessBoard::isWhiteCell(currentCell) ? whiteColor : blackColor};
            gl::drawSolidCircle(vec2{}, 30.0f);

            gl::ScopedColor scopedHighlight{
                ChessBoard::isWhiteCell(currentCell) ? whiteHighlight : blackHighlight};
            gl::drawStrokedCircle(vec2{}, 30.0f, 3.0f);
        }

        // draw glyph
        gl::ScopedColor scopedColor{
            ChessBoard::isWhiteCell(currentCell) ? whiteGlyph : blackGlyph};

        const auto dim = 24;
        const auto glyphTarget = Rectf{-dim, -dim, dim, dim};
        switch (currentCell) {
            case EMPTY: break;
            case W_PAWN: gl::draw(mGlyphs.at(3), glyphTarget); break;
            case B_PAWN: gl::draw(mGlyphs.at(3), glyphTarget); break;
            case W_BISHOP: gl::draw(mGlyphs.at(0), glyphTarget); break;
            case W_KING: gl::draw(mGlyphs.at(1), glyphTarget); break;
            case W_KNIGHT: gl::draw(mGlyphs.at(2), glyphTarget); break;
            case W_QUEEN: gl::draw(mGlyphs.at(4), glyphTarget); break;
            case W_ROOK: gl::draw(mGlyphs.at(5), glyphTarget); break;
            case B_BISHOP: gl::draw(mGlyphs.at(0), glyphTarget); break;
            case B_KING: gl::draw(mGlyphs.at(1), glyphTarget); break;
            case B_KNIGHT: gl::draw(mGlyphs.at(2), glyphTarget); break;
            case B_QUEEN: gl::draw(mGlyphs.at(4), glyphTarget); break;
            case B_ROOK: gl::draw(mGlyphs.at(5), glyphTarget); break;
        }
    }
}

} // namespace chess

