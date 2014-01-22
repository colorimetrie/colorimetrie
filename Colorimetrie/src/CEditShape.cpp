//
//  CEditShape.cpp
//  ColorimetrieMac
//
//  Created by Éric Renaud-Houde on 2012-12-19.
//
//

#include "CEditShape.h"
#include "cinder/app/AppBasic.h"
using namespace cmi;

CEditShape::CEditShape(const RGBcolor &rgbCol)
: CShape(rgbCol) {
    
}

void CEditShape::addColorValue(std::string frenchGuiName)
{
    float * valueRef = mColorGroup.getColorComponent(frenchGuiName);
    if( valueRef != nullptr) {
        mColorReferences.push_back(valueRef);
        float c = *valueRef;
        mColorValueWaitingList.push(c);
        if (mColorValueWaitingList.size() == 2 ) {
            float x = mColorValueWaitingList.front();
            mColorValueWaitingList.pop();
            float y = mColorValueWaitingList.front();
            mColorValueWaitingList.pop();
            addVertex(x, y);
        }
    }
}

void CEditShape::clear()
{
    mColorReferences.clear();
    while(!mColorValueWaitingList.empty()) {
        mColorValueWaitingList.pop();
    }
    mVertices.clear();
    if (mPath !=nullptr && !mPath->empty()) {
        mPath->clear();
    }
    if (mInterpolatedPath !=nullptr && !mInterpolatedPath->empty()) {
        mInterpolatedPath->clear();
    }
}

void CEditShape::addVertex(float x, float y)
{
    mVertices.push_back(ci::Vec2f(x, y));
    updatePath();
}

void CEditShape::draw(CGContextRef ctx)
{
//    float scale = 2.5f;
//    float w = ci::app::getWindowWidth();
//    float h = ci::app::getWindowHeight();
//    ctx.save();
//    ctx.translate( app::toPixels(w/2.0f - 0.1*w), app::toPixels(h/2.0f - 0.15*w));
//    ctx.scale(app::toPixels(scale), app::toPixels(scale));
//    if (mInterpolatedPath !=nullptr && !mInterpolatedPath->empty()) {
//        ctx.save();
//        
//        ctx.appendPath(*mInterpolatedPath);
//        ctx.setSource(mInterpolatedColor);
//        ctx.setLineWidth(3.0f);
//        ctx.setLineCap(ci::cairo::LINE_CAP_ROUND);
//        ctx.stroke();
////        drawVertices(ctx);
//        ctx.restore();
//    }
//    ctx.setSource(Color::white());
////    ctx.line(Vec2f(0, -255.0f), Vec2f(0, 255.0f));
////    ctx.line(Vec2f(-10.0f, 0), Vec2f(255.0f, 0));
//    ctx.line(Vec2f(0, -10.0f), Vec2f(0, 255.0f));
//    ctx.line(Vec2f(-10.0f, 0), Vec2f(255.0f, 0));
//    ctx.setLineWidth(0.5f);
//    ctx.stroke();
//    ctx.restore();
}

void CEditShape::updateVertices()
{
    for(int i=0; i<mVertices.size(); ++i) {
        mVertices[i].x = * mColorReferences[2*i];
        mVertices[i].y = * mColorReferences[2*i + 1];
    }
    sortVertices();
}

void CEditShape::updatePath() {
    if (mVertices.size() > 0) {
        computeMidpoint();
        sortVertices();
        generateLines(true);
        mInterpolatedPath = PathRef(new Path2d(*mPath));
        //mPath->transform( MatrixAffine2f::makeRotate(- M_PI_2));
        mPreciseBoundingBox = mPath->calcPreciseBoundingBox();
    }
}