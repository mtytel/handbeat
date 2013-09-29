//
//  WaveGrid.h
//  openNiSample007
//
//  Created by Matt Tytel on 9/28/13.
//
//

#ifndef __openNiSample007__WaveGrid__
#define __openNiSample007__WaveGrid__

#include <iostream>

#define GRID_WIDTH 640
#define GRID_HEIGHT 400

class __declspec(align(64)) WaveGrid {
public:
  WaveGrid();
  
  void update(float input[GRID_WIDTH][GRID_HEIGHT]);
  void setRigidness(float rigidness) { rigidness_ = rigidness; }
  void setDamping(float damping) { damping_ = damping; }
  
  inline float getPosition(int x, int y) { return positions_[x][y]; }
  inline float getVelocity(int x, int y) { return velocities_[x][y]; }
  
private:
  float positions_[GRID_WIDTH][GRID_HEIGHT];
  float velocities_[GRID_WIDTH][GRID_HEIGHT];
  float impulses_[GRID_WIDTH][GRID_HEIGHT];
  
  float damping_;
  float rigidness_;
};

#endif /* defined(__openNiSample007__WaveGrid__) */
