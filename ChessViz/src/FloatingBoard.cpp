// FloatingBoard.cpp

#include <string>

#include "cinder/app/App.h"
#include "cinder/Rand.h"

#include "FloatingBoard.hpp"


namespace chess {

void FloatingBoard::setup() {
    for (int i = 0; i < 18; ++i) {
        mFilenames.emplace_back("tile_" + std::to_string(i) + ".png");
    }

    for (const auto& filename : mFilenames) {
        const auto asset = loadAsset(filename);
        const auto image = loadImage(asset);
        mTiles.emplace_back(gl::Texture::create(image));
    }

    const auto width = 50;
    const auto height = 50;
    const auto leftMargin = 50;
    const auto topMargin = 50;
    const auto rowOffset = height;
    const auto colOffset = width;
    const auto numRows = 8;
    const auto numCols = 8;

    for (int r = 0; r < numRows; ++r) {
        for (int c = 0; c < numCols; ++c) {
            mTargets.emplace_back(leftMargin + (c * colOffset),
                    topMargin + ((numRows - r - 1) * rowOffset),
                    leftMargin + (c * colOffset) + width,
                    topMargin + ((numRows - r - 1) * rowOffset) + height);
        }
    }

    // assign a tile type to each cell
    for (int i = 0; i < mTargets.size(); ++i) {
        mTileMappings.emplace_back(randInt(0, static_cast<int>(mTiles.size())));
    }

}

void FloatingBoard::draw() {
    for (int i = 0; i < mTargets.size(); ++i) {
        gl::draw(mTiles.at(mTileMappings.at(i)), mTargets.at(i));
    }
}

} // namespace chess

