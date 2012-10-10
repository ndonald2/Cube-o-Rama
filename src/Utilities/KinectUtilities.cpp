//
//  KinectUtilities.cpp
//  Cube-o-Rama
//
//  Created by Nick Donaldson on 10/9/12.
//
//

#include "KinectUtilities.h"
#include "ofxOpenCv.h"

static ofxCvGrayscaleImage _tempDepthImage;

void kctGetNormalizedDepthInRegions(ofxKinect & kinect, float * const result, int columns, int rows, float scale, int smoothing)
{
    _tempDepthImage.setFromPixels(kinect.getDepthPixelsRef());
    if (smoothing > 0){
        _tempDepthImage.blur(smoothing);
    }

    // hacky but it works
    _tempDepthImage.resize(columns, rows);
    
    unsigned char *pixels = _tempDepthImage.getPixels();
    for (int i=0; i<rows*columns; i++){
        result[i] = pixels[i]*scale/255.0f;
    }
}