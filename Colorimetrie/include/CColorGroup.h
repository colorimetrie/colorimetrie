//
//  CColorGroup.h
//  ColorimetrieMac
//
//  Created by Éric Renaud-Houde on 12-05-30.
//  Copyright (c) 2012 num3ric.com. All rights reserved.
//

#pragma once
#ifndef CCOLORGROUP_H
#define CCOLORGROUP_H

#include "CColor.h"
#include "cinder/Color.h"

namespace colorimetrie {
    
    class CColorGroup {
    private:
        RGBcolor mRgbColor;
        HSLcolor mHslColor;
        HSVcolor mHsvColor;
        LABcolor mLabColor;
        CMYKcolor mCmykColor;
        XYZcolor mXyzColor;
        ci::Color mDisplayColor;
    public:
        CColorGroup(const RGBcolor rgbCol);
        CColorGroup(float red, float green, float blue);
        void setGroupColors(const RGBcolor rgbCol);
        void setGroupColors(float red, float green, float blue);
        
        const RGBcolor& getRgb() const;
        const HSLcolor& getHsl() const;
        const HSVcolor& getHsv() const;
        const LABcolor& getLab() const;
        const CMYKcolor& getCmyk() const;
        const XYZcolor& getXyz() const;
        const ci::Color getDisplayColor() const;
        float * getColorComponent(std::string frenchGuiName) const;
    };
}

#endif