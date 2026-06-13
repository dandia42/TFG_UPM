#pragma once
#include <stdlib.h>

#include <iostream>

#include "vector2d.h"
#include "glut.h"

using namespace std;

class Trafficlight {
  Vector2D position_;
  int state_ = 0, redState_, greenState_;  // 0vv-rh 1av-rh 2rv-vh 3rv-ah TODO enum de esto
  int affectedLane_[4], streetIndex_[4];  // Street type, street index     0-up 1-right 2-down 3-left
  int temp_ = 0, auxiliarTimer_ = 0;  // States timer
  bool outStreet_[4];  // up right down left
  int kSize = 8, orientation_ = 0, sizeaux = 1;  // orientation_=horizontal o vertical TODO sizeaux pa que
  int stateMode_ = 0;  // 0 normal, 1 variable with more horizontal time, 2 variable with more vertical time
                        // 3 horizontal fixed, 4 vertical fixed TODO enum de esto
  int verticalGreenTime_, verticalAmberTime_, horizontalGreenTime_, horizontalAmberTime_;  // States time

 public:
  Trafficlight(float, float, int);
  ~Trafficlight();
  void setPosition(float, float);
  void setState(int);
  int getState() {
    return state_;
  }
  bool getOutDirection(int);  // return exit direction

  void draw();
  void onTimer(int);  // traffic light behaviour

  void setTimer(int, int, int, int);  // 0 to hold state, 1 to change state
  int getTimer(int);
  void setTemp(int x, bool f) {
    f ? auxiliarTimer_ = x : temp_ = x;
  }
  int getTemp(int x) {
    return x ? auxiliarTimer_ : temp_;
  }
  int getCID(int x) {
    return streetIndex_[x];
  }
  void stateMachine();
  void initialLightsState();
  void setStateMode(int x) {
    stateMode_ = x;
  }
  int getStateMode() {
    return stateMode_;
  }
  int getStateTime(int);

  Vector2D getPosition() {
    return position_;
  }

  friend class Interaction;
  friend class Editor;
};
