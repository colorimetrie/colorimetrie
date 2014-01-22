//
//  CShape.cpp
//  ColorimetrieMac
//
//  Created by Éric Renaud-Houde on 12-06-12.
//  Copyright (c) 2012 num3ric.com. All rights reserved.
//

#include "CShape.h"
#include "cinder/gl/gl.h"
#include "cinder/CinderMath.h"
#include "cinder/BSpline.h"
#include "cinder/Color.h"
#include <algorithm>

#include <list>

using namespace ci;
using namespace cmi;

CShape::CShape(const RGBcolor &rgbCol) :
  mColorGroup(rgbCol)
, mPreciseBoundingBox(Rectf())
, mMidpoint(Vec2f())
, mScale(1.0f)
{
	mColor = mColorGroup.getDisplayColor();
	mInterpolatedColor = mColor;
}


void CShape::resetBaseColor(const RGBcolor& rbgCol)
{
    mColorGroup.setGroupColors(rbgCol);
	mColor = mColorGroup.getDisplayColor();
	mInterpolatedColor = mColor;
}

void CShape::generatePath()
{
	int degree = (mVertices.size() == 3) ? kRule->degree -1 : kRule->degree;
	
	if (kRule->curveType == CRule::STRAIGHT) {
		generateLines(kRule->loopCurve);
	} else if (kRule->curveType == CRule::CATMULLROM) {
		generateCatmullRomSpline(degree);
	} else if (kRule->curveType == CRule::BEZIER) {
		generateBezier(degree);
	}  else if (kRule->curveType == CRule::BSPLINE) {
		generateBSpline(degree);
	}
	mInterpolatedPath = PathRef(new Path2d(*mPath)); 
    mPreciseBoundingBox = mPath->calcPreciseBoundingBox();
}

void CShape::generateBezier(int degree)
{
    mPath = PathRef(new Path2d());
    if (degree == 2) {
        Vec2f firstControl = Vec2f( 0.5f*(mVertices[0].x + mVertices[1].x), 0.5f*(mVertices[0].y + mVertices[1].y) );
        Vec2f secondControl = Vec2f( 0.5f*(mVertices[1].x + mVertices[2].x), 0.5f*(mVertices[1].y + mVertices[2].y) );
        mPath->moveTo(mVertices[0]);
        mPath->curveTo(firstControl, secondControl, mVertices[2]);
    } else {
        // TODO
    }
}

void CShape::generateBSpline(int degree) {
    mPath = PathRef(new Path2d(BSpline2f( mVertices, degree, kRule->loopCurve, kRule->openCurve)));
}

void CShape::generateCatmullRomSpline(int degree) {
    
    std::vector<ci::Vec2f> catmullCurvePoints;
    catmullCurvePoints.push_back(mVertices[0]);
    for(std::vector<int>::size_type i = 0; i < mVertices.size()-1; ++i) {
        if (i > 0) {
            Vec2f p0 = mVertices[i-1];
            Vec2f p1 = mVertices[i];
            Vec2f p2 = mVertices[(i+1) % mVertices.size()];
            Vec2f p3 = mVertices[(i+2) % mVertices.size()];
            Vec2f b1 = p1 + (p2 - p0) / 6.0f;
            Vec2f b2 = p2 - (p3 - p1) / 6.0f;
            catmullCurvePoints.push_back(b1);
            catmullCurvePoints.push_back(b2);
            catmullCurvePoints.push_back(p2);
        }
    }
    mPath = PathRef(new Path2d(BSpline2f( catmullCurvePoints, degree, true, false )));
}

void CShape::generateLines(bool close)
{
    mPath = PathRef(new Path2d());
    mPath->moveTo(mVertices.front());
    for(int i=1; i<mVertices.size(); ++i) {
        mPath->lineTo(mVertices[i]);
    }
    if (close) {
       mPath->close();
    }
}

void CShape::computeMidpoint()
{
    mMidpoint.set(0,0);
    for(Vec2f vertex: mVertices) {
        mMidpoint += vertex;
    }
    mMidpoint /= mVertices.size();
}

void CShape::sortVertices()
{
    sort(mVertices.begin(), mVertices.end(),
         [=](const Vec2f &v1, const Vec2f &v2) {
             return (  math<float>::atan2(v1.x - mMidpoint.x, v1.y - mMidpoint.y) <
                       math<float>::atan2(v2.x - mMidpoint.x, v2.y - mMidpoint.y) );
         });
}

void CShape::swapToInterpolated()
{
	mPath = PathRef(new Path2d(*mInterpolatedPath));
	mColor = mInterpolatedColor;
}

void CShape::animate(const CShape& nextShape, float interpolation)
{
	std::vector<ci::Vec2f>& points = mPath->getPoints();
	const std::vector<ci::Vec2f>& nextPoints = nextShape.getPathPoints();
	for (int i=0; i<points.size(); ++i) {
		Vec2f point = points[i];
		point = (1.0f - interpolation) * point + interpolation * nextPoints[i];
		mInterpolatedPath->setPoint(i, point);
	}
	mInterpolatedColor = (1.0f - interpolation) * mColor + interpolation * nextShape.getDisplayColor();
}

void CShape::draw(cairo::Context &ctx)
{
    ctx.setSource(mInterpolatedColor);
    ctx.appendPath(*mInterpolatedPath);
    kRule->applyStyle(ctx);
}

void CShape::drawVertices(cairo::Context &ctx) {
//    ctx.setSource(ci::Color(0,0,0));    
    for(Vec2f vertex: mVertices) {
        ctx.rectangle(vertex.x-2.0f, vertex.y-2.0f, 4.0f, 4.0f);
        ctx.fill();
    }
}
