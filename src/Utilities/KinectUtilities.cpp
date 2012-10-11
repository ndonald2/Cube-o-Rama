//
//  KinectUtilities.cpp
//  Cube-o-Rama
//
//  Created by Nick Donaldson on 10/9/12.
//
//

#include "KinectUtilities.h"
#include "ofxOpenCv.h"

static ofxCvGrayscaleImage _depthImage;
static ofxCvGrayscaleImage _resizedDepthImage;

void kctGetNormalizedDepthInRegions(ofxKinect & kinect, float * const result, int columns, int rows, float scale, int smoothing)
{
    
    if (_depthImage.width != kinect.width || _depthImage.height != kinect.height){
        _depthImage.allocate(kinect.width, kinect.height);
    }
    
    _depthImage.setFromPixels(kinect.getDepthPixelsRef());
    
    if (_resizedDepthImage.width != columns || _resizedDepthImage.height != rows){
        _resizedDepthImage.allocate(columns, rows);
    }
    
    // hacky but it works
    _resizedDepthImage.scaleIntoMe(_depthImage, CV_INTER_NN);
    _resizedDepthImage.mirror(false, true);
    
    if (smoothing > 0){
        _resizedDepthImage.blur((smoothing-1)*2 + 1);
    }
    
    unsigned char *pixels = _resizedDepthImage.getPixels();
    for (int i=0; i<rows*columns; i++){
        result[i] = pixels[i]*scale/255.0f;
    }
}