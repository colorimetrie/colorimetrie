//
//  CColor.h
//  ColorimetrieMac
//
//  Created by Éric Renaud-Houde on 12-05-28.
//  Copyright (c) 2012 num3ric.com. All rights reserved.
//

#include "CColor.h"
#include <boost/foreach.hpp>
#include <assert.h>
#include <cmath>


using namespace cmi;

CColor::CColor()
{
}
        
        
CColor::CColor(float C1, float C2, float C3, float C4)
: c1(C1), c2(C2), c3(C3), c4(C4)
{
    mCColorModel = CM_RGB;
}
        
CColor::CColor(CColorModel cm, float C1, float C2, float C3, float C4)
: mCColorModel(cm), c1(C1), c2(C2), c3(C3), c4(C4)
{
    //pass
}
    

void CColor::set( const CColor &color)
{
    this->c1 = color.c1;
    this->c2 = color.c2;
    this->c3 = color.c3;
    this->c4 = color.c4;
}

CColorModel CColor::getColorModel()
{
    return mCColorModel;
}

void CColor::setColorModel(const CColorModel cm)
{
    mCColorModel = cm;
}

std::vector<char> CColor::getLabels()
{
    switch(mCColorModel) {
        case CM_RGB: return {'R', 'G', 'B'}; break;
        case CM_HSL: return {'H', 'S', 'L'}; break;
        case CM_HSV: return {'H', 'S', 'V'}; break;
        case CM_LAB: return {'L', 'A', 'B'}; break;
        case CM_CMYK: return {'C', 'M', 'Y', 'K'}; break;
        case CM_XYZ: return {'X', 'Y', 'Z'}; break;
    }
}

float& CColor::operator[]( int n )
{
    if(mCColorModel == CM_CMYK) {
        assert( n >= 0 && n <= 3 );
    } else {
        assert( n >= 0 && n <= 2 );
    }
    switch(n) {
        case 0: return c1;
        case 1: return c2;
        case 2: return c3;
        case 3: return c4;
    }
}

const float& CColor::operator[]( int n ) const
{
    if(mCColorModel == CM_CMYK) {
        assert( n >= 0 && n <= 3 );
    } else {
        assert( n >= 0 && n <= 2 );
    }
    switch(n) {
        case 0: return c1;
        case 1: return c2;
        case 2: return c3;
        case 3: return c4;
    }
}

std::ostream& CColor::printToStream(std::ostream &out)
{
    std::vector<char> labels = getLabels();
    if (mCColorModel == CM_CMYK) {
        std::cout << labels[0] << ":" << c1 << " "
        << labels[1] << ":" << c2 << " "
        << labels[2] << ":" << c3 << " "
        << labels[3] << ":" << c4 << std::endl;
    } else {
        std::cout << labels[0] << ":" << c1 << " "
        << labels[1] << ":" << c2 << " "
        << labels[2] << ":" << c3 << std::endl;
    }
    return out;
}

namespace colorimetrie {
    
    std::ostream& operator<< (std::ostream &out, CColor &col)
    {
        return col.printToStream(out);
    };
}

RGBcolor::RGBcolor()
: red(this->c1), green(this->c2), blue(this->c3)
{
}

RGBcolor::RGBcolor(ci::Color col)
: CColor(CM_RGB, 255.0f* col.r, 255.0f*col.g, 255.0f*col.b)
, red(this->c1)
, green(this->c2)
, blue(this->c3)
{

}

RGBcolor::RGBcolor(float _red, float _green, float _blue) 
: CColor(CM_RGB, _red, _green, _blue), red(this->c1),
  green(this->c2), blue(this->c3)
{
}

RGBcolor& RGBcolor::operator=(const RGBcolor& newval)    
{
    return *this;
}



HSLcolor::HSLcolor()
: hue(this->c1), saturation(this->c2), lightness(this->c3)
{
}

HSLcolor::HSLcolor(float _hue, float _saturation, float _lightness)
: CColor(CM_HSL, _hue, _saturation, _lightness),
  hue(this->c1), saturation(this->c2), lightness(this->c3)
{
}

HSLcolor& HSLcolor::operator=(const HSLcolor& newval)
{
    return *this;
}



HSVcolor::HSVcolor()
: hue(this->c1), saturation(this->c2), value(this->c3)
{
}

HSVcolor::HSVcolor(float _hue, float _saturation, float _value)
: CColor(CM_HSL, _hue, _saturation, _value),
  hue(this->c1), saturation(this->c2), value(this->c3)
{
}

HSVcolor& HSVcolor::operator=(const HSVcolor& newval)
{
    return *this;
}


LABcolor::LABcolor()
: lightness(this->c1), aStar(this->c2), bStar(this->c3)
{
}

LABcolor::LABcolor(float _lightness, float _aStar, float _bStar)
: CColor(CM_LAB, _lightness, _aStar, _bStar), lightness(this->c1), aStar(this->c2), bStar(this->c3)
{
}

LABcolor& LABcolor::operator=(const LABcolor& newval)
{
    return *this;
}


CMYKcolor::CMYKcolor()
: cyan(this->c1), magenta(this->c2), yellow(this->c3), black(this->c4)
{
}

CMYKcolor::CMYKcolor(float _cyan, float _magenta, float _yellow, float _black) 
: CColor(CM_CMYK, _cyan, _magenta, _yellow , _black),
  cyan(this->c1), magenta(this->c2), yellow(this->c3), black(this->c4)
{
}

CMYKcolor& CMYKcolor::operator=(const CMYKcolor& newval)
{
    return *this;
}

XYZcolor::XYZcolor()
: x(this->c1), y(this->c2), z(this->c3)
{
}

XYZcolor::XYZcolor(float _x, float _y, float _z)
: CColor(CM_XYZ, _x, _y, _z), x(this->c1), y(this->c2), z(this->c3)
{
}

XYZcolor& XYZcolor::operator=(const XYZcolor& newval)
{
    return *this;
}

// Utilities ---------------------------------------------------
namespace colorimetrie {

RGBcolor RandomRGBColor()
{
    return RGBcolor(255.0f*ci::Rand::randFloat(), 255.0f*ci::Rand::randFloat(), 255.0f*ci::Rand::randFloat());
}
    
RGBcolor TestRGBColor()
{
    return RGBcolor(170, 66, 200);
}

RGBcolor convertHSVtoRGB( const HSVcolor &col )
{
    float hue = col.hue;
    float sat = col.saturation;
    float val = col.value;
    
    float x = 0, y = 0, z = 0;
    
    if( hue == 1 ) hue = 0;
    else
        hue *= 6;
    
    int i = static_cast<int>( std::floor( hue ) );
    float f = hue - i;
    float p = val * ( 1 - sat );
    float q = val* ( 1 - ( sat * f ) );
    float t = val* ( 1 - ( sat * ( 1 - f ) ) );
    
    switch( i ) {
        case 0: x = val; y = t; z = p; break;
        case 1: x = q; y = val; z = p; break;
        case 2: x = p; y = val; z = t; break;
        case 3: x = p; y = q; z = val; break;
        case 4: x = t; y = p; z = val; break;
        case 5: x = val; y = p; z = q; break;
    }
    
    return RGBcolor( x, y, z );
}

HSVcolor convertRGBtoHSV( const RGBcolor &col )
{
    const float &x = col.red;
    const float &y = col.green;
    const float &z = col.blue;
    
    float max = (x > y) ? ((x > z) ? x : z) : ((y > z) ? y : z);
    float min = (x < y) ? ((x < z) ? x : z) : ((y < z) ? y : z);
    float range = max - min;
    float val = max;
    float sat = 0;
    float hue = 0;
    
    if( max != 0 )
        sat = range/max;
    
    if( sat != 0 ) {
        float h;
        
        if( x == max )
            h = (y - z) / range;
        else if( y == max )
            h = 2 + ( z - x ) / range;
        else
            h = 4 + ( x - y ) / range;
        
        hue = h / 6.0;
        
        if( hue < 0 )
            hue += 1;
    }
    
    return HSVcolor( hue, sat, val );
}

//conversion matrix from http://www.brucelindbloom.com/index.html?Eqn_RGB_to_XYZ.html
XYZcolor convertRGBtoXYZ( const RGBcolor &col)
{
    float r = col.red;
    float g = col.green;
    float b = col.blue;
    float x = (0.6326696 * r + 0.2045558 * g + 0.1269946 * b);
    float y = (0.2284569 * r + 0.7373523 * g + 0.0341908 * b);
    float z = (0 * r + 0.0095142 * g + 0.8156958 * b);
    
    return XYZcolor(x/255.0, y/255.0, z/255.0);
}

//conversion http://www.easyrgb.com/index.php?X=MATH&H=18#text18
HSLcolor convertRGBtoHSL( const RGBcolor &col)
{
    float tempR = col.red / 255.0;
    float tempG = col.green / 255.0;
    float tempB = col.blue / 255.0;
    
    float Lvalue = 0;
    float Hvalue = 0;
    float Svalue = 0;    
    
    float maxValue = std::max(std::max(tempR, tempG), tempB);
    float minValue = std::min(std::min(tempR, tempG), tempB);
    float diff = maxValue - minValue;
    
    Lvalue = (maxValue + minValue) / 2.0;
    
    Svalue = diff;
    if(Svalue > 0)
    {
        if(Lvalue < 0.5) Svalue /= (maxValue + minValue);
        else Svalue /= (2 - maxValue - minValue);
    }
    
    float someRed = (maxValue - tempR) / diff;
    float someGreen = (maxValue - tempG) / diff;
    float someBlue = (maxValue - tempB) / diff;
    
    if(tempR == maxValue)
    {
        if(tempG == minValue) Hvalue = 5 + someBlue;
        else Hvalue = 1 - someGreen;
    }
    else if(tempG == maxValue)
    {
        if(tempB == minValue) Hvalue = 1 + someRed;
        else Hvalue = 3 - someBlue;
    }
    else
    {
        if(tempR == minValue) Hvalue = 3 + someGreen;
        else Hvalue = 5 - someRed;
    }
    Hvalue /= 6.0;
    
    return HSLcolor(Hvalue*360, Svalue*100, Lvalue*100);
}

CMYKcolor convertRGBtoCMYK( const RGBcolor &col)
{
    float cyan = 1 - (col.red / 255.0);
    float magenta = 1 - (col.green / 255.0);
    float yellow = 1 - (col.blue / 255.0);
    float black = 1;
    
    if(cyan < black) black = cyan;
    if(magenta < black) black = magenta;
    if(yellow < black) black = yellow;
    
    if(black == 1) //black
    {
        cyan = 0;
        magenta = 0;
        yellow = 0;
    }
    else
    {
        cyan = (cyan - black) / (1 - black);
        magenta = (magenta - black) / (1 - black);
        yellow = (yellow - black) / (1 - black);
    }
    
    return CMYKcolor(cyan*100, magenta*100, yellow*100, black*100);
}

// conversion http://www.brucelindbloom.com/index.html?Eqn_RGB_to_XYZ.html
LABcolor convertXYZtoLAB( const XYZcolor &col)
{
    float tempX = (col.x > 0.008856) ? pow(col.x, 0.333) : (903.3 * col.x + 16) / 116.0;
    float tempY = (col.y > 0.008856) ? pow(col.y, 0.333) : (903.3 * col.y + 16) / 116.0;
    float tempZ = (col.z > 0.008856) ? pow(col.z, 0.333) : (903.3 * col.z + 16) / 116.0;
    
    float Light = (116 * tempY) - 16;
    float aStar = 500 * (tempX - tempY);
    float bStar = 200 * (tempY - tempZ);
    
    return LABcolor(Light, aStar, bStar);
}
    
}

namespace cmi = colorimetrie;