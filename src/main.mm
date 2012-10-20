#include "ofApplication.h"
#include "ofAppGlutWindow.h"
#include <Quartz/Quartz.h>

//--------------------------------------------------------------
int main(){
	ofAppGlutWindow window; // create a window
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
	//ofSetupOpenGL(&window, CGDisplayPixelsWide(CGMainDisplayID()), CGDisplayPixelsHigh(CGMainDisplayID()), OF_WINDOW);
    ofSetupOpenGL(&window, 1920, 1080, OF_WINDOW);
    //window.setGlutDisplayString("rgba double samples>=4 depth");
	ofRunApp(new ofApplication()); // start the app
}
