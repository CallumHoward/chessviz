// FloatingBoard.hpp

#ifndef FLOATINGBOARD_HPP
#define FLOATINGBOARD_HPP

#include <vector>
#include <string>

#include "cinder/gl/gl.h"


namespace chess {

using namespace ci;
using namespace app;

class FloatingBoard {
public:
    void setup();
    void draw();

private:
    std::vector<gl::TextureRef> mTiles;
    std::vector<std::string> mFilenames;
    std::vector<Rectf> mTargets;
    std::vector<size_t> mTileMappings;
};

} // namespace chess

#endif
