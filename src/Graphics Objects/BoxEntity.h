//
//  BoxEntity.h
//  Cube-o-Rama
//
//  Created by Nick Donaldson on 10/8/12.
//
//

#pragma once

#include "ofBaseTypes.h"
#include "ofVectorMath.h"

class BoxEntity{
    
public:
    BoxEntity(const ofPoint &startingPosition = ofPoint(), float size = 10.0f);
    BoxEntity(const BoxEntity &other);
    ~BoxEntity();

    BoxEntity &operator =(const BoxEntity &other);
    
    void update();
    void draw();

    bool getfilled() const { return _filled; };
    void setFilled(bool filled) { _filled = filled; };

    float getSize() const { return _size; };
    void setSize(float size) { _size = size >= 1.0f ? size : 1.0f; };
    
    ofPoint getPosition() const { return _position; };
    void setPosition(float x, float y, float z);
    void setPosition(const ofPoint &position);

    ofColor getColor() const { return _color; };
    void setColor(float r, float g, float b);
    void setColor(const ofColor &color);

private:

    // geometry
    ofPoint _position;
    float _size;
    ofQuaternion _currentOrientation;

    // appearance
    ofFloatColor _color;
    bool _filled;

};