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
    
    float mAlpha;
    static bool kShaderLoaded;
    static gl::GlslProg kColorPickerShader;
    Color mColor;
    
    void pick();
    
public:
    ColorPicker() {};
    ColorPicker(Rectf rect);
    std::shared_ptr<float> mHue; // 0.0f - 1.0f
    void loadShader();
    void setRect(Rectf rect);
    bool containsMouse(const Vec2f& mousePos) const;
    Color pickColor(const Vec2f& mousePos);
    Color pickColorLastPosition();
    void setHue(float hue);
    bool hasColor();
    void setOpacity(float opacity);
    void draw();
};
