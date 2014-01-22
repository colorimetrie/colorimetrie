//
//  CShape.h
//  ColorimetrieMac
//
//  Created by Éric Renaud-Houde on 12-06-12.
//  Copyright (c) 2012 num3ric.com. All rights reserved.
//

#pragma once
#ifndef CSHAPE_H
#define CSHAPE_H

#include "CColorGroup.h"
#include "cinder/Vector.h"
#include "cinder/Color.h"
#include "cinder/cairo/Cairo.h"
#include "cinder/Path2d.h"

#include "CRule.h"


using namespace ci;

namespace colorimetrie {
	typedef std::shared_ptr<Path2d> PathRef;
	
    class CShape {
    protected:
        PathRef mPath, mInterpolatedPath;
        Rectf mPreciseBoundingBox;
		Color mColor, mInterpolatedColor;
        float mScale;
        Vec2f mMidpoint;
        
        CColorGroup mColorGroup;
        
        std::vector<ci::Vec2f> mVertices;
		std::vector<ci::Vec2f> mPoints;
		std::vector<ci::Vec2f> mNextPoints;
        
        void generateBezier(int degree);
        void generateBSpline(int degree);
        void generateCatmullRomSpline(int degree);
        void generateLines(bool close);
    public:
        static std::unique_ptr<CRule> kRule;
        CShape(const RGBcolor& rgbCol);
        
        void setScale(float scale) { mScale = scale; }
        Vec2f getSize() const { return mPreciseBoundingBox.getSize(); }
        Vec2f getCenter() const { return mPreciseBoundingBox.getCenter(); }
        const RGBcolor& getColor() const { return mColorGroup.getRgb(); }
        const CColorGroup& getColorGroup() const { return mColorGroup; }
		Color getDisplayColor() const { return mColor; }
		Color getInterpolatedDisplayColor() const { return mInterpolatedColor; }
		PathRef getPath() const { return mPath; }
		PathRef	 getInterpolatedPath() const { return mInterpolatedPath; }
        std::vector<Vec2f>& getVertices() { return mVertices; }
		const std::vector<Vec2f>& getPathPoints() const { return mPath->getPoints(); }
        
        void generatePath();
        void computeMidpoint();
        void sortVertices();
        
        void resetBaseColor(const RGBcolor& rbgCol);
		void swapToInterpolated();
		void animate(const CShape& nextShape, float interpolation);
        virtual void draw(cairo::Context &ctx);
        void drawVertices(cairo::Context &ctx);
    };
    
typedef std::shared_ptr<CShape> CShapeRef;
typedef std::unique_ptr<CShape> CShapeUniqueRef;

} // namespace colorimetrie

#endif