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

void kctGetNormalizedDepthInRegions(ofxKinect & kinect, float * const result, int columns, int rows, int smoothing = 0);