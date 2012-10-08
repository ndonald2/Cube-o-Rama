//
//  WallOBoxes.h
//  Cube-o-Rama
//
//  Created by Nick Donaldson on 10/7/12.
//
//

#pragma once

#include "ofBaseTypes.h"
#include "ofCamera.h"
#include "BoxEntity.h"

class WallOBoxes {
    
public:
    
    WallOBoxes(int columns = 10, int rows = 10);
    ~WallOBoxes();

    void update();
    void draw();
    void reset();

    int getNumberOfBoxes() { return _columns*_rows; };
    int getBoxRows() { return _rows; };
    int getBoxColumns() { return _columns; };
    void setNumberOfBoxes(int columns, int rows);

    
    void setCenterPosition(const ofPoint &center);
    void setRotation(const ofQuaternion &rotation);
    void setDefaultBoxSize(float size);
    void setBoxSpacing(float spacing);
    
    void highlightBoxUnderCursor(ofCamera &cam, ofVec2f mouseCoord);

    BoxEntity * const getBoxUnderCursor(ofCamera &cam, ofVec2f mouseCoord);
    inline BoxEntity * const getBoxAtPosition(int column, int row) const;

private:

    BoxEntity       *_boxes;

    // geometry
    int             _columns;
    int             _rows;
    float           _boxSize;
    float           _boxSpacing;
    ofPoint         _wallSize;
    ofPoint         _center;
    ofQuaternion    _orientation;


};
