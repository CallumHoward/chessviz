// UIButton.hpp
// Callum Howard, 2017

#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "cinder/gl/gl.h"

#include "cinder/Perlin.h"
#include "cinder/params/Params.h"


namespace ch {

using namespace ci;


class Background {

    using Frame = std::vector<uint8_t>;

public:
    void setup(int width, int height);
    void update(vec2 offset = vec2{});
    void draw();

private:
    Frame mBuffer;
    gl::TextureRef mCanvas;
    gl::TextureRef mImageBackground;

    int mWidth;
    int mHeight;
    float mResolution = 16.0f;
    vec2 mOffset;

    int mSeed;
    int mOctaves = 2;
    float mTime = 0.0f;
    float mFrequency = 1.0f / 20.f;
    Perlin mPerlin;
};

void Background::setup(int width, int height) {
    mSeed = clock() & 65535;

    mWidth = width / mResolution;
    mHeight = height / mResolution;

    mBuffer = Frame(mWidth * mHeight * 3);  // rgb for each
    mCanvas = gl::Texture::create(mWidth, mHeight);
    mCanvas->setTopDown();
}

void Background::update(vec2 offset) {
    offset /= (mResolution + 2.0f);

    mPerlin = Perlin(mOctaves, mSeed);

    for (uint32_t row = 0; row < mHeight; ++row) {
        for (uint32_t col = 0; col < mWidth; ++col) {

            float v = (mPerlin.fBm(vec3(col + offset.x, row + offset.y, mTime) *
                    mFrequency) + 1.0f) / 2.0f;
            v *= v * v;
            uint8_t val = v * 255;

            const auto pos = (row * mWidth + col) * 3;
            mBuffer[pos + 0] = val;
            mBuffer[pos + 1] = val;
            mBuffer[pos + 2] = val;
        }
    }

    mTime += 0.04f;
}

void Background::draw() {
    gl::ScopedModelMatrix modelMatrix;
    gl::scale(mResolution, mResolution);

    mCanvas->update(
            mBuffer.data(), GL_RGB, GL_UNSIGNED_BYTE, 0, mWidth, mHeight);
    gl::draw(mCanvas);
}

}  // namespace ch

#endif /* BACKGROUND_HPP */
