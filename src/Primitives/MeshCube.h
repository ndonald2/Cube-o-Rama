//
//  MeshCube.h
//  Cube-o-Rama
//
//  Created by Nick Donaldson on 10/6/12.
//
//

#pragma once
#include "ofTypes.h"
#include "ofBaseTypes.h"
#include "ofGLRenderer.h"
#include "ofVboMesh.h"

void ofMeshCubeSetResolution(int resolution);

void ofMeshCube(float x, float y, float z, float size);
void ofMeshCube(const ofPoint& position, float size);
void ofMeshCube(float size);