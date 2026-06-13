#pragma once
#include "interaction.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>

int availableLanes(int, int, int);
void getCurrentLane(Car* car, Vector2D pos);
int relativeStreetPosition(Street*, Street*);

using namespace std;
/* Cars behaviour in street lights */
void Interaction::crossroads(CarList carList, TrafficlightList trafficlightList, StreetList streetList, Graph* graph, double* time, int iterations,
                        int rows, int columns, int routeOptimization) {
  Vector2D aux;
  for (int i = 0; i < carList.number_; i++)
    for (int j = 0; j < trafficlightList.number_; j++) {
      aux = carList.carList_[i]->position_ - trafficlightList.trafficlightList_[j]->position_;
      if (aux.module() < (trafficlightList.trafficlightList_[j]->kSize * 1.5f))  // Distance to affected cars
        crossroads(carList, i, trafficlightList, j, streetList, graph, time, iterations, i, rows, columns, routeOptimization);
    }
}
void Interaction::crossroads(CarList carList, int indexc, TrafficlightList trafficlightList, int indexs, StreetList streetList, Graph* graph,
                        double* time, int iterations, int icoche, int rows, int columns, int routeOptimization) {
  Car* car = carList.carList_[indexc];
  Trafficlight* trafficlight = trafficlightList.trafficlightList_[indexs];
  Vector2D aux = car->position_ - trafficlight->position_;
  float auxx = car->position_.x - trafficlight->position_.x, auxy = car->position_.y - trafficlight->position_.y;
  // static int n=0;	//TODO quitar? carril al que va a ir en el turning_
  float dist = aux.module();
  int masc = bitmask(car, trafficlight, dist);
  if (!carList.carList_[indexc]->debugFlag_) {
    cout << "\nCar " << indexc << " tiene el caso: " << hex << masc << dec << endl;
    carList.carList_[indexc]->debugFlag_ = true;
  }
  if (car->trafficlightFlag_ == false) {
    car->trafficlightFlag_ = Interaction::gettingCloser(car, trafficlight, dist);
  } else {  // 2048 - 4095
    // 2176 - 2303 || 2432 - 2559
    // 2560 - 3071 || 3072 - 3199
    // 3328 - 3455 || 3584 - 4095
    if (masc >= 0x880 && masc <= 0x8FF || masc >= 0x980 && masc <= 0x9FF || masc >= 0xA00 && masc <= 0xBFF ||
        masc >= 0xC00 && masc <= 0xC7F || masc >= 0xD00 && masc <= 0xD7F || masc >= 0xE00 && masc <= 0xFFF) {
      car->changeStop(1);  // stop
    }
    // 2048 - 2175 || 2304 - 2431
    // 3200 - 3327 || 3456 - 3583
    else if (masc >= 0x800 && masc <= 0x87F || masc >= 0x900 && masc <= 0x97F || masc >= 0xC80 && masc <= 0xCFF ||
             masc >= 0xD80 && masc <= 0xDFF) {
      car->changeStop(0);  // start
      car->kDefaultAngle = 0.0125f;
    }
    // 0 - 127 || 256 - 383
    // 1152 - 1279 || 1408 - 1535
    else if (masc >= 0x000 && masc <= 0x07F || masc >= 0x100 && masc <= 0x17F || masc >= 0x480 && masc <= 0x4FF ||
             masc >= 0x580 && masc <= 0x5FF) {
      car->changeStop(0);  // velocity_ x1
      car->kDefaultAngle = 0.0125f;
    }
    // 512 - 639 || 768 - 895
    // 1664 - 1791 || 1920 - 2047
    else if (masc >= 0x200 && masc <= 0x27F || masc >= 0x300 && masc <= 0x37F || masc >= 0x680 && masc <= 0x6FF ||
             masc >= 0x780 && masc <= 0x7FF) {
      car->changeStop(0);  // velocity_ x3
      car->defaultVelocity_ = 3;
      car->kDefaultAngle = 0.0375f;
    }
    // 2048 - 2175 || 2304 - 2431
    // 3200 - 3327 || 3456 - 3583
    else if (masc >= 0x800 && masc <= 0x87F || masc >= 0x900 && masc <= 0x97F || masc >= 0xC80 && masc <= 0xCFF ||
             masc >= 0xD80 && masc <= 0xDFF) {
      // keep going x1
    } else if (masc >= 0x800 && masc <= 0x87F || masc >= 0x900 && masc <= 0x97F || masc >= 0xC80 && masc <= 0xCFF ||
               masc >= 0xD80 && masc <= 0xDFF) {
      // keep going x3
    }
    car->setVelocityDirection();
    int auxrand = rand() % 2;
    int auxn = 0;
    switch (masc) {  // go straight x1 y x3
      // turn
      case 0x011:  ////
      case 0x013:
      case 0x015:
      case 0x017:
      case 0x019:
      case 0x01B:
      case 0x01D:
      case 0x01F:
      case 0x483:
      case 0x487:
      case 0x48B:
      case 0x48F:
      case 0x493:
      case 0x497:
      case 0x49B:
      case 0x49F:  // Closer
      case 0x105:
      case 0x107:
      case 0x10D:
      case 0x10F:
      case 0x115:
      case 0x117:
      case 0x11D:
      case 0x11F:
      case 0x589:
      case 0x58B:
      case 0x58D:
      case 0x58F:
      case 0x599:
      case 0x59B:
      case 0x59D:
      case 0x59F:  // Further away left turn X1
      case 0x045:  ////
      case 0x047:
      case 0x04D:
      case 0x04F:
      case 0x055:
      case 0x057:
      case 0x05D:
      case 0x05F:
      case 0x4C9:
      case 0x4CB:
      case 0x4CD:
      case 0x4CF:
      case 0x4D9:
      case 0x4DB:
      case 0x4DD:
      case 0x4DF:  // Closer
      case 0x151:
      case 0x153:
      case 0x155:
      case 0x157:
      case 0x159:
      case 0x15B:
      case 0x15D:
      case 0x15F:
      case 0x5C3:
      case 0x5C7:
      case 0x5CB:
      case 0x5CF:
      case 0x5D3:
      case 0x5D7:
      case 0x5DB:
      case 0x5DF:  // Further away right turn X1
      case 0x211:  ////
      case 0x213:
      case 0x215:
      case 0x217:
      case 0x219:
      case 0x21B:
      case 0x21D:
      case 0x21F:
      case 0x683:
      case 0x687:
      case 0x68B:
      case 0x68F:
      case 0x693:
      case 0x697:
      case 0x69B:
      case 0x69F:  // Closer
      case 0x305:
      case 0x307:
      case 0x30D:
      case 0x30F:
      case 0x315:
      case 0x317:
      case 0x31D:
      case 0x31F:
      case 0x789:
      case 0x78B:
      case 0x78D:
      case 0x78F:
      case 0x799:
      case 0x79B:
      case 0x79D:
      case 0x79F:  // Further away left turn X3
      case 0x245:  ////
      case 0x247:
      case 0x24D:
      case 0x24F:
      case 0x255:
      case 0x257:
      case 0x25D:
      case 0x25F:
      case 0x6C9:
      case 0x6CB:
      case 0x6CD:
      case 0x6CF:
      case 0x6D9:
      case 0x6DB:
      case 0x6DD:
      case 0x6DF:  // Closer
      case 0x351:
      case 0x353:
      case 0x355:
      case 0x357:
      case 0x359:
      case 0x35B:
      case 0x35D:
      case 0x35F:
      case 0x7C3:
      case 0x7C7:
      case 0x7CB:
      case 0x7CF:
      case 0x7D3:
      case 0x7D7:
      case 0x7DB:
      case 0x7FF:  // Further away right turn X3
        // cout << "\nThe car number: " << indexc << " is turning " << car->getTurning() << endl;
        car->turn();
        break;

      // change turning
      case 0x002:
      case 0x003:
      case 0x006:
      case 0x007:
      case 0x042:
      case 0x043:
      case 0x052:
      case 0x053:
      case 0x084:
      case 0x085:
      case 0x08C:
      case 0x08D:
      case 0x0C4:
      case 0x0C5:
      case 0x0C6:
      case 0x0C7:
      case 0x108:
      case 0x109:
      case 0x118:
      case 0x119:
      case 0x148:
      case 0x149:
      case 0x14C:
      case 0x14D:
      case 0x190:
      case 0x191:
      case 0x192:
      case 0x193:
      case 0x1D0:
      case 0x1D1:
      case 0x1D8:
      case 0x1D9:

      case 0x202:
      case 0x203:
      case 0x206:
      case 0x207:
      case 0x242:
      case 0x243:
      case 0x252:
      case 0x253:
      case 0x284:
      case 0x285:
      case 0x28C:
      case 0x28D:
      case 0x2C4:
      case 0x2C5:
      case 0x2C6:
      case 0x2C7:
      case 0x308:
      case 0x309:
      case 0x318:
      case 0x319:
      case 0x348:
      case 0x349:
      case 0x34C:
      case 0x34D:
      case 0x390:
      case 0x391:
      case 0x392:
      case 0x393:
      case 0x3D0:
      case 0x3D1:
      case 0x3D8:
      case 0x3D9:

      case 0x402:
      case 0x403:
      case 0x406:
      case 0x407:
      case 0x442:
      case 0x443:
      case 0x452:
      case 0x453:
      case 0x484:
      case 0x485:
      case 0x48C:
      case 0x48D:
      case 0x4C4:
      case 0x4C5:
      case 0x4C6:
      case 0x4C7:
      case 0x508:
      case 0x509:
      case 0x518:
      case 0x519:
      case 0x548:
      case 0x549:
      case 0x54C:
      case 0x54D:
      case 0x590:
      case 0x591:
      case 0x592:
      case 0x593:
      case 0x5D0:
      case 0x5D1:
      case 0x5D8:
      case 0x5D9:

      case 0x602:
      case 0x603:
      case 0x606:
      case 0x607:
      case 0x642:
      case 0x643:
      case 0x652:
      case 0x653:
      case 0x684:
      case 0x685:
      case 0x68C:
      case 0x68D:
      case 0x6C4:
      case 0x6C5:
      case 0x6C6:
      case 0x6C7:
      case 0x708:
      case 0x709:
      case 0x718:
      case 0x719:
      case 0x748:
      case 0x749:
      case 0x74C:
      case 0x74D:
      case 0x790:
      case 0x791:
      case 0x792:
      case 0x793:
      case 0x7D0:
      case 0x7D1:
      case 0x7D8:
      case 0x7D9:
        cout << "\nTurning of car: " << indexc << " is changed from " << car->getTurning() << " to 0" << endl;
        car->setTurn(0);
        break;

      case 0x030:
      case 0x031:
      case 0x050:
      case 0x051:
      case 0x0A2:
      case 0x0A3:
      case 0x0C2:
      case 0x0C3:
      case 0x124:
      case 0x125:
      case 0x144:
      case 0x145:
      case 0x1A8:
      case 0x1A9:
      case 0x1C8:
      case 0x1C9:

      case 0x230:
      case 0x231:
      case 0x250:
      case 0x251:
      case 0x2A2:
      case 0x2A3:
      case 0x2C2:
      case 0x2C3:
      case 0x324:
      case 0x325:
      case 0x344:
      case 0x345:
      case 0x3A8:
      case 0x3A9:
      case 0x3C8:
      case 0x3C9:

      case 0x430:
      case 0x431:
      case 0x450:
      case 0x451:
      case 0x4A2:
      case 0x4A3:
      case 0x4C2:
      case 0x4C3:
      case 0x524:
      case 0x525:
      case 0x544:
      case 0x545:
      case 0x5A8:
      case 0x5A9:
      case 0x5C8:
      case 0x5C9:

      case 0x630:
      case 0x631:
      case 0x650:
      case 0x651:
      case 0x6A2:
      case 0x6A3:
      case 0x6C2:
      case 0x6C3:
      case 0x724:
      case 0x725:
      case 0x744:
      case 0x745:
      case 0x7A8:
      case 0x7A9:
      case 0x7C8:
      case 0x7C9:
        cout << "\nTurning of car: " << indexc << " is changed from " << car->getTurning() << " to -1" << endl;
        car->setTurn(-1);
        break;

      case 0x004:
      case 0x005:
      case 0x024:
      case 0x025:
      case 0x034:
      case 0x035:
      case 0x088:
      case 0x089:
      case 0x0A8:
      case 0x0A9:
      case 0x0AA:
      case 0x0AB:
      case 0x110:
      case 0x111:
      case 0x130:
      case 0x131:
      case 0x134:
      case 0x135:
      case 0x182:
      case 0x183:
      case 0x1A2:
      case 0x1A3:
      case 0x1AA:
      case 0x1AB:

      case 0x204:
      case 0x205:
      case 0x224:
      case 0x225:
      case 0x234:
      case 0x235:
      case 0x288:
      case 0x289:
      case 0x2A8:
      case 0x2A9:
      case 0x2AA:
      case 0x2AB:
      case 0x310:
      case 0x311:
      case 0x330:
      case 0x331:
      case 0x334:
      case 0x335:
      case 0x382:
      case 0x383:
      case 0x3A2:
      case 0x3A3:
      case 0x3AA:
      case 0x3AB:

      case 0x404:
      case 0x405:
      case 0x424:
      case 0x425:
      case 0x434:
      case 0x435:
      case 0x488:
      case 0x489:
      case 0x4A8:
      case 0x4A9:
      case 0x4AA:
      case 0x4AB:
      case 0x510:
      case 0x511:
      case 0x530:
      case 0x531:
      case 0x534:
      case 0x535:
      case 0x582:
      case 0x583:
      case 0x5A2:
      case 0x5A3:
      case 0x5AA:
      case 0x5AB:

      case 0x604:
      case 0x605:
      case 0x624:
      case 0x625:
      case 0x634:
      case 0x635:
      case 0x688:
      case 0x689:
      case 0x6A8:
      case 0x6A9:
      case 0x6AA:
      case 0x6AB:
      case 0x710:
      case 0x711:
      case 0x730:
      case 0x731:
      case 0x734:
      case 0x735:
      case 0x782:
      case 0x783:
      case 0x7A2:
      case 0x7A3:
      case 0x7AA:
      case 0x7AB:
        cout << "\nTurning of car: " << indexc << " is changed from " << car->getTurning() << " to 1" << endl;
        car->setTurn(1);
        break;
    }

    if (masc >= 0x800) {  // Big amber area
      if (car->trafficlightFlag_ = Interaction::gettingCloser(car, trafficlight, dist)) {
        /*if(trafficlight->state_==0 && car->direction_%2==0 || trafficlight->state_==2 && car->direction_%2){
                car->changeStop(0);
                car->defaultVelocity_=1;	//state_ en verde

        }
        else 	car->changeStop(1);	//state_ en ambar o rojo*/
      }
      /////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////
      else {
        car->changeStop(0);
        // car->defaultVelocity_=1;
        // cout<<"\nel proximo turning_ es: "<<car->route_[car->actualTurn_];
        car->actualTurn_++;
        ////TODO ?? esto estaba al final
        car->setExitPosition(trafficlight->position_, trafficlight->affectedLane_);
        if (car->actualTurn_ == car->numberOfTurns_) {  // Route completed
          int numberOfTurns = car->numberOfTurns_;
          cout << "\n\tROUTE COMPLETED\n";
          delete[] car->route_;
          cout << "\nPrevious route deleted\n";
          timerRegistry(&car->initialRouteTime_, &car->endRouteTime_, time, iterations, numberOfTurns, true);
          streetOccupancy(carList, streetList);                        // update variable weights in streets
          graph->buildGraph(rows, columns, streetList, trafficlightList, false);  // update variable weights in graphs
          setRoute(car, graph, getStreetIndex(car, streetList), streetList, icoche, routeOptimization);
          if (routeOptimization != 2) timerRegistry(&car->initialRouteTime_, &car->endRouteTime_, time, iterations, numberOfTurns, false);
        }
        ////
        car->turning_ = car->route_[car->actualTurn_];  //((car->actualTurn_==car->numberOfTurns_)?0:car->actualTurn_)];	// Decide next turn
        car->turningFlag_ = false;
        cout << "\nThe car " << indexc << "\t has " << car->getTurning() << " turning and "
             << car->getDirection() << " direction" << endl;  // TODO cambiar quitar esto, es para ver donde falla
        if (car->getTurning() >= 2 || car->getDirection() >= 4 || car->getTurning() <= -2 || car->getDirection() <= -1) {
          cout << "ERROR: Turning error.\t\tTurn: " << car->getTurning() << "\t\tDirection: " << car->getDirection();
          int auxx;
          cin >> auxx;
        }  ////////

        /*if(car->direction_%2) car->setPosition(car->position_.x,trafficlight->position_.y+3*(2-car->n[1]/2));
        else car->setPosition(trafficlight->position_.x+3*(2-car->n[1]/2), car->position_.y);*/
      }
      car->setVelocityDirection();
    }
  }
}

/////////////////////////////////////

bool Interaction::gettingCloser(Car* car, Trafficlight* trafficlight, float dist) {
  if (dist < 1.5f * trafficlight->kSize) switch (car->direction_) {
      case 0:  // up
        if ((dist = trafficlight->position_.y - car->position_.y) < 1.5f * trafficlight->kSize && dist > 0)
          return true;
        else
          return false;
        break;
      case 1:  // right
        if ((dist = trafficlight->position_.x - car->position_.x) < 1.5f * trafficlight->kSize && dist > 0)
          return true;
        else
          return false;
        break;
      case 2:  // down
        if ((dist = car->position_.y - trafficlight->position_.y) < 1.5f * trafficlight->kSize && dist > 0)
          return true;
        else
          return false;
        break;
      case 3:  // left
        if ((dist = car->position_.x - trafficlight->position_.x) < 1.5f * trafficlight->kSize && dist > 0)
          return true;
        else
          return false;
        break;
      default:
        cout << "\n\tERROR DIRECTION in gettingCloser(Car, trafficlight, dist)";
        return -1;
        break;
    }
  else
    return false;
}
////////////////////////////////////////////////////////

/* Random car creation on streets */
void Interaction::addCar(CarList* carList, StreetList streetList) {
  for (int i = 0; i < streetList.number_; i++)
    if (!streetList.streetList_[i]->getEdge()) Interaction::addCar(streetList.streetList_[i], carList);
}
void Interaction::addCar(Street* street, CarList* carList) {
  for (int i = 0; i < street->lanes_; i++) {
    int aux = rand() % 3;
    if (!aux)  // create car
      switch (street->type_) {
        case 1:  // 1 Lane
          carList->add(new Car(street->position_, street->direction_));
          break;
        case 2:  // 1 lane
          carList->add(new Car(street->position_, street->direction_ + 2));
          break;
        case 3:  // 2 lanes
          carList->add(new Car({street->position_.x + (street->direction_ - 1) * ((i - 0.5f) * street->width_),
                                  street->position_.y + street->direction_ * ((i - 0.5f) * street->width_)},
                                 street->direction_));
          break;
        case 4:  // 2 lanes
          carList->add(new Car({street->position_.x + (street->direction_ - 1) * ((i - 0.5f) * street->width_),
                                  street->position_.y + street->direction_ * ((i - 0.5f) * street->width_)},
                                 street->direction_ + 2));
          break;
        case 5:  // 2 lanes
          carList->add(new Car({street->position_.x + (street->direction_ - 1) * ((i - 0.5f) * street->width_),
                                  street->position_.y + street->direction_ * ((i - 0.5f) * street->width_)},
                                 street->direction_ + 2 * i));
          break;
        case 6:  // 3 lanes
          carList->add(new Car({street->position_.x + (street->direction_ - 1) * ((i - 1) * street->width_),
                                  street->position_.y + street->direction_ * ((i - 1) * street->width_)},
                                 street->direction_));
          break;
        case 7:  // 3 lanes
          carList->add(new Car({street->position_.x + (street->direction_ - 1) * ((i - 1) * street->width_),
                                  street->position_.y + street->direction_ * ((i - 1) * street->width_)},
                                 street->direction_ + 2));
          break;
        case 8:  // 3 lanes
          carList->add(new Car({street->position_.x + (street->direction_ - 1) * ((i - 1) * street->width_),
                                  street->position_.y + street->direction_ * ((i - 1) * street->width_)},
                                 (i < 2 ? (street->direction_) : (street->direction_ + 2))));
          break;
        case 9:  // 3 lanes
          carList->add(new Car({street->position_.x + (street->direction_ - 1) * ((i - 1) * street->width_),
                                  street->position_.y + street->direction_ * ((i - 1) * street->width_)},
                                 (i < 1 ? (street->direction_) : (street->direction_ + 2))));
          break;
        case 10:  // 4 lanes
          carList->add(new Car({street->position_.x + (street->direction_ - 1) * ((i - 1.5f) * street->width_),
                                  street->position_.y + street->direction_ * ((i - 1.5f) * street->width_)},
                                 street->direction_));
          break;
        case 11:  // 4 lanes
          carList->add(new Car({street->position_.x + (street->direction_ - 1) * ((i - 1.5f) * street->width_),
                                  street->position_.y + street->direction_ * ((i - 1.5f) * street->width_)},
                                 street->direction_ + 2));
          break;
        case 12:  // 4 lanes
          carList->add(new Car({street->position_.x + (street->direction_ - 1) * ((i - 1.5f) * street->width_),
                                  street->position_.y + street->direction_ * ((i - 1.5f) * street->width_)},
                                 (i < 3 ? (street->direction_) : (street->direction_ + 2))));
          break;
        case 13:  // 4 lanes
          carList->add(new Car({street->position_.x + (street->direction_ - 1) * ((i - 1.5f) * street->width_),
                                  street->position_.y + street->direction_ * ((i - 1.5f) * street->width_)},
                                 (i < 2 ? (street->direction_) : (street->direction_ + 2))));
          break;
        case 14:  // 4 lanes
          carList->add(new Car({street->position_.x + (street->direction_ - 1) * ((i - 1.5f) * street->width_),
                                  street->position_.y + street->direction_ * ((i - 1.5f) * street->width_)},
                                 (i < 1 ? (street->direction_) : (street->direction_ + 2))));
          break;
      }
  }
}
void Interaction::deleteCar(CarList carList, int aux) {
  for (int i = carList.number_; i > 0; i--) {
    if (rand() % aux == 0) {
      carList.deleteIndex(i);
      i--;
    }
  }
}
////////////////////////////////////////////////////////////////

/* Function to maintain cars over the edge */
void Interaction::exitWorld(CarList carList, int fil, int col, int lon) {
  for (int i = 0; i < carList.number_; i++) Interaction::exitWorld(carList.carList_[i], fil, col, lon);
}
void Interaction::exitWorld(Car* car, int fil, int col, int lon) {
  Vector2D max = {(float) col * lon, (float) fil * lon};
  switch (car->direction_) {
    case 0:
      if (car->position_.y > max.y) car->position_.y = -lon;
      break;
    case 1:
      if (car->position_.x > max.x) car->position_.x = -lon;
      break;
    case 2:
      if (car->position_.y < -lon) car->position_.y = max.y;
      break;
    case 3:
      if (car->position_.x < -lon) car->position_.x = max.x;
      break;
  }
}
///////////////////////////////////////////////////////////////////////
int Interaction::safetyDistance(Car* car, int v, const char* x) {
  car->defaultVelocity_ = v;
  car->setVelocityDirection();
  car->r_ = 0.5f;
  return car->defaultVelocity_;
}
/* Avoid frontal crash */
void Interaction::safetyDistance(CarList carList) {
  for (int i = 0; i < carList.number_ - 1; i++)
    for (int j = i + 1; j < carList.number_; j++) {  // j>i to not repeat combinations and reduce complexity
      Vector2D aux = carList.carList_[i]->getPosition() - carList.carList_[j]->getPosition();
      if (aux.module() < 8 && carList.carList_[i]->getDirection() == carList.carList_[j]->getDirection()) Interaction::safetyDistance(carList, i, j, aux);
    }
}

void Interaction::safetyDistance(CarList carList, int i, int j, Vector2D aux) {
  // if(carList.carList_[i]->trafficlightFlag_==carList.carList_[j]->trafficlightFlag_==0){//  TODO ??? para que solo funcione fuera de los trafficlights_, mirar si falla
  // justo en la salida
  int auxx = 0;
  switch (carList.carList_[i]->getDirection()) {
    case 0:  // up
             // if(sqrt(pow(aux.x,2))<0.5f){
      if (aux.y >= 0 && aux.y < 6 && !carList.carList_[j]->getStop() && !carList.carList_[j]->trafficlightFlag_) {  // car1 ahead y car2 moving on
        carList.carList_[j]->changeStop(1);                                                      // stop car2
        carList.carList_[j]->setLateralFlag(1);
        break;
      } else if (aux.y > 6 && carList.carList_[j]->getStop() && !carList.carList_[i]->getStop()) {  // car1 ahead y car2 stopped
        carList.carList_[j]->changeStop(0);                                                // start car2
        carList.carList_[j]->setLateralFlag(0);
        break;
      } else if (aux.y <= 0 && aux.y > -6 && !carList.carList_[i]->getStop() &&
                 !carList.carList_[i]->trafficlightFlag_) {  // car2 ahead y car1 moving on
        carList.carList_[i]->changeStop(1);        // stop car1
        carList.carList_[i]->setLateralFlag(1);
        break;
      } else if (aux.y < -6 && carList.carList_[i]->getStop() && !carList.carList_[j]->getStop()) {  // car2 ahead y car1 stopped
        carList.carList_[i]->changeStop(0);                                                 // start car1
        carList.carList_[i]->setLateralFlag(0);
        break;
      }
      //}
      break;
    case 1:  // right
             // if(sqrt(pow(aux.y,2))<0.5f){
      if (aux.x >= 0 && aux.x < 6 && !carList.carList_[j]->getStop() && !carList.carList_[j]->trafficlightFlag_) {  // car1 ahead y car2 moving on
        carList.carList_[j]->changeStop(1);                                                      // stop car2
        carList.carList_[j]->setLateralFlag(1);
        break;
      } else if (aux.x > 6 && carList.carList_[j]->getStop() && !carList.carList_[i]->getStop()) {  // car1 ahead y car2 stopped
        carList.carList_[j]->changeStop(0);                                                // start car2
        carList.carList_[j]->setLateralFlag(0);
        break;
      } else if (aux.x <= 0 && aux.x > -6 && !carList.carList_[i]->getStop() &&
                 !carList.carList_[i]->trafficlightFlag_) {  // car2 ahead y car1 moving on
        carList.carList_[i]->changeStop(1);        // stop car1
        carList.carList_[i]->setLateralFlag(1);
        break;
      } else if (aux.x < -6 && carList.carList_[i]->getStop() && !carList.carList_[j]->getStop()) {  // car2 ahead y car1 stopped
        carList.carList_[i]->changeStop(0);                                                 // start car1
        carList.carList_[i]->setLateralFlag(0);
        break;
      }

      //}
      break;
    case 2:  // down
             // if(sqrt(pow(aux.x,2))<0.5f){
      if (aux.y >= 0 && aux.y < 6 && !carList.carList_[i]->getStop() && !carList.carList_[i]->trafficlightFlag_) {  // car2 ahead y car1 moving on
        carList.carList_[i]->changeStop(1);                                                      // stop car1
        carList.carList_[i]->setLateralFlag(1);
        break;
      } else if (aux.y > 6 && carList.carList_[i]->getStop() && !carList.carList_[j]->getStop()) {  // car2 ahead y car1 stopped
        carList.carList_[i]->changeStop(0);                                                // start car1
        // cout<<"\ncaso1";
        carList.carList_[i]->setLateralFlag(0);
        break;
      } else if (aux.y <= 0 && aux.y > -6 && !carList.carList_[j]->getStop() &&
                 !carList.carList_[j]->trafficlightFlag_) {  // car1 ahead y car2 moving on
        carList.carList_[j]->changeStop(1);        // stop car2
        carList.carList_[j]->setLateralFlag(1);
        break;
      } else if (aux.y < -6 && carList.carList_[j]->getStop() && !carList.carList_[i]->getStop()) {  // car1 ahead y car2 stopped
        carList.carList_[j]->changeStop(0);                                                 // start car2
        // cout<<"\ncaso2";
        carList.carList_[j]->setLateralFlag(0);
        break;
      }
      //}
      break;
    case 3:  // left
             // if(sqrt(pow(aux.y,2))<0.5f){
      if (aux.x >= 0 && aux.x < 6 && !carList.carList_[i]->getStop() && !carList.carList_[i]->trafficlightFlag_) {  // car2 ahead y car1 moving on
        carList.carList_[i]->changeStop(1);                                                      // stop car1
        carList.carList_[i]->setLateralFlag(1);
        break;
      } else if (aux.x > 6 && carList.carList_[i]->getStop() && !carList.carList_[j]->getStop()) {  // car2 ahead y car1 stopped
        carList.carList_[i]->changeStop(0);                                                // start car1
        // cout<<"\ncaso1";
        carList.carList_[i]->setLateralFlag(0);
        break;
      } else if (aux.x <= 0 && aux.x > -6 && !carList.carList_[j]->getStop() &&
                 !carList.carList_[j]->trafficlightFlag_) {  // car1 ahead y car2 moving on
        carList.carList_[j]->changeStop(1);        // stop car2
        carList.carList_[j]->setLateralFlag(1);
        break;
      } else if (aux.x < -6 && carList.carList_[j]->getStop() && !carList.carList_[i]->getStop()) {  // car1 ahead y car2 stopped
        carList.carList_[j]->changeStop(0);                                                 // start car2
        // cout<<"\ncaso2";
        carList.carList_[j]->setLateralFlag(0);
        break;
      }
      //}
      break;
  }
  // if(auxx){cout<<"error pocho3:";
  // cin>>auxx;}
  // }
}

////////////////////////////////////////////////////////////////////////////////////
/* Get how many streets are available in each street light */
void Interaction::streetsInCrossroad(StreetList carList, TrafficlightList trafficlightList) {
  Vector2D aux;
  for (int i = 0; i < carList.number_; i++)
    for (int j = 0; j < trafficlightList.number_; j++) {
      aux = carList.streetList_[i]->position_ - trafficlightList.trafficlightList_[j]->position_;
      if (aux.module() < (carList.streetList_[i]->length_ * 1.1f))  // distance to get streets
        streetsInCrossroad(carList.streetList_[i], trafficlightList.trafficlightList_[j], i);
    }
}
void Interaction::streetsInCrossroad(Street* car, Trafficlight* trafficlight, int id) {
  if (sqrt(pow(trafficlight->position_.x - car->position_.x, 2)) < 1 && trafficlight->position_.y < car->position_.y)  // street up
  {
    if (car->type_ == 2 || car->type_ == 4 || car->type_ == 7 || car->type_ == 11)
      trafficlight->outStreet_[0] = false;
    else
      trafficlight->outStreet_[0] = true;
    trafficlight->affectedLane_[0] = car->type_;
    trafficlight->streetIndex_[0] = id;
  } else if (sqrt(pow(trafficlight->position_.x - car->position_.x, 2)) < 1 && trafficlight->position_.y > car->position_.y)  // street down
  {
    if (car->type_ == 1 || car->type_ == 3 || car->type_ == 6 || car->type_ == 10)
      trafficlight->outStreet_[2] = false;
    else
      trafficlight->outStreet_[2] = true;
    trafficlight->affectedLane_[2] = car->type_;
    trafficlight->streetIndex_[2] = id;
  } else if (sqrt(pow(trafficlight->position_.y - car->position_.y, 2)) < 1 && trafficlight->position_.x < car->position_.x)  // street right
  {
    if (car->type_ == 2 || car->type_ == 4 || car->type_ == 7 || car->type_ == 11)
      trafficlight->outStreet_[1] = false;
    else
      trafficlight->outStreet_[1] = true;
    trafficlight->affectedLane_[1] = car->type_;
    trafficlight->streetIndex_[1] = id;
  } else if (sqrt(pow(trafficlight->position_.y - car->position_.y, 2)) < 1 && trafficlight->position_.x > car->position_.x)  // street left
  {
    if (car->type_ == 1 || car->type_ == 3 || car->type_ == 6 || car->type_ == 10)
      trafficlight->outStreet_[3] = false;
    else
      trafficlight->outStreet_[3] = true;
    trafficlight->affectedLane_[3] = car->type_;
    trafficlight->streetIndex_[3] = id;
  } else {
    static int count = 0;
    count++;
    cout << "ERROR in streetsInCrossroad" << count;
    int errorAux;
    cin >> errorAux;
    car->setColor(1, 0, 0);
    car->width_ /= 2;
  }
}

// Return lane depending on the lane type, car turning and initial direction
int availableLanes(int type, int turning, int direction) {
  return 4; /*
   static int n=0;
   switch(type){
   case 1:
   case 2:
           return 4;
   break;
   case 3:
           return (turning<0?(5):(3));
   break;
   case 4:
           return (turning<0?(3):(5));
   break;
   case 5:
           return ((direction==0||direction==3)?(turning<0?5:3):(turning<0?3:5));
   break;
   case 6:
           return (turning<0?(6):(2));
   break;
   case 7:
           return (turning<0?(2):(6));
   break;
   case 8:
           switch(direction){
           case 0:
                   n=turning<0?(6):(2);
           break;
           case 1:
                   n=turning<0?(4):(6);
           break;
           case 2:
                   n=turning<0?(4):(6);
           break;
           case 3:
                   n=turning<0?(2):(6);
           break;
           }
           return n;
   break;
   case 9:
           switch(direction){
           case 0:
                   n=turning<0?(4):(2);
           break;
           case 1:
                   n=turning<0?(2):(6);
           break;
           case 2:
                   n=turning<0?(6):(2);
           break;
           case 3:
                   n=turning<0?(4):(2);
           break;
           }
           return n;
   break;
   case 10:
           return turning<0?(7):(1);
   break;
   case 11:
           return turning<0?(1):(7);
   break;
   case 12:
           switch(direction){
           case 0:
                   n=turning<0?(7):(1);
           break;
           case 1:
                   n=turning<0?(5):(7);
           break;
           case 2:
                   n=turning<0?(5):(7);
           break;
           case 3:
                   n=turning<0?(7):(1);
           break;
           }
           return n;
   break;
   case 13:
           switch(direction){
           case 0:
                   n=turning<0?(5):(1);
           break;
           case 1:
                   n=turning<0?(3):(7);
           break;
           case 2:
                   n=turning<0?(3):(7);
           break;
           case 3:
                   n=turning<0?(5):(1);
           break;
           }
           return n;
   break;
   case 14:
           switch(direction){
           case 0:
                   n=turning<0?(3):(1);
           break;
           case 1:
                   n=turning<0?(1):(7);
           break;
           case 2:
                   n=turning<0?(7):(1);
           break;
           case 3:
                   n=turning<0?(3):(1);
           break;
           }
           return n;
   break;
   default:
           cout<<"\n\tERROR FUNCTION availableLanes(type, turning, direction)";
           return -1;
   break;
   }*/
}
//////////////////////////////////////////////////////////////////////////

/*funciones para el desplazamiento lateral*/
void Interaction::changeLane(CarList carList, StreetList streetList) {
  for (int i = 0; i < carList.number_; i++)
    for (int j = 0; j < streetList.number_; j++) {
      Vector2D aux = carList.carList_[i]->position_ - streetList.streetList_[j]->position_;
      if (aux.module() < (streetList.streetList_[j]->length_ / 2) &&
          !carList.carList_[i]->trafficlightFlag_) {  // Current street of the car
        changeLane(carList.carList_[i], streetList.streetList_[j]);
        break;  // exit for(j...) to save memory
      }
    }
}
void Interaction::changeLane(Car* car, Street* street) {
  car->n[1] = 4;
  /*  TODO ???
  getCurrentLane(car, street->getPosition());	//obtener el carril inicial en el que se encuentra Car

  switch(street->getType()){		//elegir el carril de destino en funcion de la direction_ dCar, el turning_ que vaya
  a hacer en el proximo cruce y el type_ de calle, se llama desde changeLane en la misma calle case 1: car->n[1]=4;
  break;
  case 2:
          car->n[1]=4;
  break;
  case 3:
          switch(car->getTurning()){
          case -1:
                  car->n[1]=5;
          break;
          case 0:
                  car->n[1]=car->n[0];	//cambiar si hay mucho trafico en un carril -futuro
          break;
          case 1:
                  car->n[1]=3;
          break;
          }
  break;
  case 4:
          switch(car->getTurning()){
          case -1:
                  car->n[1]=3;
          break;
          case 0:
                  car->n[1]=car->n[0];	//cambiar si hay mucho trafico en un carril -futuro
          break;
          case 1:
                  car->n[1]=5;
          break;
          }
  break;
  case 5:
          car->n[1]=(car->getDirection()<2)?(3):(5);
  break;
  case 6:
          switch(car->getTurning()){
          case -1:
                  car->n[1]=6;
          break;
          case 0:
                  car->n[1]=4;
          break;
          case 1:
                  car->n[1]=2;
          break;
          }
  break;
  case 7:
          switch(car->getTurning()){
          case -1:
                  car->n[1]=2;
          break;
          case 0:
                  car->n[1]=4;
          break;
          case 1:
                  car->n[1]=6;
          break;
          }
  break;
  case 8:
          switch(car->getTurning()){
          case -1:
                  car->n[1]=(car->getDirection()<2)?(4):(6);
          break;
          case 0:
                  car->n[1]=car->n[0];	//cambiar si hay mucho trafico en un carril -futuro
          break;
          case 1:
                  car->n[1]=(car->getDirection()<2)?(2):(6);
          break;
          }
  break;
  case 9:
          switch(car->getTurning()){
          case -1:
                  car->n[1]=(car->getDirection()<2)?(2):(4);
          break;
          case 0:
                  car->n[1]=car->n[0];	//cambiar si hay mucho trafico en un carril -futuro
          break;
          case 1:
                  car->n[1]=(car->getDirection()<2)?(2):(6);
          break;
          }
  break;
  case 10:
          switch(car->getTurning()){
          case -1:
                  car->n[1]=7;
          break;
          case 0:
                  car->n[1]=3;//o 5	//cambiar si hay mucho trafico en un carril -futuro
          break;
          case 1:
                  car->n[1]=1;
          break;
          }
  break;
  case 11:
          switch(car->getTurning()){
          case -1:
                  car->n[1]=1;
          break;
          case 0:
                  car->n[1]=3;//o 5	//cambiar si hay mucho trafico en un carril -futuro
          break;
          case 1:
                  car->n[1]=7;
          break;
          }
  break;
  case 12:
          switch(car->getTurning()){
          case -1:
                  car->n[1]=(car->getDirection()<2)?(5):(7);
          break;
          case 0:
                  car->n[1]=car->n[0];	//cambiar si hay mucho trafico en un carril -futuro
          break;
          case 1:
                  car->n[1]=(car->getDirection()<2)?(1):(7);
          break;
          }
  break;
  case 13:
          switch(car->getTurning()){
          case -1:
                  car->n[1]=(car->getDirection()<2)?(3):(5);
          break;
          case 0:
                  car->n[1]=car->n[0];	//cambiar si hay mucho trafico en un carril -futuro
          break;
          case 1:
                  car->n[1]=(car->getDirection()<2)?(1):(7);
          break;
          }
  break;
  case 14:
          switch(car->getTurning()){
          case -1:
                  car->n[1]=(car->getDirection()<2)?(1):(3);
          break;
          case 0:
                  car->n[1]=car->n[0];	//cambiar si hay mucho trafico en un carril -futuro
          break;
          case 1:
                  car->n[1]=(car->getDirection()<2)?(1):(7);
          break;
          }
  break;
  default:
          cout<<"\nError en la seleccion de type_ de calle destino\t";
  break;
  }*/
}
void Interaction::changeLane(Car* car, int affectedLane_[4], Vector2D pos) {  // depends on affected street type
  car->n[1] = 4; /*
getCurrentLane(car, pos);	//obtener el carril en el que esta Car antes del cruce

switch(affectedLane_[car->getDirection()]){	//los tipos de la calle destino
case 1:
     car->n[1]=4;
break;
case 2:
     car->n[1]=4;
break;
case 3:
     car->n[1]=3;
break;
case 4:
     car->n[1]=5;
break;
case 5:
     car->n[1]=(car->getDirection()<2)?(3):(5);
break;
case 6:
     car->n[1]=2;
break;
case 7:
     car->n[1]=6;
break;
case 8:
     car->n[1]=(car->getDirection()<2)?(2):(6);
break;
case 9:
     car->n[1]=(car->getDirection()<2)?(2):(6);
break;
case 10:
     car->n[1]=3;
break;
case 11:
     car->n[1]=5;
break;
case 12:
     car->n[1]=(car->getDirection()<2)?(3):(7);
break;
case 13:
     car->n[1]=(car->getDirection()<2)?(3):(5);
break;
case 14:
     car->n[1]=(car->getDirection()<2)?(1):(5);
break;
default:
     cout<<"\nError en la seleccion de type_ de calle destino\t";
break;
}*/
}
void getCurrentLane(Car* car, Vector2D pos) {
  car->n[0] = 4; /*
switch(car->getDirection()){
case 0:	//vertical
case 2:
///////esto solo al inicio
if(!car->getLateralFlag()){
if((car->getPosition().x-pos.x)<-3.75){		//carril 7
     car->n[0]=7;
}
else if((car->getPosition().x-pos.x)<-2.25){	//carril 6
     car->n[0]=6;
}
else if((car->getPosition().x-pos.x)<-0.75){	//carril 5
     car->n[0]=5;
}
else if((car->getPosition().x-pos.x)<0.75){	//carril 4
     car->n[0]=4;
}
else if((car->getPosition().x-pos.x)<2.25){	//carril 3
     car->n[0]=3;
}
else if((car->getPosition().x-pos.x)<3.75){	//carril 2
     car->n[0]=2;
}
else {							//carril 1
     car->n[0]=1;
}
car->setLateralFlag(true);
}
else{

car->lateralMovement(car->n[1]-car->n[0]);
if((car->getPosition().x-pos.x)<-4.45&&(car->getPosition().x-pos.x)>-4.55){		//carril 7
     car->n[0]=7;
}
else if((car->getPosition().x-pos.x)<-2.95&&(car->getPosition().x-pos.x)>-3.05){		//carril 6
     car->n[0]=6;
}
else if((car->getPosition().x-pos.x)<-1.45&&(car->getPosition().x-pos.x)>-1.55){		//carril 5
     car->n[0]=5;
}
else if((car->getPosition().x-pos.x)<0.05&&(car->getPosition().x-pos.x)>-0.05){		//carril 4
     car->n[0]=4;
}
else if((car->getPosition().x-pos.x)<1.55&&(car->getPosition().x-pos.x)>1.45){		//carril 3
     car->n[0]=3;
}
else if((car->getPosition().x-pos.x)<3.05&&(car->getPosition().x-pos.x)>2.95){		//carril 2
     car->n[0]=2;
}
else if((car->getPosition().x-pos.x)<4.55&&(car->getPosition().x-pos.x)>4.45){		//carril 1
     car->n[0]=1;
}
if(car->n[0]==car->n[1]){	//ha llegado a su carril
     car->setLateralFlag(false);
     //car->setPosition(pos.x+6-1.5f*car->n[0],car->getPosition().y);
}
}
break;
case 1:	//horizontal
case 3:
///////esto solo al inicio
if(!car->getLateralFlag()){
if((car->getPosition().y-pos.y)<-3.75){		//carril 1
     car->n[0]=1;
}
else if((car->getPosition().y-pos.y)<-2.25){	//carril 2
     car->n[0]=2;
}
else if((car->getPosition().y-pos.y)<-0.75){	//carril 3
     car->n[0]=3;
}
else if((car->getPosition().y-pos.y)<0.75){	//carril 4
     car->n[0]=4;
}
else if((car->getPosition().y-pos.y)<2.25){	//carril 5
     car->n[0]=5;
}
else if((car->getPosition().y-pos.y)<3.75){	//carril 6
     car->n[0]=6;
}
else {							//carril 7
     car->n[0]=7;
}
car->setLateralFlag(true);
}
else{
car->lateralMovement(car->n[1]-car->n[0]);
if((car->getPosition().y-pos.y)<-4.45&&(car->getPosition().y-pos.y)>-4.55){		//carril 1
     car->n[0]=1;
}
else if((car->getPosition().y-pos.y)<-2.95&&(car->getPosition().y-pos.y)>-3.05){		//carril 2
     car->n[0]=2;
}
else if((car->getPosition().y-pos.y)<-1.45&&(car->getPosition().y-pos.y)>-1.55){		//carril 3
     car->n[0]=3;
}
else if((car->getPosition().y-pos.y)<0.05&&(car->getPosition().y-pos.y)>-0.05){		//carril 4
     car->n[0]=4;
}
else if((car->getPosition().y-pos.y)<1.55&&(car->getPosition().y-pos.y)>1.45){		//carril 5
     car->n[0]=5;
}
else if((car->getPosition().y-pos.y)<3.05&&(car->getPosition().y-pos.y)>2.95){		//carril 6
     car->n[0]=6;
}
else if((car->getPosition().y-pos.y)<4.55&&(car->getPosition().y-pos.y)>4.45){		//carril 7
     car->n[0]=7;
}
if(car->n[0]==car->n[1]){	//ha llegado a su carril
     car->setLateralFlag(false);
     //car->setPosition(car->getPosition().x,pos.y-6+1.5f*car->n[0]);
}
}
break;
}*/
}

/////////////////////////////////////////////
/* Route selection and turning behaviour */
void Interaction::setRoute(CarList carList, Graph* graph, StreetList streetList, int routeOptimization) {
  for (int i = 0; i < carList.number_; i++)
    for (int j = 0; j < streetList.number_; j++) {
      Vector2D aux = carList.carList_[i]->position_ - streetList.streetList_[j]->position_;  // start street
      if (aux.module() < streetList.streetList_[j]->getLength() / 1.95f) {          // change if the streets have different length
        cout << "\nStart setRoute " << i << " " << j << " " << routeOptimization;
        setRoute(carList.carList_[i], graph, j, streetList, i, routeOptimization);
        cout << "\nEnd setRoute";
        break;  // exit for(j...) to save memory
      }
    }
}
void Interaction::setRoute(Car* car, Graph* graph, int ind, StreetList streetList, int icoche, int routeOptimization) {
  cout << "\n\tSTART setRoute";
  int ini = -1;
  car->actualTurn_ = 0;
  switch (routeOptimization) {
    case 0:
      car->numberOfTurns_ = rand() % 3 + 3;     // Change average turns number
      car->route_ = new int[car->numberOfTurns_];  // Save turns needed to complete route
      for (int i = 0; i < car->numberOfTurns_; i++)
        car->route_[i] = rand() % 3 - 1;  // random turning, changes automatically if it is unabailable
      cout << "\n\tCar " << icoche << " has to do " << car->numberOfTurns_ << " turns";
      for (int i = 0; i < car->numberOfTurns_; i++) cout << "\nturn" << i << ": " << car->route_[i];
      cout << "\n\n\tEND ROUTE SELECTION\n";

      break;
    case 1:  // dijkstra
    {
      for (int i = 0; i < graph->nConjugateNodes_; i++)  // TODO cambiar mirar si se puede poner un map en vez
        if (graph->conjugateNode_[i].index_ == ind) {
          ini = i;
          break;
        }
      // Update links variable weight

      Algorithm::dijkstra(ini, graph, false);  // Find best route
      do {
        graph->end_ = rand() % graph->nConjugateNodes_;  // Choose end of route
      } while (graph->end_ == ini);               // Check start and end are different
      if (graph->end_ == ini) {
        int i = 0;
        cout << "ERROR: same start and end in route" << endl;
        cin >> i;
      }
      int aux = graph->end_;
      cout << "\n\tDestination node: " << graph->end_;
      car->numberOfTurns_ = graph->conjugateNode_[graph->end_].dijkstraVariable_.iteration_;
      cout << "\n\tCar " << icoche << " has to do " << car->numberOfTurns_ << " turns";
      car->route_ = new int[car->numberOfTurns_];  // Save car turns
      for (int i = graph->conjugateNode_[graph->end_].dijkstraVariable_.iteration_ - 1; i >= 0; i--) {
        car->route_[i] = relativeStreetPosition(streetList.streetList_[graph->conjugateNode_[graph->conjugateNode_[aux].dijkstraVariable_.predecessorID_].index_],
                                        streetList.streetList_[graph->conjugateNode_[aux].index_]);
        aux = graph->conjugateNode_[aux].dijkstraVariable_.predecessorID_;
      }
      for (int i = 0; i < graph->conjugateNode_[graph->end_].dijkstraVariable_.iteration_; i++) cout << "\nturn " << i << ": " << car->route_[i];
      cout << "\n\n\tEND ROUTE SELECTION\n";

      car->turning_ = car->route_[car->actualTurn_];  // Choose first turn of the route
      graph->resetGraph();                  // Reset dijkstra variables
    } break;
    case 2:  // Floyd-Warshall
    {
      car->numberOfTurns_ = 1;
      for (int i = 0; i < graph->nConjugateNodes_; i++)  // TODO cambiar mirar si se puede poner un map en vez
        if (graph->conjugateNode_[i].index_ == ind) {
          ini = i;
          break;
        }
      graph->end_ = rand() % graph->nConjugateNodes_;  // choose route end
      int aux = graph->end_;
      while (graph->end_ == ini)  // Check start and end are different
        graph->end_ = rand() % graph->nConjugateNodes_;
      cout << "\n\tOrigin node: " << ini << " \tDestination node: " << graph->end_;

      while (graph->originFloydWarshall_[ini][aux] != ini) {
        aux = graph->originFloydWarshall_[ini][aux];
        car->numberOfTurns_++;
      }

      // car->numberOfTurns_=1;//cambiar
      cout << "\n\tCar " << icoche << " has to do " << car->numberOfTurns_ << " turns";
      car->route_ = new int[car->numberOfTurns_];  // Save car turns
      car->route_[0] = 1;  // TODO por que esto esta en 1? cambiar
      aux = graph->end_;
      for (int i = car->numberOfTurns_ - 1; i >= 0; i--) {
        car->route_[i] =
            relativeStreetPosition(streetList.streetList_[graph->conjugateNode_[graph->originFloydWarshall_[ini][aux]].index_], streetList.streetList_[graph->conjugateNode_[aux].index_]);
        aux = graph->originFloydWarshall_[ini][aux];
      }
      for (int i = 0; i < car->numberOfTurns_; i++) cout << "\nturn " << i << ": " << car->route_[i];
      cout << "\n\n\tEND ROUTE SELECTION\n";

      car->turning_ = car->route_[car->actualTurn_];  // Choose first turn of the route
    } break;
    default:
      cout << "\nERROR ROUTE OPTIMIZATION CHECK";
      cin >> routeOptimization;
      break;
  }
}

int relativeStreetPosition(Street* street1, Street* street2) {  // Start and end streets
  // 0=forward 1=right -1=left 	in a streetlight
  if (street1->getDirection() == street2->getDirection())
    return 0;
  else {
    if (!street1->getDirection()) {         // vertical
      if (street1->getType() == 2) {  // down
        if (street2->getType() == 2)  // left
          return 1;
        else  // right
          return -1;
      } else {                    // up
        if (street2->getType() == 2)  // left
          return -1;
        else  // right
          return 1;
      }
    } else {                      // horizontal
      if (street1->getType() == 2) {  // left
        if (street2->getType() == 2)  // down
          return -1;
        else  // up
          return 1;
      } else {                    // right
        if (street2->getType() == 2)  // down
          return 1;
        else  // up
          return -1;
      }
    }
  }
}

void Interaction::calcularCalles(StreetList streetList, float dx, float dy) {
  // TODO no se para que es esto
}

///////////////////////////////////////////////////////////////////////
/* Register simulation time */
void Interaction::timerRegistry(double* startTime, double* endTime, double* time, int iterations, int numberOfTurns, bool route) {
  static double averageTime = 0, totalTime = 0, algorithmAverageTime = 0;
  static int i = 0, averageTurnsNumber = 0;
  if (route) {         // time to complete route
    *endTime = clock();  // end route time
    *time = (double) ((*endTime - *startTime) / CLOCKS_PER_SEC);
    cout << "\n\n******************** Time: " << *time << " seconds ********************\n\n";
    *startTime = clock();  // start route time

    averageTime += *time;
    averageTurnsNumber += numberOfTurns;
    i++;
  } else {            // calculate route time
    *endTime = clock();  // end calculate route time
    *time = (double) ((*endTime - *startTime) / CLOCKS_PER_SEC);
    cout << "\n\n******************** Algorithm time: " << *time << " seconds ********************\n\n";
    algorithmAverageTime += *time;
  }
  if (i >= iterations) {  // TODO que es este comentario: se deja xmedio/i para poder sacar una simulacion en caso de colapso de los cars_
    averageTime = averageTime / i;
    averageTurnsNumber = averageTurnsNumber / i;
    algorithmAverageTime = algorithmAverageTime / i;
    totalTime = (double) (clock() / CLOCKS_PER_SEC);
    cout << "\n\n ***************************************************************************\n "
            "********************\tAverage time: "
         << averageTime << " seconds\t********************\n ********************\tTotal time: " << totalTime
         << " seconds\t********************\n\n ********************\tAlgorithm average time: " << algorithmAverageTime
         << " seconds\t********************\n\n ********************\tAverage turn number: " << averageTurnsNumber
         << " turns\t********************\n ********************\tRoutes completed: " << i
         << " routes\t********************\n ******************** END OF SIMULATION "
            "*********************************** \n";
    int aux;
    cin >> aux;
  }
}
////////////////////////////////////////////////////////////

/* Avoid unreachable situation */
void Interaction::unreachableCheck(TrafficlightList trafficlightList, StreetList streetList) {
  bool baux = 0, aux2 = 1;

  while (aux2) {  // check for other streetlight affected when a street is changed
    aux2 = 0;
    for (int i = 0; i < trafficlightList.number_; i++) {
      if (trafficlightList.trafficlightList_[i]->outStreet_[0] == false && trafficlightList.trafficlightList_[i]->outStreet_[2] == false && trafficlightList.trafficlightList_[i]->outStreet_[1] == false &&
              trafficlightList.trafficlightList_[i]->outStreet_[3] == false ||
          ((trafficlightList.trafficlightList_[i]->affectedLane_[0] == 1 || trafficlightList.trafficlightList_[i]->affectedLane_[0] == 3 || trafficlightList.trafficlightList_[i]->affectedLane_[0] == 6 || trafficlightList.trafficlightList_[i]->affectedLane_[0] == 10) &&
           (trafficlightList.trafficlightList_[i]->affectedLane_[1] == 1 || trafficlightList.trafficlightList_[i]->affectedLane_[1] == 3 || trafficlightList.trafficlightList_[i]->affectedLane_[1] == 6 || trafficlightList.trafficlightList_[i]->affectedLane_[1] == 10) &&
           (trafficlightList.trafficlightList_[i]->affectedLane_[2] == 2 || trafficlightList.trafficlightList_[i]->affectedLane_[2] == 4 || trafficlightList.trafficlightList_[i]->affectedLane_[2] == 7 || trafficlightList.trafficlightList_[i]->affectedLane_[2] == 11) &&
           (trafficlightList.trafficlightList_[i]->affectedLane_[3] == 2 || trafficlightList.trafficlightList_[i]->affectedLane_[3] == 4 || trafficlightList.trafficlightList_[i]->affectedLane_[3] == 7 ||
            trafficlightList.trafficlightList_[i]->affectedLane_[3] == 11))) {         // check trafficlights
        for (int j = 0; j < streetList.number_; j++) {  // check streets
          Vector2D aux = streetList.streetList_[j]->position_ - trafficlightList.trafficlightList_[i]->position_;
          if (aux.module() < streetList.streetList_[j]->length_ && streetList.streetList_[j]->getPosition().module() > trafficlightList.trafficlightList_[i]->getPosition().module()) {
            baux = changeStreet(
                (i != (trafficlightList.number_ - 1))
                    ? ((trafficlightList.trafficlightList_[i + 1]->getPosition().y > trafficlightList.trafficlightList_[i]->getPosition().y) ? (streetList.streetList_[j]) : (streetList.streetList_[j + 1]))
                    : (streetList.streetList_[j - 2]),
                true, (i != (trafficlightList.number_ - 1)) ? (trafficlightList.trafficlightList_[i]->outStreet_[0]) : (trafficlightList.trafficlightList_[i]->outStreet_[2]));  // TODO cambiar quizas
            /*streetsInCrossroad((i!=(trafficlightList.number_-1))?((trafficlightList.trafficlightList_[i+1]->getPosition().y>trafficlightList.trafficlightList_[i]->getPosition().y)?(streetList.streetList_[j]):(streetList.streetList_[j+1])):(streetList.streetList_[j-2]),trafficlightList.trafficlightList_[i]);
            streetsInCrossroad((i!=(trafficlightList.number_-1))?((trafficlightList.trafficlightList_[i+1]->getPosition().y>trafficlightList.trafficlightList_[i]->getPosition().y)?(streetList.streetList_[j]):(streetList.streetList_[j+1])):(streetList.streetList_[j-2]),trafficlightList.trafficlightList_[i]);*///cambiar para que solo se actualicen los dos trafficlights_ afectados
            if (baux) aux2 = baux;
            break;  // just change one street
          }
        }
      }
    }
    streetsInCrossroad(streetList, trafficlightList);  // TODO ? quitar cuando se cambie lo de up
  }
}
/* Compatible edge streets */
void Interaction::exteriorStreets(StreetList* streets_, int i, int j, int k, int length_) {
  // if(k)	//up
  for (int n = 0; n < streets_->number_; n++)
    if (streets_->streetList_[n]->getPosition().x < 0 && streets_->streetList_[n]->getPosition().y == (length_ * j - length_ / 2 * (k - 1)) ||
        streets_->streetList_[n]->getPosition().y < 0 && streets_->streetList_[n]->getPosition().x == (length_ * i + length_ / 2 * k))
      streets_->add(
          new Street({(float) (length_ * i + length_ / 2 * k), (float) (length_ * j - length_ / 2 * (k - 1))}, k,
                    streets_->streetList_[n]->getType(), true));
}
////////////////////////////////////////////////////////////////
/* Return exit street */
bool Interaction::changeStreet(Trafficlight* trafficlight, Street* street) {
  if (sqrt(pow(trafficlight->position_.x - street->position_.x, 2)) < 1 && trafficlight->position_.y < street->position_.y)  // street up
  {
    if (street->type_ == 1 || street->type_ == 3 || street->type_ == 6 || street->type_ == 10)
      return true;
    else
      return false;
  } else if (sqrt(pow(trafficlight->position_.x - street->position_.x, 2)) < 1 && trafficlight->position_.y > street->position_.y)  // street down
  {
    if (street->type_ == 2 || street->type_ == 4 || street->type_ == 7 || street->type_ == 11)
      return true;
    else
      return false;
  } else if (sqrt(pow(trafficlight->position_.y - street->position_.y, 2)) < 1 && trafficlight->position_.x < street->position_.x)  // street right
  {
    if (street->type_ == 1 || street->type_ == 3 || street->type_ == 6 || street->type_ == 10)
      return true;
    else
      return false;
  } else if (sqrt(pow(trafficlight->position_.y - street->position_.y, 2)) < 1 && trafficlight->position_.x > street->position_.x)  // street left
  {
    if (street->type_ == 2 || street->type_ == 4 || street->type_ == 7 || street->type_ == 11)
      return true;
    else
      return false;
  } else {
    cout << "\n\tERROR FUNCTION changeStreet(trafficlight, street)";
    return -1;
  }
}
void Interaction::changeStreet(StreetList streetList) {
  for (int i = 0; i < streetList.number_; i++) bool a = changeStreet(streetList.streetList_[i], false, false);
}
bool Interaction::changeStreet(Street* street, bool inalcanzable, bool entrada) {
  if (inalcanzable) {
    cout << "Changed because of incompatibility";
    if (entrada) {
      street->setType(2);
      cout << " of entry";
      street->setColor(1, 0, 0);
    } else {
      street->setType(1);
      cout << " of exit";
      street->setColor(0, 1, 0);
    }
    return 1;
  } else {
    if (street->getType() < 1) {
      street->setType(2);  // change 14 to multiple lanes
      cout << "change street to 2";
    } else if (street->getType() > 2) {  // change 14 to multiple lanes
      street->setType(1);
      cout << "change street to 1";
    }
    return 0;
  }
}
/* Assign occupancy value depending on number of cars */
void Interaction::streetOccupancy(CarList carList, StreetList streetList) {
  for (int i = 0; i < streetList.number_; i++) {
    streetList.streetList_[i]->setOccupancy(0);  // restart occupancy in every street
  }
  for (int i = 0; i < carList.number_; i++) {
    int aux;  // TODO cambiar en cierto momento esto se va a la mierda en 0 0, Car 0 empieza un turning_ y desaparece del
              // mapa
    aux = getStreetIndex(carList.carList_[i], streetList);                            // return street index
    streetList.streetList_[aux]->setOccupancy((streetList.streetList_[aux]->getOccupancy() + 1));  // add 1 to occupancy for each car
  }
}
void Interaction::streetOccupancy(CarList carList, Street* street) {
  street->occupancy_ = 0;  // restart occupancy value
  for (int i = 0; i < carList.number_; i++) {
    if ((carList.carList_[i]->getPosition() - street->getPosition()).module() < street->getLength() / 2) {  // car on street
      street->occupancy_++;
    }
  }
}
/////////////////////////////////////////////////////////////////////////////////
/* Return street index */
int Interaction::getStreetIndex(Car* car, StreetList streetList) {
  Vector2D aux;
  for (int i = 0; i < streetList.number_; i++) {
    aux = car->position_ - streetList.streetList_[i]->position_;
    if (aux.module() < (streetList.streetList_[i]->length_ / 1.95f)) return i;
  }
}
//////////////////////////////////////////////////////////////////////

/*TODO quesesto Comportamiento del semaforo*/
int getRC(Graph* graph, int* r, int n) {
  DOUT << "\n\tSe calculan tiempos de semaforo segun length_ de calle (TODO)" << endl;
  /*for(int i=0;i<n;i++){
          graph->getLink
  }*/
  return -1;
}

void Interaction::onTimerSem(StreetList& streetList, CarList& carList, TrafficlightList trafficlightList, int opt, Graph* trafficlightGraph) {
  //DOUT << "\n\t[DEBUG] - Start general onTimerSem" << endl;
  static bool flagaux = false;
  if (!flagaux && opt == 2) {
    DOUT << "\nLoop route to green wave" << endl;
    int *rsem, *rc, n = -1;
    Algorithm::greenWave(trafficlightGraph, rsem, n, streetList);
    DOUT << "\nGreen wave route finished" << endl;
    // Se calculan tiempos de trafficlights_ segun length_ de calle
    for (int i = 0; i < n; i++) {
      // rc[i]=getRC(trafficlightGraph,rsem,n);
      // onTimerSem(streetList, trafficlightList.trafficlightList_[i]);
      DOUT << "\nDescomentar esto (TODO)" << endl;
    }
  }
  for (int i = 0; i < trafficlightList.number_; i++)
    onTimerSem(streetList,carList,trafficlightList.trafficlightList_[i],opt,&trafficlightGraph[i],flagaux,i);
    flagaux = true;  // to assign times just the first time

  //DOUT << "\n\t[DEBUG] - Exit general onTimerSem" << endl;
}
void Interaction::onTimerSem(StreetList& streetList, CarList& carList, Trafficlight* trafficlight, int opt, Graph* trafficlightGraph, bool flagaux,
                             int ind) {
  // DOUT << "\n\t[DEBUG] - Start specific onTimerSem " << ind << endl;
  if (trafficlightGraph->getAuxiliarTimer() > (10 * 40)) {  // each 0.25s
    switch (trafficlight->getStateMode()) {
      case 0:  // normal case with equal times
        switch (opt) {
          case 0:  // normal case
            DOUT << "\nOnTimerSem opt 0 case, equal times" << endl;
            // initally generated times
            break;
          case 1:  // local optimization
            DOUT << "\nOnTimerSem opt 1 case, equal times" << endl;
            trafficlightGraph->calculateTrafficlightWeight(streetList, carList, trafficlight, ind);
            break;
          case 2:  // general optimization
            DOUT << "\nOnTimerSem opt 2 case, equal times, no changes" << endl;
            break;
          default:
            cout << "\nERROR STREETLIGHT OPTIMIZATION SELECTION 0";
            cin >> opt;
            break;
        }
        break;
      case 1:  // variable case with more horizontal time
        switch (opt) {
          case 0:  // normal case
            DOUT << "\nOnTimerSem opt 0 case, more horizontal time" << endl;
            if (!flagaux)
              trafficlight->setTimer(rand() % 200 + 360, rand() % 160 + 320, rand() % 200 + 520,
                          rand() % 160 + 320);  // assign values the first time
            break;
          case 1:  // local optimization 
            DOUT << "\nOnTimerSem opt 1 case, more horizontal time" << endl;
            trafficlightGraph->calculateTrafficlightWeight(streetList, carList, trafficlight, ind);
            break;
          case 2:  // optimizacion general
            DOUT << "\nOnTimerSem opt 2 case, more horizontal time, no changes" << endl;
            break;
          default:
            cout << "\nERROR STREETLIGHT OPTIMIZATION SELECTION 1";
            cin >> opt;
            break;
        }
        break;
      case 2:  // variable case with more vertical time
        switch (opt) {
          case 0:  // normal case
            DOUT << "\nOnTimerSem opt 0 case, more vertical time" << endl;
            if (!flagaux)
              trafficlight->setTimer(rand() % 200 + 520, rand() % 160 + 320, rand() % 200 + 360,
                          rand() % 160 + 320);  // assign values the first time
            break;
          case 1:  // local optimization
            DOUT << "\nOnTimerSem opt 1 case, more vertical time" << endl;
            trafficlightGraph->calculateTrafficlightWeight(streetList, carList, trafficlight, ind);
            break;
          case 2:  // general optimization
            DOUT << "\nOnTimerSem opt 2 case, more vertical time, no changes" << endl;
            break;
          default:
            cout << "\nERROR STREETLIGHT OPTIMIZATION SELECTION 2";
            cin >> opt;
            break;
        }
        break;
      case 3:  // fixed horizontal case
        DOUT << "\nOnTimerSem fixed horizontal case" << endl;
        trafficlight->setTimer(1, 1, 0, 1);
        trafficlight->setTemp(0, 1);  // avoid memory leaks when the simulation is running
        break;
      case 4:  // fixed vertical case
        DOUT << "\nOnTimerSem fixed vertical case" << endl;
        trafficlight->setTimer(0, 1, 1, 1);
        trafficlight->setTemp(0, 1);  // avoid memory leaks when the simulation is running
        break;
    }
    trafficlightGraph->setAuxiliarTimer(0);
  }
  trafficlightGraph->setAuxiliarTimer(trafficlightGraph->getAuxiliarTimer() + 1);
  trafficlight->stateMachine();
  // cout << "\n\t[DEBUG] - Exit specific onTimerSem " << ind << endl;
}

void Interaction::onTimerSem(StreetList& streetList, Trafficlight* trafficlight) {
  DOUT << "onTimerSem green wave (TODO)" << endl;
}
///////////////////////////////

int Interaction::checkMissingCars(CarList carList) {
  int aux = 0;
  for (int i = 0; i < carList.number_; i++)
    if (carList.carList_[i]->position_.module() > 150) aux++;
  return aux;
}
//////////////////////////////////////////////////////

int Interaction::bitmask(Car* car, Trafficlight* trafficlight, float dist) {
  int numdec = 0, numbin[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  float auxx = car->position_.x - trafficlight->position_.x, auxy = car->position_.y - trafficlight->position_.y;
  if (dist <= trafficlight->kSize)
    numbin[11] = 0;  // dentro del semaforo
  else if (dist < 1.3f * trafficlight->kSize)
    numbin[11] = 1;  // fuera del semaforo
  switch (trafficlight->state_) {
    case 0:  // vertical green-horizontal red
      numbin[10] = 0;
      numbin[9] = 0;
      break;
    case 1:  // vertical amber-horizontal red 
      numbin[10] = 0;
      numbin[9] = 1;
      break;
    case 2:  // vertical red-horizontal green
      numbin[10] = 1;
      numbin[9] = 0;
      break;
    case 3:  // vertical red-horizontal amber
      numbin[10] = 1;
      numbin[9] = 1;
      break;
  }
  switch (car->direction_) {
    case 0:  // up
      numbin[8] = 0;
      numbin[7] = 0;
      break;
    case 1:  // right
      numbin[8] = 0;
      numbin[7] = 1;
      break;
    case 2:  // down
      numbin[8] = 1;
      numbin[7] = 0;
      break;
    case 3:  // left
      numbin[8] = 1;
      numbin[7] = 1;
      break;
  }
  switch (car->turning_) {
    case -1:  // turning left
      numbin[6] = 0;
      numbin[5] = 0;
      break;
    case 0:  // going forward
      numbin[6] = 0;
      numbin[5] = 1;
      break;
    case 1:  // turning right
      numbin[6] = 1;
      numbin[5] = 0;
      break;
  }                  // todo revisar
  if (trafficlight->outStreet_[0])  // up street available
    numbin[1] = 1;
  if (trafficlight->outStreet_[1])  // right street available
    numbin[2] = 1;
  if (trafficlight->outStreet_[2])  // down street available
    numbin[3] = 1;
  if (trafficlight->outStreet_[3])  // left street available
    numbin[4] = 1;

  if (car->direction_ == 0 && auxy >= ((float) (car->n[1] - 6)) || car->direction_ == 2 && auxy <= ((float) (car->n[1] - 2)) ||
      car->direction_ == 1 && auxx >= ((float) (2 - car->n[1])) || car->direction_ == 3 && auxx <= ((float) (6 - car->n[1])))
    numbin[0] = 1;
  else
    numbin[0] = 0;

  for (int i = 0; i < 12; i++) numdec += numbin[i] * pow(2, i);

  // if(numdec>500)
  // cout << "  " << numdec << "  ";
  return numdec;
}

// Associate graph links with streets
int Interaction::associateLinkIndex(StreetList streetList, Trafficlight* trafficlight, int relativePosition, int rows, int columns, bool flag) {
  // relativePosition=0 up, 1 right, 2 down, 3 left
  for (int i = 0; i < streetList.number_; i++) {
    if ((trafficlight->getPosition() - streetList.streetList_[i]->getPosition()).module() < streetList.streetList_[i]->getLength()) {
      switch (relativePosition) {
        case 0:
          if (trafficlight->getPosition().y < streetList.streetList_[i]->getPosition().y) {  // return index street or occupancy
            if (flag)
              return i;
            else {
              if (streetList.streetList_[i]->getPosition().y < ((rows - 1) * streetList.streetList_[i]->getLength()))
                return streetList.streetList_[i]->getOccupancy();
              else
                for (int j = 0; j < streetList.number_; j++)
                  if (streetList.streetList_[i]->getPosition().x == streetList.streetList_[j]->getPosition().x && streetList.streetList_[j]->getPosition().y < 0)
                    return streetList.streetList_[i]->getOccupancy() + streetList.streetList_[j]->getOccupancy();
            }
          }
          break;
        case 1:
          if (trafficlight->getPosition().x < streetList.streetList_[i]->getPosition().x) {
            if (flag)
              return i;
            else {
              if (streetList.streetList_[i]->getPosition().x < ((columns - 1) * streetList.streetList_[i]->getLength()))
                return streetList.streetList_[i]->getOccupancy();
              else
                for (int j = 0; j < streetList.number_; j++)
                  if (streetList.streetList_[i]->getPosition().y == streetList.streetList_[j]->getPosition().y && streetList.streetList_[j]->getPosition().x < 0)
                    return streetList.streetList_[i]->getOccupancy() + streetList.streetList_[j]->getOccupancy();
            }
          }
          break;
        case 2:
          if (trafficlight->getPosition().y > streetList.streetList_[i]->getPosition().y) {
            if (flag)
              return i;
            else {
              if (streetList.streetList_[i]->getPosition().y > 0)
                return streetList.streetList_[i]->getOccupancy();
              else
                for (int j = 0; j < streetList.number_; j++)
                  if (streetList.streetList_[i]->getPosition().x == streetList.streetList_[j]->getPosition().x &&
                      streetList.streetList_[j]->getPosition().y > ((rows - 1) * streetList.streetList_[i]->getLength()))
                    return streetList.streetList_[i]->getOccupancy() + streetList.streetList_[j]->getOccupancy();
            }
          }
          break;
        case 3:
          if (trafficlight->getPosition().x > streetList.streetList_[i]->getPosition().x) {
            if (flag)
              return i;
            else {
              if (streetList.streetList_[i]->getPosition().x > 0)
                return streetList.streetList_[i]->getOccupancy();
              else
                for (int j = 0; j < streetList.number_; j++)
                  if (streetList.streetList_[i]->getPosition().y == streetList.streetList_[j]->getPosition().y &&
                      streetList.streetList_[j]->getPosition().x > ((columns - 1) * streetList.streetList_[i]->getLength()))
                    return streetList.streetList_[i]->getOccupancy() + streetList.streetList_[j]->getOccupancy();
            }
          }
          break;
        default:
          return -1;
      }
    }
  }
}

/* Avoid stop problems and change direction (TODO cambiar: se debería arreglar en otra parte del código
 * pero no se cual)*/
void Interaction::setDirectionPatch(CarList carList, StreetList streetList) {
  for (int i = 0; i < carList.number_; i++) {
    int cont = 0;
    for (int j = 0; j < carList.number_; j++) {
      if (i != j) {
        Vector2D aux = carList.carList_[i]->getPosition() - carList.carList_[j]->getPosition();
        if (aux.module() < 8 || carList.carList_[i]->trafficlightFlag_) {  // close to other car or traffic light
          cont++;
        }
      }
    }
    if (!cont && carList.carList_[i]->getStop()) {  // if counter dont change and car is stopped
      carList.carList_[i]->changeStop(0);  // restart car movement TODO cambiar comprobar que no se vaya en otra direction_
      cout << "\nRestart car movement: " << i;
    }
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////