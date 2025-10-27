#pragma once
#include "Vector2D.h"
#include "glut.h"
class Coche
{
	float largo=4, ancho=1.8;	//largo=4.1 	ancho=1.8
	bool flaggiro=false, flagsem=false, flagsemaux=false, flagstop=false, flagLateral=false;	//flag semaforo
	float vn=1, an=0.0125;	//velocidad normal y angulo normal
	Vector2D posicion, velocidad, aceleracion{0,0};	
	int giro=1, giroaux, giroanterior;	 	//-1=izquierda    0=recto    1=derecha	
	
	float angrad=0, angd=0, angdaux=0;//angulo en rad y en grados
	int distseg;		//distancia de seguridad trasera
	int dir;		//0=arriba    1=derecha    2=abajo    3=izquierda
	float r,g,b;		//color
	int* ruta;
	int ngiros=0,igiros=0;
	double tiemporutaini=0,tiemporutafin=0,talgini=0,talgfin=0;//tiempo en completar la ruta y en calcularla
	int tempaux=0;
	//int n=4;			//carril de destino
	//float t=0.0;
	
public:


int n[2];	//carril actual [0] y proximo carril [1]
bool flagdebug=false;


	Coche(Vector2D, int);
	~Coche();
	void setPos(float px, float py);
	void setVel(float vx, float vy);
	void setVel(int v){vn=v;}
	void setVelDir();
	void setDir(int);
	int getDir();
	void dibuja();
	void mueve(float t);
	void girar();				//comportamiento en los giros
	void despLat(int);
	void setPosSalida(Vector2D, int*);
	void changeStop(int x);
	bool getStop(){return flagstop;}
	void setColor(float r, float g, float b);
	void setGiro(int g);				//decidir giro del coche con w,a,d
	int getGiro();
	Vector2D getPos(){return posicion;}
	Vector2D getVel(){return velocidad;}
	float getAngRad(){return angrad;}
	float getAngD(){return angd;}
	bool getflaggiro(){return flaggiro;}
	bool getflagLateral(){return flagLateral;}
	void setflagLateral(bool fl){flagLateral=fl;}
	void setTIni(double t){tiemporutaini=t;}
	friend class interaccion;
	friend class editor;
};


