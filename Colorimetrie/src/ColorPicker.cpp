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
, mHue(new float(0.0f))
, mAlpha(1.0f)
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

bool ColorPicker::containsMouse(const Vec2f& mousePos) const
{
    return mRect.contains(mousePos);
}

void ColorPicker::pick()
{
    unsigned char pixels[3];
    glReadPixels(app::toPixels(mCoordinates.x),
                 app::toPixels(app::getWindowHeight() - mCoordinates.y),
                 1,
                 1,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 &pixels);
    
    mColor.r = pixels[0]/255.0f;
    mColor.g = pixels[1]/255.0f;
    mColor.b = pixels[2]/255.0f;
    gl::drawStrokedEllipse(mCoordinates, 8.0f, 8.0f);
}

Color ColorPicker::pickColor(const Vec2f& mousePos)
{
    mCoordinates = Vec2f(mousePos.x, mousePos.y);
    pick();
    return mColor;
}

Color ColorPicker::pickColorLastPosition()
{
    pick();
    return mColor;
}

void ColorPicker::setHue(float hue)
{
    *mHue = (float) fmod(hue, 1.0);
}

void ColorPicker::setOpacity(float opacity) {
    mAlpha = opacity;
}

void ColorPicker::draw()
{
    kColorPickerShader.bind();
    kColorPickerShader.uniform("time", *mHue);
    kColorPickerShader.uniform("alpha", mAlpha);
    gl::drawSolidRect(mRect);
    kColorPickerShader.unbind();
}