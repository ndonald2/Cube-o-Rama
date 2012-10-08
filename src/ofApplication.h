#pragma once

#include "ofMain.h"
#include "WallOBoxes.h"

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
    
        ofPoint     windowCenter;
    
        ofEasyCam   worldCamera;
        ofLight     mainLight;
        ofImage     texImage;
    
        WallOBoxes  wallOBoxes;
};