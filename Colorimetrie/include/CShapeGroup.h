//
//  CShapeGroup.h
//  ColorimetrieMac
//
//  Created by Éric Renaud-Houde on 12-07-08.
//  Copyright (c) 2012 num3ric.com. All rights reserved.
//

#pragma once
#ifndef ColorimetrieMac_CShapeGroup_h
#define ColorimetrieMac_CShapeGroup_h

#include "CShape.h"
#include "cinder/Vector.h"
#include <vector>
#include "cinder/Timeline.h"
#include "cinder/Tween.h"
#include "cinder/Easing.h"
#include "cinder/app/AppNative.h"

namespace colorimetrie {

class CShapeGroup {
private:
    ci::Vec2f mPosition;
    std::shared_ptr<float> mScaleRef;
    std::shared_ptr<Vec2f> mOffsetRef;
    std::vector<CShapeRef> mShapes;
	std::vector<CShapeRef> mNextShapes;
    ci::Anim<float> * mExpansionFactorRef;
	void addRandomShapesTo(std::vector<CShapeRef>& shapes, int nb);
public:
    CShapeGroup();
	Anim<float>  mInterpolation;
	bool IsAnimating;
    void addShape(const RGBcolor &rgbCol);
    void addRandomShapes(int nb);
	void addRandomNextShapes(int nb);
    void adjustNumberShapes(int nb);
    void addTestShape();
    void applyRule();
    float getMaxSize();
    ci::Vec2f getAverageCenter();
    void resetRandomBaseColor();
	void resetNextRandomBaseColor();
    void sortBySize();
    void setGroupPosition(const Vec2f& position);
    void setPositioningReferences(ci::Anim<float> * expansionFactorRef,
                                  std::shared_ptr<float> scaleRef,
                                  std::shared_ptr<Vec2f> offsetRef);
    std::vector<CShapeRef>& getShapes();
	void initAnimation();
	void animate();
	void finishAnimation();
    void draw(CGContextRef ctx);
    
};

typedef std::shared_ptr<CShapeGroup> CShapeGroupRef;
typedef std::unique_ptr<CShapeGroup> CShapeGroupUniqueRef;
    
} // namespace colorimetrie

#endif
