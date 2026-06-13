#pragma once
#include "editor.h"

#include <iostream>
using namespace std;

Vector2D Editor::position_ = {-100, -100};
int Editor::index_ = 0, Editor::maxIndex_ = 0;
int Editor::object_ = 0;
bool Editor::loadFlag_ = false;
bool Editor::enable_ = false;

void Editor::draw() {
  if (enable_) {  // Editor enabled and no cars edited
    glPushMatrix();
    glColor3f(1, 1, 1);
    glDisable(GL_LIGHTING);
    glTranslatef(position_.x, position_.y, -2);
    glRotatef(45, 0, 0, 1);
    glBegin(GL_POLYGON);  // Rectangle 1
    glVertex2d(-15, -3);
    glVertex2d(-15, 3);
    glVertex2d(15, 3);
    glVertex2d(15, -3);
    glEnd();
    glBegin(GL_POLYGON);  // Rectangle 2
    glVertex2d(-3, -15);
    glVertex2d(-3, 15);
    glVertex2d(3, 15);
    glVertex2d(3, -15);
    glEnd();

    glPopMatrix();
  }
}
void Editor::setPosition(int type, int index, CarList cars, StreetList streets, TrafficlightList trafficlights) { //TODO quitar type index
  switch (object_) {
    case 1:  // street
      position_ = streets.streetList_[index_]->getPosition();
      cout << "\nse cambia la position_ a la street " << index_ << endl;
      break;
    case 2:  // coche
      position_ = cars.carList_[index_]->getPosition();
      break;
    case 3:  // trafficlight
      position_ = trafficlights.trafficlightList_[index_]->getPosition();
      cout << "\nse cambia la state_ al trafficlight " << index_ << ", ahora es " << trafficlights.trafficlightList_[index_]->getState() << endl;
      break;
  }
}
bool Editor::getEnable() {
  return enable_;
}
int Editor::getObjectType() {
  return object_;
}
int Editor::getIndex() {
  return index_;
}
void Editor::loadData(CarList co, StreetList ca, TrafficlightList s) {
  if (loadFlag_ == false) {  // Load static data the first time

    /*cars=&co;
    streets=&ca;
    trafficlights=&s;*/
    loadFlag_ = true;
    cout << "Static data have been loaded" << endl;
  }
}
void Editor::updateData(CarList* pco, StreetList* pca, TrafficlightList* ps) {}

void Editor::toggleEnable() {
  enable_ = !enable_;
}

void Editor::editorFunctions(unsigned char key, CarList carList_, StreetList streetList, TrafficlightList trafficlightList) {
  switch (key) {
    case 'i':  // Increase object state (street type, trafficlight state)
      switch (object_) {
        case 1:  // street
          changeStreet(1, streetList, trafficlightList);
          cout << "Increase street";
          break;
        case 2:  // car
          changeCarNumber(1, carList_, streetList);
          cout << "Increase car";
          break;
        case 3:  // trafficlight
          changeTrafficlightState(1, trafficlightList);
          cout << "Increase trafficlight";
          break;
      }
      break;
    case 'o':  // Decrease object state (street type, trafficlight state)
      switch (object_) {
        case 1:  // street
          changeStreet(-1, streetList, trafficlightList);
          cout << "Decrease street";
          break;
        case 2:  // car
          changeCarNumber(0, carList_, streetList);
          cout << "Decrease car";
          break;
        case 3:  // trafficlight
          changeTrafficlightState(-1, trafficlightList);
          cout << "Decrease trafficlight";
          break;
      }
      break;
    case 'x':
      switch (object_) {
        case 1:  // street
          streetList.deleteIndex(index_);
          cout << "Delete street";
          break;
        case 2:  // coche
          carList_.deleteIndex(index_);
          cout << "Delete cars";
          break;
        case 3:  // trafficlight
          trafficlightList.deleteIndex(index_);
          cout << "Delete trafficlight";
          break;
      }
      break;
    case '.':  // Increase longitud
      if (object_ == 1) changeLength(streetList, carList_, trafficlightList, 1);
      break;
    case ',':  // Decrease longitud
      if (object_ == 1) changeLength(streetList, carList_, trafficlightList, -1);
      break;
    case '1':  // Change to street
      changeObjectType(1);
      changeObject(0, carList_, streetList, trafficlightList);
      cout << "Object: street" << object_ << "\t";
      break;
    case '2':  // Change to coche
      changeObjectType(2);
      changeObject(0, carList_, streetList, trafficlightList);
      cout << "Object: coche" << object_ << "\t";
      break;
    case '3':  // Change to trafficlight
      changeObjectType(3);
      changeObject(0, carList_, streetList, trafficlightList);
      cout << "Object: trafficlight" << object_ << "\t";
      break;
    case '9':  // Restart world
      changeMatrix();
      break;
    case 'r_':  // Increase object type index
      changeObject(1, carList_, streetList, trafficlightList);
      cout << "Index: " << index_ << "\t";
      break;
    case 'f':  // Decrease object type index
      changeObject(-1, carList_, streetList, trafficlightList);
      cout << "Index: " << index_ << "\t";
      break;
    case 's':
      cout << "Stop " << carList_.carList_[index_]->getStop();
      break;
  }
  setPosition(getObjectType(), getIndex(), carList_, streetList, trafficlightList);
}

void Editor::changeObject(int i, CarList carList, StreetList streetList, TrafficlightList trafficlightList) {
  if (i) {
    if (reachEdge(i, carList, streetList, trafficlightList)) {
    } else
      index_ += i;
  } else
    index_ = 0;  // If the object type is changed, go to the first of the list
}

void Editor::changeObjectType(int o) {
  object_ = o;
}

/*streets*/
void Editor::changeStreet(int order, StreetList streets, TrafficlightList trafficlights) {  // 1 Increase, -1 Decrease

  streets.streetList_[index_]->setType(streets.streetList_[index_]->getType() + order);
  Interaction::changeStreet(streets);
  Interaction::unreachableCheck(trafficlights, streets);
  Interaction::streetsInCrossroad(streets, trafficlights);
  cout << "\nStreet type has been changed";
}

void Editor::changeLength(StreetList streets, CarList cars, TrafficlightList trafficlights, int mult) {
  Vector2D aux = streets.streetList_[index_]->getPosition();
  if (streets.streetList_[index_]->direction_ && !((streets.streetList_[index_]->getLength() < 25 && mult < 0) ||
                                         (streets.streetList_[index_]->getLength() > 50 && mult > 0))) {  // horizontal
    for (int i = 0; i < streets.number_; i++) {
      if (streets.streetList_[i]->getPosition().x == aux.x) {
        streets.streetList_[i]->setLength(streets.streetList_[i]->getLength() + 5 * mult);
        streets.streetList_[i]->setPosition(streets.streetList_[i]->getPosition().x + 5 * mult / 2,
                              streets.streetList_[i]->getPosition().y);  // Adjust street position of the same column
      } else if (streets.streetList_[i]->getPosition().x > aux.x)
        streets.streetList_[i]->setPosition(streets.streetList_[i]->getPosition().x + 5 * mult,
                              streets.streetList_[i]->getPosition().y);  // Adjust right streets position
    }
    for (int i = 0; i < cars.number_; i++) {
      if (cars.carList_[i]->getPosition().x > aux.x)
        cars.carList_[i]->setPosition(cars.carList_[i]->getPosition().x + 5 * mult,
                              cars.carList_[i]->getPosition().y);  // Adjust right cars position
    }
    for (int i = 0; i < trafficlights.number_; i++) {
      if (trafficlights.trafficlightList_[i]->getPosition().x > aux.x)
        trafficlights.trafficlightList_[i]->setPosition(trafficlights.trafficlightList_[i]->getPosition().x + 5 * mult,
                                trafficlights.trafficlightList_[i]->getPosition().y);  // Adjust right trafficlights position
    }
  } else if (streets.streetList_[index_]->direction_ == 0 && !((streets.streetList_[index_]->getLength() < 25 && mult < 0) ||
                                                     (streets.streetList_[index_]->getLength() > 50 && mult > 0))) {  // vertical
    for (int i = 0; i < streets.number_; i++) {
      if (streets.streetList_[i]->getPosition().y == aux.y) {
        streets.streetList_[i]->setLength(streets.streetList_[i]->getLength() + 5 * mult);
        streets.streetList_[i]->setPosition(
            streets.streetList_[i]->getPosition().x,
            streets.streetList_[i]->getPosition().y + 5 * mult / 2);  // Adjust street position of the same column
      } else if (streets.streetList_[i]->getPosition().y > aux.y)
        streets.streetList_[i]->setPosition(streets.streetList_[i]->getPosition().x,
                              streets.streetList_[i]->getPosition().y + 5 * mult);  // Adjust right streets position
    }
    for (int i = 0; i < cars.number_; i++) {
      if (cars.carList_[i]->getPosition().y > aux.y)
        cars.carList_[i]->setPosition(cars.carList_[i]->getPosition().x,
                              cars.carList_[i]->getPosition().y + 5 * mult);  // Adjust right cars position
    }
    for (int i = 0; i < trafficlights.number_; i++) {
      if (trafficlights.trafficlightList_[i]->getPosition().y > aux.y)
        trafficlights.trafficlightList_[i]->setPosition(
            trafficlights.trafficlightList_[i]->getPosition().x,
            trafficlights.trafficlightList_[i]->getPosition().y + 5 * mult);  // Adjust right trafficlights position
    }
  }
}

/*cars*/
void Editor::changeCarNumber(int i, CarList carList_, StreetList streetList) {
  CarList* plco = &carList_;
  if (i) {
    Interaction::addCar(plco, streetList);
    cout << "\tNew cars have been created, there are " << carList_.number_ << " cars now\n";
  } else {
    Interaction::deleteCar(carList_, 2);
    cout << "\tCars have been deleted, there are " << carList_.number_ << " cars now\n";
  }
}

/*trafficlights*/
void Editor::changeTrafficlightState(int i, TrafficlightList trafficlights) {
  i += trafficlights.trafficlightList_[index_]->getState();
  if (i < 0)  // Avoid unreachable cases
    trafficlights.trafficlightList_[index_]->setState(3);
  else if (i > 3)
    trafficlights.trafficlightList_[index_]->setState(0);
  else
    trafficlights.trafficlightList_[index_]->setState(i);
  cout << "Trafficlight state has been changed\t";
}

/*world*/
void Editor::changeMatrix() {}

/*helper*/
bool Editor::reachEdge(int i, CarList carList, StreetList streetList, TrafficlightList trafficlightList) {
  switch (object_) {
    case 1:  // street
      maxIndex_ = streetList.number_;
      break;
    case 2:  // car
      maxIndex_ = carList.number_;
      break;
    case 3:  // trafficlight
      maxIndex_ = trafficlightList.number_;
      break;
  }
  if (index_ == (maxIndex_ - 1) && i == 1) {
    index_ = 0;
    return true;
  } else if (index_ == 0 && i == -1) {
    index_ = maxIndex_ - 1;
    return true;
  } else
    return false;
}
