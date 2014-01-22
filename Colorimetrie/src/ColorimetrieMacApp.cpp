
#include "ColorimetrieMacApp.h"
#include "CRule.h"

std::unique_ptr<CRule> CShape::kRule = std::unique_ptr<CRule>(new CRule4());

void ColorimetrieMacApp::prepareSettings( Settings *settings )
{
//    settings->enableHighDensityDisplay();
//    settings->setWindowSize( 1024, 768 );
//    settings->setFullScreen();
//    settings->setWindowSize(1920, 1080);
//    settings->setWindowSize(1450, 800);
    settings->setWindowSize(1650, 1000);
    settings->setFrameRate(24.0f);
//    settings->enableMultiTouch();
}

void ColorimetrieMacApp::setup()
{
    mEasingMap["EaseNone"] = std::function<float (float)>(EaseNone());
    mEasingMap["EaseInOutQuad"] = std::function<float (float)>(EaseInOutQuad());
    mEasingMap["EaseInOutCubic"] = std::function<float (float)>(EaseInOutCubic());
    mEasingMap["EaseInOutQuart"] = std::function<float (float)>(EaseInOutQuart());
    mEasingMap["EaseInOutQuint"] = std::function<float (float)>(EaseInOutQuint());
    mEasingMap["EaseInOutSine"] = std::function<float (float)>(EaseInOutSine());
    mEasingMap["EaseInOutExpo"] = std::function<float (float)>(EaseInOutExpo());
    mEasingMap["EaseInOutCirc"] = std::function<float (float)>(EaseInOutCirc());
    mEasingMap["EaseInOutAtan"] = std::function<float (float)>(EaseInOutAtan());
    mEasingMap["EaseInOutBack"] = std::function<float (float)>(EaseInOutBack());
    mEasingMap["EaseOutBounce"] = std::function<float (float)>(EaseOutBounce());
    mEaseFunction = std::function<float (float)>(EaseInOutQuad());
    
    mGroupScaleFactor = std::shared_ptr<float>(new float(1.0f));
    mGroupOffset = std::shared_ptr<Vec2f>(new Vec2f(0, 0));
    mOffset = Vec2f(50.0f, 50.0f);
    mTouchDistance = 0.0f;
    mNbShapes = 6;
    mBackground = 1.0f;
    mInCloseup = false;
    mExpansionFactor = 1.0f;
    mAnimationSpeed = 0.8f;
    mRandomSpeed = false;
    mGuiOpacity = 1.0f;
    mLastMouseMove = 100.0f; //large time to make sure gui is visible at first
    mStartedAnimation = false; //TODO: find better way than bool flag
    
    mGroupManager.init(mGroupScaleFactor, mGroupOffset, &mExpansionFactor);
    mTimeline = Timeline::create();
    mMouseInfo = std::shared_ptr<MouseInfo>(new MouseInfo);
    mEditShape = std::unique_ptr<CEditShape>( new CEditShape(RGBcolor(255.0, 0, 0)));
    
    mGuiManager.addGuiCallback(boost::bind(&ColorimetrieMacApp::guiEvent, this, boost::lambda::_1));
    mGuiManager.addColorCallback(boost::bind(&ColorimetrieMacApp::setEditShapeColor, this, boost::lambda::_1));
    
    mGuiManager.createGuis();
    mGuiManager.setupColorPicker(mMouseInfo); //FIXME: for now must precede setup because ref on hue
    mGuiManager.setupEditGui();
    mGuiManager.setupMenuGui();
    //FIXME: this argument list is ridiculous
    mGuiManager.setupStylesGui(mNbShapes,
                               &mAnimationSpeed,
                               &mRandomSpeed,
                               mGroupScaleFactor.get(),
                               mExpansionFactor.ptr(),
                               &mOffset,
                               CShape::kRule.get());
    mGuiManager.setupRulesGui();
    mRecording = false;
    
    for( CShapeGroupRef group: mGroupManager.getGroups()) {
        loopAnimation(group);
    }
    mGroupManager.triggerRuleChange(4);
    
//    fs::path path = getSaveFilePath();
//	if( path.empty() )
//		return; // user cancelled save
//    qtime::MovieWriter::Format format;
//    if( qtime::MovieWriter::getUserCompressionSettings( &format) ) {
//		mMovieWriter = qtime::MovieWriter( path, getWindowWidth(), getWindowHeight(), format );
//	}
}

void ColorimetrieMacApp::mouseMove( MouseEvent event )
{
    mLastMouseMove = (float) ci::app::getElapsedSeconds();
    mStartedAnimation =  false;
    if (mGuiOpacity != 1.0f) {
        mGuiOpacity.stop();
        mGuiOpacity = 1.0f;
        mGuiManager.setOpacity(mGuiOpacity.ptr());
    }
}

void ColorimetrieMacApp::mouseDown( MouseEvent event )
{
    mMouseInfo->isDown = true;
    mLastMouseMove = (float) ci::app::getElapsedSeconds();
    mMouseInfo->pos = event.getPos();
}

void ColorimetrieMacApp::mouseDrag( MouseEvent event )
{
    mLastMouseMove = (float) ci::app::getElapsedSeconds();
    mMouseInfo->pos = event.getPos();
}

void ColorimetrieMacApp::mouseUp( MouseEvent event )
{
    mMouseInfo->isDown = false;
}

void ColorimetrieMacApp::triggerZoom()
{
    if (mInCloseup) {
        timeline().apply( &mExpansionFactor, 1.0f, 1.5f, EaseOutAtan( 10 ) );
    } else {
        timeline().apply( &mExpansionFactor, 5.0f, 1.5f, EaseOutAtan( 10 ) );
    }
}

void ColorimetrieMacApp::triggerBackgroundTransition()
{
    if (mInCloseup) {
        timeline().apply( &mBackground, 1.0f, 1.0f, EaseOutAtan( 10 ) );
    } else {
        timeline().apply( &mBackground, 0.07f, 1.0f, EaseOutAtan( 10 ) );
    }
}


void ColorimetrieMacApp::keyDown( KeyEvent event )
{
    mGuiManager.keyDown(event);
    switch(event.getChar()) {
        case ' ':
            mGroupManager.resetRandomColorBase();
            break;
        case 'r':
            mRecording = !mRecording;
            break;
        case 's':
            mGroupManager.writeAllColors();
            break;
        case 'l':
            mGroupManager.loadAllColors();
            break;
        default: break;
    }
}

inline float ColorimetrieMacApp::getTouchesDistance(const vector<TouchEvent::Touch> touches) {
    return touches[0].getPos().distance(touches[1].getPos());
}

void ColorimetrieMacApp::touchesBegan( TouchEvent event )
{
//    BOOST_FOREACH(TouchEvent::Touch touch, event.getTouches()) {
//        if (touch.getId() == 1 || touch.getId() == 2) {
//            mInitTouches.push_back(touch);
//        }
//    }
//    if (mInitTouches.size() == 2) {
//        mTouchDistance = getTouchesDistance(mInitTouches);
//    }
}

void ColorimetrieMacApp::touchesMoved( TouchEvent event )
{
//    vector<TouchEvent::Touch> currentTouches;
//    BOOST_FOREACH(TouchEvent::Touch touch, event.getTouches()) {
//        if (touch.getId() == 1 || touch.getId() == 2) {
//            currentTouches.push_back(touch);
//        }
//    }
//    if (mInitTouches.size() == 2) {
//        cout << getTouchesDistance(currentTouches) - mTouchDistance << endl;
//    }
}

void ColorimetrieMacApp::touchesEnded( TouchEvent event )
{
//    cout << "ended" << endl;
//    BOOST_FOREACH(TouchEvent::Touch touch, event.getTouches()) {
//        if (touch.getId() == 1 || touch.getId() == 2) {
//            mInitTouches.pop_back();
//        }
//    }
//    mTouchDistance = 0.0f;
}

void ColorimetrieMacApp::loopAnimation(CShapeGroupRef group)
{
//    if (mContinuous) {
        group->mInterpolation = 0.0f;
        float duration = mRandomSpeed ? 70.0f*Rand::randFloat() + 30.0f : 100.0f;
        mTimeline->apply( &(group->mInterpolation), 1.0f, duration, mEaseFunction )
        .startFn( std::bind(&CShapeGroup::initAnimation, group.get() ))
        .updateFn( std::bind(&CShapeGroup::animate, group.get() ))
        .finishFn( std::bind(&CShapeGroup::finishAnimation, group.get()))
        .finishFn(std::bind(&ColorimetrieMacApp::loopAnimation, this, group));
//    }
}

void ColorimetrieMacApp::guiEvent(ciUIEvent* event)
{
    string name = event->widget->getName();
    
    if(name == "zoom") {
        ciUILabelButton *b = (ciUILabelButton *) event->widget;
        if( b->getValue() ) {
            triggerZoom();
            mInCloseup = !mInCloseup;
        }
    } else if (name == "stroke width") {
        ciUISlider *s = (ciUISlider *) event->widget;
        CShape::kRule->strokeWidth = s->getScaledValue();
    } else if (name == "remplir") {
        ciUIToggle *b = (ciUIToggle *) event->widget;
        CShape::kRule->fill = b->getValue();
    }
    else if (name == "1" || name == "2" || name == "3" || name == "4" ||
             name == "5" || name == "6" || name == "7" || name == "8" ) {
        mGroupManager.triggerRuleChange(std::atoi(name.c_str()));
        ((ciUISlider *) mGuiManager.getStylesGui()->getWidget("stroke width"))->setValue(CShape::kRule->strokeWidth);
        ((ciUIToggle *) mGuiManager.getStylesGui()->getWidget("remplir"))->setValue(CShape::kRule->fill);
    } else if (name == "regénérer") {
        ciUILabelButton *b = (ciUILabelButton *) event->widget;
        if( b->getValue() ) {
            mGroupManager.triggerReset();
        }
    } else if (name == "nombre") {
        ciUIRotarySlider *rs = (ciUIRotarySlider *) event->widget;
        mNbShapes = (int) rs->getScaledValue();
        mGroupManager.adjustNumberOfShapes(mNbShapes);
    } else if (name == "Styles") {
        ciUILabelButton *b = (ciUILabelButton *) event->widget;
        if( b->getValue() ) {
            if (!mInCloseup) {
                triggerZoom();
                triggerBackgroundTransition();
                mInCloseup = true;
            }
            mGuiManager.toggleGuiState(GuiManager::STYLES);
        }
    } else if (name == "Règles") {
        ciUILabelButton *b = (ciUILabelButton *) event->widget;
        if( b->getValue() ) {
            if (mInCloseup) {
                triggerZoom();
                triggerBackgroundTransition();
                mInCloseup = false;
            }
            mGuiManager.toggleGuiState(GuiManager::RULES);
//            mGuiManager.addRulesButtonBelow(CShape::kRule->mRuleNumber);
        }
    } else if (name == "Editeur (beta)") {
        ciUILabelButton *b = (ciUILabelButton *) event->widget;
        if( b->getValue() ) {
            if (!mInCloseup) {
                triggerZoom();
                triggerBackgroundTransition();
                mInCloseup = true;
            }
            mGuiManager.toggleGuiState(GuiManager::EDIT);
        }
    } else if (name == "zoom slider") {
        ciUISlider *s = (ciUISlider *) event->widget;
        mExpansionFactor = s->getScaledValue();
    } else if (name == "R" || name == "V" || name == "B" ||
               name == "T" || name == "S" || name == "L" ||
               name == "L*" || name == "a*" || name == "b*" ||
               name == "C" || name == "M" || name == "J" || name == "N") {
        ciUILabelButton *b = (ciUILabelButton *) event->widget;
        if( b->getValue() ) {
            mEditShape->addColorValue(name);
        }
    } else if (name == "Effacer et recommencer") {
        ciUILabelButton *b = (ciUILabelButton *) event->widget;
        if( b->getValue() ) {
            mEditShape->clear();
        }
    }
    
    if(event->widget->getParent() != NULL && event->widget->getParent()->getName() == "Easing Type")
    {
        ciUIRadio *r = (ciUIRadio *) event->widget;
        mEaseFunction = mEasingMap[r->getName()];
    }
}

void ColorimetrieMacApp::resize()
{
}

void ColorimetrieMacApp::setEditShapeColor(Color color)
{
    mEditShape->resetBaseColor(RGBcolor(color));
    mEditShape->updateVertices();
    mEditShape->updatePath();
}

void ColorimetrieMacApp::update()
{
    mTimeline->step(mAnimationSpeed*mAnimationSpeed);
    mGuiManager.update();
    
    if (!mMouseInfo->isDown &&
        float(ci::app::getElapsedSeconds()) - mLastMouseMove > 5.0f &&
        !mStartedAnimation) {
//        mGuiManager.getState() == GuiManager::RULES) {
        mStartedAnimation = true;
        ci::app::timeline().apply(&mGuiOpacity, 0.0f, 2.0f)
        .updateFn( std::bind(&GuiManager::setOpacity, &mGuiManager, mGuiOpacity.ptr() ));
    }
}

void ColorimetrieMacApp::draw()
{
    gl::color(Color::white());
    gl::clear(Color::white());
    
    cairo::SurfaceImage cairoSurface = cairo::SurfaceImage(toPixels(getWindowWidth()), toPixels(getWindowHeight()));
    cairo::Context ctx( cairoSurface );
    ctx.setTolerance(0.2);
    ctx.setSourceRgb( mBackground.value(), mBackground.value(), mBackground.value() );
    ctx.paint();
    
    if (mGuiManager.getState() != GuiManager::EDIT) {
        Vec2f offset = mExpansionFactor * (*mGroupScaleFactor.get()) * 2.0f * (mOffset - Vec2f(50,50));
        ctx.translate(toPixels(getWindowWidth()/2.0f + offset.x), toPixels(getWindowHeight()/2.0f + offset.y));
        ctx.scale(mExpansionFactor, mExpansionFactor);
        
        mGroupManager.drawGroups(ctx);
    } else {
        mEditShape->draw(ctx);
    }
    gl::draw(gl::Texture(cairoSurface.getSurface()), getWindowBounds());

//    if(mMovieWriter && mRecording)
//    {
//		mMovieWriter.addFrame( copyWindowSurface() );
//    }
    
    mGuiManager.draw();
}

void ColorimetrieMacApp::shutdown()
{
    mGuiManager.destroyGuis();
//    if(mMovieWriter)
//        mMovieWriter.finish();
}



CINDER_APP_NATIVE( ColorimetrieMacApp, RendererGl )
