//
//  GroupManager.cpp
//  ColorimetrieMac
//
//  Created by Éric Renaud-Houde on 2012-10-20.
//
//

#include "GroupManager.h"
#include "cinder/app/AppBasic.h"
#include "cinder/Xml.h"

void GroupManager::init(std::shared_ptr<float> groupScaleFactor,
                        std::shared_ptr<Vec2f> groupOffset,
                        Anim<float> * expansionFactor)
{
    mGroupScaleFactor = groupScaleFactor;
    mGroupOffset = groupOffset;
    int halfWidth = app::getWindowWidth()/2;
    int halfHeight = app::getWindowHeight()/2;
    int stepX = app::getWindowWidth()/6.0f;
    int stepY = app::getWindowHeight()/4.0f;
    
    float maxSize = 0.0f;
    Vec2f offset = Vec2f();
    for(int x=stepX; x < app::getWindowWidth()-5; x += stepX) {
        for(int y=stepY; y < app::getWindowHeight()-5; y += stepY) {
            CShapeGroupRef group = CShapeGroupRef(new CShapeGroup());
            group->addRandomShapes(8);
            group->setGroupPosition(Vec2f(x - halfWidth - 5 , y - halfHeight - 5));
            group->setPositioningReferences(expansionFactor, mGroupScaleFactor, mGroupOffset);
            float groupSize = group->getMaxSize();
            if (groupSize > maxSize) {
                maxSize = groupSize;
            }
            offset -= group->getAverageCenter();
            mShapeGroups.push_back(group);
        }
    }
    offset /= mShapeGroups.size();
    adjustShapes(maxSize, offset);
}

void GroupManager::adjustShapes(float maxSize, const Vec2f& offset)
{
    // rescale and reposition
    *mGroupScaleFactor = math<float>::pow(150.0f/maxSize, 0.75);
    mGroupOffset->set(offset);
}

void GroupManager::adjustNumberOfShapes(int number)
{
    for(CShapeGroupRef shapeGroup: mShapeGroups) {
        shapeGroup->adjustNumberShapes(number);
    }
}

void GroupManager::triggerReset()
{
    for(CShapeGroupRef group: mShapeGroups) {
        group->resetRandomBaseColor();
    }
}

void GroupManager::triggerRuleChange(int rule)
{
    switch (rule) {
        case 1: CShape::kRule = std::unique_ptr<CRule>(new CRule1); break;
        case 2: CShape::kRule = std::unique_ptr<CRule>(new CRule2); break;
        case 3: CShape::kRule = std::unique_ptr<CRule>(new CRule3); break;
        case 4: CShape::kRule = std::unique_ptr<CRule>(new CRule4); break;
        case 5: CShape::kRule = std::unique_ptr<CRule>(new CRule5); break;
        case 6: CShape::kRule = std::unique_ptr<CRule>(new CRule6); break;
        case 7: CShape::kRule = std::unique_ptr<CRule>(new CRule7); break;
        case 8: CShape::kRule = std::unique_ptr<CRule>(new CRule8); break;
        default: break;
    }
    
    float maxSize = 0.0f;
    Vec2f offset = Vec2f();
    for(CShapeGroupRef group: mShapeGroups) {
        group->applyRule();
        float groupSize = group->getMaxSize();
        if (groupSize > maxSize) {
            maxSize = groupSize;
        }
        offset -= group->getAverageCenter();
    }
    offset /= mShapeGroups.size();
    adjustShapes(maxSize, offset);
}

void GroupManager::resetRandomColorBase()
{
    for(CShapeGroupRef group: mShapeGroups) {
        group->resetRandomBaseColor();
    }
}


void GroupManager::loadAllColors()
{
    fs::path path = app::getOpenFilePath();
    if( path.empty() )
        return;
    
    XmlTree doc( loadFile( path ) );
    XmlTree xmlCollection = doc.getChild( "collection" );
    for( XmlTree::Iter group = xmlCollection.begin(); group != xmlCollection.end(); ++group )
    {
        int g_id = group->getAttributeValue<int>("id");
        for( XmlTree::Iter color = group->begin(); color != group->end(); ++color )
        {
            int c_id = color->getAttributeValue<int>("id");
            float r = color->getAttributeValue<float>("r");
            float g = color->getAttributeValue<float>("g");
            float b = color->getAttributeValue<float>("b");
            cmi::RGBcolor rgbCol(r, g, b);
            mShapeGroups[g_id]->getShapes()[c_id]->resetBaseColor(rgbCol);
            CShape::kRule->apply(mShapeGroups[g_id]->getShapes()[c_id].get());
        }
    }
}

void GroupManager::writeAllColors()
{
    fs::path path = app::getSaveFilePath();
    if( path.empty() )
        return;
    
    XmlTree xmlCollection( "collection", "" );    
    int g_id = 0;
    for(CShapeGroupRef group: mShapeGroups) {
        XmlTree xmlGroup( "group", "");
        xmlGroup.setAttribute("id", g_id);
        int c_id = 0;
        for(CShapeRef shape: group->getShapes()) {
            XmlTree xmlColor( "color", "");
            RGBcolor c = shape->getColor();
            xmlColor.setAttribute("id", c_id);
            xmlColor.setAttribute("r", c.red);
            xmlColor.setAttribute("g", c.green);
            xmlColor.setAttribute("b", c.blue);
            xmlGroup.push_back(xmlColor);
            c_id++;
        }
        xmlCollection.push_back(xmlGroup);
        g_id++;
    }
    xmlCollection.write( writeFile(path) );
}


void GroupManager::drawGroups(cairo::Context &ctx)
{
    for(CShapeGroupRef group: mShapeGroups) {
        group->draw(ctx);
    }
}

std::vector<CShapeGroupRef>& GroupManager::getGroups()
{
    return mShapeGroups;
}