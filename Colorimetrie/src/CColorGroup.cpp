//
//  CColorGroup.cpp
//  ColorimetrieMac
//
//  Created by Éric Renaud-Houde on 12-06-30.
//  Copyright (c) 2012 num3ric.com. All rights reserved.
//

#include <iostream>

#include "CColorGroup.h"

using namespace cmi;

CColorGroup::CColorGroup(const RGBcolor rgbCol) {
    setGroupColors(rgbCol);
}

CColorGroup::CColorGroup(float red, float green, float blue) {
    setGroupColors(red, green, blue);
}

void CColorGroup::setGroupColors(const RGBcolor rgbCol) {
    mRgbColor.set(rgbCol);
    mHsvColor.set(convertRGBtoHSV(mRgbColor));
    mXyzColor.set(convertRGBtoXYZ(mRgbColor));
    mHslColor.set(convertRGBtoHSL(mRgbColor));
    mCmykColor.set(convertRGBtoCMYK(mRgbColor));
    mLabColor.set(convertXYZtoLAB(mXyzColor));
    mDisplayColor = ci::Color(mRgbColor.red/255.0f, mRgbColor.green/255.0f, mRgbColor.blue/255.0f);
}

void CColorGroup::setGroupColors(float red, float green, float blue) {
    setGroupColors(RGBcolor(red, green, blue));
}

const RGBcolor& CColorGroup::getRgb() const {
    return mRgbColor;
}
const HSLcolor& CColorGroup::getHsl() const {
    return mHslColor;
}
const HSVcolor& CColorGroup::getHsv() const {
    return mHsvColor;
}
const LABcolor& CColorGroup::getLab() const {
    return mLabColor;
}
const CMYKcolor& CColorGroup::getCmyk() const {
    return mCmykColor;
}
const XYZcolor& CColorGroup::getXyz() const {
    return mXyzColor;
}

float * CColorGroup::getColorComponent(std::string frenchGuiName) const {
    if (frenchGuiName == "R") {
        return &mRgbColor.red;
    } else if (frenchGuiName == "V") {
        return &mRgbColor.green;
    } else if (frenchGuiName == "B") {
        return &mRgbColor.blue;
    } else if (frenchGuiName == "T") {
        return &mHslColor.hue;
    } else if (frenchGuiName == "S") {
        return &mHslColor.saturation;
    } else if (frenchGuiName == "L") {
        return &mHslColor.lightness;
    } else if (frenchGuiName == "L*") {
        return &mLabColor.lightness;
    } else if (frenchGuiName == "a*") {
        return &mLabColor.aStar;
    } else if (frenchGuiName == "b*") {
        return &mLabColor.bStar;
    } else if (frenchGuiName == "C") {
        return &mCmykColor.cyan;
    } else if (frenchGuiName == "M") {
        return &mCmykColor.magenta;
    } else if (frenchGuiName == "J") {
        return &mCmykColor.yellow;
    } else if (frenchGuiName == "N") {
        return &mCmykColor.black;
    }
    return nullptr;
}

const ci::Color CColorGroup::getDisplayColor() const {
    return mDisplayColor;
}