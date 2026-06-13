#pragma once
#include "vector2d.h"
#include "glut.h"
class Car { // TODO revisar asignacion aqui o en constructor
  float kLength = 4, kWidth = 1.8;  // length=4 	width=1.8
  bool turningFlag_ = false, trafficlightFlag_ = false, auxiliarTrafficlightFlag_ = false;
  bool stopFlag_ = false, lateralFlag_ = false;  // traffic light flag
  float defaultVelocity_ = 1, kDefaultAngle = 0.0125;  // default velocity and angle
  Vector2D position_, velocity_, acceleration_{0, 0};
  int turning_ = 1, lastTurning_ = 0;  //-1=left    0=straight    1=right

  float angleRad_ = 0, angleDegrees_ = 0, auxiliarAngleDegrees_ = 0;  // rad and degrees angle
  int safetyDistance_; // safety distance
  int direction_; // TODO enum 0=up    1=right    2=down    3=left
  float r_, g_, b_; // color
  int* route_;
  int numberOfTurns_ = 0, actualTurn_ = 0;
  double initialRouteTime_ = 0, endRouteTime_ = 0;  // timer to finish route
  int auxiliarTimer_ = 0;

 public:
  int n[2];  // current lane [0] and next lane [1]
  bool debugFlag_ = false;

  Car(Vector2D, int);
  ~Car();
  void setPosition(float, float);
  void setVelocity(float, float);
  void setVelocity(int);
  void setVelocityDirection();
  void setDirection(int);
  int getDirection();
  void draw();
  void move(float);
  void turn();  // Turning behaviour
  void lateralMovement(int);
  void setExitPosition(Vector2D, int*);
  void changeStop(bool);
  bool getStop();
  void setColor(float, float, float);
  void setTurn(int);  // Set car turn with w,a,d
  int getTurning();
  Vector2D getPosition();
  Vector2D getVelocity();
  float getAngleRad();
  float getAngleDegrees();
  bool getTurningFlag();
  bool getLateralFlag();
  void setLateralFlag(bool);
  void setInitialTime(double);
  friend class Interaction;
  friend class Editor;
};
