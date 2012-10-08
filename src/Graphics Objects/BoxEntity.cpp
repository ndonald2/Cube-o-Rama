//
//  BoxEntity.cpp
//  Cube-o-Rama
//
//  Created by Nick Donaldson on 10/8/12.
//
//

#include "BoxEntity.h"
#include "ofGraphics.h"
#include "ofMath.h"

BoxEntity::BoxEntity(const ofPoint &startingPosition, float size)
{
    _position = startingPosition;
    _size = size;
    _color = ofFloatColor(1.0f, 1.0f, 1.0f);
    _filled = true;
}

BoxEntity::BoxEntity(const BoxEntity &other)
{
    _position = other._position;
}


BoxEntity::~BoxEntity()
{
    
}

void BoxEntity::update()
{
    // will animate here
}

void BoxEntity::draw()
{
    ofPushMatrix();
    ofTranslate(_position);
    
    float rotationAmt;
    ofVec3f rotationAngle;
    _currentOrientation.getRotate(rotationAmt, rotationAngle);
    
    ofRotate(rotationAmt, rotationAngle.x, rotationAngle.y, rotationAngle.z);
    
    ofSetColor(_color);
    
    if (_filled){
        ofFill();
    }
    else{
        ofNoFill();
    }
    
    ofBox(_size);
    
    ofPopMatrix();
}

void BoxEntity::setPosition(float x, float y, float z)
{
    _position = ofPoint(x, y, z);
}

void BoxEntity::setPosition(const ofPoint &position)
{
    _position = position;
}

void BoxEntity::setColor(float r, float g, float b)
{
    _color = ofFloatColor(r,g,b);
}

void BoxEntity::setColor(const ofColor &color)
{
    _color = ofFloatColor(color);
}
