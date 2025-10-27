#pragma once
#include "editor.h"
#include <iostream>
using namespace std;

Vector2D editor::posicion={-100,-100};
int editor::indice=0, editor::maxindice=0;
int editor::objeto=0;
bool editor::flagCarga=false;
bool editor::enable=false;


void editor::dibuja(){
	if(enable){	//editor activado y no se editan coches
	glPushMatrix();
	glColor3f(1,1,1);
	glDisable(GL_LIGHTING);
	glTranslatef(posicion.x,posicion.y,-2);
	glRotatef(45,0,0,1);
	glBegin(GL_POLYGON);			//rectangulo1
		glVertex2d(-15,-3);
		glVertex2d(-15,3);
		glVertex2d(15,3);
		glVertex2d(15,-3);
	glEnd();
	glBegin(GL_POLYGON);			//rectangulo2
		glVertex2d(-3,-15);
		glVertex2d(-3,15);
		glVertex2d(3,15);
		glVertex2d(3,-15);
	glEnd();
	
	glPopMatrix();
	}
}
void editor::setPos(int tipo, int ind, listaCoches coches, listaCalles calles, listaSemaforos semaforos){
	switch(objeto){
	case 1:	//calle
		posicion=calles.lca[indice]->getPos();
		cout<<"\nse cambia la posicion a la calle "<<indice<<endl;
	break;
	case 2:	//coche
		posicion=coches.lco[indice]->getPos();
	break;
	case 3:	//semaforo
		posicion=semaforos.ls[indice]->getPos();
		cout<<"\nse cambia la luz al semaforo "<<indice<<", ahora es "<<semaforos.ls[indice]->getLuz()<<endl;
	break;
	}
}
bool editor::getEnable(){
	
	return enable;
}
int editor::getTipoObjeto(){
	return objeto;
}
int editor::getIndice(){
	return indice;
}
void editor::cargaDatos(listaCoches co, listaCalles ca, listaSemaforos s){
	if(flagCarga==false){		//solo carga los datos estaticos la primera vez
		
		/*coches=&co;
		calles=&ca;
		semaforos=&s;*/
		flagCarga=true;
		cout<<"se han cargado los datos estaticos"<<endl;
		
	}
}
void editor::actualizaDatos(listaCoches *pco, listaCalles *pca, listaSemaforos *ps){
	
}

void editor::toggleEnable(){
	enable = !enable;
}

void editor::funcionesEditor(unsigned char key, listaCoches lco, listaCalles lca, listaSemaforos ls){
	switch(key){
	case 'i':	//aumenta el estado del objeto (tipo calle, luz semaforo)
		switch(objeto){
		case 1:	//calle
			cambiarTipoCalle(1,lca,ls);
			cout<<"aumentar calle";
		break;
		case 2:	//coche
			cambiarCantidad(1,lco,lca);
			cout<<"aumentar coches";
		break;
		case 3:	//semaforo
			cambiarColor(1,ls);
			cout<<"aumentar semaforo";
		break;
		}
	break;
	case 'o':	//disminuye el estado del objeto (tipo calle, luz semaforo)
		switch(objeto){
		case 1:	//calle
			cambiarTipoCalle(-1,lca,ls);
			cout<<"disminuir calle";
		break;
		case 2:	//coche
			cambiarCantidad(0,lco,lca);
			cout<<"disminuir coches";
		break;
		case 3:	//semaforo
			cambiarColor(-1,ls);
			cout<<"disminuir semaforo";
		break;
		}
	break;
	case 'x':
		switch(objeto){
		case 1:	//calle
			lca.eliminarindex(indice);
			cout<<"eliminar calle";
		break;
		case 2:	//coche
			lco.eliminarindex(indice);
			cout<<"eliminar coches";
		break;
		case 3:	//semaforo
			ls.eliminarindex(indice);
			cout<<"eliminar semaforo";
		break;
		}
	break;
	case '.':	//aumentar longitud
		if(objeto==1)
			cambiarLongitud(lca,lco,ls,1);
	break;
	case ',':	//disminuir longitud
		if(objeto==1)
			cambiarLongitud(lca,lco,ls,-1);
	break;
	case '1':	//cambia a calle
		cambiarTipoObjeto(1);
		cambiarObjeto(0,lco,lca,ls);
		cout<<"objeto: calle"<<objeto<<"\t";
	break;
	case '2':	//cambia a coche
		cambiarTipoObjeto(2);
		cambiarObjeto(0,lco,lca,ls);
		cout<<"objeto: coche"<<objeto<<"\t";
	break;
	case '3':	//cambia a semaforo
		cambiarTipoObjeto(3);
		cambiarObjeto(0,lco,lca,ls);
		cout<<"objeto: semaforo"<<objeto<<"\t";
	break;
	case '9':	//reinicia el mundo
		cambiarMatriz();
	break;
	case 'r':	//aumenta el indice del tipo de objeto a editar
		cambiarObjeto(1,lco,lca,ls);
		cout<<"indice: "<<indice<<"\t";
	break;
	case 'f':	//disminuye el indice del tipo de objeto a editar
		cambiarObjeto(-1,lco,lca,ls);
		cout<<"indice: "<<indice<<"\t";
	break;
	case 's':
		cout<<"stop "<<lco.lco[indice]->getStop();
	break;
	}
	setPos(getTipoObjeto(),getIndice(),lco,lca,ls);
}

void editor::cambiarObjeto(int i, listaCoches lco, listaCalles lca, listaSemaforos ls){
	if(i){
		if(alcanzarExtremo(i, lco, lca, ls)){}
		else
			indice+=i;
	}
	else
		indice=0;	//si se cambia el tipo de objeto se pasa al primero de la lista
}

void editor::cambiarTipoObjeto(int o){
	objeto=o;
}


/*calles*/
void editor::cambiarTipoCalle(int orden, listaCalles calles, listaSemaforos semaforos){	//1 aumentar, -1 disminuir
	
	calles.lca[indice]->setTipo(calles.lca[indice]->getTipo()+orden);
	interaccion::cambiarTipoCalle(calles);
	interaccion::comprobarInalcanzable(semaforos, calles);
	interaccion::callesem(calles,semaforos);
	cout<<"\nse ha cambiado el tipo de la calle";

}

void editor::cambiarLongitud(listaCalles calles, listaCoches coches, listaSemaforos semaforos, int mult){
	Vector2D aux=calles.lca[indice]->getPos();
	if(calles.lca[indice]->direccion &&!((calles.lca[indice]->getLong()<25&&mult<0)||(calles.lca[indice]->getLong()>50&&mult>0))){	//horizontal
		for(int i=0;i<calles.numero;i++){
			if(calles.lca[i]->getPos().x==aux.x){
				calles.lca[i]->setLong(calles.lca[i]->getLong()+5*mult);
				calles.lca[i]->setPos(calles.lca[i]->getPos().x+5*mult/2,calles.lca[i]->getPos().y);	//ajustar la posicion de las calles de la misma columna
			}
			else if(calles.lca[i]->getPos().x>aux.x)
				calles.lca[i]->setPos(calles.lca[i]->getPos().x+5*mult,calles.lca[i]->getPos().y);	//ajustar la posicion de las calles de la derecha
		}
		for(int i=0;i<coches.numero;i++){
			if(coches.lco[i]->getPos().x>aux.x)
				coches.lco[i]->setPos(coches.lco[i]->getPos().x+5*mult,coches.lco[i]->getPos().y);	//ajustar la posicion de los coches de la derecha
		}
		for(int i=0;i<semaforos.numero;i++){
			if(semaforos.ls[i]->getPos().x>aux.x)
				semaforos.ls[i]->setPos(semaforos.ls[i]->getPos().x+5*mult,semaforos.ls[i]->getPos().y);	//ajustar la posicion de los semaforos de la derecha
		}
	}
	else if(calles.lca[indice]->direccion==0 &&!((calles.lca[indice]->getLong()<25&&mult<0)||(calles.lca[indice]->getLong()>50&&mult>0))){		//vertical
		for(int i=0;i<calles.numero;i++){
			if(calles.lca[i]->getPos().y==aux.y){
				calles.lca[i]->setLong(calles.lca[i]->getLong()+5*mult);
				calles.lca[i]->setPos(calles.lca[i]->getPos().x,calles.lca[i]->getPos().y+5*mult/2);	//ajustar la posicion de las calles de la misma columna
			}
			else if(calles.lca[i]->getPos().y>aux.y)
				calles.lca[i]->setPos(calles.lca[i]->getPos().x,calles.lca[i]->getPos().y+5*mult);	//ajustar la posicion de las calles de la derecha
		}
		for(int i=0;i<coches.numero;i++){
			if(coches.lco[i]->getPos().y>aux.y)
				coches.lco[i]->setPos(coches.lco[i]->getPos().x,coches.lco[i]->getPos().y+5*mult);	//ajustar la posicion de los coches de la derecha
		}
		for(int i=0;i<semaforos.numero;i++){
			if(semaforos.ls[i]->getPos().y>aux.y)
				semaforos.ls[i]->setPos(semaforos.ls[i]->getPos().x,semaforos.ls[i]->getPos().y+5*mult);	//ajustar la posicion de los semaforos de la derecha
		}
	}
	
}


/*coches*/
void editor::cambiarCantidad(int i, listaCoches lco, listaCalles lca){
	listaCoches *plco=&lco;
	if(i){
		interaccion::crearcoche(plco, lca);
		cout<<"\tse han creado nuevos coches, ahora hay "<<lco.numero<<" coches\n";
	}
	else{
		interaccion::destruircoche(lco, 2);
		cout<<"\tse han destruido coches, ahora hay "<<lco.numero<<" coches\n";
	}
}


/*semaforos*/
void editor::cambiarColor(int i, listaSemaforos semaforos){
	i+=semaforos.ls[indice]->getLuz();
	if(i<0)					//evita estados inalcanzables
		semaforos.ls[indice]->setLuz(3);
	else if(i>3)
		semaforos.ls[indice]->setLuz(0);
	else
		semaforos.ls[indice]->setLuz(i);
	cout<<"se ha cambiado la luz del semaforo\t";
}


/*mundo*/
void editor::cambiarMatriz(){

}


/*auxiliar*/
bool editor::alcanzarExtremo(int i, listaCoches lco, listaCalles lca, listaSemaforos ls){
	switch(objeto){
	case 1:	//calle
		maxindice=lca.numero;
	break;
	case 2:	//coche
		maxindice=lco.numero;
	break;
	case 3:	//semaforo
		maxindice=ls.numero;
	break;
	}
	if(indice==(maxindice-1) && i==1){
		indice=0;
		return true;
	}
	else if(indice==0 && i==-1){
		indice=maxindice-1;
		return true;
	}
	else return false;
}













