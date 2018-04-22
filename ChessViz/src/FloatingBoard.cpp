// FloatingBoard.cpp

#include <string>

#include "cinder/app/App.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"

#include "FloatingBoard.hpp"
#include "ChessBoard.hpp"


namespace chess {

void FloatingBoard::setup() {
    for (int i = 0; i < mNumTiles; ++i) {
        mFilenames.emplace_back("tile_" + std::to_string(i) + ".png");
    }

    for (const auto& filename : mFilenames) {
        const auto asset = loadAsset(filename);
        const auto image = loadImage(asset);
        mTiles.emplace_back(gl::Texture::create(image));
    }

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
        if (mCurrentBoard.getCell(row, col) != Cell::EMPTY) {
            gl::drawSolidCircle(vec2{}, 30.0f);
        }
    }
}

} // namespace chess

