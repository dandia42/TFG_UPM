#pragma once
#include "interaction.h"
#include "lists.h"

class Editor {
 public:
  static Vector2D position_;
  static int index_, maxIndex_;
  static int object_;  // enum 1 street	2 car	3 trafficlight
  static bool loadFlag_;
  static bool enable_;
  // general
  static void draw();
  static void setPosition(int, int, CarList, StreetList, TrafficlightList);
  static bool getEnable();
  static int getObjectType();
  static int getIndex();
  static void loadData(CarList, StreetList, TrafficlightList);  // Load static world data
  static void updateData(CarList*, StreetList*,
                             TrafficlightList*);  // Reference variables for changing objects
  static void toggleEnable();                   // Enable or disable editor
  static void editorFunctions(unsigned char, CarList, StreetList, TrafficlightList);
  static void changeObject(int, CarList, StreetList, TrafficlightList);  // Toggle between same type objects
  static void changeObjectType(int);                                        // Toggle between different type objects

  // streets
  static void changeStreet(int, StreetList, TrafficlightList);  // Change street type
  // static void aumentarTipoCalle();
  // static void disminuirTipoCalle();
  static void changeLength(StreetList, CarList, TrafficlightList, int);  // Change street length

  // cars
  static void changeCarNumber(int, CarList, StreetList);  // Change number of cars

  // trafficlights
  static void changeTrafficlightState(int, TrafficlightList);  // Change trafficlight state

  // World
  static void changeMatrix();  // Restart world

  // Helper
  static bool reachEdge(int, CarList, StreetList, TrafficlightList);
};
