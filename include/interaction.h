#pragma once
#include <time.h>

#include "algorithms.h"
#include "lists.h"

class Interaction {
 public:
  // Cars behaviour at trafficlight
  static void crossroads(CarList, TrafficlightList, StreetList, Graph*, double*, int, int, int, int);
  static void crossroads(CarList, int, TrafficlightList, int, StreetList, Graph*, double*, int, int, int, int, int);
  static bool gettingCloser(Car*, Trafficlight*, float);
  // Traffic light behaviour with graph
  static void onTimerSem(StreetList&, CarList&, TrafficlightList, int, Graph*);
  static void onTimerSem(StreetList&, CarList&, Trafficlight*, int, Graph*, bool, int);
  static void onTimerSem(StreetList&, Trafficlight*);
  // Random car creation at street
  static void addCar(CarList*, StreetList);
  static void addCar(Street*, CarList*);
  // Delete cars
  static void deleteCar(CarList, int);
  // Cars appear at the other side of the world
  static void exitWorld(CarList, int, int, int);
  static void exitWorld(Car*, int, int, int);
  // Behaviour of cars with the same direction
  static void safetyDistance(CarList);
  static void safetyDistance(CarList, int, int, Vector2D);
  static int safetyDistance(Car*, int, const char*);
  // Trafficlights calculations to know how many streets connects at each direction
  static void streetsInCrossroad(StreetList, TrafficlightList);
  static void streetsInCrossroad(Street*, Trafficlight*, int);
  // Lane changes
  static void changeLane(CarList, StreetList);
  static void changeLane(Car*, Street*);
  static void changeLane(Car*, int*, Vector2D);
  // Route generation
  static void setRoute(CarList, Graph*, StreetList, int);
  static void setRoute(Car*, Graph*, int, StreetList, int, int);
  static void calcularCalles(StreetList, float, float);  // TODO quesesto

  // Timer registry
  static void timerRegistry(double*, double*, double*, int, int, bool);
  // Helpers
  static void unreachableCheck(TrafficlightList, StreetList);
  // Top and right streets are the same as bottom and left
  static void exteriorStreets(StreetList*, int, int, int, int);

  static bool changeStreet(Trafficlight*, Street*);  // Check only out streets
  static void changeStreet(StreetList);  // Avoid out of range streets and unreachable cases
  static bool changeStreet(Street*, bool, bool);
  static void streetOccupancy(CarList, StreetList); // Assign occupancy value to streets depending on the cars in it
  static void streetOccupancy(CarList, Street*);
  static int getStreetIndex(Car*, StreetList);  // Return street index occupied by the car
  static void setDirectionPatch(CarList, StreetList);  // Avoid stop problems and change direction (TODO: se
                                          // debería arreglar en otra parte del código pero no se cual)
  // static void setDirectionPatch(Car*,Street*);
  static int bitmask(Car*, Trafficlight*, float);

  static int checkMissingCars(CarList);
  // Associate index true, associate occupancy false
  static int associateLinkIndex(StreetList, Trafficlight*, int, int, int, bool); 
};
