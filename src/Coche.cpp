#pragma once
#include "Coche.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;
Coche::Coche(Vector2D p, int d){
				//largo=4.1 	ancho=1.8
	setPos(p.x, p.y);
	setDir(d);
	setVelDir();
	giro=1;//rand()%3-1;cambiar
	setColor(1,0,1);
	angdaux=-90*dir;
}

Coche::~Coche(){
	cout<<"\nse ha eliminado un coche";
}


void Coche::dibuja(){
	glPushMatrix();
	glColor3f(r,g,b);
	glDisable(GL_LIGHTING);
	glTranslatef(posicion.x,posicion.y,0);
	glRotatef(angdaux-angd*giro,0,0,1);
	glBegin(GL_POLYGON);			//rectangulo
		glVertex3d(-ancho/2,-largo/2,1);
		glVertex3d(-ancho/2,largo/2,1);
		glVertex3d(ancho/2,largo/2,1);
		glVertex3d(ancho/2,-largo/2,1);
	glEnd();
	glColor3f(0,0,0);
	glBegin(GL_LINE_LOOP);					//octagono exterior
		glVertex3d(-ancho/2,-largo/2,1.1);
		glVertex3d(-ancho/2,largo/2,1.1);
		glVertex3d(ancho/2,largo/2,1.1);
		glVertex3d(ancho/2,-largo/2,1.1);
	glEnd();
	glColor3f(1,1,1);
	glBegin(GL_POLYGON);			//triangulo (sentido)
		glVertex3d(0,0.5f,1.5f);
		glVertex3d(-0.35f,-0.5f,1.5f);
		glVertex3d(0.35f,-0.5f,1.5f);
	glEnd();
	glPopMatrix();
}

void Coche::mueve(float t){
	posicion=posicion+velocidad*t+aceleracion*0.5f*t*t;
	velocidad=velocidad+aceleracion*t;
	
	if(flagstop) tempaux++;	//40 por segundo
	if(tempaux>(40*75)){	//lleva 75 segundos parado
		changeStop(0);
		tempaux=0;
		cout<<"\n\tSe reanuda la marcha internamente\n";
	}
							//////////vigilar aceleracion
}

void Coche::setPos(float px, float py){
	posicion={px,py};
}
void Coche::setVel(float vx, float vy){
	velocidad={vx,vy};
}

void Coche::setDir(int d){
	dir=d;
}
int Coche::getDir(){
	static int aux;
	if(velocidad.y>0 &&velocidad.x<0.06&&velocidad.x>-0.06){aux=0; return 0;}	//arriba
	else if(velocidad.x>0 &&velocidad.y<0.06&&velocidad.y>-0.06){aux=1; return 1;}//derecha
	else if(velocidad.y<0 &&velocidad.x<0.06&&velocidad.x>-0.06){aux=2; return 2;}//abajo
	else if(velocidad.x<0 &&velocidad.y<0.06&&velocidad.y>-0.06){aux=3; return 3;}//izqu
	else return aux;		//si se para, luego sigue con la direccion anterior
}

/*Comportamiento en los giros*/
void Coche::girar(){	
	angrad+=an;
	angd=angrad*360/2/PI;
	switch(dir){		//giros en funcion de la direccion del coche
	case 0:
		velocidad={giro*vn*sin(angrad),vn*cos(angrad)};
		break;
	case 1:
		velocidad={vn*cos(angrad),-giro*vn*sin(angrad)};
		break;
	case 2:
		velocidad={-giro*vn*sin(angrad),-vn*cos(angrad)};
		break;
	case 3:
		velocidad={-vn*cos(angrad),giro*vn*sin(angrad)};
		break;
	}
	
	if(angd>89.3){		//normalizar trayectoria a la salida del semaforo
		giroanterior=giro;
		giro=0;
		vn=1;
		dir=getDir();
		setVelDir();
		angdaux=-90*dir;
		angrad=0;
		angd=0;
		//giro=-giro;//rand()%3-1;//decidir proximo giro
		flaggiro=false;
		//flagsem=false;
	}
}
void Coche::despLat(int ncarril){
	/*switch(dir){		//giros en funcion de la direccion del coche
	case 0:
	case 2:
		posicion.x-=(0.01*ncarril);
	break;
	case 1:
	case 3:
		posicion.y+=(0.01*ncarril);
	break;
	}*/


}
///////////////////////////////

/* Normalizacion de la trayectoria al salir del cruce */
void Coche::setPosSalida(Vector2D sp, int caf[4]){
if(pow(sp.x-posicion.x,2)>pow(sp.y-posicion.y,2))	//caso horizontal
	if(sp.x>posicion.x)				//coche a la izquierda del semaforo
		switch(giroanterior){
		case -1:
			switch(caf[3]){
			case 2:
				setPos(posicion.x, sp.y);
				break;
			case 4:
				setPos(posicion.x, sp.y-1.5);
				break;
			case 5:
				setPos(posicion.x, sp.y+1.5);
				break;
			case 7:
				setPos(posicion.x, sp.y-3);
				break;
			case 8:
				setPos(posicion.x, sp.y+3);
				break;
			case 9:
				setPos(posicion.x, sp.y);
				break;
			case 11:
				setPos(posicion.x, sp.y-4.5);
				break;
			case 12:
				setPos(posicion.x, sp.y+4.5);
				break;
			case 13:
				setPos(posicion.x, sp.y+1.5);
				break;
			case 14:
				setPos(posicion.x, sp.y-1.5);
				break;
			}
		break;
		case 1:
			switch(caf[3]){
			case 2:
				setPos(posicion.x, sp.y);
				break;
			case 4:
				setPos(posicion.x, sp.y+1.5);
				break;
			case 5:
				setPos(posicion.x, sp.y+1.5);
				break;
			case 7:
				setPos(posicion.x, sp.y+3);
				break;
			case 8:
				setPos(posicion.x, sp.y+3);
				break;
			case 9:
				setPos(posicion.x, sp.y+3);
				break;
			case 11:
				setPos(posicion.x, sp.y+4.5);
				break;
			case 12:
				setPos(posicion.x, sp.y+4.5);
				break;
			case 13:
				setPos(posicion.x, sp.y+4.5);
				break;
			case 14:
				setPos(posicion.x, sp.y+4.5);
				break;
			}
		break;
		
		}
	else						//coche a la derecha del semaforo
		switch(giroanterior){
		case -1:
			switch(caf[1]){
			case 1:
				setPos(posicion.x, sp.y);
				break;
			case 3:
				setPos(posicion.x, sp.y+1.5);
				break;
			case 5:
				setPos(posicion.x, sp.y-1.5);
				break;
			case 6:
				setPos(posicion.x, sp.y+3);
				break;
			case 8:
				setPos(posicion.x, sp.y);
				break;
			case 9:
				setPos(posicion.x, sp.y-3);
				break;
			case 10:
				setPos(posicion.x, sp.y+4.5);
				break;
			case 12:
				setPos(posicion.x, sp.y+1.5);
				break;
			case 13:
				setPos(posicion.x, sp.y-1.5);
				break;
			case 14:
				setPos(posicion.x, sp.y-4.5);
				break;
			}
		break;
		case 1:
			switch(caf[1]){
			case 1:
				setPos(posicion.x, sp.y);
				break;
			case 3:
				setPos(posicion.x, sp.y-1.5);
				break;
			case 5:
				setPos(posicion.x, sp.y-1.5);
				break;
			case 6:
				setPos(posicion.x, sp.y-3);
				break;
			case 8:
				setPos(posicion.x, sp.y-3);
				break;
			case 9:
				setPos(posicion.x, sp.y-3);
				break;
			case 10:
				setPos(posicion.x, sp.y-4.5);
				break;
			case 12:
				setPos(posicion.x, sp.y-4.5);
				break;
			case 13:
				setPos(posicion.x, sp.y-4.5);
				break;
			case 14:
				setPos(posicion.x, sp.y-4.5);
				break;
			}
		break;
		
		}

else						//caso vertical
	if(sp.y>posicion.y)				//coche debajo del semaforo
		switch(giroanterior){
		case -1:
			switch(caf[2]){
			case 2:
				setPos(sp.x, posicion.y);
				break;
			case 4:
				setPos(sp.x+1.5, posicion.y);
				break;
			case 5:
				setPos(sp.x-1.5, posicion.y);
				break;
			case 7:
				setPos(sp.x+3, posicion.y);
				break;
			case 8:
				setPos(sp.x-3, posicion.y);
				break;
			case 9:
				setPos(sp.x, posicion.y);
				break;
			case 11:
				setPos(sp.x+4.5, posicion.y);
				break;
			case 12:
				setPos(sp.x-4.5, posicion.y);
				break;
			case 13:
				setPos(sp.x-1.5, posicion.y);
				break;
			case 14:
				setPos(sp.x+1.5, posicion.y);
				break;
			}
		break;
		case 1:
			switch(caf[2]){
			case 2:
				setPos(sp.x, posicion.y);
				break;
			case 4:
				setPos(sp.x-1.5, posicion.y);
				break;
			case 5:
				setPos(sp.x-1.5, posicion.y);
				break;
			case 7:
				setPos(sp.x-3, posicion.y);
				break;
			case 8:
				setPos(sp.x-3, posicion.y);
				break;
			case 9:
				setPos(sp.x-3, posicion.y);
				break;
			case 11:
				setPos(sp.x-4.5, posicion.y);
				break;
			case 12:
				setPos(sp.x-4.5, posicion.y);
				break;
			case 13:
				setPos(sp.x-4.5, posicion.y);
				break;
			case 14:
				setPos(sp.x-4.5, posicion.y);
				break;
			}
		break;
		}
	else						//coche encima del semaforo
		switch(giroanterior){
		case -1:
			switch(caf[0]){
			case 1:
				setPos(sp.x, posicion.y);
				break;
			case 3:
				setPos(sp.x-1.5, posicion.y);
				break;
			case 5:
				setPos(sp.x+1.5, posicion.y);
				break;
			case 6:
				setPos(sp.x-3, posicion.y);
				break;
			case 8:
				setPos(sp.x, posicion.y);
				break;
			case 9:
				setPos(sp.x+3, posicion.y);
				break;
			case 10:
				setPos(sp.x-4.5, posicion.y);
				break;
			case 12:
				setPos(sp.x-1.5, posicion.y);
				break;
			case 13:
				setPos(sp.x+1.5, posicion.y);
				break;
			case 14:
				setPos(sp.x+4.5, posicion.y);
				break;
			}
		break;
		case 1:
			switch(caf[0]){
			case 1:
				setPos(sp.x, posicion.y);
				break;
			case 3:
				setPos(sp.x+1.5, posicion.y);
				break;
			case 5:
				setPos(sp.x+1.5, posicion.y);
				break;
			case 6:
				setPos(sp.x+3, posicion.y);
				break;
			case 8:
				setPos(sp.x+3, posicion.y);
				break;
			case 9:
				setPos(sp.x+3, posicion.y);
				break;
			case 10:
				setPos(sp.x+4.5, posicion.y);
				break;
			case 12:
				setPos(sp.x+4.5, posicion.y);
				break;
			case 13:
				setPos(sp.x+4.5, posicion.y);
				break;
			case 14:
				setPos(sp.x+4.5, posicion.y);
				break;
			}
		break;
		
		}

}


void Coche::setVelDir(){
	switch(dir){		//velocidad en funcion de la direccion
	case 0:
		setVel(0,vn);
		break;
	case 1:
		setVel(vn,0);
		break;
	case 2:
		setVel(0,-vn);
		break;
	case 3:
		setVel(-vn,0);
		break;
	}
}

void Coche::setColor(float red, float green, float blue){
	r=red;
	g=green;
	b=blue;
}
void Coche::setGiro(int g){		//decidir el giro de un coche con w,a,d
	giro=g;
}
int Coche::getGiro(){
	return giro;
}
void Coche::changeStop(int x){
	flagstop=x;
	if(x)
		vn=0;
	else{
		tempaux=0;
		vn=1;
	}
	setVelDir();
}








