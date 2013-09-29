//
//  Pulsar.cpp
//  openNiSample007
//
//  Created by Matt Tytel on 9/28/13.
//
//

#include "Pulsar.h"

Pulsar::Pulsar(float h, int r, float T, float d)
: h_(h)
, r_(r)
, T_(T)
, d_(d)
, x_(0)
, y_(0)
, t_(0) {
}

void Pulsar::update() {
  float time = ofGetElapsedTimef();
  if (time - t_ > T_) {
    t_ = time;
    pulse();
  } else {
    flatten_grid();
  }
}

void Pulsar::pulse() {
  flatten_grid();

  int square_left = CLAMP(x_ - r_, 0, GRID_WIDTH);
  int square_top = CLAMP(y_ - r_, 0, GRID_HEIGHT);
  int square_right = CLAMP(x_ + r_, 0, GRID_WIDTH);
  int square_bottom = CLAMP(y_ + r_, 0, GRID_HEIGHT);
  
  for (int x = square_left; x <= square_right; ++x) {
    for (int y = square_top; y <= square_bottom; ++y) {
      int dx = x - x_;
      int dy = y - y_;
      if (dx * dx + dy * dy < r_ * r_)
        grid_[x][y] = h_ * (1 - 1.0 * (dx * dx + dy * dy) / (r_ * r_));
    }
  }
}

void Pulsar::flatten_grid() {
  memset(grid_, 0, sizeof(float) * GRID_WIDTH * GRID_HEIGHT);
}