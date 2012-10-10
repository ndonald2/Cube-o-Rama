#include "ofApplication.h"
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
int main(){
	ofAppGlutWindow window; // create a window
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
	ofSetupOpenGL(&window, 1280, 800, OF_FULLSCREEN);
    //window.setGlutDisplayString("rgba double samples>=4 depth");
	ofRunApp(new ofApplication()); // start the app
}
