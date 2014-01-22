//
//  ColorPicker.cpp
//  ColorPicker
//
//  Created by Éric Renaud-Houde on 2012-12-04.
//
//
#include <math.h>
#include "ColorPicker.h"

bool ColorPicker::kShaderLoaded = false;
gl::GlslProg ColorPicker::kColorPickerShader;

ColorPicker::ColorPicker(Rectf rect)
: mRect(rect)
, mHue(0.0f)
, mColor(1.0f, 0, 0)
{

}

void ColorPicker::loadShader()
{
    if (!kShaderLoaded) {
        try {
            kColorPickerShader = gl::GlslProg( ci::app::loadResource("color_picker.vert"), ci::app::loadResource("color_picker.frag"));
        }
        catch( ci::gl::GlslProgCompileExc &exc ) {
            std::cout << "Shader compile error: " << std::endl;
            std::cout << exc.what();
        }
        kShaderLoaded = true;
    }
}

void ColorPicker::setRect(Rectf rect)
{
    mRect = rect;
}

void ColorPicker::pickColor(Vec2f mousePos)
{
    
    if(mRect.contains(mousePos)) {
        mCoordinates = (mousePos - mRect.getUpperLeft())/mRect.getWidth();
        unsigned char pixels[3];
        glReadPixels(mousePos.x,
                     ci::app::getWindowHeight() - mousePos.y,
                     1,
                     1,
                     GL_RGB,
                     GL_UNSIGNED_BYTE,
                     &pixels);
        
        mColor.r = pixels[0];
        mColor.g = pixels[1];
        mColor.b = pixels[2];
//        std::cout << mColor << std::endl;
        gl::drawStrokedEllipse(mousePos, 8.0f, 8.0f);
    }
}

void ColorPicker::setHue(float hue)
{
    mHue = (float) fmod(hue, 1.0);
}

ci::Color ColorPicker::getColor()
{
    return mColor;
}

void ColorPicker::draw()
{
    kColorPickerShader.bind();
    kColorPickerShader.uniform("time", (float) ci::app::getElapsedSeconds());
    gl::drawSolidRect(mRect);
    kColorPickerShader.unbind();
}