//
//  KinectUtilities.h
//  Cube-o-Rama
//
//  Created by Nick Donaldson on 10/9/12.
//
//

#pragma once

#include "ofBaseTypes.h"
#include "ofxKinect.h"

float kinectNormalizedDepthInRegion(ofxKinect & kinect, int x, int y, int w, int h);