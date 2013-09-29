#ifndef _TEST_APP
#define _TEST_APP

#include "ofxOpenNI.h"
#include "ofMain.h"

#include "WaveGrid.h"
#include "Pulsar.h"

#define MAX_USERS 2

class testApp : public ofBaseApp{

public:
  
  void setup();
  void update();
  void draw();
  void drawWaves();
  void exit();
  
  void keyPressed  (int key);
  void keyReleased(int key);
  void mouseMoved(int x, int y );
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void windowResized(int w, int h);

  ofxOpenNI openNIRecorder;
  ofxOpenNI openNIPlayer;
  
  ofTrueTypeFont verdana;
  
  void userEvent(ofxOpenNIUserEvent & event);
  void gestureEvent(ofxOpenNIGestureEvent & event);
  
private:
  WaveGrid grid_;
  ofImage image_;
  Pulsar pulsar_[MAX_USERS];
  float impulse_input_[GRID_WIDTH][GRID_HEIGHT];
  int num_users_;
};

#endif
