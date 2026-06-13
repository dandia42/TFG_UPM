#pragma once
#include "trafficlight.h"

Trafficlight::Trafficlight(float px, float py, int l) {
  setPosition(px, py);
  state_ = rand() % 4;  // 0 TODO quitar este comentario?
  setTimer(rand() % 200 + 400, rand() % 160 + 320, rand() % 200 + 400, rand() % 160 + 320);
}

Trafficlight::~Trafficlight() {}

void Trafficlight::draw() {
  glPushMatrix();
  glDisable(GL_LIGHTING);
  glTranslatef(position_.x, position_.y, 0.5f);

  glBegin(GL_POLYGON);  // interior octagon
  glColor3f(1, 0, 0);
  glVertex3d(kSize * cos(3 * PI / 8), -kSize * sin(3 * PI / 8), 0);
  glVertex3d(kSize * cos(PI / 8), -kSize * sin(PI / 8), 0);
  glVertex3d(kSize * cos(PI / 8), kSize * sin(PI / 8), 0);
  glVertex3d(kSize * cos(3 * PI / 8), kSize * sin(3 * PI / 8), 0);
  glVertex3d(-kSize * cos(3 * PI / 8), kSize * sin(3 * PI / 8), 0);
  glVertex3d(-kSize * cos(PI / 8), kSize * sin(PI / 8), 0);
  glVertex3d(-kSize * cos(PI / 8), -kSize * sin(PI / 8), 0);
  glVertex3d(-kSize * cos(3 * PI / 8), -kSize * sin(3 * PI / 8), 0);
  glEnd();
  glRotatef(orientation_, 0, 0, 1);
  glBegin(GL_POLYGON);  // rectangle
  glColor3f(redState_, greenState_, 0);
  glVertex3d(kSize * cos(3 * PI / 8), -kSize * sin(3 * PI / 8), 0.1f);
  glVertex3d(kSize * cos(3 * PI / 8), kSize * sin(3 * PI / 8), 0.1f);
  glVertex3d(-kSize * cos(3 * PI / 8), kSize * sin(3 * PI / 8), 0.1f);
  glVertex3d(-kSize * cos(3 * PI / 8), -kSize * sin(3 * PI / 8), 0.1f);
  glEnd();
  if (stateMode_ == 3 || stateMode_ == 4) {
    glColor3f(1, 1, 0);
    glBegin(GL_LINE_LOOP);  // exterior octagon
    glVertex3d(14 * cos(3 * PI / 8), -14 * sin(3 * PI / 8), 0);
    glVertex3d(14 * cos(PI / 8), -14 * sin(PI / 8), 0);
    glVertex3d(14 * cos(PI / 8), 14 * sin(PI / 8), 0);
    glVertex3d(14 * cos(3 * PI / 8), 14 * sin(3 * PI / 8), 0);
    glVertex3d(-14 * cos(3 * PI / 8), 14 * sin(3 * PI / 8), 0);
    glVertex3d(-14 * cos(PI / 8), 14 * sin(PI / 8), 0);
    glVertex3d(-14 * cos(PI / 8), -14 * sin(PI / 8), 0);
    glVertex3d(-14 * cos(3 * PI / 8), -14 * sin(3 * PI / 8), 0);
    glEnd();
  } else if (stateMode_ == 1 || stateMode_ == 2) {
    glColor3f(1, 0, 1);
    glBegin(GL_LINE_LOOP);  // exterior octagon
    glVertex3d(14 * cos(3 * PI / 8), -14 * sin(3 * PI / 8), 0);
    glVertex3d(14 * cos(PI / 8), -14 * sin(PI / 8), 0);
    glVertex3d(14 * cos(PI / 8), 14 * sin(PI / 8), 0);
    glVertex3d(14 * cos(3 * PI / 8), 14 * sin(3 * PI / 8), 0);
    glVertex3d(-14 * cos(3 * PI / 8), 14 * sin(3 * PI / 8), 0);
    glVertex3d(-14 * cos(PI / 8), 14 * sin(PI / 8), 0);
    glVertex3d(-14 * cos(PI / 8), -14 * sin(PI / 8), 0);
    glVertex3d(-14 * cos(3 * PI / 8), -14 * sin(3 * PI / 8), 0);
    glEnd();
  }
  glPopMatrix();
}

void Trafficlight::setPosition(float px, float py) {
  position_.x = px;
  position_.y = py;
}

bool Trafficlight::getOutDirection(int x) {
  return outStreet_[x];
}

void Trafficlight::stateMachine() {
  if (temp_ >= 0) {
    switch (state_) {                  // State machine
      case 0:                       // vertical green-horizontal red
        if (temp_ == 0) temp_ = verticalGreenTime_;  // timer
        redState_ = 0;                   // color
        greenState_ = 1;                   // color
        orientation_ = 0;                     // vertical
        if (temp_ == 1) setState(1);     // change state
        break;
      case 1:  // vertical amber-horizontal red
        if (temp_ == 0) temp_ = verticalAmberTime_;
        redState_ = 1;
        greenState_ = 1;
        if (temp_ == 1) setState(2);
        break;
      case 2:  // vertical red-horizontal green
        if (temp_ == 0) temp_ = horizontalGreenTime_;
        redState_ = 0;
        greenState_ = 1;
        orientation_ = 90;  // horizontal
        if (temp_ == 1) setState(3);
        break;
      case 3:  // vertical red-horizontal amber
        if (temp_ == 0) temp_ = horizontalAmberTime_;
        redState_ = 1;
        greenState_ = 1;
        if (temp_ == 1) setState(0);
        break;
    }
    auxiliarTimer_++;
    temp_--;
  } else {
  }  // mantenerse en el estado fijo porque se le pasará -1 al time_ del estado y 1 a los demás tiempos para que llegue
     // solo siguiendo la máquina de estados    TODO quesesto
}

void Trafficlight::setTimer(int a, int b, int c, int d) {
  verticalGreenTime_ = a;
  verticalAmberTime_ = b;
  horizontalGreenTime_ = c;
  horizontalAmberTime_ = d;
}

void Trafficlight::setState(int i) {
  if(i >= 0 && i <= 3)  
    state_ = i;
  else {
    cout << "ERROR setState (0-3) traffic light, state: " << i << endl;
    int error;
    cin >> error;
  }
}

/*Returns time of each state*/
int Trafficlight::getTimer(int x) {
  switch (x) {
    case 0:
      return verticalGreenTime_;
      break;
    case 1:
      return verticalAmberTime_;
      break;
    case 2:
      return horizontalGreenTime_;
      break;
    case 3:
      return horizontalAmberTime_;
      break;
    default:
      return -1;
  }
}

int Trafficlight::getStateTime(int i) {
  switch (i) {
    case 0:
      return verticalGreenTime_;
      break;
    case 1:
      return verticalAmberTime_;
      break;
    case 2:
      return horizontalGreenTime_;
      break;
    case 3:
      return horizontalAmberTime_;
      break;
    default:
      cout << "ERROR getStateTime index out of range" << endl;
  }
} 

/*Initialize traffic light state, after Interaction::callesem and Interaction::unreachableCheck*/
void Trafficlight::initialLightsState() {
  int bitmask = outStreet_[0] * 1 + outStreet_[1] * 2 + outStreet_[2] * 4 +
                outStreet_[3] * 8;  // This value depends on surrounding streets

  switch (bitmask) {
    case 3:
    case 6:
    case 9:
    case 12:
      setStateMode(0);
      break;
    case 1:
    case 4:
      setStateMode(1);
      break;
    case 2:
    case 8:
      setStateMode(2);
      break;
    case 5:
    case 7:
    case 13:
      setStateMode(3);
      break;
    case 10:
    case 11:
    case 14:
      setStateMode(4);
      break;
    default:
      cout << "\n\tERROR WITH IN/OUT ASSIGNATION";
  }
}
//////////////////////////////////////////
