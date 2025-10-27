#pragma once
#include "Semaforo.h"

Semaforo::Semaforo(float px, float py, int l){
	setPos(px,py);
	luz=rand()%4;//0
	setTimer(rand()%200+400,rand()%160+320,rand()%200+400,rand()%160+320);
}

Semaforo::~Semaforo(){

}

void Semaforo::dibuja(){
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glTranslatef(posicion.x,posicion.y,0.5f);
	
	glBegin(GL_POLYGON);					//octagono interior
		glColor3f(1,0,0);
		glVertex3d(size*cos(3*PI/8),-size*sin(3*PI/8),0);
		glVertex3d(size*cos(PI/8),-size*sin(PI/8),0);
		glVertex3d(size*cos(PI/8),size*sin(PI/8),0);
		glVertex3d(size*cos(3*PI/8),size*sin(3*PI/8),0);
		glVertex3d(-size*cos(3*PI/8),size*sin(3*PI/8),0);
		glVertex3d(-size*cos(PI/8),size*sin(PI/8),0);
		glVertex3d(-size*cos(PI/8),-size*sin(PI/8),0);
		glVertex3d(-size*cos(3*PI/8),-size*sin(3*PI/8),0);
	glEnd();
	glRotatef(hv,0,0,1);
	glBegin(GL_POLYGON);					//rectangulo
		glColor3f(luzr,luzg,0);
		glVertex3d(size*cos(3*PI/8),-size*sin(3*PI/8),0.1f);
		glVertex3d(size*cos(3*PI/8),size*sin(3*PI/8),0.1f);
		glVertex3d(-size*cos(3*PI/8),size*sin(3*PI/8),0.1f);
		glVertex3d(-size*cos(3*PI/8),-size*sin(3*PI/8),0.1f);
	glEnd();
	if(estadoluces==3||estadoluces==4){
	glColor3f(1,1,0);
	glBegin(GL_LINE_LOOP);					//octagono exterior
		glVertex3d(14*cos(3*PI/8),-14*sin(3*PI/8),0);
		glVertex3d(14*cos(PI/8),-14*sin(PI/8),0);
		glVertex3d(14*cos(PI/8),14*sin(PI/8),0);
		glVertex3d(14*cos(3*PI/8),14*sin(3*PI/8),0);
		glVertex3d(-14*cos(3*PI/8),14*sin(3*PI/8),0);
		glVertex3d(-14*cos(PI/8),14*sin(PI/8),0);
		glVertex3d(-14*cos(PI/8),-14*sin(PI/8),0);
		glVertex3d(-14*cos(3*PI/8),-14*sin(3*PI/8),0);
	glEnd();}
	else if(estadoluces==1||estadoluces==2){
	glColor3f(1,0,1);
	glBegin(GL_LINE_LOOP);					//octagono exterior
		glVertex3d(14*cos(3*PI/8),-14*sin(3*PI/8),0);
		glVertex3d(14*cos(PI/8),-14*sin(PI/8),0);
		glVertex3d(14*cos(PI/8),14*sin(PI/8),0);
		glVertex3d(14*cos(3*PI/8),14*sin(3*PI/8),0);
		glVertex3d(-14*cos(3*PI/8),14*sin(3*PI/8),0);
		glVertex3d(-14*cos(PI/8),14*sin(PI/8),0);
		glVertex3d(-14*cos(PI/8),-14*sin(PI/8),0);
		glVertex3d(-14*cos(3*PI/8),-14*sin(3*PI/8),0);
	glEnd();}
	glPopMatrix();
}


void Semaforo::setPos(float px, float py){
	posicion.x=px;
	posicion.y=py;
}

bool Semaforo::getSalida(int x){
	return salida[x];
}


void Semaforo::maquinaEstados(){
	if(temp>=0){
	switch(luz){				//maquina de estados 
		case 0:					//verde vertical-rojo horizontal
			if(temp==0) temp=tvv;//temporizador
			luzr=0;			//color
			luzg=1;			//color
			hv=0;				//vertical
			if(temp==1) luz=1;		//cambio de estado
			break;
		case 1:					//ambar vertical-rojo horizontal
			if(temp==0) temp=tav;
			luzr=1;
			luzg=1;
			if(temp==1) luz=2;
			break;
		case 2:					//rojo vertical-verde horizontal
			if(temp==0) temp=tvh;
			luzr=0;
			luzg=1;
			hv=90;				//horizontal
			if(temp==1) luz=3;
			break;
		case 3:					//rojo vvertical-ambar horizontal
			if(temp==0) temp=tah;
			luzr=1;
			luzg=1;
			if(temp==1) luz=0;
			break;
	}
	tempaux++;
	temp--;
	}
	else{}		//mantenerse en el estado fijo porque se le pasará -1 al tiempo del estado y 1 a los demás tiempos para que llegue solo siguiendo la máquina de estados
}


void Semaforo::setTimer(int a,int b,int c,int d){
	tvv=a;
	tav=b;
	tvh=c;
	tah=d;

}
/*funcion para devolver el tiempo de cada estado*/
int Semaforo::getTimer(int x){
	switch(x){
	case 0:
		return tvv;
	break;
	case 1:
		return tav;
	break;
	case 2:
		return tvh;
	break;
	case 3:
		return tah;
	break;
	default:
		return -1;
	}
}
/*inicializar el estado del semaforo, tiene que hacerse despues de interaccion::callesem e interaccion::comprobarInalcanzable*/
void Semaforo::iniEstadoLuces(){
	int mascara=salida[0]*1+salida[1]*2+salida[2]*4+salida[3]*8;	//valor que se obtiene segun los tipos de calles que rodean al semaforo
	
	switch(mascara){
	case 3:
	case 6:
	case 9:
	case 12:
		setEstadoLuces(0);
	break;
	case 1:
	case 4:
		setEstadoLuces(1);
	break;
	case 2:
	case 8:
		setEstadoLuces(2);
	break;
	case 5:
	case 7:
	case 13:
		setEstadoLuces(3);
	break;
	case 10:
	case 11:
	case 14:
		setEstadoLuces(4);
	break;
	default:
		cout<<"\n\tERROR CON LA ASIGNACION DE DIRECCIONES ENTRADA/SALIDA";
	}
	
}
//////////////////////////////////////////









