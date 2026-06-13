#pragma once
#include <stdlib.h>

#include <iostream>

#include "lists.h"

#define MAX_WEIGHT 1000000
#define MAX_FLOYD_WARSHALL_NODES 50  // up to 500 max, time test to bigger matrix

using namespace std;

class Dijkstra {
  int distance_, predecessorID_, iteration_;
  bool visited_;
  // int* route_;		//TODO por que esta esto aqui? se guardan los id_ de las aristas conjugadas
 public:
  Dijkstra();
  ~Dijkstra();
  int getDist() {
    return distance_;
  }
  int getID() {
    return predecessorID_;
  }
  int getIteration() {
    return iteration_;
  }
  bool getVisited() {
    return visited_;
  }
  void setVisited(bool x) {
    visited_ = x;
  }
  void setDistance(int x) {
    distance_ = x;
  }
  void setID(int x) {
    predecessorID_ = x;
  }
  void setIteration(int x) {
    iteration_ = x;
  }
  friend class Node;
  friend class Link;
  friend class Interaction;
};

class Node {
  int id_, index_, occupancy_;
  int numberAdjacentNodes_ = 0, currentAdjacentNode_ = 0;  // Adjacent nodes number
  int *adjacentNode_, *adjacentNodeWeight_;      // Adjacent nodes index
  Dijkstra dijkstraVariable_;             // Variable needed by dijkstra algorithm
 public:
  Node();
  ~Node();
  int getID() {
    return id_;
  }
  Dijkstra getDijkstraVariable_() {
    return dijkstraVariable_;
  }
  friend class Algorithm;
  friend class Graph;
  friend class Interaction;
};
class Link {
  int origin_, destination_, index_, constantWeight_, variableWeight_;  
    // weight values {(1,2,3),(4,5,6),(7,8,9)} depending on out streets of next crossroad,
    // same crossroad and occupancy

 public:
  int getOrigin() {
    return origin_;
  }
  int getDestination() {
    return destination_;
  }
  int getIndex() {
    return index_;
  }
  void setIndex(int x) {
    index_ = x;
  }
  void setVariableWeight(int p) {
    variableWeight_ = p;
  }
  int getWeight() {
    return constantWeight_ + variableWeight_;
  }
  Link();
  ~Link();
  friend class Graph;
  friend class Interaction;
};

class Graph {
  Node *node_ = NULL, *conjugateNode_ = NULL;
  Link *link_ = NULL, *conjugateLink_ = NULL;
  int nNodes_ = 0, nLinks_ = 0, nConjugateNodes_ = 0, nConjugateLinks_ = 0, end_ = 0, auxiliarTimer_ = 0;
  int weightFloydWarshall_[MAX_FLOYD_WARSHALL_NODES][MAX_FLOYD_WARSHALL_NODES],
      originFloydWarshall_[MAX_FLOYD_WARSHALL_NODES][MAX_FLOYD_WARSHALL_NODES];  // Floyd-Warshall algorithm matrix
 public:
  Graph();
  ~Graph();
  void buildGraph(int, int, StreetList, TrafficlightList, bool);            // Route graph
  void buildGraph(StreetList&, CarList&, TrafficlightList&, int, int);  // Traffic light graph
  void buildGraph(Graph&);
  void buildConjugateGraph(bool);
  int countDirections(StreetList streets_);
  void assignLinkDirection(StreetList, TrafficlightList, int, int, bool);
  Link getLink(int n) {
    return link_[n];
  }
  void drawLink(TrafficlightList, int, int, int);
  void asociateStreetNode(StreetList);
  void asociateStreetLink(StreetList, TrafficlightList);
  void adjacencyList(int n, bool);
  void calculateConjugateWeight(bool);
  void calculateTrafficlightWeight(StreetList&, CarList&, Trafficlight*, int);
  void resetGraph();
  void setAuxiliarTimer(int x = 0) {
    auxiliarTimer_ = x;
  }
  int getAuxiliarTimer() {
    return auxiliarTimer_;
  }

  friend class Algorithm;
  friend class Interaction;
};
// TODO que hace esto aqui
#pragma once
#include "interaction.h"
