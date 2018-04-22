// chessVizApp.cpp

#include <functional>
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
    void update() override;
    void draw() override;
    void advanceGame();

private:
    chrono::milliseconds mMoveDuration = std::chrono::milliseconds(1000);
    chrono::milliseconds mLastUpdate;

    std::string mPgnFilename = "games.pgn";
    std::string mPgnFilePath;

    chess::CommsManager mCommsManager;
    chess::PgnParser mPgnParser;
    chess::FloatingBoard mFloatingBoard;

    decltype(mPgnParser.cbegin()) mCurrentGame;
    decltype(mCurrentGame->cbegin()) mCurrentMove;

    bool mIsReady = false;

    ch::Background mBackground;
    gl::TextureRef mImageBackground;
};

void ChessVisApp::setup() {
    mPgnFilePath = getAssetPath(mPgnFilename).string();
    if (mPgnFilePath.empty()) {
        throw std::runtime_error("could not find pgn file: " + mPgnFilename);
    }
    mPgnParser.parse(mPgnFilePath);
    mCommsManager.setup(std::bind(&ChessVisApp::advanceGame, this));
    mFloatingBoard.setup();

    mBackground.setup(getWindowWidth(), getWindowHeight());

    const auto asset = loadAsset("Background80.png");
    const auto image = loadImage(asset);
    mImageBackground = gl::Texture::create(image);

    mCurrentGame = mPgnParser.cbegin();
    mCurrentMove = mCurrentGame->cbegin();

    //const auto now = chrono::system_clock::now().time_since_epoch();
    //mLastUpdate = chrono::duration_cast<chrono::milliseconds>(now);
    //mFloatingBoard.update(*mCurrentMove);
}

void ChessVisApp::advanceGame() {
    mIsReady = true;

    // loop through the games forever
    if (mCurrentGame == mPgnParser.cend()) {
        mCurrentGame = mPgnParser.cbegin();
        mCurrentMove = mCurrentGame->cbegin();
    }

    if (mCurrentMove == mCurrentGame->cend()) {
        // proceed to the next game
        ++mCurrentGame;
        if (mCurrentGame != mPgnParser.cend()) {
            mCurrentMove = mCurrentGame->cbegin();
        }

    } else { //if (mCurrentMove != mCurrentGame->cend()) {
        mFloatingBoard.update(*mCurrentMove);
        mCommsManager.updateBoard(*mCurrentMove);

        // proceed to the next move
        ++mCurrentMove;
    }
}

void ChessVisApp::update() {
    //const auto now = chrono::system_clock::now().time_since_epoch();
    //const auto nowMilliseconds = chrono::duration_cast<chrono::milliseconds>(now);

    //// loop through the games forever
    //if (mCurrentGame == mPgnParser.cend()) {
    //    mCurrentGame = mPgnParser.cbegin();
    //    mCurrentMove = mCurrentGame->cbegin();
    //}

    //if (nowMilliseconds - mLastUpdate > mMoveDuration) {

    //    if (mCurrentMove == mCurrentGame->cend()) {
    //        // proceed to the next game
    //        ++mCurrentGame;
    //        if (mCurrentGame != mPgnParser.cend()) {
    //            mCurrentMove = mCurrentGame->cbegin();
    //        }

    //    } else { //if (mCurrentMove != mCurrentGame->cend()) {
    //        mFloatingBoard.update(*mCurrentMove);
    //        mCommsManager.updateBoard(*mCurrentMove);
    //        mLastUpdate = nowMilliseconds;

    //        // proceed to the next move
    //        ++mCurrentMove;
    //    }
    //}
}

void ChessVisApp::draw() {
    gl::clear(Color{0.1f, 0.1f, 0.2f});

    gl::color(1.0f, 1.0f, 1.0f, 1.0f);
    gl::draw(mImageBackground);

    mBackground.update(vec2{0.0f, 0.0f});
    gl::color(0.4f, 0.4f, 0.7f, 0.6f);
    mBackground.draw();

    if (mIsReady) { mFloatingBoard.draw(); }
}

auto settingsFunc = [](App::Settings *settings) {
#if defined(CINDER_MSW)
    settings->setConsoleWindowEnabled();
#endif
    settings->setMultiTouchEnabled(false);
    settings->setWindowSize(1280, 720);
    //settings->setFullScreen();
};

CINDER_APP(ChessVisApp, RendererGl, settingsFunc)
