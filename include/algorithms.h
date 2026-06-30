#pragma once
#include "debug.h"
#include "graph.h"
#include "vector"

class Algorithm {
 public:
  static void dijkstra(int, Graph*, bool);
  static void floydWarshall(Graph*);
  static void greenWave(Graph*, int*&, int&, StreetList&);
  static void auxFunction(int, int, Graph*&, int&, int*&, int*&);
};
