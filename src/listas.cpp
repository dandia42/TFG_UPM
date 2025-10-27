#pragma once
#include "listas.h"
					//constructores
listaCoches::listaCoches() {
	numero=0;
	for(int i=0;i<MAX_ELEM;i++)
		lco[i]=0;
}
listaCalles::listaCalles() {
	numero=0;
	for(int i=0;i<MAX_ELEM;i++)
		lca[i]=0;
}
listaSemaforos::listaSemaforos() {
	numero=0;
	for(int i=0;i<MAX_ELEM;i++)
		ls[i]=0;
}

					//agregar
bool listaCoches::agregar(Coche* co){
	for(int i=0;i<numero;i++)
		if(lco[i]==co) 
			return false;
	if(numero<MAX_ELEM)
		lco[numero++]=co;
	else
		return false; 	//capacidad maxima alcanzada
	return true;
}
bool listaCalles::agregar(Calle* ca){
	for(int i=0;i<numero;i++)
		if(lca[i]==ca) 
			return false;
	if(numero<MAX_ELEM)
		lca[numero++]=ca;
	else
		return false; 	//capacidad maxima alcanzada
	return true;
}
bool listaSemaforos::agregar(Semaforo* s){
	for(int i=0;i<numero;i++)
		if(ls[i]==s) 
			return false;
	
	if(numero<MAX_ELEM)
		ls[numero++]=s;
	else
		return false; 	//capacidad maxima alcanzada
	return true;
	
}

					//dibuja
void listaCoches::dibuja() {
	for(int i=0;i<numero;i++)
		lco[i]->dibuja();
}
void listaCalles::dibuja() {
	for(int i=0;i<numero;i++)
		lca[i]->dibuja();
}
void listaSemaforos::dibuja() {
	for(int i=0;i<numero;i++)
		ls[i]->dibuja();
}

					//setPos
void listaCoches::setPos(float px, float py){
	for(int i=0;i<numero;i++)
		lco[i]->setPos(px,py);
}
void listaCalles::setPos(float px, float py){
	for(int i=0;i<numero;i++)
		lca[i]->setPos(px,py);
}
void listaSemaforos::setPos(float px, float py){
	for(int i=0;i<numero;i++)
		ls[i]->setPos(px,py);
}


					//mueve
void listaCoches::mueve(float t) {
	for(int i=0;i<numero;i++)
		lco[i]->mueve(t);
}
/*void listaSemaforos::OnTimer(int opt){
	for(int i=0;i<numero;i++)
		ls[i]->OnTimer(opt);
}*/

					//destruir contenido
void listaCoches::destruirContenido() {
	for(int i=0;i<numero;i++)
		delete lco[i];
	numero=0;
}
void listaCalles::destruirContenido() {
	for(int i=0;i<numero;i++)
		delete lca[i];
	numero=0;
}
void listaSemaforos::destruirContenido() {
	for(int i=0;i<numero;i++)
		delete ls[i];
	numero=0;
}

					//eliminar index
void listaCoches::eliminarindex(int index){
	if((index<0)||(index>=numero))
		return;
	delete lco[index];
	numero--;
	for(int i=index;i<numero;i++)
		lco[i]=lco[i+1];
}
void listaCalles::eliminarindex(int index){
	if((index<0)||(index>=numero))
		return;
	delete lca[index];
	numero--;
	for(int i=index;i<numero;i++)
		lca[i]=lca[i+1];
}
void listaSemaforos::eliminarindex(int index){
	if((index<0)||(index>=numero))
		return;
	delete ls[index];
	numero--;
	for(int i=index;i<numero;i++)
		ls[i]=ls[i+1];
}

					//eliminar
void listaCoches::eliminar(Coche* dco){
	for(int i=0;i<numero;i++)
		if(lco[i]==dco){
			eliminarindex(i);
			return;
		}
}
void listaCalles::eliminar(Calle* dca){
	for(int i=0;i<numero;i++)
		if(lca[i]==dca){
			eliminarindex(i);
			return;
		}
}
void listaSemaforos::eliminar(Semaforo* ds){
	for(int i=0;i<numero;i++)
		if(ls[i]==ds){
			eliminarindex(i);
			return;
		}
}



void listaCoches::setgiro(int g){
	for(int i=0;i<numero;i++)
		lco[i]->setGiro(g);
}
int listaCalles::getLong(){
	return lca[0]->getLong();
}




















