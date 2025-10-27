#pragma once
#include "Vector2D.h"
#include "glut.h"
class Calle
{
	Vector2D posicion;
	int r, g, b;
	int carriles=1;	//numero de carriles
	int tipo;		//de 1 a 14, pagina 4
	int direccion=0;	//0=vertical    1=horizontal
	int sentido=0;		//sentido     0=arriba/derecha      1=abajo/izquierda
	int longitud=0;
	float anchura=3;		//3 por carril
	int vmax=-1, vmed=-1;	///////////////////////velocidad maxima, media, desviacion tipica
	//Vector2D zona[2];		//0 izquierda/arriba	1 derecha/abajo 
	int nodo[2]={-1,-1}, ocupacion = 0;	//nodo? ocupacion de coches
	bool extremoEntrada = false;
	
public:
	Calle(Vector2D pos, int d);
	Calle(Vector2D, int, int, bool);
	~Calle();
	void setPos(float px, float py);
	void setDir(int d);
	int getDir();
	void setSentido(int s){sentido=s;}
	int getSentido(){return sentido;}
	void dibuja();
	void setColor(int red, int green, int blue){r=red;g=green;b=blue;}
	
	void setLong(int);
	float getLong();
	void setCarriles();
	void dibujaCarriles();
	void setTipo(int t){tipo=t;}
	int getTipo(){return tipo;}
	Vector2D getPos(){return posicion;}
	//void setZonas();
	//Vector2D getZonas(int);
	void setNodo(int, int);
	int* getNodo(){return nodo;}
	void setOcupacion(int o){ocupacion=o;}
	int getOcupacion(){return ocupacion;}
	void setExtremo(bool aux) { extremoEntrada = aux; }
	bool getExtremo() { return extremoEntrada; }
	friend class interaccion;
	friend class editor;
};

