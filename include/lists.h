#pragma once
#include "street.h"
#include "car.h"
#include "trafficlight.h"
#define MAX_ELEM 1000

class CarList {
  Car* carList_[MAX_ELEM];
  int number_ = 0;

 public:
  CarList();
  void draw();
  void setPosition(float, float);
  void move(float);
  bool add(Car*);
  void destruirContenido();
  void deleteIndex(int);
  void eliminar(Car*);

  void setgiro(int);

  friend class World;  // TODO por ahora
  friend class Interaction;
  friend class Editor;
};

class StreetList {
  Street* streetList_[MAX_ELEM];
  int number_ = 0;

 public:
  StreetList();
  void draw();
  void setPosition(float, float);
  bool add(Street*);
  void destruirContenido();
  void deleteIndex(int);
  void eliminar(Street*);

  int getLength();

  friend class World;
  friend class Interaction;
  friend class Editor;
  friend class Graph;
  friend class Algorithm;
};

class TrafficlightList {
  Trafficlight* trafficlightList_[MAX_ELEM];
  int number_ = 0;

 public:
  TrafficlightList();
  void draw();
  // void onTimer(int);
  void setPosition(float, float);
  bool add(Trafficlight*);
  void destruirContenido();
  void deleteIndex(int);
  void eliminar(Trafficlight*);

  friend class Interaction;
  friend class Editor;
  friend class World;
  friend class Graph;
};
