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
    
    _mainLight.setPosition(0, _wallOBoxes.getWallSize().y*0.4, 250.0f);
    
    const ofPoint & wallSize = _wallOBoxes.getWallSize();
    _spotlight1.setPosition(-wallSize.x*0.5f, wallSize.y*0.5f, 1000.0f);
    _spotlight1.setSpotConcentration(45);
    _spotlight1.setSpotlightCutOff(15);
    
    _spotlight2.setPosition(wallSize.x*0.5f, wallSize.y*0.5f, 1000.0f);
    _spotlight2.setSpotConcentration(45);
    _spotlight2.setSpotlightCutOff(15);

}

//--------------------------------------------------------------
void ofApplication::setup(){
    
    _debug = false;
    
    // kinect
    
    _clipMinMm = 500.0f;
    _clipMaxMm = 2000.0f;
    
    // enable depth->video image calibration
	_kinect.setRegistration(true);
	_kinect.init(false, true, false);
    _kinect.setDepthClipping(_clipMinMm, _clipMaxMm);
	//_kinect.open();		// opens first available kinect

        
    // renderer
    ofSetFrameRate(60.0);
    ofSetVerticalSync(true);
    ofSetSmoothLighting(true);
    
    // geometry
    _windowCenter = ofPoint(ofGetWidth()/2.0f, ofGetHeight()/2.0f);

    // camera
    _worldCamera.setupPerspective(false, 60, 10, 10000.0);
    _worldCamera.disableMouseInput();

    // lighting
    _mainLight.setPointLight();
    _spotlight1.setSpotlight();
    _spotlight2.setSpotlight();
    
    _mainLight.setSpecularColor(ofColor::fromHsb(0.0f, 0.0f, 255.0f*0.75f));
    _mainLight.setDiffuseColor(ofColor::fromHsb(0.0f, 0.0f, 255.0f*0.5f));
    _mainLight.setAmbientColor(ofColor::fromHsb(0.0f, 0.0f, 0.05f));

    _spotlight1.setDiffuseColor(ofColor(0.0f, 255.0f, 0.0f));
    _spotlight1.setSpecularColor(ofColor(220.0f, 255.0f, 220.0f));
    _spotlight1.setAmbientColor(ofColor::fromHsb(0.0f, 0.0f, 0.05f));
    
    _spotlight2.setDiffuseColor(ofColor(115.0f, 0.0f, 255.0f));
    _spotlight2.setSpecularColor(ofColor(250.0f, 227.0f, 255.0f));
    _spotlight2.setAmbientColor(ofColor::fromHsb(0.0f, 0.0f, 0.05f));
    
    // box geometry
    setBoxColumns(40);
    
}

//--------------------------------------------------------------
void ofApplication::update(){

    // ==================
    //   Update Kinect
    // ==================
    if (_kinect.isConnected()){
        _kinect.update();
        
        if (_kinect.isFrameNew())
        {
            _wallOBoxes.updateFromKinectDepths(_kinect, _boxSize*20.0f);
        }
    }
}

//--------------------------------------------------------------
void ofApplication::draw(){
    
    float elapsedTime = ofGetElapsedTimef();
    float timePhase = elapsedTime*2.0*M_PI;
    
    glEnable(GL_DEPTH_TEST);
    ofBackground(10);
    
    static const ofPoint & wallSize = _wallOBoxes.getWallSize();
    
    _worldCamera.begin();
    
    if (!_debug){
        _worldCamera.resetTransform();
        _worldCamera.setPosition(cosf(timePhase*0.08f)*wallSize.x*0.3f, sinf(timePhase*0.04f)*wallSize.y*0.4f, 3000.0f);
        _worldCamera.lookAt(ofVec3f(0,0,0));
    }
    
    if (_debug){
        ofSetColor(220.0f,220.0f,220.0f);
        ofSphere(_mainLight.getPosition(), 20);
        ofSetColor(_spotlight1.getDiffuseColor());
        ofSphere(_spotlight1.getPosition(), 20);
        ofSetColor(_spotlight2.getDiffuseColor());
        ofSphere(_spotlight2.getPosition(), 20);
    }
    
    
    _spotlight1.lookAt(ofVec3f(cosf(0.4f*timePhase)*wallSize.x*0.4f, sinf(0.2f*timePhase)*wallSize.y*0.4f, 0.0f), ofVec3f(0,1,0));
    _spotlight2.lookAt(ofVec3f(-cosf(0.4f*timePhase)*wallSize.x*0.4f, sinf(0.2f*timePhase)*wallSize.y*0.4f, 0.0f), ofVec3f(0,1,0));

    _mainLight.enable();
    _spotlight1.enable();
    _spotlight2.enable();
    
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
    _spotlight1.disable();
    _spotlight2.disable();
    ofDisableLighting();
    
    // world camera end
    _worldCamera.end();
    
    glDisable(GL_DEPTH_TEST);

    if (_debug){
        ofDrawBitmapString(ofToString((int) ofGetFrameRate()) + " fps", 10, 20);
        ofDrawBitmapString(ofToString(_boxRows*_boxRows) + " boxes", 10, 35);
        
        stringstream clipStream;
        clipStream << "< and > to change min kinect clip: " << _clipMinMm/10 << " cm" << endl;
        clipStream << "[ and ] to change max kinect clip: " << _clipMaxMm/10 << " cm" << endl;
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
            if (_debug){
                _worldCamera.enableMouseInput();
            }
            else{
                _worldCamera.disableMouseInput();
            }
            _worldCamera.reset();
            _worldCamera.setDistance(3000.0f);
            _worldCamera.lookAt(ofVec3f(0,0,0));
            break;
            
        case 'o':
            if (!_kinect.isConnected()){
                _kinect.open();
            }
            break;
            
        case 'a':
            _mainLight.setPosition(0, 0, _mainLight.getPosition().z - 50.0f);
            break;
            
        case 'z':
            _mainLight.setPosition(0, 0, _mainLight.getPosition().z + 50.0f);
            break;
            
        case ',':
            _clipMinMm = CLAMP(_clipMinMm - 100.0f, 500.0f, _clipMaxMm - 100.0f);
            _kinect.setDepthClipping(_clipMinMm, _clipMaxMm);
            break;
            
        case '.':
            _clipMinMm = CLAMP(_clipMinMm + 100.0f, 500.0f, _clipMaxMm - 100.0f);
            _kinect.setDepthClipping(_clipMinMm, _clipMaxMm);
            break;
            
        case '[':
            _clipMaxMm = CLAMP(_clipMaxMm - 100.0f, _clipMinMm+100.0f, 4000.0f);
            _kinect.setDepthClipping(_clipMinMm, _clipMaxMm);
            break;
            
        case ']':
            _clipMaxMm = CLAMP(_clipMaxMm + 100.0f, _clipMinMm+100.0f, 4000.0f);
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