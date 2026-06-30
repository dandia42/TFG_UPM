#pragma once
#include "world.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fstream>
void World::worldGeneration(int rows_, int columns_) {
  for (float i = 0; i < columns_; i++)
    for (float j = 0; j < rows_; j++) {
      trafficlights_.add(new Trafficlight(((auxiliarStreet_->getLength()) * i), ((auxiliarStreet_->getLength()) * j), rand() % 3));
      for (int k = 0; k < 2; k++)
        if (i == (columns_ - 1) && k || k == 0 && j == (rows_ - 1)) {
          Interaction::exteriorStreets(pStreets_, i, j, k, auxiliarStreet_->getLength());
        } else
          streets_.add(
              new Street({(auxiliarStreet_->getLength()) * i + (auxiliarStreet_->getLength()) / 2 * k,
                         (auxiliarStreet_->getLength()) * j - (auxiliarStreet_->getLength()) / 2 * (k - 1)},
                        k));  // Coordinates are sent this way so the position_ is the central point
                              // and make a correct horizontal and vertical building
      if (i == 0)
        streets_.add(new Street({-(auxiliarStreet_->getLength()) + (auxiliarStreet_->getLength()) / 2, (auxiliarStreet_->getLength()) * j}, 1));
      if (j == 0)
        streets_.add(new Street({(auxiliarStreet_->getLength()) * i, -(auxiliarStreet_->getLength()) + (auxiliarStreet_->getLength()) / 2}, 0));
    }
  // cars_.add(new Car({-10,60},3));// TODO quitar coche en 6, tiene que ir a 0
  // cars_.add(new Car(streets_.streetList_[16]->getPosition(),3));
  // cars_.add(new Car(streets_.streetList_[27]->getPosition(),2));
  // Interaction::callesemsize(streets_,trafficlights_);
  cout << "number de streets_: " << streets_.number_ << "\tnumero de trafficlights_: " << trafficlights_.number_ << "ff";
}

World::World() {
  auxiliarStreet_ = new Street({-10, -10}, 0);
  // init();
}

World::~World() {
  delete[] trafficlightGraph_;
  delete auxiliarStreet_;
  cars_.deleteContent();
  streets_.deleteContent();
  trafficlights_.deleteContent();
}

void World::init() {
  /*float xxx=0;
  for(int i = 0; i < 40; i++)
          xxx+=3 * cos(0.0125f * i * 3) * 0.025;
          cout << xxx << endl;*/
  while (rows_ <= 0) {
    cout << "Define rows: " << endl;
    cin >> rows_;
  }
  while (columns_ <= 0) {
    cout << "Define columns: " << endl;
    cin >> columns_;
  }
  xPosition_ = (auxiliarStreet_->getLength()) / 2 * (columns_ - 1);
  yPosition_ = (auxiliarStreet_->getLength()) / 2 * (rows_ - 1);
  zPosition_ = (xPosition_ > yPosition_ ? ((xPosition_ + (2 * (auxiliarStreet_->getLength()))) / tan(PI / 6))
                   : ((yPosition_ + (3 * (auxiliarStreet_->getLength()))) / tan(PI / 6)));
  /*xPosition_=(auxiliarStreet_->getLength()+12)/2*(columns_-1);
  yPosition_=(auxiliarStreet_->getLength()+12)/2*(rows_-1);
  zPosition_=(xPosition_>yPosition_ ? ((xPosition_ + (2 * (auxiliarStreet_->getLength() + 12 ))) / tan(PI / 6))
            : ((yPosition_ + (3 * (auxiliarStreet_->getLength() + 12))) / tan(PI / 6)));*/

  worldGeneration(rows_, columns_);
  // streets_.streetList_[0]->setType(1); // TODO quitar
  Interaction::streetsInCrossroad(streets_, trafficlights_);
  Interaction::unreachableCheck(trafficlights_, streets_);
  for (int i = 0; i < trafficlights_.number_; i++) trafficlights_.trafficlightList_[i]->initialLightsState();

  Interaction::addCar(pCars_, streets_);
  Interaction::streetOccupancy(cars_, streets_);
  iterations_ = 10 * cars_.number_;
  for (int i = 0; i < cars_.number_; i++)
    cars_.carList_[i]->setInitialTime(clock());  // Start time_ counter for the first iteration
  cout << "\n\nDefine optimizations:";
  while (routeOptimization_ < 0) {
    cout << "\nRutas:\n\t0) None\n\t1) dijkstra\n\t2) Floyd-Warshall\n";
    cin >> routeOptimization_;
    if (routeOptimization_ < 0 || routeOptimization_ > 2) {
      cout << "\n\nWrong value, try again: ";
      routeOptimization_ = -1;
    } else {
      graph_.buildGraph(rows_, columns_, streets_, trafficlights_, true);
      cout << "\nGraph created";
      if (routeOptimization_ == 2) {
        double tini = clock();
        Algorithm::floydWarshall(pGraph_);
        time_ = (double) (clock() - tini) / CLOCKS_PER_SEC;
        cout << "\n\n******************** Floyd-Warshall algorithm ********************\n******************** Time "
                "alg: "
             << time_ << " seconds ********************\n\n";
      }
      Interaction::setRoute(cars_, pGraph_, streets_, routeOptimization_);
    }
  }
  if (trafficlightGraph_ != nullptr) {
    delete[] trafficlightGraph_;
    trafficlightGraph_ = nullptr;
  }
  while (trafficlightOptimization_< 0) {
    cout << "\nTraffic lights:\n\t0) None\n\t1) Local\n\t2) General\n\t\t";
    cin >> trafficlightOptimization_;
    switch (trafficlightOptimization_) {
      case 0:  // None
        trafficlightGraph_ = new Graph[trafficlights_.number_];
        for (int i = 0; i < trafficlights_.number_; i++) trafficlightGraph_[i].buildGraph(streets_, cars_, trafficlights_, i, trafficlightOptimization_);
        cout << "\nState machine created for " << trafficlights_.number_ << " trafficlights";
        break;
      case 1:  // Local
        trafficlightGraph_ = new Graph[trafficlights_.number_];
        for (int i = 0; i < trafficlights_.number_; i++) trafficlightGraph_[i].buildGraph(streets_, cars_, trafficlights_, i, trafficlightOptimization_);
        cout << "\nLocal graph created for " << trafficlights_.number_ << " trafficlights";
        break;
      case 2:  // General/green wave
        cout << "\nInitialize grafoGsem";
        trafficlightGraph_ = new Graph[1];
        trafficlightGraph_->buildGraph(rows_, columns_, streets_, trafficlights_, true);
        cout << "\nGreen wave graph created" << endl;
        break;
      default:
        cout << "\n\nWrong value, try again: ";
        trafficlightOptimization_ = -1;
        break;
        // Weight definition in real time in traffic light onTimer method
    }
  }
}

void World::initGL() {
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);

  glMatrixMode(GL_PROJECTION);
  gluPerspective(45.0, 2000 / 1500.0f, 0.1, 500);
}

void World::onKeyboardDown(unsigned char key, int x, int y) {
  static int i = 0;
  // Used keys
  // a s d w e ' ' v b_ i h k m p z 1 2 3 4 .
  if (Editor::getEnable()) {
    // edit.loadData(cars_, streets_, trafficlights_);
    Editor::editorFunctions(key, cars_, streets_, trafficlights_);
  }
  switch (key) {  // a,s,d,w,e, ,v,b_,i,h,k,m,p,1,2,3,4,.,
    case 'a':
      cars_.carList_[0]->setTurn(-1);
      break;
    /*case 's':
            i++;
            break;*/
    case 'd':
      cars_.carList_[0]->setTurn(1);
      break;
    case 'w':
      cars_.carList_[0]->setTurn(0);
      cars_.carList_[0]->setColor(1, 0.5f, 1);
      cars_.carList_[0]->setVelocity(1);
      cars_.carList_[0]->setVelocityDirection();
      break;
    case 'e':
      Editor::toggleEnable();
      break;
    case ' ':
      cout << "\n";
      for (int i = 0; i < trafficlights_.number_; i++) {
        cout << "\nTraffic light " << i << " has the next times: ";
        for (int j = 0; j < 4; j++) cout << trafficlights_.trafficlightList_[i]->getTimer(j) << " ";
        cout << " temporization and auxiliar temporization: " << trafficlights_.trafficlightList_[i]->getTemp(0) << " " << trafficlights_.trafficlightList_[i]->getTemp(1);
      }
      /*for(int i=0; i<cars_.number_; i++)
              cout<<"\nel coche "<<i<<" tiene direccion "<<cars_.carList_[i]->getDirection()<<"   stop
      "<<cars_.carList_[i]->getStop(); cout<<"\nel proximo turning_ es: "<<cars_.carList_[0]->getTurning();
      //cout<<Interaction::checkMissingCars(cars_);*/
      break;
    case 'v':
      cout << "\n getTemp(0) temp_: " << trafficlights_.trafficlightList_[0]->getTemp(0) << endl;
      cout << "\n getTemp(1) auxiliarTimer_: " << trafficlights_.trafficlightList_[0]->getTemp(1) << endl;
      cout << "\n getTemp(0) temp_: " << trafficlights_.trafficlightList_[1]->getTemp(0) << endl;
      cout << "\n getTemp(1) auxiliarTimer_: " << trafficlights_.trafficlightList_[1]->getTemp(1) << endl;

      for (int i = 0; i < 4; i++)
      cout << "\n traffic light 0 state times: " << trafficlights_.trafficlightList_[0]->getStateTime(i) << endl;
      for (int i = 0; i < 4; i++)
      cout << "\n traffic light 1 state times: " << trafficlights_.trafficlightList_[1]->getStateTime(i) << endl;

      cout << "\n traffic light 0 state: " << trafficlights_.trafficlightList_[0]->getState() << endl;
      cout << "\n traffic light 1 state: " << trafficlights_.trafficlightList_[0]->getState() << endl;


      //trafficlights_.trafficlightList_[0]->setState(0);
      break;
    /*case 'b_':
            for(int i=0; i<streets_.number_; i++)
            streets_.streetList_[i]->setType(streets_.streetList_[0]->getType()+1);
            Interaction::changeStreet(streets_);
            Interaction::unreachableCheck(trafficlights_, streets_);
            Interaction::streetsInCrossroad(streets_,trafficlights_);
            break;*/
    case 'i':
      for (int i = 0; i < streets_.number_; i++) streets_.streetList_[i]->setType(streets_.streetList_[i]->getType() + 1);
      Interaction::changeStreet(streets_);
      Interaction::unreachableCheck(trafficlights_, streets_);
      Interaction::streetsInCrossroad(streets_, trafficlights_);
      break;
    case 'h':
      *pIterations_ = 0;
      cout << "\n\tThe simulation will end when the next route is finished\n\n";
      break;
    case 'k':
      break;
    case 'm':
      for (int i = 0; i < trafficlights_.number_; i++) {
        cout << "\nTraffic light " << i << " has the next out streets: ";
        for (int j = 0; j < 4; j++) cout << trafficlights_.trafficlightList_[i]->getOutDirection(j) << " ";
      }
      break;
    case '1':
      cars_.carList_[0]->setVelocity(0, 3);
      break;
    case '2':
      cars_.carList_[0]->setVelocity(3, 0);
      break;
    case '3':
      cars_.carList_[0]->setVelocity(0, -3);
      break;
    case '4':
      cars_.carList_[0]->setVelocity(-3, 0);
      break;
    case 'p':
      char aux;
      cout << "\n\nPAUSED\n\n";
      cin >> aux;
      break;
    case 'z':
      cout << "\n Trafficlights have the next times: vertical green, vertical amber, ";
      "horizontal green horizontal amber";
      for (int i = 0; i < trafficlights_.number_; i++) {
        cout << "\nTraffic light " << i;
        for (int j = 0; j < 4; j++) cout << "\t" << trafficlights_.trafficlightList_[i]->getTimer(j) << " ";
      }
      break;
    case '.':  // debug
      int auxgiro = 8;
      for (int i = 0; i < cars_.number_; i++) {
        auxgiro = cars_.carList_[i]->getTurning();
        cars_.carList_[i]->debugFlag_ = false;
        cout << "\nCar " << i << " has turning: " << auxgiro;
      }
      break;
  }
}

void World::onTimer() {
  float t = 0.025;
  if (!Editor::getEnable()) {  // TODO: remove this
    cars_.move(t);
    Interaction::onTimerSem(streets_, cars_, trafficlights_, trafficlightOptimization_, trafficlightGraph_);
    Interaction::crossroads(cars_, trafficlights_, streets_, pGraph_, &time_, iterations_, rows_, columns_, routeOptimization_);
    Interaction::exitWorld(cars_, rows_, columns_, streets_.getLength());
    Interaction::setDirectionPatch(cars_, streets_);  // before safetyDistance because stop changes
    Interaction::safetyDistance(cars_);
    Interaction::changeLane(cars_, streets_);
  }
  // cout<<" x  "<<cars_.carList_[0]->getPosition().x<<" y  "<<cars_.carList_[0]->getPosition().y;
  // cout<<"\ncarril inicio "<<cars_.carList_[0]->n[0]<<" carril destino "<<cars_.carList_[0]->n[1]<<"\t";
  // printf("\nvx=%f\tvy=%f\tpx=%f\tpy=%f\nangulo=%f\t%f",cars_.carList_[0]->getVelocity().x,cars_.carList_[0]->getVelocity().y,cars_.carList_[0]->getPosition().x,
  // cars_.carList_[0]->getPosition().y, cars_.carList_[0]->getAngleRad(), cars_.carList_[0]->getAngleDegrees());
}

void World::onDraw() {
  // Clear window
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Define point of view
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(xPosition_, yPosition_, zPosition_,   // eye position_
            xPosition_, yPosition_, 0.0,   // looking at  (0,0,0)
            0.0, 1.0, 0.0);  // define up (Y axis)

  // Start drawing
  cars_.draw();
  streets_.draw();
  trafficlights_.draw();
  Editor::draw();

  // Change buffer at the end
  glutSwapBuffers();
}
