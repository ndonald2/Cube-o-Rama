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

void kctResizeAndNormalizeDepthImage(ofxKinect & kinect, float * const result, int columns, int rows, int smoothing, float lag_ms)
{
    
    float filterCoef = 1.0f - (1.0f/(ofGetFrameRate()*lag_ms/1000.0f));
    
    if (_depthImage.width != kinect.width || _depthImage.height != kinect.height){
        _depthImage.allocate(kinect.width, kinect.height);
    }
    
    _depthImage.setFromPixels(kinect.getDepthPixelsRef());
    
    if (_resizedDepthImage.width != columns || _resizedDepthImage.height != rows){
        _resizedDepthImage.allocate(columns, rows);
    }
    
    // hacky but it works
    _resizedDepthImage.scaleIntoMe(_depthImage, CV_INTER_CUBIC);
    _resizedDepthImage.mirror(false, true);
    
    if (smoothing > 0){
        _resizedDepthImage.blurGaussian((smoothing-1)*2 + 1);
    }
    
    unsigned char *pixels = _resizedDepthImage.getPixels();
    float prevResult;
    for (int i=0; i<columns*rows; i++){
        prevResult = result[i];
        result[i] = CLAMP((1.0f-filterCoef)*pixels[i]/255.0f + (filterCoef*prevResult), 0.0f, 1.0f);
    }
}

void kctDrawResizedImage(int x, int y, int w, int h)
{
    if (_resizedDepthImage.width)
    {
        _resizedDepthImage.draw(x, y, w, h);
    }
}