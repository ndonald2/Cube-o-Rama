#include "ofApplication.h"
#include "BoxEntity.h"
#include "KinectUtilities.h"

#define CAM_RADIUS  3000.0f

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
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    
    // framebuffers
    ofFbo::Settings fbosettings;
    fbosettings.width = ofGetWidth();
    fbosettings.height = ofGetHeight();
    fbosettings.numColorbuffers = 1;
    fbosettings.useDepth = true;
    fbosettings.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
    fbosettings.internalformat = GL_RGBA;
    _wallFbo.allocate(fbosettings);
    
    fbosettings.width /= 2;
    fbosettings.height /= 2;
    _reflectFbo.allocate(fbosettings);
    
    // geometry
    _windowCenter = ofPoint(ofGetWidth()/2.0f, ofGetHeight()/2.0f);

    // camera
    _worldCamera.setupPerspective(false, 60, 10, 10000.0);
    _worldCamera.disableMouseInput();

    // lighting
    _mainLight.setPointLight();
    
    _mainLight.setSpecularColor(ofColor::fromHsb(0.0f, 0.0f, 255.0f*0.75f));
    _mainLight.setDiffuseColor(ofColor::fromHsb(0.0f, 0.0f, 255.0f*0.25f));

    // box geometry
    setBoxColumns(40);
    _wallOBoxes.setKinectPositionScale(750.0f);
    
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
            _wallOBoxes.updateFromKinectDepths(_kinect);
        }
    }

    //================
    // Draw to FBOs
    //=================
    _wallFbo.begin();
    ofClear(0, 0, 0, 0);
    _worldCamera.begin();
    positionCamera();
    positionLights();
    renderScene();
    _worldCamera.end();
    _wallFbo.end();
    
//    _reflectFbo.begin();
//    ofClear(0, 0, 0, 0);
//    _worldCamera.begin();
//    positionCamera();
//    positionLights();
//    renderScene();
//    _reflectFbo.end();
}

//--------------------------------------------------------------
void ofApplication::draw(){

    glDisable(GL_DEPTH_TEST);

    ofBackground(10);
    _wallFbo.draw(0, 0);
    //_reflectFbo.draw(0, 0, ofGetWidth(), ofGetHeight());

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

void ofApplication::renderScene()
{
    
    glEnable(GL_DEPTH_TEST);

    
    if (_debug){
        ofSetColor(220.0f,220.0f,220.0f);
        ofSphere(_mainLight.getPosition(), 20);
    }
    
    ofEnableLighting();
    _mainLight.enable();
    _wallOBoxes.draw();
    _mainLight.disable();
    ofDisableLighting();

    glDisable(GL_DEPTH_TEST);

}

void ofApplication::positionCamera()
{
    if (!_debug){
        
        _worldCamera.resetTransform();

        static const ofPoint & wallSize = _wallOBoxes.getWallSize();
        
        float elapsedTime = ofGetElapsedTimef();
        float timePhase = elapsedTime*2.0*M_PI;
        
        float camAngle = HALF_PI*sinf(timePhase*0.05f)/2.0f;
        ofVec3f camPosition = ofVec3f(sinf(camAngle)*CAM_RADIUS, sinf(timePhase*0.04f)*wallSize.y*0.4f, cosf(camAngle)*CAM_RADIUS);
        _worldCamera.setPosition(camPosition);
        //_worldCamera.setPosition(cosf(timePhase*0.08f)*wallSize.x*0.3f, sinf(timePhase*0.04f)*wallSize.y*0.4f, 3000.0f);
        _worldCamera.lookAt(ofVec3f(0,0,0));
    }
}

void ofApplication::positionLights()
{
    _mainLight.setPosition(0, 0, 2000.0f);

}