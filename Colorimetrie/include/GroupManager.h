//
//  GroupManager.h
//  ColorimetrieMac
//
//  Created by Éric Renaud-Houde on 2012-10-20.
//
//

#ifndef __ColorimetrieMac__GroupManager__
#define __ColorimetrieMac__GroupManager__

#include <vector>
#include "CShapeGroup.h"

using namespace cmi;
using namespace ci;

class GroupManager {
    
private:
    std::shared_ptr<float> mGroupScaleFactor;
    std::shared_ptr<Vec2f> mGroupOffset;
    std::vector<CShapeGroupRef> mShapeGroups;
public:
    void init(std::shared_ptr<float> groupScaleFactor,
              std::shared_ptr<Vec2f> groupOffset,
              Anim<float> * expansionFactor);
    void adjustShapes(float maxSize, const Vec2f& offset);
    void triggerReset();
    void restartAnimation(CShapeGroupRef groupRef);
    void triggerRuleChange(int rule);
    void adjustNumberOfShapes(int number);
    void resetRandomColorBase();
    
    void loadAllColors();
    void writeAllColors();
    
    void drawGroups(cairo::Context &ctx);
    std::vector<CShapeGroupRef>& getGroups();
};

#endif /* defined(__ColorimetrieMac__GroupManager__) */
