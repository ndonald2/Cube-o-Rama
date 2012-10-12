#pragma once

#include "ofMain.h"
#include "WallOBoxes.h"

#include "ofxOpenCv.h"
#include "ofxKinect.h"

class ofApplication : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void setBoxColumns(int boxColumns);
    
    private:
    
        int   _boxColumns;
        int   _boxRows;
        float _boxSize;
    
        bool  _debug;
    
        ofPoint     _windowCenter;
    
        // cam/lighting
        ofEasyCam   _worldCamera;
        ofLight     _mainLight;
    
        // 3d objects
        WallOBoxes  _wallOBoxes;
    
        // GL
        ofFbo       _wallFbo;
        ofFbo       _reflectFbo;
    
        // kinect
        ofxKinect       _kinect;
        float           _clipMinMm;
        float           _clipMaxMm;
    
        void    positionCamera();
        void    positionLights();
        void    renderScene();
};
