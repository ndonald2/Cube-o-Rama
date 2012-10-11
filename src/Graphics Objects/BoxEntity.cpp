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
#include "MeshCube.h"

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
    this->draw(ofVec3f(), ofQuaternion());
}

void BoxEntity::draw(const ofVec3f & translation, const ofQuaternion & rotation)
{
    ofPushMatrix();
    ofTranslate(_position);
    ofTranslate(translation);
    
    float rotationAmt;
    ofVec3f rotationAngle;
    ofQuaternion combinedRotation = _currentOrientation*rotation;
    combinedRotation.getRotate(rotationAmt, rotationAngle);
    
    ofRotate(rotationAmt, rotationAngle.x, rotationAngle.y, rotationAngle.z);
    
    ofSetColor(_color);
    
    if (_filled){
        ofFill();
    }
    else{
        ofNoFill();
    }
    
    ofMeshCube(_size);
    
    //ofBox(_size);
    
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
