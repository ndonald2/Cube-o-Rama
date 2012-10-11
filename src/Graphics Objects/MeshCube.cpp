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
#include "ofVbo.h"
#include "ofVboMesh.h"

static ofMesh _meshCubeVertexData;
static ofVbo _meshCubeVBO;

static const ofVec3f sp[6] = {
    ofVec3f(-0.5, 0.5, 0.5),   // FRONT - left top front
    ofVec3f(0.5, 0.5, 0.5),    // RIGHT - right top front
    ofVec3f(0.5, 0.5, -0.5),     // BACK  - right top back
    ofVec3f(-0.5, 0.5, -0.5),    // LEFT  - left top back
    ofVec3f(-0.5, 0.5, -0.5),    // TOP   - left top back
    ofVec3f(-0.5, -0.5, 0.5)     // BOTTOM - left bottom front
};

static const ofVec3f nrm[6] = {
    ofVec3f(0,0,+1),    // forward
    ofVec3f(+1,0,0),    // right
    ofVec3f(0,0,-1),    // back
    ofVec3f(-1,0,0),    // left
    ofVec3f(0,+1,0),    // up
    ofVec3f(0,-1,0)     // down
};

void ofMeshCubeSetResolution(int resolution)
{
    int res = resolution;
    
    _meshCubeVertexData.clear();
    
    const int nSubFaces = res*res;
    const int nVertices = (res+1)*(res+1)*6;
    const int nIndices = nSubFaces*6*6;
    
    float inc = 1.0/res;

    
    // generate indices, normals, and texture coordinates for each face
    
    // increment vectors for each face
    
    const ofVec3f icv[6] = {
        ofVec3f(inc, 0, 0),     // right
        ofVec3f(0, 0, -inc),     // back
        ofVec3f(-inc, 0, 0),    // left
        ofVec3f(0, 0, inc),    // forward
        ofVec3f(inc, 0, 0),     // right
        ofVec3f(inc, 0, 0)     // right
    };
    
    const ofVec3f irv[6] = {
        ofVec3f(0, -inc, 0),    // down
        ofVec3f(0, -inc, 0),    // down
        ofVec3f(0, -inc, 0),    // down
        ofVec3f(0, -inc, 0),    // down
        ofVec3f(0, 0, inc),    // forward
        ofVec3f(0, 0, -inc)      // back
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

                indices[ict++] = (ofIndexType)(fvo + sfr*(res+1) + sfc);        // top left
                indices[ict++] = (ofIndexType)(fvo + (sfr+1)*(res+1) + sfc);    // bottom left
                indices[ict++] = (ofIndexType)(fvo + sfr*(res+1) + (sfc+1));    // top right

                indices[ict++] = (ofIndexType)(fvo + (sfr+1)*(res+1) + sfc);        // bottom left
                indices[ict++] = (ofIndexType)(fvo + (sfr+1)*(res+1) + (sfc+1));    // bottom right
                indices[ict++] = (ofIndexType)(fvo + sfr*(res+1) + (sfc+1));        // top right

            }
        }
        
    }

    _meshCubeVertexData.addVertices(vertices, nVertices);
    _meshCubeVertexData.addNormals(normals, nVertices);
    _meshCubeVertexData.addTexCoords(tex, nVertices);
    _meshCubeVertexData.addIndices(indices, nIndices);

    // shorthand references
    ofMesh & mesh = _meshCubeVertexData;
    ofVbo & vbo = _meshCubeVBO;
    
    int usage = GL_STATIC_DRAW;
    
    vbo.clear();
    if (!vbo.getIsAllocated()){
        vbo.setVertexData(mesh.getVerticesPointer(), mesh.getNumVertices(), usage);
        vbo.setNormalData(mesh.getNormalsPointer(), mesh.getNumNormals(), usage);
        vbo.setTexCoordData(mesh.getTexCoordsPointer(), mesh.getNumTexCoords(), usage);
        vbo.setIndexData(mesh.getIndexPointer(), mesh.getNumIndices(), usage);
    }

    
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
    if (!_meshCubeVertexData.getVertices().size())
    {
        ofMeshCubeSetResolution(10);
    }
    
    ofPushMatrix();
	if(ofGetCoordHandedness() == OF_LEFT_HANDED){
		ofScale(size, size, -size);
	}
    else{
		ofScale(size, size, size);
    }
    
    glEnable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    
    _meshCubeVBO.drawElements(GL_TRIANGLES, _meshCubeVertexData.getNumIndices());
    
    glDisable(GL_POLYGON_SMOOTH);
    glDisable(GL_CULL_FACE);
    glDisable(GL_NORMALIZE);

    ofPopMatrix();

}
