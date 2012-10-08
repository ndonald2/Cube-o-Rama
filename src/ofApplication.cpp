#include "ofApplication.h"
#include "BoxEntity.h"

void ofApplication::setBoxColumns(int boxColumns)
{
    int cols = boxColumns < 1 ? 1 : boxColumns;
    int rows = boxColumns*3/4;
    float size = ofGetWidth()/boxColumns;
    
    _boxColumns = cols;
    _boxRows = rows;
    _boxSize = size;
    
    wallOBoxes.setNumberOfBoxes(_boxColumns, _boxRows);
    wallOBoxes.setDefaultBoxSize(_boxSize);
    wallOBoxes.setBoxSpacing(_boxSize*0.5f);
}

//--------------------------------------------------------------
void ofApplication::setup(){
    
    _debug = false;
    
    // renderer
    ofSetFrameRate(60.0);
    ofSetVerticalSync(true);
    ofSetSmoothLighting(true);
    
    // geometry
    windowCenter = ofPoint(ofGetWidth()/2.0f, ofGetHeight()/2.0f);
    
    // box geometry
    setBoxColumns(30);
    
    // camera
    worldCamera.setupPerspective(false, 60, 10, 10000.0);
    worldCamera.setDistance(2000);

    // lighting
    ofColor lightColor = ofColor(255.0f,254.0,222.0f);
    mainLight.setSpecularColor(lightColor);
    lightColor.setBrightness(lightColor.getBrightness()*0.66);
    mainLight.setDiffuseColor(lightColor);
    mainLight.setPosition(0, 0, 500.0f);
    
    ofDisableArbTex();
    texImage.loadImage("images/of.png");

}

//--------------------------------------------------------------
void ofApplication::update(){

}

//--------------------------------------------------------------
void ofApplication::draw(){
    
    float elapsedTime = ofGetElapsedTimef();
    float timePhase = elapsedTime*2.0*M_PI;
    
    glEnable(GL_DEPTH_TEST);
    ofBackground(10);
    
    worldCamera.begin();
        
    //worldCamera.setPosition(cosf(timePhase*0.05f)*100, sinf(timePhase*0.05f)*100, worldCamera.getPosition().z);
    //worldCamera.lookAt(ofVec3f(0,0,0));
    
    
    if (_debug){
        ofSetColor(220.0f,220.0f,220.0f);
        ofSphere(mainLight.getPosition(), 20);
    }
    
    mainLight.enable();
    
    wallOBoxes.draw();

    // translate to center of boxes
//    float boxSpacing = _boxSize*1.5;
//    float centerOffsetX = (boxSpacing*(_boxColumns-1) + _boxSize)/2.0f;
//    float centerOffsetY = (boxSpacing*(_boxRows-1) + _boxSize)/2.0f;
//    ofTranslate(-centerOffsetX, centerOffsetY);
//    
//    for (int y=0; y<_boxRows; y++){
//        
//        float yPhase = 0.5*M_PI*y/_boxRows;
//        
//        ofSetColor(y*255/_boxRows, 255, 255);
//        
//        for (int x=0; x<_boxColumns; x++){
//            
//            
//            float xPhase = 2.0*M_PI*x/_boxColumns;
//            
//            ofPushMatrix();
//            
//            ofTranslate(x*boxSpacing, -y*boxSpacing, cosf(xPhase + yPhase + timePhase*0.4)*20);
//            ofBox(_boxSize);
//            ofPopMatrix();
//        }
//    }
    
    mainLight.disable();
    ofDisableLighting();
    
    // world camera end
    worldCamera.end();
    
    glDisable(GL_DEPTH_TEST);

    if (_debug){
        ofDrawBitmapString(ofToString((int) ofGetFrameRate()) + " fps", 10, 20);
        ofDrawBitmapString(ofToString(_boxRows*_boxRows) + " boxes", 10, 35);
    }
}

//--------------------------------------------------------------
void ofApplication::keyPressed(int key){
    switch (key) {
        case OF_KEY_UP:
            setBoxColumns(_boxColumns + 5);
            break;
            
        case OF_KEY_DOWN:
            if (_boxColumns > 5){
                setBoxColumns(_boxColumns - 5);
            }
            break;
            
        case 'd':
            _debug = !_debug;
            break;
            
        case 'a':
            mainLight.setPosition(0, 0, mainLight.getPosition().z - 50.0f);
            break;
            
        case 'z':
            mainLight.setPosition(0, 0, mainLight.getPosition().z + 50.0f);
            break;
            
            
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApplication::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApplication::mouseMoved(int x, int y){
    if (_debug){
        wallOBoxes.highlightBoxUnderCursor(worldCamera, ofVec2f(x,y));
    }
}

//--------------------------------------------------------------
void ofApplication::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApplication::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApplication::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApplication::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApplication::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApplication::dragEvent(ofDragInfo dragInfo){ 

}