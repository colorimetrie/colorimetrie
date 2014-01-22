//
//  CEditShape.h
//  ColorimetrieMac
//
//  Created by Éric Renaud-Houde on 2012-12-19.
//
//

#pragma once

#include "CShape.h"
#include <vector>
#include <queue>
using namespace ci;

namespace colorimetrie {
class CEditShape : public CShape {
    
    std::vector<float *> mColorReferences;
    std::queue<float> mColorValueWaitingList;
public:
    CEditShape(const RGBcolor &rgbCol);
    void addColorValue(std::string frenchGuiName);
    void addVertex(float x, float y);
    void updateVertices();
    void updatePath();
    void clear();
    virtual void draw(cairo::Context &ctx) override;
};
}
