//
//  WaveGrid.cpp
//  openNiSample007
//
//  Created by Matt Tytel on 9/28/13.
//
//

#include "WaveGrid.h"
#include "ofMain.h"

#include <Accelerate/Accelerate.h>

WaveGrid::WaveGrid() : damping_(1), rigidness_(1) {
  for (int x = 0; x < GRID_WIDTH; ++x) {
    for (int y = 0; y < GRID_HEIGHT; ++y) {
      positions_[x][y] = 0.0;
      velocities_[x][y] = 0.0;
      impulses_[x][y] = 0.0;
    }
  }
}

void WaveGrid::update(float input[GRID_WIDTH][GRID_HEIGHT]) {
  for (int x = 1; x < GRID_WIDTH - 1; ++x) {
    for (int y = 1; y < GRID_HEIGHT - 1; ++y) {
      impulses_[x][y] = input[x][y] / 10 + (positions_[x - 1][y] + positions_[x + 1][y] +
                                            positions_[x][y - 1] + positions_[x][y + 1]);
    }
  }
  
  vSscal(GRID_WIDTH * GRID_HEIGHT, 0.25, (vFloat*)impulses_);
  vSgesub(GRID_WIDTH, GRID_HEIGHT, (vFloat*)impulses_, 'N', (vFloat*)positions_, 'N', (vFloat*)impulses_);
  
  vSscal(GRID_WIDTH * GRID_HEIGHT, rigidness_, (vFloat*)impulses_);
  vSgeadd(GRID_WIDTH, GRID_HEIGHT, (vFloat*)impulses_, 'N', (vFloat*)velocities_, 'N', (vFloat*)velocities_);
  vSscal(GRID_WIDTH * GRID_HEIGHT, damping_, (vFloat*)velocities_);
  vSgeadd(GRID_WIDTH, GRID_HEIGHT, (vFloat*)velocities_, 'N', (vFloat*)positions_, 'N', (vFloat*)positions_);
}
