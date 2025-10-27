#pragma once
#include "grafo.h"

class Algoritmo{
public:
	static void Dijkstra(int inicio, grafo* G, bool);
	static void FloydWarshall(grafo* G);
	static void greenWave(grafo* G);
	static void funcionAux(int,int,grafo*,int,int*);
};
