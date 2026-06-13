#pragma once
#include "lists.h"
// constructores
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

// agregar
bool CarList::add(Car* co) {
  for (int i = 0; i < number_; i++)
    if (carList_[i] == co) return false;
  if (number_ < MAX_ELEM)
    carList_[number_++] = co;
  else
    return false;  // capacidad maxima alcanzada
  return true;
}
bool StreetList::add(Street* ca) {
  for (int i = 0; i < number_; i++)
    if (streetList_[i] == ca) return false;
  if (number_ < MAX_ELEM)
    streetList_[number_++] = ca;
  else
    return false;  // capacidad maxima alcanzada
  return true;
}
bool TrafficlightList::add(Trafficlight* s) {
  for (int i = 0; i < number_; i++)
    if (trafficlightList_[i] == s) return false;

  if (number_ < MAX_ELEM)
    trafficlightList_[number_++] = s;
  else
    return false;  // capacidad maxima alcanzada
  return true;
}

// dibuja
void CarList::draw() {
  for (int i = 0; i < number_; i++) carList_[i]->draw();
}
void StreetList::draw() {
  for (int i = 0; i < number_; i++) streetList_[i]->draw();
}
void TrafficlightList::draw() {
  for (int i = 0; i < number_; i++) trafficlightList_[i]->draw();
}

// setPos
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

// destruir contenido
void CarList::destruirContenido() {
  for (int i = 0; i < number_; i++) delete carList_[i];
  number_ = 0;
}
void StreetList::destruirContenido() {
  for (int i = 0; i < number_; i++) delete streetList_[i];
  number_ = 0;
}
void TrafficlightList::destruirContenido() {
  for (int i = 0; i < number_; i++) delete trafficlightList_[i];
  number_ = 0;
}

// eliminar index
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

// eliminar
void CarList::eliminar(Car* dco) {
  for (int i = 0; i < number_; i++)
    if (carList_[i] == dco) {
      deleteIndex(i);
      return;
    }
}
void StreetList::eliminar(Street* dca) {
  for (int i = 0; i < number_; i++)
    if (streetList_[i] == dca) {
      deleteIndex(i);
      return;
    }
}
void TrafficlightList::eliminar(Trafficlight* ds) {
  for (int i = 0; i < number_; i++)
    if (trafficlightList_[i] == ds) {
      deleteIndex(i);
      return;
    }
}

void CarList::setgiro(int g_) {
  for (int i = 0; i < number_; i++) carList_[i]->setTurn(g_);
}
int StreetList::getLength() {
  return streetList_[0]->getLength();
}
