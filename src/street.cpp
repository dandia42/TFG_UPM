#pragma once
#include "street.h"
void drawOrientation(int, float);
void drawTriangle(float, int, float);
void drawZones(int, Vector2D*);
Street::Street(Vector2D position, int direction) {
  setPosition(position.x, position.y);
  setDirection(direction);
  setLength(30);
  type_ = rand() % 2 + 1;  // rand()%14+1; //cambiar
  setLanes();
  setColor(0, 0, 1);
  if (position.x < 0 || position.y < 0) {
    bool edge = true;
    setEdge(edge);
  }
  // setZonas();
}
Street::Street(Vector2D position, int direction, int type, bool edge) {
  setPosition(position.x, position.y);
  setDirection(direction);
  setLength(30);
  type_ = type;
  setLanes();
  setColor(0, 0, 1);
  if (position.x < 0 || position.y < 0) edge = true;
  setEdge(edge);
  // setZonas();
}

Street::~Street() {}

void Street::draw() {
  glPushMatrix();
  glColor3f(r_, g_, b_);
  glDisable(GL_LIGHTING);
  glTranslatef(position_.x, position_.y, 0);
  glRotatef(-90 * direction_, 0, 0, 1);  // positivo es orientation antihorario
  glBegin(GL_POLYGON);                  // rectangulo
  glVertex3d(-width_ / 2 * lanes_, -length_ / 2, 0);
  glVertex3d(-width_ / 2 * lanes_, length_ / 2, 0);
  glVertex3d(width_ / 2 * lanes_, length_ / 2, 0);
  glVertex3d(width_ / 2 * lanes_, -length_ / 2, 0);
  glEnd();
  glColor3f(1, 1, 1);
  drawLanes();  // dibuja contornos de lanes_

  drawOrientation(type_, width_);
  glRotatef(90 * direction_, 0, 0, 1);
  glTranslatef(-position_.x, -position_.y, 0);
  // for(int i=0;i<2;i++)
  // drawZones(i,zona);

  setLanes();
  glPopMatrix();
}
void drawOrientation(int type_, float kWidth) {
  switch (type_) {
    case 6:
      drawTriangle(1, 0, kWidth);
      drawTriangle(-1, 0, kWidth);
      drawTriangle(0, 0, kWidth);
      break;
    case 8:
      drawTriangle(1, 0, kWidth);
      drawTriangle(-1, 1, kWidth);
    case 1:
      drawTriangle(0, 0, kWidth);
      break;
    case 7:
      drawTriangle(-1, 1, kWidth);
      drawTriangle(1, 1, kWidth);
      drawTriangle(0, 1, kWidth);
      break;
    case 9:
      drawTriangle(-1, 1, kWidth);
      drawTriangle(1, 0, kWidth);
    case 2:
      drawTriangle(0, 1, kWidth);
      break;
    case 12:
      drawTriangle(1.5, 0, kWidth);
      drawTriangle(-1.5, 1, kWidth);
      drawTriangle(0.5, 0, kWidth);
      drawTriangle(-0.5, 0, kWidth);
      break;
    case 10:
      drawTriangle(1.5, 0, kWidth);
      drawTriangle(-1.5, 0, kWidth);
    case 3:
      drawTriangle(0.5, 0, kWidth);
      drawTriangle(-0.5, 0, kWidth);
      break;
    case 14:
      drawTriangle(-1.5, 1, kWidth);
      drawTriangle(1.5, 0, kWidth);
      drawTriangle(0.5, 1, kWidth);
      drawTriangle(-0.5, 1, kWidth);
      break;
    case 11:
      drawTriangle(1.5, 1, kWidth);
      drawTriangle(-1.5, 1, kWidth);
    case 4:
      drawTriangle(0.5, 1, kWidth);
      drawTriangle(-0.5, 1, kWidth);
      break;
    case 13:
      drawTriangle(1.5, 0, kWidth);
      drawTriangle(-1.5, 1, kWidth);
    case 5:
      drawTriangle(0.5, 0, kWidth);
      drawTriangle(-0.5, 1, kWidth);
      break;
  }
}
void drawTriangle(float aux, int turning_, float kWidth) {
  glTranslatef(kWidth * aux, 0, 0.2f);
  glRotatef(180 * turning_, 0, 0, 1);
  glBegin(GL_POLYGON);  // triangulo (orientation)
  glVertex3d(0, 6, 0.5f);
  glVertex3d(-0.8, -6, 0.5f);
  glVertex3d(0.8, -6, 0.5f);
  glEnd();
  glRotatef(180 * turning_, 0, 0, 1);
  glTranslatef(-kWidth * aux, 0, -0.2f);
}
/*void drawZones(int i, Vector2D* zona){
        glTranslatef(zona[i].x,zona[i].y,0.2f);
        glBegin(GL_POLYGON);
                glVertex3d(-1,-1,0);
                glVertex3d(-1,1,0);
                glVertex3d(1,1,0);
                glVertex3d(1,-1,0);
        glEnd();
        glTranslatef(-zona[i].x,-zona[i].y,-0.2f);
}*/

void Street::drawLanes() {
  switch (lanes_) {
    case 4:
      glBegin(GL_LINES);
      glVertex3d(-width_, -length_ / 2, 0.1f);
      glVertex3d(-width_, length_ / 2, 0.1f);
      glVertex3d(width_, length_ / 2, 0.1f);
      glVertex3d(width_, -length_ / 2, 0.1f);
      glEnd();
    case 2:
      glBegin(GL_LINES);
      glVertex3d(0, -length_ / 2, 0.1f);
      glVertex3d(0, length_ / 2, 0.1f);
      glEnd();
    case 1:
      glBegin(GL_LINE_LOOP);
      glVertex3d(-width_ / 2 * lanes_, -length_ / 2, 0.1f);
      glVertex3d(-width_ / 2 * lanes_, length_ / 2, 0.1f);
      glVertex3d(width_ / 2 * lanes_, length_ / 2, 0.1f);
      glVertex3d(width_ / 2 * lanes_, -length_ / 2, 0.1f);
      glEnd();
      break;
    case 3:
      glBegin(GL_LINES);
      glVertex3d(-width_ / 2, -length_ / 2, 0.1f);
      glVertex3d(-width_ / 2, length_ / 2, 0.1f);
      glVertex3d(width_ / 2, length_ / 2, 0.1f);
      glVertex3d(width_ / 2, -length_ / 2, 0.1f);
      glEnd();

      glBegin(GL_LINE_LOOP);
      glVertex3d(-width_ / 2 * lanes_, -length_ / 2, 0.1f);
      glVertex3d(-width_ / 2 * lanes_, length_ / 2, 0.1f);
      glVertex3d(width_ / 2 * lanes_, length_ / 2, 0.1f);
      glVertex3d(width_ / 2 * lanes_, -length_ / 2, 0.1f);
      glEnd();
      break;
  }
}

void Street::setPosition(float px, float py) {
  position_.x = px;
  position_.y = py;
}
void Street::setDirection(int direction) {
  direction_ = direction;
}
int Street::getDirection() {
  return direction_;
}
void Street::setLength(int i) {
  length_ = i;
}
float Street::getLength() {
  return length_;
}

void Street::setLanes() {
  switch (type_) {
    case 1:
    case 2:
      lanes_ = 1;
      break;
    case 3:
    case 4:
    case 5:
      lanes_ = 2;
      break;
    case 6:
    case 7:
    case 8:
    case 9:
      lanes_ = 3;
      break;
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
      lanes_ = 4;
      break;
  }
}

/*void Street::setZonas(){
        zona[0].x=direction_?(position_.x):(position_.x-6);
        zona[0].y=direction_?(position_.y+6):(position_.y);
        zona[1].x=direction_?(position_.x):(position_.x+6);
        zona[1].y=direction_?(position_.y-6):(position_.y);
}
Vector2D Street::getZonas(int i){
        return zona[i];
}*/

void Street::setNode(int n, int i) {
  node_[i] = n;
}
