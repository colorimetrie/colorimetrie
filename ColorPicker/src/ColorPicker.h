//
//  ColorPicker.h
//  ColorPicker
//
//  Created by Éric Renaud-Houde on 2012-12-04.
//
//

#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Color.h"

using namespace ci;

class ColorPicker {
    Rectf mRect; //square
    Vec2f mCoordinates;  // 0.0f - 1.0f
    float mHue; // 0.0f - 1.0f
    static bool kShaderLoaded;
    static gl::GlslProg kColorPickerShader;
    Color mColor;
public:
    ColorPicker() {};
    ColorPicker(Rectf rect);
    void loadShader();
    void setRect(Rectf rect);
    void pickColor(Vec2f mousePos);
    void setHue(float hue);
    Color getColor();
    void draw();
};
