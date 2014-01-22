//
//  GuiManager.cpp
//  ColorimetrieMac
//
//  Created by Éric Renaud-Houde on 2012-09-18.
//
//

#ifndef ColorimetrieMac_GuiManager_cpp
#define ColorimetrieMac_GuiManager_cpp

#include "GuiManager.h"
#include "cinder/CinderMath.h"
#include <string>

GuiManager::GuiManager()
: mGuiState(RULES)
, mLastGuiState(RULES)
{
    
}

void GuiManager::createGuis()
{
    mMenuGui = new ciUICanvas(ci::app::getWindowWidth() - 120, 40, 100, 100);
    mStylesGui = new ciUICanvas(60,50,380,1080);
    mRulesGui = new ciUICanvas(0,0,500,100);
    mEditGui = new ciUICanvas(60,50, 380,1080);
    setGuiState(RULES);
}

void GuiManager::destroyGuis()
{
    delete mMenuGui;
    delete mStylesGui;
    delete mRulesGui;
    delete mEditGui;
}

void GuiManager::update()
{
    mEditGui->update();
    mMenuGui->update();
    mStylesGui->update();
    mRulesGui->update();
    
}

void GuiManager::draw()
{
    mEditGui->draw();
    mMenuGui->draw();
    mStylesGui->draw();
    mRulesGui->draw();
    
    if (mEditGui->isVisible()) {
        mPicker.draw();
        if (mMouseInfo->isDown && mPicker.containsMouse(mMouseInfo->pos)) {
            mColorEventSignal(mPicker.pickColor(mMouseInfo->pos));
        }
    }
}

void GuiManager::keyDown( const ci::app::KeyEvent & event )
{
    int rule = event.getCode()-48;
    if (rule >= 1 && rule <= 8) {
        string id = std::to_string( rule );
        mRulesGui->getWidget(id)->triggerSelf();
    }
}

void hideLabels(ciUICanvas* canvas) {
    
    for(ciUIWidget* w: canvas->getWidgetsOfType(CI_UI_WIDGET_MINIMALSLIDER)) {
        if( w->hasLabel()) {
            ciUIWidgetWithLabel * wl = dynamic_cast<ciUIWidgetWithLabel*>(w);
            wl->getLabelWidget()->setVisible(false);
        }
    }
}

void GuiManager::setupEditGui()
{
    int h = 65;
    mEditGui->setFont("StagSans-Book.ttf");
    mEditGui->setDrawBack(false);
    mEditGui->addWidgetDown(new ciUISpacer(410, 1));
    mEditGui->addWidgetDown(new ciUILabel("Deux coordonnés pour ajouter un point", CI_UI_FONT_MEDIUM));
    //    mMenuGui->addWidgetDown(new ciUILabelButton(false, "à propos", CI_UI_FONT_SMALL), CI_UI_ALIGN_RIGHT);
    mEditGui->addWidgetDown(new ciUILabelButton(h, false, "R", CI_UI_FONT_MEDIUM, h));
    mEditGui->addWidgetEastOf(new ciUILabelButton(h, false, "V", CI_UI_FONT_MEDIUM, h), "R");
    mEditGui->addWidgetEastOf(new ciUILabelButton(h, false, "B", CI_UI_FONT_MEDIUM, h), "V");
    
    mEditGui->addWidgetDown(new ciUILabelButton(h, false, "T", CI_UI_FONT_MEDIUM, h));
    mEditGui->addWidgetEastOf(new ciUILabelButton(h, false, "S", CI_UI_FONT_MEDIUM, h), "T");
    mEditGui->addWidgetEastOf(new ciUILabelButton(h, false, "L", CI_UI_FONT_MEDIUM, h), "S");
    
    mEditGui->addWidgetDown(new ciUILabelButton(h, false, "L*", CI_UI_FONT_MEDIUM, h));
    mEditGui->addWidgetEastOf(new ciUILabelButton(h, false, "a*", CI_UI_FONT_MEDIUM, h), "L*");
    mEditGui->addWidgetEastOf(new ciUILabelButton(h, false, "b*", CI_UI_FONT_MEDIUM, h), "a*");
    
    mEditGui->addWidgetDown(new ciUILabelButton(h, false, "C", CI_UI_FONT_MEDIUM, h));
    mEditGui->addWidgetEastOf(new ciUILabelButton(h, false, "M", CI_UI_FONT_MEDIUM, h), "C");
    mEditGui->addWidgetEastOf(new ciUILabelButton(h, false, "J", CI_UI_FONT_MEDIUM, h), "M");
    mEditGui->addWidgetEastOf(new ciUILabelButton(h, false, "N", CI_UI_FONT_MEDIUM, h), "J");
    
    mEditGui->addWidgetDown(new ciUILabelButton(false, "Effacer et recommencer", CI_UI_FONT_MEDIUM));
    
    mEditGui->addWidgetDown(new ciUISpacer(410, 1));
    mEditGui->addWidgetDown(new ciUILabel("Sélection de couleur", CI_UI_FONT_MEDIUM));
    ciUIRotarySlider* slider = new ciUIRotarySlider(0, 0, 193, 0.0f, 2*M_PI, mPicker.mHue.get(), "huewheel");
    slider->setLabelVisible(false);
    mEditGui->addWidgetDown(slider);
    mEditGui->setTheme(CI_UI_THEME_DEFAULTONBLACK);
    
    mEditGui->autoSizeToFitWidgets();
    mEditGui->registerUIEvents(this, &GuiManager::guiEvent);
}

void GuiManager::setupMenuGui()
{
    mMenuGui->setFont("StagSans-Book.ttf");
    mMenuGui->setDrawBack(false);
//    mMenuGui->addWidgetDown(new ciUILabelButton(false, "à propos", CI_UI_FONT_SMALL), CI_UI_ALIGN_RIGHT);
    mMenuGui->addWidgetDown(new ciUILabelButton(false, "Règles", CI_UI_FONT_LARGE), CI_UI_ALIGN_RIGHT);
    mMenuGui->addWidgetDown(new ciUILabelButton(false, "Styles", CI_UI_FONT_LARGE), CI_UI_ALIGN_RIGHT);
    mMenuGui->addWidgetDown(new ciUILabelButton(false, "Editeur (beta)", CI_UI_FONT_LARGE), CI_UI_ALIGN_RIGHT);
    mMenuGui->autoSizeToFitWidgets();
    mMenuGui->registerUIEvents(this, &GuiManager::guiEvent);
}

void GuiManager::setupStylesGui(float nbShapes,
                                float* animationSpeed,
                                bool* randomSpeed,
                                float* groupScaleFactor,
                                float* expansionFactor,
                                Vec2f* translationOffset,
                                cmi::CRule* styler)
{
    int h = 96/2;
    int w = 365;
    
    mStylesGui->setFont("StagSans-Book.ttf");
    mStylesGui->centerWidgetsHorizontallyOnCanvas();
    
    mStylesGui->addWidgetDown(new ciUISpacer(w, 1));
    
    mStylesGui->addWidgetDown(new ciUILabel("Nombre de formes", CI_UI_FONT_MEDIUM));
    ciUISlider* slider = new ciUISlider(w, h, 1.0, 8.0, nbShapes, "nombre");
    slider->setLabelVisible(false);
    mStylesGui->addWidgetDown(slider);
    
    mStylesGui->addWidgetDown(new ciUILabel("Epaisseur du trait", CI_UI_FONT_MEDIUM));
    slider = new ciUISlider(w, h, w, h, 0.0, 30.0, styler->strokeWidth, "stroke width");
    slider->setLabelVisible(false);
    mStylesGui->addWidgetDown(slider);
    
    mStylesGui->addWidgetDown(new ciUILabel("Vitesse de l'animation", CI_UI_FONT_MEDIUM));
    slider = new ciUISlider(w, h, 0.0, 7.0f, animationSpeed, "durée animation");
    slider->setLabelVisible(false);
    mStylesGui->addWidgetDown(slider);
    
    mStylesGui->addWidgetDown(new ciUILabel("Espacement", CI_UI_FONT_MEDIUM));
    slider = new ciUISlider(w, h, 0.02, 10.0, expansionFactor, "zoom slider");
    slider->setLabelVisible(false);
    mStylesGui->addWidgetDown(slider);
        
    mStylesGui->addWidgetDown(new ciUISpacer(w, 1));
    
    mStylesGui->addWidgetDown(new ciUIToggle(h, h, styler->fill, "remplir", CI_UI_FONT_MEDIUM));
    
    mStylesGui->registerUIEvents(this, &GuiManager::guiEvent);
    mStylesGui->setTheme(CI_UI_THEME_DEFAULTONBLACK);
    mStylesGui->setDrawBack(false);
    hideLabels(mStylesGui);
    
    mStylesGui->autoSizeToFitWidgets();
    
    
    
}

void GuiManager::setupRulesGui()
{
    int h = 80;
    mRulesGui->setFont("StagSans-Book.ttf");
    mRulesGui->addWidgetDown(new ciUILabelButton(h, false, "1", CI_UI_FONT_MEDIUM, h));
    mRulesGui->addWidgetEastOf(new ciUILabelButton(h, false, "2", CI_UI_FONT_MEDIUM, h), "1");
    mRulesGui->addWidgetEastOf(new ciUILabelButton(h, false, "3", CI_UI_FONT_MEDIUM, h), "2");
    mRulesGui->addWidgetEastOf(new ciUILabelButton(h, false, "4", CI_UI_FONT_MEDIUM, h), "3");
    mRulesGui->addWidgetEastOf(new ciUILabelButton(h, false, "5", CI_UI_FONT_MEDIUM, h), "4");
    mRulesGui->addWidgetEastOf(new ciUILabelButton(h, false, "6", CI_UI_FONT_MEDIUM, h), "5");
    mRulesGui->addWidgetEastOf(new ciUILabelButton(h, false, "7", CI_UI_FONT_MEDIUM, h), "6");
    mRulesGui->addWidgetEastOf(new ciUILabelButton(h, false, "8", CI_UI_FONT_MEDIUM, h), "7");
    mRulesGui->registerUIEvents(this, &GuiManager::guiEvent);
    mRulesGui->setTheme(CI_UI_THEME_DEFAULT);
    mRulesGui->setDrawBack(false);
    mRulesGui->autoSizeToFitWidgets();
    
    //TODO: How do center functions works? Should use them instead.
    h = mRulesGui->getRect()->getHeight();
    float w = mRulesGui->getRect()->getWidth();
    mRulesGui->getRect()->setY((getWindowHeight()-h)/2);
    mRulesGui->getRect()->setX((getWindowWidth()-w)/2);
    hideLabels(mRulesGui);
}

void GuiManager::addRulesButtonBelow(int i)
{
    int h = 80;
    std::string old = std::to_string(i);
    std::string name = "new";
    
    for (ciUIWidget* b: mRulesGui->getWidgetsOfType(CI_UI_WIDGET_LABELBUTTON)) {
        if( b->getName().length() > 2) {
            mRulesGui->removeWidget(b);
            break;
        }
    }
    mRulesGui->addWidgetSouthOf(new ciUILabelButton(h, false, name, CI_UI_FONT_SMALL, h), old);
}

void GuiManager::setOpacity(float * opacity)
{
//    std::cout << "Updating opacity: " << *opacity << std::endl;
    mRulesGui->setOpacity(*opacity);
    mStylesGui->setOpacity(*opacity);
//    mPicker.setOpacity(*opacity);
}

void GuiManager::setupColorPicker(std::shared_ptr<MouseInfo> mouseInfoRef)
{
    mMouseInfo = mouseInfoRef;
    
//    int margin = 100;
    int w = 200;
    int h = 200;
    
//    int x = ci::app::getWindowWidth() - w - margin;
//    int y = ci::app::getWindowHeight() - h - margin;
    int x = 280;
    int y = 500;
    
    mPicker = ColorPicker(ci::Rectf(x, y, x+w, y+h));
    mPicker.loadShader();
}

void GuiManager::toggleGuiState(GuiState nextState)
{
//    setGuiState((mGuiState == nextState) ? MENU : nextState);
    setGuiState(nextState);
}

void GuiManager::setGuiState(GuiState state)
{
//    state = (mGuiState == state) ? mLastGuiState : state;
//    mLastGuiState = mGuiState;
    mGuiState = state;
    mMenuGui->setVisible(false);
    mStylesGui->setVisible(false);
    mRulesGui->setVisible(false);
    mEditGui->setVisible(false);
    switch (state) {
        case STYLES:
            mMenuGui->setVisible(true);
            mStylesGui->setVisible(true);
            break;
        case RULES:
            mMenuGui->setVisible(true);
            mRulesGui->setVisible(true);
            break;
        case EDIT:
            mMenuGui->setVisible(true);
            mEditGui->setVisible(true);
        default:
            mMenuGui->setVisible(true);
            break;
    }
}

GuiManager::GuiState GuiManager::getState()
{
    return mGuiState;
}

void GuiManager::guiEvent(ciUIEvent *event)
{
    mGuiEventSignal(event);
}

#endif
