//
//  WallOBoxes.cpp
//  Cube-o-Rama
//
//  Created by Nick Donaldson on 10/7/12.
//
//

#include "WallOBoxes.h"
#include "KinectUtilities.h"
#include "MeshObjects.h"

WallOBoxes::WallOBoxes(int columns, int rows)
{
    _columns = columns >= 1 ? columns : 1;
    _rows = rows >= 1 ? rows : 1;
    _center = ofPoint();
    _boxSize = 50.0f;
    _boxSpacing = 50.0f;
    _orientation = ofQuaternion(0.0, 0.0, 0.0, 1.0);
    _boxes = NULL;
    _kinectOffsets = NULL;
    
    _boxMaterial.setShininess(100);
    _boxMaterial.setSpecularColor(ofFloatColor(1.0f,1.0f,1.0f));
    
    _brightLight.setDiffuseColor(ofFloatColor(1.0f,1.0f,1.0f));
    _brightLight.setPosition(0, 0, 500);
    
    ofMeshCubeSetResolution(1);
    
    this->reset();
}

WallOBoxes::~WallOBoxes()
{
    if (_boxes)
    {
        delete [] _boxes;
        _boxes = NULL;
    }
}

void WallOBoxes::draw()
{
    ofPushMatrix();
    
    // translate to center
    ofTranslate(_center);

    // rotate
    
    // translate to top left box
    ofTranslate(-(_wallSize.x/2.0f - _boxSize/2.0f), _wallSize.y/2.0f - _boxSize/2.0f);
        
    // draw boxes
    for (int i=0; i<_rows*_columns; i++){
        ofColor materialColor = ofColor::fromHsb((1.0f-_kinectOffsets[i])*100, CLAMP(_kinectOffsets[i],0.0f,0.5f)*2.0f*255, 255);
        _boxes[i].setColor(materialColor);
        _boxMaterial.begin();
        _boxes[i].draw(ofVec3f(0.0f, 0.0f, _kinectOffsets[i]*_kinectOffsets[i]*_kinectPositionScale), ofQuaternion());
        _boxMaterial.end();
    }
    
    ofPopMatrix();
}

void WallOBoxes::reset()
{
    if (_boxes)
    {
        delete [] _boxes;
        _boxes = NULL;
    }
    
    if (_kinectOffsets){
        delete [] _kinectOffsets;
        _kinectOffsets = NULL;
    }
    
    int nBoxes = _columns*_rows;
    _boxes = new BoxEntity[nBoxes];
    _kinectOffsets = new float[nBoxes];
    
    float interBoxSpacing = _boxSize + _boxSpacing;
    _wallSize = ofPoint(_columns*interBoxSpacing - _boxSpacing, _rows*interBoxSpacing - _boxSpacing);
    
    for (int y=0; y<_rows; y++){
        for (int x=0; x<_columns; x++){
            BoxEntity *box = this->getBoxAtPosition(x, y);
            box->setPosition(x*interBoxSpacing, -y*interBoxSpacing, 0.0f);
            box->setSize(_boxSize);
            _kinectOffsets[y*_columns + x] = 0.0f;
        }
    }
}

void WallOBoxes::updateFromKinectDepths(ofxKinect & kinect)
{
    // quantize kinect image to box faces
    kctResizeAndNormalizeDepthImage(kinect, _kinectOffsets, _columns, _rows, 2, 150);
}


void WallOBoxes::setCenterPosition(const ofPoint &center)
{
    _center = center;
}

void WallOBoxes::setRotation(const ofQuaternion &rotation)
{
    _orientation = rotation;
}

void WallOBoxes::setNumberOfBoxes(int columns, int rows)
{
    _columns = columns >= 1 ? columns : 1;
    _rows = rows >= 1 ? rows : 1;
    this->reset();
}

void WallOBoxes::setDefaultBoxSize(float size)
{
    _boxSize = size >= 1.0f ? size : 1.0f;
    this->reset();
}

void WallOBoxes::setBoxSpacing(float spacing)
{
    _boxSpacing = spacing > 0.0f ? spacing : 0.0f;
    this->reset();
}

void WallOBoxes::highlightBoxUnderCursor(ofCamera &cam, ofVec2f mouseCoord)
{
    // reset color
    for (int i=0; i<this->getNumberOfBoxes(); i++)
    {
        BoxEntity *box = &_boxes[i];
        box->setColor(1.0f, 1.0f, 1.0f);
    }
    
    BoxEntity * nearestbox = this->getBoxUnderCursor(cam, mouseCoord);
    
    if (nearestbox){
        nearestbox->setColor(255.0f, 0, 0);
    }
}

BoxEntity * const WallOBoxes::getBoxUnderCursor(ofCamera &cam, ofVec2f mouseCoord)
{
    BoxEntity *nearestbox = NULL;
    ofVec3f halfSize = _wallSize/2.0f;
    halfSize.y *= -1;
    float nearestDistance = FLT_MAX;
    float minDistance = _boxSize;
    for (int i=0; i<this->getNumberOfBoxes(); i++)
    {
        BoxEntity *box = &_boxes[i];
        ofVec3f prCenter = cam.worldToScreen(box->getPosition() - halfSize);
        float distance = prCenter.distance(mouseCoord);
        if (distance < nearestDistance && distance < minDistance){
            nearestDistance = distance;
            nearestbox = box;
        }
    }
    return nearestbox;
}

inline BoxEntity * const WallOBoxes::getBoxAtPosition(int column, int row) const
{
    if (_boxes && column < _columns && row < _rows && row >= 0 && column >= 0){
        return &_boxes[row*_columns + column];
    }
    
    return NULL;
}