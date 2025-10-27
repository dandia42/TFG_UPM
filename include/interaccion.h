#pragma once
#include "listas.h"
#include "algoritmos.h"
#include <time.h>

class interaccion{
public:
//comportamiento de coches en el semaforo
	static void cruce(listaCoches lc, listaSemaforos ls, listaCalles lca, grafo* G,double*,int,int,int,int);
	static void cruce(listaCoches,int, listaSemaforos ls, int, listaCalles lca, grafo* G,double*,int,int,int,int,int);
	static bool acercarse(Coche*, Semaforo*, float);
//comportamiento del semaforo con el grafo
	static void OnTimerSem(listaCalles&,listaCoches&,listaSemaforos, int, grafo*);
	static void OnTimerSem(listaCalles&,listaCoches&,Semaforo*,int, grafo*, bool,int);
//crear coches de forma aleatoria en las calles	
	static void crearcoche(listaCoches*, listaCalles);
	static void crearcoche(Calle*, listaCoches*);
//destruir coches
	static void destruircoche(listaCoches, int);
//los coches aparezcan en el otro extremo del mundo al salir	
	static void salirMundo(listaCoches, int fil, int col, int lon);
	static void salirMundo(Coche*, int fil, int col, int lon);
//comportamiento entre los coches que van en la misma direccion
	static void distSeg(listaCoches);
	static void distSeg(listaCoches,int,int,Vector2D);
	static int dSeg(Coche*, int, const char*);
//comprobacion de semaforos para saber cuantas calles conecta en cada direccion
	static void callesem(listaCalles lc, listaSemaforos ls);
	static void callesem(Calle*, Semaforo*, int);
//desplazamiento lateral 
	static void cambiarCarril(listaCoches, listaCalles);
	static void cambiarCarril(Coche*, Calle*);
	static void cambiarCarril(Coche*, int*, Vector2D);
//generacion de rutas de desplazamientos y comportamiento
	static void setRuta(listaCoches, grafo*, listaCalles,int);
	static void setRuta(Coche*, grafo*, int, listaCalles,int,int);
	static void calcularCalles(listaCalles,float,float);//???

//funcion para llevar un registro del tiempo
	static void temporal(double*,double*,double*,int,int,bool);
//funciones auxiliares
	static void comprobarInalcanzable(listaSemaforos, listaCalles);
	static void callesExteriores(listaCalles*, int, int, int, int);	//hacer que las de arriba y las de la derecha sean iguales que las de abajo y las de la izquierda respectivamente
	static bool cambiarTipoCalle(Semaforo*, Calle*);	//comprobar que las calles solo sean de salida
	static void cambiarTipoCalle(listaCalles);	//evitar calles fuera de rango y solo de entrada o solo de salida en el cruce
	static bool cambiarTipoCalle(Calle*, bool, bool);
	static void ocupacionCalle(listaCoches, listaCalles);	//asgina el valor ocupacion a las calles en funcion de la cantidad de coches que haya en ésta
	static void ocupacionCalle(listaCoches, Calle*);
	static int getCalleindex(Coche*, listaCalles);	//devuelve el indice de la calle en la que esta el coche
	static void parcheSetDir(listaCoches, listaCalles);	//evitar que el coche se quede parado y le cambie la direccion (cambiar: se debería arreglar en otra parte del código pero no se cual)
	//static void parcheSetDir(Coche*,Calle*);
	static int mascara(Coche*, Semaforo*, float);
	
	static int comprobarDesaparecidos(listaCoches);
	static int asociarAristaIndex(listaCalles, Semaforo*, int,int,int,bool);	//asociar index true, asociar ocupacion false
	
};


