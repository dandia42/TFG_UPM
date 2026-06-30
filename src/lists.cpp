#pragma once
#include "lists.h"
// builders
CarList::CarList() {
  number_ = 0;
  for (int i = 0; i < MAX_ELEM; i++) carList_[i] = 0;
}
StreetList::StreetList() {
  number_ = 0;
  for (int i = 0; i < MAX_ELEM; i++) streetList_[i] = 0;
}
TrafficlightList::TrafficlightList() {
  number_ = 0;
  for (int i = 0; i < MAX_ELEM; i++) trafficlightList_[i] = 0;
}

// add
bool CarList::add(Car* co) {
  for (int i = 0; i < number_; i++)
    if (carList_[i] == co) return false;
  if (number_ < MAX_ELEM)
    carList_[number_++] = co;
  else
    return false;  // max number
  return true;
}
bool StreetList::add(Street* ca) {
  for (int i = 0; i < number_; i++)
    if (streetList_[i] == ca) return false;
  if (number_ < MAX_ELEM)
    streetList_[number_++] = ca;
  else
    return false;  // max number
  return true;
}
bool TrafficlightList::add(Trafficlight* s) {
  for (int i = 0; i < number_; i++)
    if (trafficlightList_[i] == s) return false;

  if (number_ < MAX_ELEM)
    trafficlightList_[number_++] = s;
  else
    return false;  // max number
  return true;
}

// draw
void CarList::draw() {
  for (int i = 0; i < number_; i++) carList_[i]->draw();
}
void StreetList::draw() {
  for (int i = 0; i < number_; i++) streetList_[i]->draw();
}
void TrafficlightList::draw() {
  for (int i = 0; i < number_; i++) trafficlightList_[i]->draw();
}

// setPosition
void CarList::setPosition(float px, float py) {
  for (int i = 0; i < number_; i++) carList_[i]->setPosition(px, py);
}
void StreetList::setPosition(float px, float py) {
  for (int i = 0; i < number_; i++) streetList_[i]->setPosition(px, py);
}
void TrafficlightList::setPosition(float px, float py) {
  for (int i = 0; i < number_; i++) trafficlightList_[i]->setPosition(px, py);
}

// move
void CarList::move(float t) {
  for (int i = 0; i < number_; i++) carList_[i]->move(t);
}
/*void TrafficlightList::onTimer(int opt){
        for(int i=0;i<number_;i++)
                trafficlightList_[i]->onTimer(opt);
}*/

// delete content
void CarList::deleteContent() {
  for (int i = 0; i < number_; i++) delete carList_[i];
  number_ = 0;
}
void StreetList::deleteContent() {
  for (int i = 0; i < number_; i++) delete streetList_[i];
  number_ = 0;
}
void TrafficlightList::deleteContent() {
  for (int i = 0; i < number_; i++) delete trafficlightList_[i];
  number_ = 0;
}

// deleteObject index
void CarList::deleteIndex(int index) {
  if ((index < 0) || (index >= number_)) return;
  delete carList_[index];
  number_--;
  for (int i = index; i < number_; i++) carList_[i] = carList_[i + 1];
}
void StreetList::deleteIndex(int index) {
  if ((index < 0) || (index >= number_)) return;
  delete streetList_[index];
  number_--;
  for (int i = index; i < number_; i++) streetList_[i] = streetList_[i + 1];
}
void TrafficlightList::deleteIndex(int index) {
  if ((index < 0) || (index >= number_)) return;
  delete trafficlightList_[index];
  number_--;
  for (int i = index; i < number_; i++) trafficlightList_[i] = trafficlightList_[i + 1];
}

// deleteObject
void CarList::deleteObject(Car* deleteCar) {
  for (int i = 0; i < number_; i++)
    if (carList_[i] == deleteCar) {
      deleteIndex(i);
      return;
    }
}
void StreetList::deleteObject(Street* deleteStreet) {
  for (int i = 0; i < number_; i++)
    if (streetList_[i] == deleteStreet) {
      deleteIndex(i);
      return;
    }
}
void TrafficlightList::deleteObject(Trafficlight* deleteTrafficlight) {
  for (int i = 0; i < number_; i++)
    if (trafficlightList_[i] == deleteTrafficlight) {
      deleteIndex(i);
      return;
    }
}

void CarList::setTurn(int turn) {
  for (int i = 0; i < number_; i++) carList_[i]->setTurn(turn);
}
int StreetList::getLength() {
  return streetList_[0]->getLength();
}
