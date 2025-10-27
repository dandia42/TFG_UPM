#pragma once
#include "listas.h"
#include "interaccion.h"

class editor{	
public:
	static Vector2D posicion;
	static int indice, maxindice;
	static int objeto;		//1 calle	2 coche	3 semaforo
	static bool flagCarga;
	static bool enable;
//general
	static void dibuja();
	static void setPos(int, int, listaCoches, listaCalles, listaSemaforos);
	static bool getEnable();
	static int getTipoObjeto();
	static int getIndice();
	static void cargaDatos(listaCoches, listaCalles, listaSemaforos);		//se cargan los datos estaticos del mundo
	static void actualizaDatos(listaCoches*, listaCalles*, listaSemaforos*);		//aqui hay que meter como referencia las cosas que cambian
	static void toggleEnable();	//habilita o deshabilita el editor
	static void funcionesEditor(unsigned char, listaCoches, listaCalles, listaSemaforos);
	static void cambiarObjeto(int, listaCoches, listaCalles, listaSemaforos);		//cambia entre objetos del mismo tipo
	static void cambiarTipoObjeto(int);	//cambia entre coche, semaforo y calle
	
//calles	
	static void cambiarTipoCalle(int, listaCalles, listaSemaforos);	//cambia el tipo de calle
	//static void aumentarTipoCalle();
	//static void disminuirTipoCalle();
	static void cambiarLongitud(listaCalles, listaCoches, listaSemaforos, int);	//cambia la longitud de la calle

//coches
	static void cambiarCantidad(int, listaCoches, listaCalles);	//cambia la cantidad de coches
	
//semaforos
	static void cambiarColor(int, listaSemaforos);		//cambia el color del semaforo
	
//mundo
	static void cambiarMatriz();		//reinicia el mundo
	
//auxiliar
	static bool alcanzarExtremo(int, listaCoches, listaCalles, listaSemaforos);
	
};
