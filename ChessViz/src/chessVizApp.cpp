// chessVizApp.cpp

#include <string>
#include <vector>
#include <stdexcept>

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "CommsManager.hpp"
#include "PgnParser.hpp"
#include "FloatingBoard.hpp"
#include "Background.hpp"


using namespace ci;
using namespace ci::app;
using namespace std;

class ChessVisApp : public App {
public:
    void setup() override;
    void draw() override;

private:
    ivec2 circlePos = ivec2{200, 200};
    std::string mPgnFilename = "games.pgn";
    std::string mPgnFilePath;

    chess::CommsManager mCommsManager;
    chess::PgnParser mPgnParser;
    chess::FloatingBoard mFloatingBoard;

    ch::Background mBackground;
    gl::TextureRef mImageBackground;
};

void ChessVisApp::setup() {
    mPgnFilePath = getAssetPath(mPgnFilename).string();
    if (mPgnFilePath.empty()) {
        throw std::runtime_error("could not find pgn file: " + mPgnFilename);
    }
    mPgnParser.parse(mPgnFilePath);
    mPgnParser.getBoardAt(0, 0);  //TODO
    mCommsManager.setup();
    mFloatingBoard.setup();

    mBackground.setup(getWindowWidth(), getWindowHeight());

    const auto asset = loadAsset("Background80.png");
    const auto image = loadImage(asset);
    mImageBackground = gl::Texture::create(image);
}

void ChessVisApp::draw() {
    gl::clear(Color{0.1f, 0.1f, 0.2f});

    gl::color(1.0f, 1.0f, 1.0f, 1.0f);
    gl::draw(mImageBackground);

    mBackground.update(vec2{0.0f, 0.0f});
    gl::color(0.4f, 0.4f, 0.7f, 0.6f);
    mBackground.draw();

    mFloatingBoard.draw();
}

auto settingsFunc = [](App::Settings *settings) {
#if defined(CINDER_MSW)
    settings->setConsoleWindowEnabled();
#endif
    settings->setMultiTouchEnabled(false);
    settings->setFullScreen();
};

CINDER_APP(ChessVisApp, RendererGl, settingsFunc)
