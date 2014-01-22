//
//  ColorimetrieMacApp.h
//  ColorimetrieMac
//
//  Created by Éric Renaud-Houde on 2012-09-19.
//
//

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Rand.h"
#include "cinder/cairo/Cairo.h"
#include "cinder/Timeline.h"
#include "cinder/Tween.h"
#include "cinder/Easing.h"
#include "cinder/app/TouchEvent.h"
//#include "cinder/qtime/MovieWriter.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <boost/lambda/lambda.hpp>
#include <vector>
#include <string>

#include "ciUI.h"

#include "GuiManager.h"
#include "GroupManager.h"
#include "CEditShape.h"

using namespace ci;
using namespace ci::app;
using namespace cmi;

class ColorimetrieMacApp : public AppNative {
    typedef std::shared_ptr<TouchEvent::Touch> TouchRef;
public:
    //    ColorimetrieMacApp();
    void prepareSettings( Settings *settings );
    void setup();
    void mouseMove( MouseEvent event );
    void mouseDown( MouseEvent event );
    void mouseDrag( MouseEvent event );
    void mouseUp( MouseEvent event );
    void keyDown( KeyEvent event );
    void touchesBegan( TouchEvent event );
    void touchesMoved( TouchEvent event );
    void touchesEnded( TouchEvent event );
    void resize();
    void guiEvent(ciUIEvent *event);
    void setEditShapeColor(Color color);
    void update();
    void draw();
    void shutdown();
    
private:
    void setupGUI();
    void setupRulesMenu();
    void setupMainMenu();
    void triggerZoom();
    void triggerBackgroundTransition();
    void loopAnimation(CShapeGroupRef group);
    void adjustShapes(float maxSize, const Vec2f& offset);
    inline float getTouchesDistance(const vector<TouchEvent::Touch> touches);
    Anim<float> mBackground;
    float mAnimationSpeed;
    int mNbShapes;
    bool mInCloseup;
    
    bool mRandomSpeed;
    
    std::vector<TouchEvent::Touch> mInitTouches;
    float mTouchDistance;
    
    GroupManager mGroupManager;
    Anim<float> mExpansionFactor;
    std::shared_ptr<float> mGroupScaleFactor;
    std::shared_ptr<Vec2f> mGroupOffset;
    Vec2f mOffset;
    Anim<float> mGuiOpacity;
    float mLastMouseMove;
    bool mStartedAnimation;
    
    std::map<std::string, std::function<float (float)> > mEasingMap;
    std::function<float (float)> mEaseFunction;
    
    std::shared_ptr<MouseInfo> mMouseInfo;
    
    GuiManager mGuiManager;
    
    std::unique_ptr<CEditShape> mEditShape;
    
    TimelineRef mTimeline;
    
    bool mRecording;
//    qtime::MovieWriter	mMovieWriter;
};
