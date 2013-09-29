#include "testApp.h"


class myUser : public ofxOpenNIUser {
public:
  void test(){
    cout << "test" << endl;
  }
};

//--------------------------------------------------------------
void testApp::setup() {
  num_users_ = 0;
  for (int u = 0; u < MAX_USERS; ++u) {
    pulsar_[u].x_ = GRID_WIDTH / 2.0;
    pulsar_[u].y_ = GRID_HEIGHT / 2.0;
    pulsar_[u].r_ = GRID_WIDTH * 0.01;
    pulsar_[u].T_ = 0.5;
    pulsar_[u].h_ = 10.0 * (1 - (2 * u));
  }
  grid_.setDamping(0.999);
  
  image_.allocate(GRID_WIDTH, GRID_HEIGHT, OF_IMAGE_COLOR);
  memset(impulse_input_, 0, sizeof(float) * GRID_HEIGHT * GRID_WIDTH);

  ofSetLogLevel(OF_LOG_VERBOSE);
  
  openNIRecorder.setup();
  openNIRecorder.addDepthGenerator();
  openNIRecorder.addImageGenerator();
  openNIRecorder.setRegister(true);
  openNIRecorder.setMirror(true);
  openNIRecorder.addUserGenerator();
  openNIRecorder.setMaxNumUsers(MAX_USERS);
  openNIRecorder.start();

  openNIPlayer.setup();
  openNIPlayer.start();
  ofSetFrameRate(60);
    
  verdana.loadFont(ofToDataPath("verdana.ttf"), 24);
}

//--------------------------------------------------------------
void testApp::update() {
  num_users_ = openNIRecorder.getNumTrackedUsers();
  std::vector<ofPoint> points;
  
  for (int u = 0; u < num_users_; ++u) {
    ofxOpenNIUser& user = openNIRecorder.getTrackedUser(u);
    ofPoint torso = user.getJoint(JOINT_TORSO).getProjectivePosition();
    ofPoint left_shoulder = user.getJoint(JOINT_LEFT_SHOULDER).getProjectivePosition();
    ofPoint right_shoulder = user.getJoint(JOINT_RIGHT_SHOULDER).getProjectivePosition();

    ofPoint heart = 0.5 * torso + 0.35 * left_shoulder + 0.15 * right_shoulder;

    pulsar_[u].x_ = GRID_WIDTH * heart.x / ofGetWidth();
    pulsar_[u].y_ = GRID_HEIGHT * heart.y / ofGetHeight();
    pulsar_[u].update();
  }
  
  for (int x = 0; x < GRID_WIDTH; ++x) {
    for (int y = 0; y < GRID_HEIGHT; ++y) {
      impulse_input_[x][y] = 0;
      for (int i = 0; i < num_users_; ++i) {
        impulse_input_[x][y] += pulsar_[i].grid_[x][y];
      }
    }
  }
  grid_.update(impulse_input_);
  
  openNIRecorder.update();
  openNIPlayer.update();
}

void testApp::drawWaves() {
  float intensity = 0;
  float scaled_velocity = 0;
  for (int x = 1; x < GRID_WIDTH - 1; ++x) {
    for (int y = 1; y < GRID_HEIGHT - 1; ++y) {
      intensity = 255 * CLAMP(grid_.getPosition(x, y) + 0.2, 0, 1);
      
      ofColor color(intensity, intensity, intensity);
      image_.setColor(x, y, color);
    }
  }
  
  image_.update();
  image_.draw(0, 0, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void testApp::draw(){
  ofPushMatrix();

  ofSetColor(255, 255, 255);
  drawWaves();

/*
  openNIRecorder.drawDebug(0, 0);
  openNIPlayer.drawDebug(0, 480);
*/
  
  Joint joint_order[] = { JOINT_RIGHT_HAND, JOINT_RIGHT_ELBOW, JOINT_RIGHT_SHOULDER,
                          JOINT_LEFT_SHOULDER, JOINT_LEFT_ELBOW, JOINT_LEFT_HAND };

  int num_users = openNIRecorder.getNumTrackedUsers();
  std::vector<ofPoint> points;
  
  for (int u = 0; u < num_users; ++u) {
    ofxOpenNIUser& user = openNIRecorder.getTrackedUser(u);

    for (int j = 0; j < sizeof(joint_order) / sizeof(Joint); ++j) {
      ofPoint joint_point = user.getJoint(joint_order[j]).getProjectivePosition();
      joint_point.z = 0;
      points.push_back(joint_point);
    }
  }
  if (points.size() == 0)
    return;
  
  ofPolyline ribbon;
  ribbon.curveTo(points[0]);
  for (int i = 0; i < points.size(); ++i) {
    ribbon.curveTo(points[i]);
    ofSetColor(255, 0, 0);
    ofCircle(points[i], 9);
  }
  ribbon.curveTo(points[points.size() - 1]);
      
  ofSetColor(0, 0, 255);
  ofSetLineWidth(10);
  ribbon.draw();
  
  // ofSetColor(200, 100, 255);
  // ofCircle(200, 200, 100);
  
  ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::userEvent(ofxOpenNIUserEvent & event){
  ofLogNotice() << getUserStatusAsString(event.userStatus) << "for user" << event.id << "from device" << event.deviceID;
}

//--------------------------------------------------------------
void testApp::gestureEvent(ofxOpenNIGestureEvent & event){
  ofLogNotice() << event.gestureName << getGestureStatusAsString(event.gestureStatus) << "from device" << event.deviceID << "at" << event.timestampMillis;
}

//--------------------------------------------------------------
void testApp::exit(){
  openNIRecorder.stop();
  openNIPlayer.stop();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

  int cloudRes = -1;
  switch (key) {
    case ' ':
      if(!openNIRecorder.isRecording()){
        openNIRecorder.startRecording(ofToDataPath("test.oni"));
      }else{
        openNIRecorder.stopRecording();
      }
      break;
    case 'p':
      openNIPlayer.startPlayer("test.oni");
      break;
    case '/':
      openNIPlayer.setPaused(!openNIPlayer.isPaused());
      break;
    case 'm':
      openNIPlayer.firstFrame();
      break;
    case '<':
    case ',':
      openNIPlayer.previousFrame();
      break;
    case '>':
    case '.':
      openNIPlayer.nextFrame();
      break;
    case 'x':
      openNIRecorder.stop();
      openNIPlayer.stop();
      break;
    case 't':
      openNIRecorder.toggleRegister();
      break;
  }

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}