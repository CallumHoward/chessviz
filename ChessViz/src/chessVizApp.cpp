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
}

void ChessVisApp::draw() {
    gl::clear(Color{0.1, 0.1, 0.2});

    mFloatingBoard.draw();
}

auto settingsFunc = [](App::Settings *settings) {
#if defined(CINDER_MSW)
    settings->setConsoleWindowEnabled();
#endif
    settings->setMultiTouchEnabled(false);
};

CINDER_APP(ChessVisApp, RendererGl, settingsFunc)
