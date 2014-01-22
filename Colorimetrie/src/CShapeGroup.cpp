//
//  CShapeGroup.cpp
//  ColorimetrieMac
//
//  Created by Éric Renaud-Houde on 12-07-08.
//  Copyright (c) 2012 num3ric.com. All rights reserved.
//

#include "CShapeGroup.h"
#include <algorithm>

using namespace cmi;

CShapeGroup::CShapeGroup() :
  mPosition(ci::Vec2f())
, mOffsetRef(std::shared_ptr<Vec2f>(new Vec2f()))
, mScaleRef(std::shared_ptr<float>(new float(1.0)))
, IsAnimating(false)
, mInterpolation(0.0f)
{    
}

void CShapeGroup::addShape(const RGBcolor &rgbCol)
{
    mShapes.push_back(CShapeRef( new CShape(rgbCol)));
}

void CShapeGroup::addRandomShapes(int nb)
{
    addRandomShapesTo(mShapes, nb);
}

void CShapeGroup::addRandomNextShapes(int nb)
{
	addRandomShapesTo(mNextShapes, nb);
}

void CShapeGroup::addRandomShapesTo(std::vector<CShapeRef>& shapes, int nb)
{
    for(int i=0; i<nb; ++i) {
        CShapeRef shape = CShapeRef( new CShape(RandomRGBColor()));
        CShape::kRule->apply(shape.get());
        shapes.push_back(shape);
    }
	//    sortBySize();
}

void CShapeGroup::adjustNumberShapes(int nb)
{
    if (nb > mShapes.size()) {
        addRandomShapes(nb - mShapes.size());
		if (IsAnimating) {
			addRandomNextShapes(nb - mNextShapes.size());
		}
    } else if ( nb > 0 && nb < mShapes.size()) {
        while (mShapes.size() != nb ) {
            mShapes.pop_back();
        }
		if (IsAnimating) {
			while (mNextShapes.size() != nb ) {
				mNextShapes.pop_back();
			}
		}
    }
}

void CShapeGroup::addTestShape()
{
    mShapes.push_back(CShapeRef( new CShape(TestRGBColor())));
}

void CShapeGroup::applyRule()
{
    for(CShapeRef shape: mShapes) {
        CShape::kRule->apply(shape.get());
    }
    
    if (IsAnimating) {
		for(CShapeRef shape: mNextShapes) {
			CShape::kRule->apply(shape.get());
		}
	}
}

void CShapeGroup::resetRandomBaseColor()
{
    for(CShapeRef shape: mShapes) {
        shape->resetBaseColor(RandomRGBColor());
        CShape::kRule->apply(shape.get());
    }
//    sortBySize();
}

void CShapeGroup::resetNextRandomBaseColor()
{
	if (IsAnimating) {
		for(CShapeRef shape: mNextShapes) {
			shape->resetBaseColor(RandomRGBColor());
            CShape::kRule->apply(shape.get());
		}
	}
}

void CShapeGroup::sortBySize()
{
    sort(mShapes.begin(), mShapes.end(),
         [](const std::shared_ptr<CShape> c1, const std::shared_ptr<CShape> c2) -> bool
         {
             Vec2f size1 = c1->getSize();
             Vec2f size2 = c2->getSize();
             return (size1.x * size1.y > size2.x * size2.y);
         });
}

float CShapeGroup::getMaxSize()
{
    float maxSize = 0;
    for(CShapeRef shape: mShapes) {
        Vec2f shapeSize = shape->getSize();
        if ( shapeSize.x > maxSize) {
            maxSize = shapeSize.x;
        }
        if ( shapeSize.y > maxSize) {
            maxSize = shapeSize.y;
        }
    }
    return maxSize;
}

ci::Vec2f CShapeGroup::getAverageCenter()
{
    ci::Vec2f center = ci::Vec2f();
    for(CShapeRef shape: mShapes) {
        center += shape->getCenter();
    }
    center /= mShapes.size();
    return center;
}

void CShapeGroup::setGroupPosition(const Vec2f& position)
{
    mPosition.set(position);
}

void CShapeGroup::setPositioningReferences(ci::Anim<float> * expansionFactorRef,
                                           std::shared_ptr<float> scaleRef,
                                           std::shared_ptr<Vec2f> offsetRef)
{
    mExpansionFactorRef = expansionFactorRef;
    mScaleRef = scaleRef;
    mOffsetRef = offsetRef;
}

std::vector<CShapeRef>& CShapeGroup::getShapes()
{
    return mShapes;
}

void CShapeGroup::initAnimation()
{	
	IsAnimating = true;
	for(CShapeRef shape: mShapes) {
		shape->swapToInterpolated();
	}
	mInterpolation = 0.0f;
	mNextShapes.clear();
	addRandomNextShapes(mShapes.size());
}

void CShapeGroup::animate()
{
	/* If the animation is interrupted by a new one,
	 animate() still get called one last time after the
	 Anim variable has been reset. To prevent that from
	 resetting our shape we add the mInterpolation != 0
	 condition. */
	if (IsAnimating and mInterpolation != 0) {
		for (int i=0; i<mShapes.size(); ++i) {
			mShapes[i]->animate(*mNextShapes[i], mInterpolation());
		}
	}
}

void CShapeGroup::finishAnimation()
{
	IsAnimating = false;
}

void CShapeGroup::draw(CGContextRef ctx)
{
    Vec2f finalPos = (*mExpansionFactorRef) * mPosition +
                     (1.0f - (*mExpansionFactorRef - 1.0f)/5.0f) * (*mScaleRef)*(*mOffsetRef);
	
	::CGContextSaveGState( ctx );
	::CGContextTranslateCTM( ctx, ci::app::toPixels(finalPos.x), ci::app::toPixels(finalPos.y) );
	::CGContextScaleCTM( ctx, ci::app::toPixels(*mScaleRef), ci::app::toPixels(*mScaleRef) );
    for(CShapeRef shape: mShapes) {
        shape->draw(ctx);
        if (CShape::kRule->showVertices) {
            shape->drawVertices(ctx);
        }
    }
	::CGContextRestoreGState( ctx );
}