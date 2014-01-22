#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "ColorPicker.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ColorPickerApp : public AppBasic {
  public:
    void prepareSettings( Settings *settings );
	void setup();
	void mouseDown( MouseEvent event );
    void mouseDrag( MouseEvent event );
    void mouseUp( MouseEvent event );
	void update();
	void draw();
    
    ColorPicker mPicker;
    bool mMouseDown;
    Vec2i mMousePos;
};

void ColorPickerApp::prepareSettings( Settings *settings )
{
    //    settings->setWindowSize( 1024, 768 );
    //    settings->setFullScreen();
    //    settings->setWindowSize(1920, 1080);
    settings->setWindowSize(900, 900);
    settings->setFrameRate(24.0f);
    mMouseDown = false;
    //    settings->enableMultiTouch();
    
    mPicker = ColorPicker(Rectf(100.f, 100.f, 400.f, 400.f));
}

void ColorPickerApp::setup()
{
    mPicker.loadShader();
}


void ColorPickerApp::mouseDown( MouseEvent event )
{
    mMousePos = event.getPos();
    mMouseDown = true;
}

void ColorPickerApp::mouseDrag( MouseEvent event )
{
    mMousePos = event.getPos();
}

void ColorPickerApp::mouseUp( MouseEvent event)
{
    mMouseDown = false;
}

void ColorPickerApp::update()
{
}

void ColorPickerApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
    mPicker.draw();
    
    if(mMouseDown) {
        mPicker.pickColor(mMousePos);
        console() << mPicker.getColor();
    }
}


CINDER_APP_BASIC( ColorPickerApp, RendererGl )
