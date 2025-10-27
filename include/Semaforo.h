#pragma once
#include "Vector2D.h"
#include "glut.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

class Semaforo
{
	Vector2D posicion;
	int luz=0, luzr, luzg;	//0vv-rh 1av-rh 2rv-vh 3rv-ah
	int caf[4], cid[4];	//tipo de calle, indice de la calle     0-arriba 1-derecha 2-abajo 3-izquierda
	int temp=0,tempaux=0;		//tiempo verde, ámbar y rojo
	bool salida[4];	//arriba derecha abajo izquierda
	int size=8, hv=0, sizeaux=1;	//hv=horizontal o vertical
	int estadoluces=0;	//0 normal, 1 variable con más tiempo horizontal, 2 variable con más tiempo vertical, 3 fijo horizontal, 4 fijo vertical
	int tvv,tav,tvh,tah;	//tiempos en cada estado del semaforo
	
public:
	Semaforo(float px, float py, int);
	~Semaforo();
	void setPos(float px, float py);
	void setLuz(int i){luz=i;}
	int getLuz(){return luz;}
	bool getSalida(int);			//devuelve si es posible salir por las distintas direcciones
	
	void dibuja();
	void OnTimer(int);			//comportamiento del semaforo
	
	void setTimer(int,int,int,int);	//se le pasa 0 para mantener el estado, se le pasa 1 para pasar de estado en un ciclo
	int getTimer(int);
	void setTemp(int x,bool f){f?tempaux=x:temp=x;}
	int getTemp(int x){return x?tempaux:temp;}
	int getCID(int x){return cid[x];}
	void maquinaEstados();
	void iniEstadoLuces();
	void setEstadoLuces(int x){estadoluces=x;}
	int getEstadoLuces(){return estadoluces;}
	
	Vector2D getPos(){return posicion;}
	
	friend class interaccion;
	friend class editor;
};

