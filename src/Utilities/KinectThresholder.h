//
//  KinectThresholder.h
//  Cube-o-Rama
//
//  Created by Nick Donaldson on 10/9/12.
//
//

#pragma once

#include "ofBaseTypes.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"


class ofxKinectThresholder {
    
public:
    ofxKinectThresholder(ofxKinect &kinect);
    ~ofxKinectThresholder();
    
    void setNearThreshold(int nearThresh);
    void setNearThreshold(float nearThresh);
    void setFarThreshold(int farThresh);
    void setFarThreshold(float farThresh);
    
    int getNearThreshold() { return _nearThresh; };
    int getFarThreshold() { return _farThresh; };
    
    void update();
    
    const ofxCvGrayscaleImage & getThresholdedImage() const;
    const unsigned char * getPixels();
    
private:
    
    ofxCvGrayscaleImage _cvImageNear;
    ofxCvGrayscaleImage _cvImageFar;
    ofxCvGrayscaleImage _cvImageThresholded;
    
    int     _nearThresh;
    int     _farThresh;
};