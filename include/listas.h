#pragma once
#include "Coche.h"
#include "Calle.h"
#include "Semaforo.h"
#define MAX_ELEM 1000

class listaCoches {
	Coche* lco[MAX_ELEM];
	int numero=0;
public:
	listaCoches();
	void dibuja();
	void setPos(float px, float py);
	void mueve(float t);
	bool agregar(Coche* co);
	void destruirContenido();
	void eliminarindex(int index);
	void eliminar(Coche* dco);	
	
	void setgiro(int);
	
	friend class Mundo;			//por ahora
	friend class interaccion;
	friend class editor;
};

class listaCalles {
	Calle* lca[MAX_ELEM];
	int numero=0;
public:
	listaCalles();
	void dibuja();
	void setPos(float px, float py);
	bool agregar(Calle* ca);
	void destruirContenido();
	void eliminarindex(int index);
	void eliminar(Calle* dca);
	
	int getLong();
	
	friend class Mundo;
	friend class interaccion;	
	friend class editor;
	friend class grafo;
};

class listaSemaforos {
	Semaforo* ls[MAX_ELEM];
	int numero=0;
public:
	listaSemaforos();
	void dibuja();
	//void OnTimer(int);
	void setPos(float px, float py);
	bool agregar(Semaforo* s);
	void destruirContenido();
	void eliminarindex(int index);
	void eliminar(Semaforo* ds);	
	
	friend class interaccion;
	friend class editor;
	friend class Mundo;
	friend class grafo;
}; 


