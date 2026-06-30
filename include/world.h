#pragma once
#if !defined(AFX_MUNDO_H__9510340A_3D75_485F_93DC_302A43B8039A__INCLUDED_)
#define AFX_MUNDO_H__9510340A_3D75_485F_93DC_302A43B8039A__INCLUDED_

#include <vector>

#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER > 1000
#endif  // AFX_MUNDO_H

#include <GL/freeglut.h>

#include "vector2d.h"
#include "editor.h"
#include "glut.h"
#include "graph.h"
#include "interaction.h"
#include "lists.h"
//#include "ETSIDI.h"
#include <time.h>

#include <iostream>

#include "algorithms.h"
using namespace std;

class World {
 public:
  void init();
  World();
  virtual ~World();

  void initGL();
  void onKeyboardDown(unsigned char, int, int);
  void onTimer();
  void onDraw();
  void worldGeneration(int, int);

  int rows_ = -1, columns_ = -1;
  int auxiliarFlag_ = 0;
  int xPosition_ = 0, yPosition_ = 0;
  int routeOptimization_ = -1, trafficlightOptimization_ = -1;
  float zPosition_ = 0;
  int iterations_ = 0, *pIterations_ = &iterations_;

  double time_;

  Street* auxiliarStreet_ = nullptr;
  CarList cars_, *pCars_ = &cars_;
  StreetList streets_, *pStreets_ = &streets_;
  TrafficlightList trafficlights_, *pTrafficlights_ = &trafficlights_;
  Graph graph_, *pGraph_ = nullptr, *trafficlightGraph_ = nullptr;  // route graph, traffic light graph
  // editor editor_;
};
