#pragma once
#include "Calle.h"
void dibujaSentidos(int,float);
void dibujaTriangulo(float,int,float);
void dibujaZonas(int, Vector2D*);
Calle::Calle(Vector2D p, int d){
	setPos(p.x,p.y);
	setDir(d);
	setLong(30);
	tipo=rand()%2+1;//rand()%14+1; //cambiar
	setCarriles();
	setColor(0,0,1);
	if (p.x < 0 || p.y < 0) {
		bool ext = true;
		setExtremo(ext);
	}
	//setZonas();
}
Calle::Calle(Vector2D p, int d, int t, bool ext){
	setPos(p.x,p.y);
	setDir(d);
	setLong(30);
	tipo=t;
	setCarriles();
	setColor(0,0,1);
	if (p.x < 0 || p.y < 0)
		ext = true;
	setExtremo(ext);
	//setZonas();
}


Calle::~Calle(){

}

void Calle::dibuja(){
	glPushMatrix();
	glColor3f(r,g,b);
	glDisable(GL_LIGHTING);
	glTranslatef(posicion.x,posicion.y,0);
	glRotatef(-90*direccion,0,0,1);	//positivo es sentido antihorario
	glBegin(GL_POLYGON);			//rectangulo
		glVertex3d(-anchura/2*carriles,-longitud/2,0);
		glVertex3d(-anchura/2*carriles,longitud/2,0);
		glVertex3d(anchura/2*carriles,longitud/2,0);
		glVertex3d(anchura/2*carriles,-longitud/2,0);
	glEnd();
	glColor3f(1,1,1);
	dibujaCarriles();			//dibuja contornos de carriles
	
	
	dibujaSentidos(tipo, anchura);
	glRotatef(90*direccion,0,0,1);
	glTranslatef(-posicion.x,-posicion.y,0);
	//for(int i=0;i<2;i++)
	//dibujaZonas(i,zona);
	
	setCarriles();
	glPopMatrix();
}
void dibujaSentidos(int tipo, float ancho){
	switch(tipo){
	case 6:
		dibujaTriangulo(1,0,ancho);
		dibujaTriangulo(-1,0,ancho);
		dibujaTriangulo(0,0,ancho);
		break;
	case 8:
		dibujaTriangulo(1,0,ancho);
		dibujaTriangulo(-1,1,ancho);
	case 1:
		dibujaTriangulo(0,0,ancho);
		break;
	case 7:
		dibujaTriangulo(-1,1,ancho);
		dibujaTriangulo(1,1,ancho);
		dibujaTriangulo(0,1,ancho);
		break;
	case 9:
		dibujaTriangulo(-1,1,ancho);
		dibujaTriangulo(1,0,ancho);
	case 2:
		dibujaTriangulo(0,1,ancho);
		break;
	case 12:
		dibujaTriangulo(1.5,0,ancho);
		dibujaTriangulo(-1.5,1,ancho);
		dibujaTriangulo(0.5,0,ancho);
		dibujaTriangulo(-0.5,0,ancho);
		break;
	case 10:
		dibujaTriangulo(1.5,0,ancho);
		dibujaTriangulo(-1.5,0,ancho);
	case 3:
		dibujaTriangulo(0.5,0,ancho);
		dibujaTriangulo(-0.5,0,ancho);
		break;
	case 14:
		dibujaTriangulo(-1.5,1,ancho);
		dibujaTriangulo(1.5,0,ancho);
		dibujaTriangulo(0.5,1,ancho);
		dibujaTriangulo(-0.5,1,ancho);
		break;
	case 11:
		dibujaTriangulo(1.5,1,ancho);
		dibujaTriangulo(-1.5,1,ancho);
	case 4:
		dibujaTriangulo(0.5,1,ancho);
		dibujaTriangulo(-0.5,1,ancho);
		break;
	case 13:
		dibujaTriangulo(1.5,0,ancho);
		dibujaTriangulo(-1.5,1,ancho);
	case 5:
		dibujaTriangulo(0.5,0,ancho);
		dibujaTriangulo(-0.5,1,ancho);
		break;
	}
	
}
void dibujaTriangulo(float aux, int giro, float ancho){
	glTranslatef(ancho*aux,0,0.2f);
	glRotatef(180*giro,0,0,1);
	glBegin(GL_POLYGON);			//triangulo (sentido)
		glVertex3d(0,6,0.5f);
		glVertex3d(-0.8,-6,0.5f);
		glVertex3d(0.8,-6,0.5f);
	glEnd();
	glRotatef(180*giro,0,0,1);
	glTranslatef(-ancho*aux,0,-0.2f);
}
/*void dibujaZonas(int i, Vector2D* zona){
	glTranslatef(zona[i].x,zona[i].y,0.2f);
	glBegin(GL_POLYGON);
		glVertex3d(-1,-1,0);
		glVertex3d(-1,1,0);
		glVertex3d(1,1,0);
		glVertex3d(1,-1,0);
	glEnd();
	glTranslatef(-zona[i].x,-zona[i].y,-0.2f);
}*/


void Calle::dibujaCarriles(){
	switch(carriles){
	case 4:
	glBegin(GL_LINES);
		glVertex3d(-anchura,-longitud/2,0.1f);
		glVertex3d(-anchura,longitud/2,0.1f);
		glVertex3d(anchura,longitud/2,0.1f);
		glVertex3d(anchura,-longitud/2,0.1f);
		glEnd();
	case 2:
	glBegin(GL_LINES);
		glVertex3d(0,-longitud/2,0.1f);
		glVertex3d(0,longitud/2,0.1f);
		glEnd();
	case 1:	
	glBegin(GL_LINE_LOOP);
		glVertex3d(-anchura/2*carriles,-longitud/2,0.1f);
		glVertex3d(-anchura/2*carriles,longitud/2,0.1f);
		glVertex3d(anchura/2*carriles,longitud/2,0.1f);
		glVertex3d(anchura/2*carriles,-longitud/2,0.1f);
		glEnd();
	break;
	case 3:
	glBegin(GL_LINES);
		glVertex3d(-anchura/2,-longitud/2,0.1f);
		glVertex3d(-anchura/2,longitud/2,0.1f);
		glVertex3d(anchura/2,longitud/2,0.1f);
		glVertex3d(anchura/2,-longitud/2,0.1f);
		glEnd();
		
	glBegin(GL_LINE_LOOP);
		glVertex3d(-anchura/2*carriles,-longitud/2,0.1f);
		glVertex3d(-anchura/2*carriles,longitud/2,0.1f);
		glVertex3d(anchura/2*carriles,longitud/2,0.1f);
		glVertex3d(anchura/2*carriles,-longitud/2,0.1f);
		glEnd();
	break;
	}
}

void Calle::setPos(float px, float py){
	posicion.x=px;
	posicion.y=py;
}
void Calle::setDir(int d){
	direccion=d;
}
int Calle::getDir(){
	return direccion;
}
void Calle::setLong(int i){
	longitud=i;
}
float Calle::getLong(){
	return longitud;
}

void Calle::setCarriles(){
	switch(tipo){
	case 1:
	case 2:
		carriles=1;
		break;
	case 3:
	case 4:
	case 5:
		carriles=2;
		break;
	case 6:
	case 7:
	case 8:
	case 9:
		carriles=3;
		break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
		carriles=4;
		break;
	}
}

/*void Calle::setZonas(){
	zona[0].x=direccion?(posicion.x):(posicion.x-6);
	zona[0].y=direccion?(posicion.y+6):(posicion.y);
	zona[1].x=direccion?(posicion.x):(posicion.x+6);
	zona[1].y=direccion?(posicion.y-6):(posicion.y);
}
Vector2D Calle::getZonas(int i){
	return zona[i];
}*/

void Calle::setNodo(int n, int i){
	nodo[i]=n;
}





