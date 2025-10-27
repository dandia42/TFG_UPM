#pragma once
#include <stdlib.h>
#include <iostream>
#include "listas.h"

#define PESO_MAX 1000000
#define NODOSFW_MAX 50	//subir a 500 como maximo, hacer prueba de tiempo para matrices mayores

using namespace std;

class Dijkstra{
	int distancia, IDant, iter;
	bool visitado;
	//int* ruta;		//se guardan los ID de las aristas conjugadas
public:
	Dijkstra();
	~Dijkstra();
	int getDist(){return distancia;}
	int getID(){return IDant;}
	int getIter(){return iter;}
	bool getVisitado(){return visitado;}
	void setVisitado(bool x){visitado =x;}
	void setDist(int x){distancia=x;}
	void setID(int x){IDant=x;}
	void setIter(int x){iter=x;}
	friend class Vertice;
	friend class Arista;
	friend class interaccion;
};


class Vertice{
	int ID, index, ocupacion;
	bool terminal;
	int nady=0,iady=0;	//cantidad de vertices adyacentes
	int* ady,*pesoady;	//indices de los vertices adyacentes
	Dijkstra dv;		//variable necesaria para algoritmo Dijkstra
public:
	Vertice();
	~Vertice();
	int getID(){return ID;}
	Dijkstra getdv(){return dv;}
	friend class Algoritmo;
	friend class grafo;
	friend class interaccion;
};
class Arista{
	int N1, N2, N3, pesoFijo, pesoVariable;	//origen, destino, indice, peso{(1,2,3),(4,5,6),(7,8,9)} en función de la cantidad de calles de salida del siguiente cruce, del mismo cruce y de la ocupacion
	//Dijkstra da;
public:
	int getN1(){return N1;}
	int getN2(){return N2;}
	int getN3(){return N3;}
	void setN3(int x){N3=x;}
	void setPesoVariable(int p){pesoVariable=p;}
	int getPeso(){return pesoFijo+pesoVariable;}
	Arista();
	~Arista();
	friend class grafo;
	friend class interaccion;
};


class grafo{
	Vertice *vertice=NULL,*verticeconj=NULL;
	Arista *arista=NULL,*aristaconj=NULL;
	int nvertices=0,naristas=0,nverticesconj=0,naristasconj=0,fin=0,tempaux=0;
	int distFW[NODOSFW_MAX][NODOSFW_MAX],origenFW[NODOSFW_MAX][NODOSFW_MAX];	//matrices necesarias para el algoritmo Floyd-Warshall
public:
	grafo();
	~grafo();
	void crearGrafo(int,int, listaCalles, listaSemaforos,bool);//grafo de rutas
	void crearGrafo(listaCalles&,listaCoches&,listaSemaforos&,int,int);//grafo de semaforos
	void crearGrafo(grafo&);
	void crearGrafoConjugado(bool);
	int contarSentidos(listaCalles calles);
	void asignarSentidoAristas(listaCalles, listaSemaforos, int, int,bool);
	Arista getArista(int n){return arista[n];}
	void dibujaArista(listaSemaforos, int, int, int);
	void asociarCalleVertice(listaCalles);
	void listaAdy(int n, bool);
	void calcularPesoconj(bool);
	void calcularPesoSem(listaCalles&,listaCoches&,Semaforo*,int);
	void resetGrafo();
	void setTempaux(int x=0){ tempaux=x;}
	int getTempaux(){return tempaux;}
	
	friend class Algoritmo;
	friend class interaccion;
};

#pragma once
#include "interaccion.h"


















		
		
		
		
		
		
		
		
		
		
		
		
		
		
		


