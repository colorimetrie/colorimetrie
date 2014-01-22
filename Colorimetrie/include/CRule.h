//
//  CRule.h
//  ColorimetrieMac
//
//  Created by Éric Renaud-Houde on 2013-02-11.
//
//

#pragma once

#include "cinder/Color.h"
#include "Cairo.h"
#include "CColor.h"

namespace colorimetrie {
    
class CShape;
    
class CRule {
private:
public:
    enum CurveType { STRAIGHT, CATMULLROM, BEZIER, BSPLINE };
    // path type
    CurveType curveType;
    int degree;
    bool openCurve;
    bool loopCurve;
    
    //rendering style
    float strokeWidth;
    bool fill;
    bool showVertices;

    CRule();
    virtual void apply(CShape * shape) const = 0;
    void applyStyle(ci::cairo::Context &ctx);
};
    
class CRule1 : public CRule {
public:
    CRule1();
    virtual void apply(CShape * shape) const override;
};

class CRule2 : public CRule {
public:
    CRule2();
    virtual void apply(CShape * shape) const override;
};

class CRule3 : public CRule {
public:
    CRule3();
    virtual void apply(CShape * shape) const override;
};

class CRule4 : public CRule {
public:
    CRule4();
    virtual void apply(CShape * shape) const override;
};

class CRule5 : public CRule {
public:
    CRule5();
    virtual void apply(CShape * shape) const override;
};

class CRule6 : public CRule {
public:
    CRule6();
    virtual void apply(CShape * shape) const override;
};

class CRule7 : public CRule {
public:
    CRule7();
    virtual void apply(CShape * shape) const override;
};

class CRule8 : public CRule {
public:
    CRule8();
    virtual void apply(CShape * shape) const override;
};

} // namespace colorimetrie
