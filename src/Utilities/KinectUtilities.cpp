//
//  KinectUtilities.cpp
//  Cube-o-Rama
//
//  Created by Nick Donaldson on 10/9/12.
//
//

#include "KinectUtilities.h"
#include "ofxOpenCv.h"

float kinectNormalizedDepthInRegion(ofxKinect & kinect, int x, int y, int w, int h)
{
    // potentially blur the image
    
//    ofxCvGrayscaleImage depthImage;
//    depthImage.setFromPixels(kinect.getDepthPixelsRef());
//    depthImage.blur();
    
    unsigned char *pixels = kinect.getDepthPixels();
    
    x = CLAMP(x, 0, kinect.width);
    w = CLAMP(w, 0, kinect.width - x);
    y = CLAMP(y, 0, kinect.height);
    h = CLAMP(h, 0, kinect.height - y);
    
    int index;
    int avgDepth = 0;
    
    for (int ys=y; ys < y+h; ys++){
        for (int xs=x; xs < x+w; xs++){
            index = ys*kinect.width + xs;
            avgDepth += (int)pixels[index];
        }
    }
    
    return (float)avgDepth/(255.0f*w*h);
}