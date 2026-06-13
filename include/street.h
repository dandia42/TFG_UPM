#pragma once
#include "vector2d.h"
#include "glut.h"

class Street {
  Vector2D position_;
  int r_, g_, b_;
  int lanes_ = 1;   // Lanes number
  int type_;           // Check notes, types 1 to 14
  int direction_ = 0;  // 0=vertical    1=horizontal
  int orientation_ = 0;    // orientation     0=up/right      1=down/left
  int length_ = 0;
  float width_ = 3;         // 3 each lane
  int maxVelocity_ = -1, averageVelocity_ = -1;  ////////velocity_ maxima, media, desviacion tipica TODO se usa? son constantes?
  // Vector2D zona[2];		//0 left/up	1 right/down
  int node_[2] = {-1, -1}, occupancy_ = 0;  // node? cars occupancy
  bool inEdge_ = false;

 public:
  Street(Vector2D, int);
  Street(Vector2D, int, int, bool);
  ~Street();
  void setPosition(float, float);
  void setDirection(int);
  int getDirection();
  void setOrientation(int s) {
    orientation_ = s;
  }
  int getOrientation() {
    return orientation_;
  }
  void draw();
  void setColor(int red, int green, int blue) {
    r_ = red;
    g_ = green;
    b_ = blue;
  }

  void setLength(int);
  float getLength();
  void setLanes();
  void drawLanes();
  void setType(int t) {
    type_ = t;
  }
  int getType() {
    return type_;
  }
  Vector2D getPosition() {
    return position_;
  }
  // void setZonas();
  // Vector2D getZonas(int);
  void setNode(int, int);
  int* getNode() {
    return node_;
  }
  void setOccupancy(int o) {
    occupancy_ = o;
  }
  int getOccupancy() {
    return occupancy_;
  }
  void setEdge(bool aux) {
    inEdge_ = aux;
  }
  bool getEdge() {
    return inEdge_;
  }
  friend class Interaction;
  friend class Editor;
};
