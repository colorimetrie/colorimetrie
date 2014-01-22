//
//  CColor.h
//  ColorimetrieMac
//
//  Created by Éric Renaud-Houde on 12-05-28.
//  Copyright (c) 2012 num3ric.com. All rights reserved.
//

#pragma once
#ifndef CCOLOR_H
#define CCOLOR_H

#include <iostream>
#include <vector>
#include "cinder/Rand.h"
#include "cinder/Color.h"

namespace colorimetrie {
    
    typedef	enum {
        CM_RGB, // Red[0 - 255] Green[0 - 255 Blue[0 - 255]
        CM_HSL, 
        CM_HSV,  // Hue[0 - 1.0] Saturation[0 - 1.0] Value[0 - 1.0]
        CM_LAB,
        CM_CMYK,
        CM_XYZ    
    } CColorModel;
    
    class CColor {
    protected:
        CColorModel mCColorModel;
    public:
        float c1, c2, c3, c4;
        CColor();
        CColor(float C1, float C2, float C3, float C4=0);
        CColor(CColorModel cm, float C1, float C2, float C3, float C4=0);
        void set( const CColor &color);
        CColorModel getColorModel();
        void setColorModel(const CColorModel cm);
        std::vector<char> getLabels();
        
        float& operator[]( int n );
        const float& operator[]( int n ) const;
        friend std::ostream& operator<< (std::ostream &out, CColor &col);
    private:
        std::ostream& printToStream(std::ostream &out);
        
    };
     
    class RGBcolor: public CColor
    {
    public:
        float &red, &green, &blue;
        RGBcolor();
        RGBcolor(ci::Color col);
        RGBcolor(float _red, float _green, float _blue);
        RGBcolor& operator=(const RGBcolor& newval);
    };
    
    class HSLcolor: public CColor
    {
    public: 
        float &hue, &saturation, &lightness;
        HSLcolor();
        HSLcolor(float _hue, float _saturation, float _lightness);
        HSLcolor& operator=(const HSLcolor& newval);
    };
    
    class HSVcolor: public CColor
    {
    public: 
        float &hue, &saturation, &value;
        HSVcolor();
        HSVcolor(float _hue, float _saturation, float _value);
        HSVcolor& operator=(const HSVcolor& newval);
    };
    
    class LABcolor: public CColor
    {
    public:
        float &lightness, &aStar, &bStar;
        LABcolor();
        LABcolor(float _lightness, float _aStar, float _bStar);
        LABcolor& operator=(const LABcolor& newval);
    };
    
    class CMYKcolor: public CColor
    {
    public: 
        float &cyan, &magenta, &yellow, &black;
        CMYKcolor();
        CMYKcolor(float _cyan, float _magenta, float _yellow, float _black);
        CMYKcolor& operator=(const CMYKcolor& newval);
    };
    
    class XYZcolor: public CColor
    {
    public:
        float &x, &y, &z;
        XYZcolor();
        XYZcolor(float _x, float _y, float _z);
        XYZcolor& operator=(const XYZcolor& newval);
    };
    
    
    // Utilities
    RGBcolor convertHSVtoRGB( const HSVcolor &col );
    HSVcolor convertRGBtoHSV( const RGBcolor &col );
    XYZcolor convertRGBtoXYZ( const RGBcolor &col);
    HSLcolor convertRGBtoHSL( const RGBcolor &col);
    CMYKcolor convertRGBtoCMYK( const RGBcolor &col);
    LABcolor convertXYZtoLAB( const XYZcolor &col); 
    
    RGBcolor RandomRGBColor();
    
    RGBcolor TestRGBColor();
    
} // namespace colorimetrie

namespace cmi = colorimetrie;

#endif