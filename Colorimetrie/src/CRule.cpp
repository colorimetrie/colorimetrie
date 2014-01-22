//
//  CRule.cpp
//  ColorimetrieMac
//
//  Created by Éric Renaud-Houde on 2013-02-12.
//
//

#include "CRule.h"
#include "CShape.h"
#include "cinder/Vector.h"

using namespace cmi;

CRule::CRule()
: curveType(CATMULLROM)
, degree(3)
, strokeWidth(10.0f)
, fill(true)
, showVertices(false)
, openCurve(false)
, loopCurve(true)
{
    
}

void CRule::applyStyle(ci::cairo::Context &ctx)
{
    ctx.setLineWidth(strokeWidth);
    ctx.setLineJoin(ci::cairo::LINE_JOIN_MITER);
    ctx.setLineCap(ci::cairo::LINE_CAP_ROUND);
    if (strokeWidth > 0 && fill) {
        ctx.fillPreserve();
        ctx.stroke();
    } else if (strokeWidth > 0) {
        ctx.stroke();
    } else if (fill) {
        ctx.fill();
    }
}

CRule1::CRule1() : CRule()
{
    strokeWidth = 5.0f;
    openCurve   = true;
    loopCurve   = false;
    fill        = false;
    curveType   = STRAIGHT;
}

void CRule1::apply(CShape * shape) const
{
    CMYKcolor cmyk = shape->getColorGroup().getCmyk();
    std::vector<Vec2f>& vertices = shape->getVertices();
    vertices.clear();
    vertices.push_back(ci::Vec2f(cmyk.cyan, cmyk.magenta));
    vertices.push_back(ci::Vec2f(cmyk.yellow, cmyk.black));
    
    shape->computeMidpoint();
    shape->sortVertices();
    shape->generatePath();
}

CRule2::CRule2() : CRule()
{
    strokeWidth = 30.0f;
    openCurve   = false;
    loopCurve   = false;
    fill        = false;
    curveType   = BEZIER;
}

void CRule2::apply(CShape * shape) const
{
    const CColorGroup& cg = shape->getColorGroup();
    std::vector<Vec2f>& vertices = shape->getVertices();
    vertices.clear();
    vertices.push_back(ci::Vec2f(cg.getLab().lightness, cg.getLab().aStar));
    vertices.push_back(ci::Vec2f(cg.getLab().bStar, cg.getCmyk().black));
    vertices.push_back(ci::Vec2f(cg.getHsl().saturation, cg.getHsl().lightness));
    
    shape->computeMidpoint();
    shape->generatePath();
}

CRule3::CRule3() : CRule() { }

void CRule3::apply(CShape * shape) const
{
    const CColorGroup& cg = shape->getColorGroup();
    std::vector<Vec2f>& vertices = shape->getVertices();
    vertices.clear();
    vertices.push_back(ci::Vec2f(cg.getLab().lightness, cg.getLab().aStar));
    vertices.push_back(ci::Vec2f(cg.getLab().bStar, cg.getCmyk().black));
    vertices.push_back(ci::Vec2f(cg.getHsl().saturation, cg.getHsl().lightness));
    vertices.push_back(ci::Vec2f(cg.getCmyk().cyan, cg.getCmyk().magenta));
    
    shape->computeMidpoint();
    shape->sortVertices();
    shape->generatePath();
}

CRule4::CRule4() : CRule() { }

void CRule4::apply(CShape * shape) const
{
    const CColorGroup& cg = shape->getColorGroup();
    std::vector<Vec2f>& vertices = shape->getVertices();
    vertices.clear();
    vertices.push_back(ci::Vec2f(cg.getCmyk().cyan, cg.getCmyk().magenta));
    vertices.push_back(ci::Vec2f(cg.getCmyk().yellow, cg.getCmyk().black));
    vertices.push_back(ci::Vec2f(cg.getLab().lightness, cg.getLab().aStar));
    vertices.push_back(ci::Vec2f(cg.getLab().bStar, cg.getHsl().hue));
    vertices.push_back(ci::Vec2f(cg.getHsl().saturation, cg.getHsl().lightness));
    
    shape->computeMidpoint();
    shape->sortVertices();
    shape->generatePath();
}

CRule5::CRule5() : CRule()
{
    strokeWidth = 0.0f;
    openCurve   = false;
    loopCurve   = true;
    fill        = true;
    curveType   = STRAIGHT;
}

void CRule5::apply(CShape * shape) const
{
    const CColorGroup& cg = shape->getColorGroup();
    std::vector<Vec2f>& vertices = shape->getVertices();
    vertices.clear();
    vertices.push_back(ci::Vec2f(cg.getRgb().red, cg.getRgb().green));
    vertices.push_back(ci::Vec2f(cg.getRgb().blue, cg.getHsl().hue));
    vertices.push_back(ci::Vec2f(cg.getHsl().saturation, cg.getHsl().lightness));
    vertices.push_back(ci::Vec2f(cg.getLab().lightness, cg.getLab().aStar));
    vertices.push_back(ci::Vec2f(cg.getLab().bStar, cg.getCmyk().cyan));
    vertices.push_back(ci::Vec2f(cg.getCmyk().yellow, cg.getCmyk().black));
    
    shape->computeMidpoint();
    shape->sortVertices();
    shape->generatePath();
}

CRule6::CRule6() : CRule()
{
    strokeWidth = 30.0f;
    openCurve   = false;
    loopCurve   = true;
    fill        = true;
    curveType   = CATMULLROM;
}

void CRule6::apply(CShape * shape) const
{
    const CColorGroup& cg = shape->getColorGroup();
    std::vector<Vec2f>& vertices = shape->getVertices();
    vertices.clear();
    vertices.push_back(ci::Vec2f(cg.getCmyk().cyan, 0.0f));
    vertices.push_back(ci::Vec2f(cg.getCmyk().magenta + 50.0f, 0.0f));
    vertices.push_back(ci::Vec2f(25.0f, 15.0f));
    vertices.push_back(ci::Vec2f(75.0f, 15.0f));
    vertices.push_back(ci::Vec2f(cg.getCmyk().yellow, 30.0f));
    vertices.push_back(ci::Vec2f(cg.getCmyk().black + 50.0f, 30.0f));
    
    shape->computeMidpoint();
    shape->sortVertices();
    shape->generatePath();
}

CRule7::CRule7() : CRule()
{
    strokeWidth = 10.0f;
    openCurve   = false;
    loopCurve   = true;
    fill        = false;
    curveType   = CATMULLROM;
}

void CRule7::apply(CShape * shape) const
{
    const CColorGroup& cg = shape->getColorGroup();
    std::vector<Vec2f>& vertices = shape->getVertices();
    vertices.clear();
    vertices.push_back(ci::Vec2f(cg.getCmyk().cyan, cg.getCmyk().magenta));
    vertices.push_back(ci::Vec2f(cg.getCmyk().yellow, cg.getCmyk().black));
    vertices.push_back(ci::Vec2f(cg.getHsl().saturation, cg.getHsl().lightness));
    
    shape->computeMidpoint();
    shape->sortVertices();
    shape->generatePath();
}

CRule8::CRule8() : CRule() {}

void CRule8::apply(CShape * shape) const
{
    const CColorGroup& cg = shape->getColorGroup();
    std::vector<Vec2f>& vertices = shape->getVertices();
    vertices.clear();
    vertices.push_back(ci::Vec2f(cg.getCmyk().cyan - 50.0f, 0.0f));
    vertices.push_back(ci::Vec2f(cg.getCmyk().magenta + 50.0f, 0.0f));
    vertices.push_back(ci::Vec2f(65.0f, 5.0f));
    vertices.push_back(ci::Vec2f(65.0f, 25.0f));
    vertices.push_back(ci::Vec2f(cg.getCmyk().black + 100.0f, 30.0f));
    vertices.push_back(ci::Vec2f(65.0f, 35.0f));
    vertices.push_back(ci::Vec2f(65.0f, 55.0f));
    vertices.push_back(ci::Vec2f(cg.getHsl().saturation + 50.0f, 60.0f));
    vertices.push_back(ci::Vec2f(cg.getHsl().lightness - 50.0f, 60.0f));
    vertices.push_back(ci::Vec2f(15.0f, 55.0f));
    vertices.push_back(ci::Vec2f(15.0f, 35.0f));
    vertices.push_back(ci::Vec2f(cg.getCmyk().yellow - 75.0f, 30.0f));
    vertices.push_back(ci::Vec2f(15.0f, 25.0f));
    vertices.push_back(ci::Vec2f(15.0f, 5.0f));
    
    shape->computeMidpoint();
    shape->generatePath();
}