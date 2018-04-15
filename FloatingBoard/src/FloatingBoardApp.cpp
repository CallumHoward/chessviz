#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "FloatingBoard.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

class FloatingBoardApp : public App {
public:
    void setup() override;
    void mouseDown(MouseEvent event) override;
    void update() override;
    void draw() override;

private:
    chess::FloatingBoard mFloatingBoard;
};

void FloatingBoardApp::setup() {
    mFloatingBoard.setup();
}

void FloatingBoardApp::mouseDown(MouseEvent event) {}

void FloatingBoardApp::update() {}

void FloatingBoardApp::draw() {
    gl::clear(Color{1, 1, 1});

    mFloatingBoard.draw();
}

CINDER_APP(FloatingBoardApp, RendererGl)
