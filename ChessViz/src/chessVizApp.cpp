// chessVizApp.cpp

#include <string>
#include <vector>

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "CommsManager.hpp"


using namespace ci;
using namespace ci::app;
using namespace std;

class ChessVisApp : public App {
public:
    void setup() override;
    void draw() override;

private:
    ivec2 circlePos = ivec2{200, 200};
    chess::CommsManager mCommsManager;
};

void ChessVisApp::setup() {
    mCommsManager.setup();
}

void ChessVisApp::draw() {
    gl::clear(GL_COLOR_BUFFER_BIT);
    gl::setMatricesWindow(getWindowSize());
    if (mCommsManager.isFilled()) {
        gl::drawStrokedCircle(circlePos, 100);
    } else {
        gl::drawSolidCircle(circlePos, 100);
    }
}

auto settingsFunc = [](App::Settings *settings) {
#if defined(CINDER_MSW)
    settings->setConsoleWindowEnabled();
#endif
    settings->setMultiTouchEnabled(false);
};

CINDER_APP(ChessVisApp, RendererGl, settingsFunc)
