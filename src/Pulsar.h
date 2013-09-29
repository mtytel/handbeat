//
//  Pulsar.h
//  openNiSample007
//
//  Created by Noura Howell on 9/28/13.
//
//

#ifndef __openNiSample007__Pulsar__
#define __openNiSample007__Pulsar__

#include <iostream>
#include "WaveGrid.h"
#include "ofMain.h"

class Pulsar {

public:

  Pulsar(float h = 1.0, int r = 5, float T = 1.0, float d = 0.1);
  
  void update(); // will pulse periodically
  void pulse();  // will force it to pulse immediately
  
  float h_; // height of pulse
  int r_; // radius of pulse
  int x_; // x position of center of pulsar
  int y_; // y position of center of pulsar
  float T_; // period of pulses
  float d_; // duration of pulse
  float grid_[GRID_WIDTH][GRID_HEIGHT];
  
protected:

  void flatten_grid();
  float t_; // start time of last pulse
  
};

#endif /* defined(__openNiSample007__Pulsar__) */
