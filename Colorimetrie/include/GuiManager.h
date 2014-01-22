//
//  GuiManager.h
//  ColorimetrieMac
//
//  Created by Éric Renaud-Houde on 2012-09-18.
//
//

#ifndef ColorimetrieMac_GuiManager_h
#define ColorimetrieMac_GuiManager_h

#include "cinder/app/AppBasic.h"
#include "cinder/Color.h"
#include "ciUI.h"
#include "CRule.h"
#include "ColorPicker.h"

#include <boost/signals2.hpp>
#include <boost/function.hpp>

struct MouseInfo {
    Vec2f pos;
    bool isDown;
};

class GuiManager {
public:
    enum GuiState { STYLES, RULES, EDIT };
    GuiManager();
    void createGuis();
    void setupEditGui();
    void setupMenuGui();
    void setupStylesGui(float nbShapes,
                        float* animationSpeed,
                        bool* randomSpeed,
                        float* groupScaleFactor,
                        float* expansionFactor,
                        Vec2f* translationOffset,
                        cmi::CRule* styler);
    void setupRulesGui();
    void setOpacity(float * opacity);
    void addRulesButtonBelow(int i);
    void setupColorPicker(std::shared_ptr<MouseInfo> mouseInfoRef);
    void destroyGuis();
    void update();
    void draw();
    void keyDown( const ci::app::KeyEvent & event );
    void guiEvent(ciUIEvent *event);
    GuiState getState();
    
    ciUICanvas* getMenuGui() { return mMenuGui; }
    ciUICanvas* getStylesGui() { return mStylesGui; }
    ciUICanvas* getRulesGui() { return mRulesGui; }
    ciUICanvas* getEditGui() { return mEditGui; }
    
    void toggleGuiState(GuiState nextState);
    void addGuiCallback(boost::function<void (ciUIEvent*)> func) { mGuiEventSignal.connect(func); }
    void addColorCallback(boost::function<void (Color)> func) { mColorEventSignal.connect(func); }
private:
    boost::signals2::signal<void (ciUIEvent*)> mGuiEventSignal;
    boost::signals2::signal<void (Color)> mColorEventSignal;
    std::shared_ptr<MouseInfo> mMouseInfo;
    ColorPicker mPicker;
    
    GuiState mGuiState, mLastGuiState;
    void setGuiState(GuiState state);
    
    ciUICanvas* mMenuGui;
    ciUICanvas* mStylesGui;
    ciUICanvas* mRulesGui;
    ciUICanvas* mEditGui;
};

#endif
