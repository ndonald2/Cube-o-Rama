//
//  MeshCube.cpp
//  Cube-o-Rama
//
//  Created by Nick Donaldson on 10/6/12.
//
//

#include "MeshCube.h"
#include "ofConstants.h"
#include "ofUtils.h"
#include "ofGraphics.h"

static ofVboMesh _meshCubeVboMesh;

static const ofVec3f sp[6] = {
    ofVec3f(-0.5, 0.5, -0.5),
    ofVec3f(0.5, 0.5, 0.5),
    ofVec3f(0.5, 0.5, 0.5),
    ofVec3f(-0.5, 0.5, -0.5),
    ofVec3f(-0.5, 0.5, 0.5),
    ofVec3f(0.5, -0.5, 0.5)
};

static const ofVec3f nrm[6] = {
    ofVec3f(0,0,-1),
    ofVec3f(+1,0,0),
    ofVec3f(0,0,+1),
    ofVec3f(-1,0,0),
    ofVec3f(0,+1,0),
    ofVec3f(0,-1,0)
};

void ofMeshCubeComputeVertices(int resolution);

//=-=================================


void ofMeshCubeSetResolution(int resolution)
{
    ofMeshCubeComputeVertices(resolution);
}

void ofMeshCubeComputeVertices(int resolution)
{
    int res = resolution;
    _meshCubeVboMesh.clear();
    
    const int nSubFaces = res*res;
    const int nVertices = (res+1)*(res+1)*6;
    const int nIndices = nSubFaces*6*6;
    
    float inc = 1.0/res;
    
    // generate indices, normals, and texture coordinates for each face
    
    // increment vectors for each face
    
    const ofVec3f icv[6] = {
        ofVec3f(inc, 0, 0),
        ofVec3f(0, 0, -inc),
        ofVec3f(-inc, 0, 0),
        ofVec3f(0, 0, inc),
        ofVec3f(0, 0, -inc),
        ofVec3f(0, 0, -inc)
    };
    
    const ofVec3f irv[6] = {
        ofVec3f(0, -inc, 0),
        ofVec3f(0, -inc, 0),
        ofVec3f(0, -inc, 0),
        ofVec3f(0, -inc, 0),
        ofVec3f(inc, 0, 0),
        ofVec3f(-inc, 0, 0)
    };
    
    
    ofVec3f *vertices = new ofVec3f[nVertices];
    ofVec3f *normals = new ofVec3f[nVertices];
    ofVec2f *tex = new ofVec2f[nVertices];
    ofIndexType *indices = new ofIndexType[nIndices];
    
    ofVec3f cv;
    int vct = 0;
    int ict = 0;
    
    // Each face of the whole cube
    for (int s=0; s<6; s++){
        
        // for each row and column
        // generate vertices, normals, tex coords for each face
        for (int r=0; r<res+1; r++){
            cv = sp[s];
            cv += r*irv[s];
            for (int c=0; c<res+1; c++){
                vertices[vct] = cv;
                normals[vct] = nrm[s];
                tex[vct] = ofVec2f((float)c/res, ((float)r/res));
                vct++;
                cv += icv[s];
            }
        }
        
        int fvo = s*(res+1)*(res+1);
        for (int sfr=0; sfr<res; sfr++){
            for (int sfc=0; sfc<res; sfc++){
                
                indices[ict++] = (ofIndexType)(fvo + sfr*(res+1) + sfc);
                indices[ict++] = (ofIndexType)(fvo + (sfr+1)*(res+1) + sfc);
                indices[ict++] = (ofIndexType)(fvo + (sfr+1)*(res+1) + (sfc+1));
                
                indices[ict++] = (ofIndexType)(fvo + sfr*(res+1) + sfc);
                indices[ict++] = (ofIndexType)(fvo + sfr*(res+1) + (sfc+1));
                indices[ict++] = (ofIndexType)(fvo + (sfr+1)*(res+1) + (sfc+1));
                
            }
        }
        
    }
    
    _meshCubeVboMesh.addVertices(vertices, nVertices);
    _meshCubeVboMesh.addNormals(normals, nVertices);
    _meshCubeVboMesh.addTexCoords(tex, nVertices);
    _meshCubeVboMesh.addIndices(indices, nIndices);
    _meshCubeVboMesh.setMode(OF_PRIMITIVE_TRIANGLES);
        
    // cleanup
    delete [] vertices;
    delete [] normals;
    delete [] tex;
    delete [] indices;
}


void ofMeshCube(float x, float y, float z, float size)
{
    ofMeshCube(ofPoint(x,y,z), size);
}

void ofMeshCube(const ofPoint& position, float size)
{
    ofPushMatrix();
    ofTranslate(position);
    ofMeshCube(size);
    ofPopMatrix();
}

void ofMeshCube(float size)
{
    glEnable(GL_NORMALIZE);
    ofPushMatrix();
	if(ofGetCoordHandedness() == OF_LEFT_HANDED){
		ofScale(size, size, -size);
	}
    else{
        ofScale(size, size, size);
    }

    _meshCubeVboMesh.draw();
    ofPopMatrix();
    glDisable(GL_NORMALIZE);
}
