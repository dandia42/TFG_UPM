#pragma once
#if !defined(AFX_MUNDO_H__9510340A_3D75_485F_93DC_302A43B8039A__INCLUDED_)
#define AFX_MUNDO_H__9510340A_3D75_485F_93DC_302A43B8039A__INCLUDED_

#include <vector>

#if _MSC_VER > 1000
#pragma once
#endif	// _MSC_VER > 1000
#endif	// AFX_MUNDO_H

#include "listas.h"
#include "interaccion.h"
#include "editor.h"
#include "grafo.h"
#include "Vector2D.h"
#include "glut.h"
#include <GL/freeglut.h>
//#include "ETSIDI.h"
#include "algoritmos.h"
#include <iostream>
#include <time.h>
using namespace std;

class Mundo
{
public:
	void Init();
	Mundo();
	virtual ~Mundo();

	void InitGL();
	void OnKeyboardDown(unsigned char key, int x, int y);
	void OnTimer();
	void OnDraw();
	void crearmatriz(int i, int j);
	
	int filas, columnas;
	int flag=0;
	int pox,poy;
	int optRuta=-1, optSem=-1;
	float poz;
	int iteraciones=0, *piteraciones=&iteraciones;
	
	double tiempo;
	
	Calle *calleaux=new Calle({-10,-10},0);
	listaCoches coches, *pcoches=&coches;
	listaCalles calles, *pcalles=&calles;
	listaSemaforos semaforos, *psemaforos=&semaforos;
	grafo G, *pg=&G, *Gsem=&G, G2;	//grafo de rutas, grafo de cada semaforo
	//editor edit;
};

