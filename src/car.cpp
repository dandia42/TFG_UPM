#pragma once
#include "car.h"

#include <stdio.h>
#include <string.h>

#include <iostream>
using namespace std;
Car::Car(Vector2D p, int d) {
  // kLength=4.1 	kWidth=1.8
  setPosition(p.x, p.y);
  setDirection(d);
  setVelocityDirection();
  turning_ = 1;  // rand()%3-1;cambiar
  setColor(1, 0, 1);
  auxiliarAngleDegrees_ = -90 * direction_;
}

Car::~Car() {
  cout << "\nDelete car";
}

void Car::draw() {
  glPushMatrix();
  glColor3f(r_, g_, b_);
  glDisable(GL_LIGHTING);
  glTranslatef(position_.x, position_.y, 0);
  glRotatef(auxiliarAngleDegrees_ - angleDegrees_ * turning_, 0, 0, 1);
  glBegin(GL_POLYGON);  // rectangulo
  glVertex3d(-kWidth / 2, -kLength / 2, 1);
  glVertex3d(-kWidth / 2, kLength / 2, 1);
  glVertex3d(kWidth / 2, kLength / 2, 1);
  glVertex3d(kWidth / 2, -kLength / 2, 1);
  glEnd();
  glColor3f(0, 0, 0);
  glBegin(GL_LINE_LOOP);  // octagono exterior
  glVertex3d(-kWidth / 2, -kLength / 2, 1.1);
  glVertex3d(-kWidth / 2, kLength / 2, 1.1);
  glVertex3d(kWidth / 2, kLength / 2, 1.1);
  glVertex3d(kWidth / 2, -kLength / 2, 1.1);
  glEnd();
  glColor3f(1, 1, 1);
  glBegin(GL_POLYGON);  // triangulo (orientation)
  glVertex3d(0, 0.5f, 1.5f);
  glVertex3d(-0.35f, -0.5f, 1.5f);
  glVertex3d(0.35f, -0.5f, 1.5f);
  glEnd();
  glPopMatrix();
}

void Car::move(float t) {
  position_ = position_ + velocity_ * t + acceleration_ * 0.5f * t * t;
  velocity_ = velocity_ + acceleration_ * t;

  if (stopFlag_) auxiliarTimer_++;    // 40 per second
  if (auxiliarTimer_ > (40 * 75)) {  // 75 seconds stopped
    changeStop(0);
    auxiliarTimer_ = 0;
    cout << "\n\tWay restarted internally\n";
  }
  ////////// TODO vigilar acceleration_
}

void Car::setPosition(float xPosition, float yPosition) {
  position_ = {xPosition, yPosition};
}
void Car::setVelocity(float xVelocity, float yVelocity) {
  velocity_ = {xVelocity, yVelocity};
}

void Car::setVelocity(int velocity) {
  defaultVelocity_ = velocity;
}

void Car::setDirection(int d) {
  direction_ = d;
}
int Car::getDirection() {
  static int aux;
  if (velocity_.y > 0 && velocity_.x < 0.06 && velocity_.x > -0.06) {
    aux = 0;
    return 0;
  }  // up
  else if (velocity_.x > 0 && velocity_.y < 0.06 && velocity_.y > -0.06) {
    aux = 1;
    return 1;
  }  // right
  else if (velocity_.y < 0 && velocity_.x < 0.06 && velocity_.x > -0.06) {
    aux = 2;
    return 2;
  }  // down
  else if (velocity_.x < 0 && velocity_.y < 0.06 && velocity_.y > -0.06) {
    aux = 3;
    return 3;
  }  // izqu
  else
    return aux;  // si se para, luego sigue con la direccion anterior
}

/*Comportamiento en los giros*/
void Car::turn() {
  angleRad_ += kDefaultAngle;
  angleDegrees_ = angleRad_ * 360 / 2 / PI;
  switch (direction_) {  // giros en funcion de la direccion del coche
    case 0:
      velocity_ = {turning_ * defaultVelocity_ * sin(angleRad_), defaultVelocity_ * cos(angleRad_)};
      break;
    case 1:
      velocity_ = {defaultVelocity_ * cos(angleRad_), -turning_ * defaultVelocity_ * sin(angleRad_)};
      break;
    case 2:
      velocity_ = {-turning_ * defaultVelocity_ * sin(angleRad_), -defaultVelocity_ * cos(angleRad_)};
      break;
    case 3:
      velocity_ = {-defaultVelocity_ * cos(angleRad_), turning_ * defaultVelocity_ * sin(angleRad_)};
      break;
  }

  if (angleDegrees_ > 89.3) {  // normalizar trayectoria a la salida del semaforo
    lastTurning_ = turning_;
    turning_ = 0;
    defaultVelocity_ = 1;
    direction_ = getDirection();
    setVelocityDirection();
    auxiliarAngleDegrees_ = -90 * direction_;
    angleRad_ = 0;
    angleDegrees_ = 0;
    // turning_=-turning_;//rand()%3-1;//decidir proximo turning_
    turningFlag_ = false;
    // flagsem=false;
  }
}
void Car::lateralMovement(int ncarril) {
  /*switch(direction_){		// turning depending on the car direction
  case 0:
  case 2:
          position_.x-=(0.01*ncarril);
  break;
  case 1:
  case 3:
          position_.y+=(0.01*ncarril);
  break;
  }*/
}
///////////////////////////////

/* Adjusting the trajectory when exiting the crossroad */
void Car::setExitPosition(Vector2D trafficlightPosition, int affectedLane_[4]) {
  if (pow(trafficlightPosition.x - position_.x, 2) > pow(trafficlightPosition.y - position_.y, 2))  // horizontal case
    if (trafficlightPosition.x > position_.x)                                    // car at the left of the traffic light
      switch (lastTurning_) {
        case -1:
          switch (affectedLane_[3]) {
            case 2:
              setPosition(position_.x, trafficlightPosition.y);
              break;
            case 4:
              setPosition(position_.x, trafficlightPosition.y - 1.5);
              break;
            case 5:
              setPosition(position_.x, trafficlightPosition.y + 1.5);
              break;
            case 7:
              setPosition(position_.x, trafficlightPosition.y - 3);
              break;
            case 8:
              setPosition(position_.x, trafficlightPosition.y + 3);
              break;
            case 9:
              setPosition(position_.x, trafficlightPosition.y);
              break;
            case 11:
              setPosition(position_.x, trafficlightPosition.y - 4.5);
              break;
            case 12:
              setPosition(position_.x, trafficlightPosition.y + 4.5);
              break;
            case 13:
              setPosition(position_.x, trafficlightPosition.y + 1.5);
              break;
            case 14:
              setPosition(position_.x, trafficlightPosition.y - 1.5);
              break;
          }
          break;
        case 1:
          switch (affectedLane_[3]) {
            case 2:
              setPosition(position_.x, trafficlightPosition.y);
              break;
            case 4:
              setPosition(position_.x, trafficlightPosition.y + 1.5);
              break;
            case 5:
              setPosition(position_.x, trafficlightPosition.y + 1.5);
              break;
            case 7:
              setPosition(position_.x, trafficlightPosition.y + 3);
              break;
            case 8:
              setPosition(position_.x, trafficlightPosition.y + 3);
              break;
            case 9:
              setPosition(position_.x, trafficlightPosition.y + 3);
              break;
            case 11:
              setPosition(position_.x, trafficlightPosition.y + 4.5);
              break;
            case 12:
              setPosition(position_.x, trafficlightPosition.y + 4.5);
              break;
            case 13:
              setPosition(position_.x, trafficlightPosition.y + 4.5);
              break;
            case 14:
              setPosition(position_.x, trafficlightPosition.y + 4.5);
              break;
          }
          break;
      }
    else  // car at the right of the traffic light
      switch (lastTurning_) {
        case -1:
          switch (affectedLane_[1]) {
            case 1:
              setPosition(position_.x, trafficlightPosition.y);
              break;
            case 3:
              setPosition(position_.x, trafficlightPosition.y + 1.5);
              break;
            case 5:
              setPosition(position_.x, trafficlightPosition.y - 1.5);
              break;
            case 6:
              setPosition(position_.x, trafficlightPosition.y + 3);
              break;
            case 8:
              setPosition(position_.x, trafficlightPosition.y);
              break;
            case 9:
              setPosition(position_.x, trafficlightPosition.y - 3);
              break;
            case 10:
              setPosition(position_.x, trafficlightPosition.y + 4.5);
              break;
            case 12:
              setPosition(position_.x, trafficlightPosition.y + 1.5);
              break;
            case 13:
              setPosition(position_.x, trafficlightPosition.y - 1.5);
              break;
            case 14:
              setPosition(position_.x, trafficlightPosition.y - 4.5);
              break;
          }
          break;
        case 1:
          switch (affectedLane_[1]) {
            case 1:
              setPosition(position_.x, trafficlightPosition.y);
              break;
            case 3:
              setPosition(position_.x, trafficlightPosition.y - 1.5);
              break;
            case 5:
              setPosition(position_.x, trafficlightPosition.y - 1.5);
              break;
            case 6:
              setPosition(position_.x, trafficlightPosition.y - 3);
              break;
            case 8:
              setPosition(position_.x, trafficlightPosition.y - 3);
              break;
            case 9:
              setPosition(position_.x, trafficlightPosition.y - 3);
              break;
            case 10:
              setPosition(position_.x, trafficlightPosition.y - 4.5);
              break;
            case 12:
              setPosition(position_.x, trafficlightPosition.y - 4.5);
              break;
            case 13:
              setPosition(position_.x, trafficlightPosition.y - 4.5);
              break;
            case 14:
              setPosition(position_.x, trafficlightPosition.y - 4.5);
              break;
          }
          break;
      }

  else                      // vertical case
    if (trafficlightPosition.y > position_.y)  // car at the bottom of the traffic light
      switch (lastTurning_) {
        case -1:
          switch (affectedLane_[2]) {
            case 2:
              setPosition(trafficlightPosition.x, position_.y);
              break;
            case 4:
              setPosition(trafficlightPosition.x + 1.5, position_.y);
              break;
            case 5:
              setPosition(trafficlightPosition.x - 1.5, position_.y);
              break;
            case 7:
              setPosition(trafficlightPosition.x + 3, position_.y);
              break;
            case 8:
              setPosition(trafficlightPosition.x - 3, position_.y);
              break;
            case 9:
              setPosition(trafficlightPosition.x, position_.y);
              break;
            case 11:
              setPosition(trafficlightPosition.x + 4.5, position_.y);
              break;
            case 12:
              setPosition(trafficlightPosition.x - 4.5, position_.y);
              break;
            case 13:
              setPosition(trafficlightPosition.x - 1.5, position_.y);
              break;
            case 14:
              setPosition(trafficlightPosition.x + 1.5, position_.y);
              break;
          }
          break;
        case 1:
          switch (affectedLane_[2]) {
            case 2:
              setPosition(trafficlightPosition.x, position_.y);
              break;
            case 4:
              setPosition(trafficlightPosition.x - 1.5, position_.y);
              break;
            case 5:
              setPosition(trafficlightPosition.x - 1.5, position_.y);
              break;
            case 7:
              setPosition(trafficlightPosition.x - 3, position_.y);
              break;
            case 8:
              setPosition(trafficlightPosition.x - 3, position_.y);
              break;
            case 9:
              setPosition(trafficlightPosition.x - 3, position_.y);
              break;
            case 11:
              setPosition(trafficlightPosition.x - 4.5, position_.y);
              break;
            case 12:
              setPosition(trafficlightPosition.x - 4.5, position_.y);
              break;
            case 13:
              setPosition(trafficlightPosition.x - 4.5, position_.y);
              break;
            case 14:
              setPosition(trafficlightPosition.x - 4.5, position_.y);
              break;
          }
          break;
      }
    else  // car at the top of the traffic light
      switch (lastTurning_) {
        case -1:
          switch (affectedLane_[0]) {
            case 1:
              setPosition(trafficlightPosition.x, position_.y);
              break;
            case 3:
              setPosition(trafficlightPosition.x - 1.5, position_.y);
              break;
            case 5:
              setPosition(trafficlightPosition.x + 1.5, position_.y);
              break;
            case 6:
              setPosition(trafficlightPosition.x - 3, position_.y);
              break;
            case 8:
              setPosition(trafficlightPosition.x, position_.y);
              break;
            case 9:
              setPosition(trafficlightPosition.x + 3, position_.y);
              break;
            case 10:
              setPosition(trafficlightPosition.x - 4.5, position_.y);
              break;
            case 12:
              setPosition(trafficlightPosition.x - 1.5, position_.y);
              break;
            case 13:
              setPosition(trafficlightPosition.x + 1.5, position_.y);
              break;
            case 14:
              setPosition(trafficlightPosition.x + 4.5, position_.y);
              break;
          }
          break;
        case 1:
          switch (affectedLane_[0]) {
            case 1:
              setPosition(trafficlightPosition.x, position_.y);
              break;
            case 3:
              setPosition(trafficlightPosition.x + 1.5, position_.y);
              break;
            case 5:
              setPosition(trafficlightPosition.x + 1.5, position_.y);
              break;
            case 6:
              setPosition(trafficlightPosition.x + 3, position_.y);
              break;
            case 8:
              setPosition(trafficlightPosition.x + 3, position_.y);
              break;
            case 9:
              setPosition(trafficlightPosition.x + 3, position_.y);
              break;
            case 10:
              setPosition(trafficlightPosition.x + 4.5, position_.y);
              break;
            case 12:
              setPosition(trafficlightPosition.x + 4.5, position_.y);
              break;
            case 13:
              setPosition(trafficlightPosition.x + 4.5, position_.y);
              break;
            case 14:
              setPosition(trafficlightPosition.x + 4.5, position_.y);
              break;
          }
          break;
      }
}

void Car::setVelocityDirection() {
  switch (direction_) {  // Velocity depending on the direction
    case 0:
      setVelocity(0, defaultVelocity_);
      break;
    case 1:
      setVelocity(defaultVelocity_, 0);
      break;
    case 2:
      setVelocity(0, -defaultVelocity_);
      break;
    case 3:
      setVelocity(-defaultVelocity_, 0);
      break;
  }
}

void Car::setColor(float red, float green, float blue) {
  r_ = red;
  g_ = green;
  b_ = blue;
}
void Car::setTurn(int g_) {  // Set turn using w,a,d
  turning_ = g_;
}
int Car::getTurning() {
  return turning_;
}
void Car::changeStop(bool x) {
  stopFlag_ = x;
  if (x)
    defaultVelocity_ = 0;
  else {
    auxiliarTimer_ = 0;
    defaultVelocity_ = 1;
  }
  setVelocityDirection();
}
bool Car::getStop() {
  return stopFlag_;
}
Vector2D Car::getPosition() {
  return position_;
}
Vector2D Car::getVelocity() {
  return velocity_;
}
float Car::getAngleRad() {
  return angleRad_;
}
float Car::getAngleDegrees() {
  return angleDegrees_;
}
bool Car::getTurningFlag() {
  return turningFlag_;
}
bool Car::getLateralFlag() {
  return lateralFlag_;
}
void Car::setLateralFlag(bool lateralFlag) {
  lateralFlag_ = lateralFlag;
}
void Car::setInitialTime(double time) {
  initialRouteTime_ = time;
}