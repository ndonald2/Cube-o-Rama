#include "ofApplication.h"
#include "BoxEntity.h"
#include "KinectUtilities.h"

void ofApplication::setBoxColumns(int boxColumns)
{
    int cols = boxColumns < 1 ? 1 : boxColumns;
    int rows = boxColumns*3/4;
    float size = ofGetWidth()/boxColumns;
    
    _boxColumns = cols;
    _boxRows = rows;
    _boxSize = size;
    
    _wallOBoxes.setNumberOfBoxes(_boxColumns, _boxRows);
    _wallOBoxes.setDefaultBoxSize(_boxSize);
    _wallOBoxes.setBoxSpacing(_boxSize*0.5f);
    
    _mainLight.setPosition(0, 0, _boxSize*5.0f);
}

//--------------------------------------------------------------
void ofApplication::setup(){
    
    _debug = false;
    
    // kinect
    
    _clipMinMm = 500.0f;
    _clipMaxMm = 3000.0f;
    
    // enable depth->video image calibration
	_kinect.setRegistration(true);
	_kinect.init(false, true, false);
	_kinect.open();		// opens first available kinect
    _kinect.setDepthClipping(_clipMinMm, _clipMaxMm);
        
    // renderer
    ofSetFrameRate(60.0);
    ofSetVerticalSync(true);
    ofSetSmoothLighting(true);
    
    // geometry
    _windowCenter = ofPoint(ofGetWidth()/2.0f, ofGetHeight()/2.0f);
    
    // box geometry
    setBoxColumns(30);
    
    // camera
    _worldCamera.setupPerspective(false, 60, 10, 10000.0);
    _worldCamera.setDistance(2000);

    // lighting
    _mainLight.setSpecularColor(ofColor::fromHex(0xFF0000));
    _mainLight.setDiffuseColor(ofColor::fromHex(0xE3E3E3));

}

//--------------------------------------------------------------
void ofApplication::update(){

    // ==================
    //   Update Kinect
    // ==================
    
    // update kinect at 1/2 frame rate
    if (1){//ofGetFrameNum() % 2){
        _kinect.update();
        
        if (_kinect.isFrameNew())
        {
            _wallOBoxes.updateFromKinectDepths(_kinect, _boxSize*4.0f);
        }
    }

}

//--------------------------------------------------------------
void ofApplication::draw(){
    
    float elapsedTime = ofGetElapsedTimef();
    float timePhase = elapsedTime*2.0*M_PI;
    
    glEnable(GL_DEPTH_TEST);
    ofBackground(10);
    
    _worldCamera.begin();
        
    //worldCamera.setPosition(cosf(timePhase*0.05f)*100, sinf(timePhase*0.05f)*100, worldCamera.getPosition().z);
    //worldCamera.lookAt(ofVec3f(0,0,0));
    
    
    if (_debug){
        ofSetColor(220.0f,220.0f,220.0f);
        ofSphere(_mainLight.getPosition(), 20);
    }
    
    const ofPoint & wallSize = _wallOBoxes.getWallSize();
    _mainLight.setPosition(cos(0.04*timePhase)*wallSize.x*0.4, sin(0.08*timePhase)*wallSize.y*0.4f, _mainLight.getPosition().z);
    _mainLight.enable();
    
    _wallOBoxes.draw();

//    // translate to center of boxes
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
    
    _mainLight.disable();
    ofDisableLighting();
    
    // world camera end
    _worldCamera.end();
    
    glDisable(GL_DEPTH_TEST);

    if (_debug){
        ofDrawBitmapString(ofToString((int) ofGetFrameRate()) + " fps", 10, 20);
        ofDrawBitmapString(ofToString(_boxRows*_boxRows) + " boxes", 10, 35);
        
        stringstream clipStream;
        clipStream << "< and > to change min kinect clip: " << _clipMinMm << " cm" << endl;
        clipStream << "[ and ] to change max kinect clip: " << _clipMaxMm << " cm" << endl;
        ofDrawBitmapString(clipStream.str(), 10, 60);
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
            _mainLight.setPosition(0, 0, _mainLight.getPosition().z - 50.0f);
            break;
            
        case 'z':
            _mainLight.setPosition(0, 0, _mainLight.getPosition().z + 50.0f);
            break;
            
        case ',':
            _clipMinMm = CLAMP(_clipMinMm - 500.0f, 500.0f, _clipMaxMm - 500.0f);
            _kinect.setDepthClipping(_clipMinMm, _clipMaxMm);
            break;
            
        case '.':
            _clipMinMm = CLAMP(_clipMinMm + 500.0f, 500.0f, _clipMaxMm - 500.0f);
            _kinect.setDepthClipping(_clipMinMm, _clipMaxMm);
            break;
            
        case '[':
            _clipMaxMm = CLAMP(_clipMaxMm - 500.0f, _clipMinMm+500.0f, 4000.0f);
            _kinect.setDepthClipping(_clipMinMm, _clipMaxMm);
            break;
            
        case ']':
            _clipMaxMm = CLAMP(_clipMaxMm + 500.0f, _clipMinMm+500.0f, 4000.0f);
            _kinect.setDepthClipping(_clipMinMm, _clipMaxMm);
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
        //wallOBoxes.highlightBoxUnderCursor(worldCamera, ofVec2f(x,y));
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