#pragma once
#include "grafo.h"


int funcion(int, int, int);

grafo::grafo(){
	nvertices=naristas=nverticesconj=naristasconj=0;
}
grafo::~grafo(){
	for(int i=0;i<nvertices;i++){
		delete[] vertice[i].ady;
		delete[] vertice[i].pesoady;
	}
	for(int i=0;i<nverticesconj;i++){
		delete[] verticeconj[i].ady;
		delete[] verticeconj[i].pesoady;
	}
	//destruir las matrices de Floyd-Warshall
	for(int i=0;i<nverticesconj;i++){
		delete[] distFW[i];
		delete[] origenFW[i];
	}
	for(int i=0;i<nverticesconj;i++){
		delete[] distFW;
		delete[] origenFW;
	}
	
	delete[] arista;
	delete[] vertice;
	delete[] aristaconj;
	delete[] verticeconj;
	cout<<endl<<endl<<"se libera memoria"<<endl;
}

void grafo::crearGrafo(int filas, int columnas, listaCalles calles, listaSemaforos semaforos,bool flag){
	static bool aux1=false;	//flag para que haga la lista de adyacencia solo la primera vez, se usa para green wave
	if(flag){
	int i, j, ivertices=0;	//ivertices: indice de vertices
	//crear vertices	
	nvertices=filas*columnas;
	vertice=new Vertice[nvertices];		//filas*columnas=nvertices
	for(j=1;j<=filas;j++){
		cout<<endl;
		for(i=1;i<=columnas;i++){
			vertice[ivertices].ID=i*j-1+(i-1)*(filas-j);
			cout<<vertice[ivertices].ID<<"\t";
			ivertices++;
		}
	}
	
	
	//crear aristas	0h 1v
	naristas=contarSentidos(calles);
	arista=new Arista[naristas];
	cout<<"\nhay "<<naristas<<" aristas\n";
	}
	asignarSentidoAristas(calles, semaforos, filas, columnas, flag);//true todo, false solo pesos variables
	for(int i=0;i<nvertices;i++)
		vertice[i].nady=0;
	for(int i=0;i<naristas;i++)	//saber cuantos vertices adyacentes hay
		vertice[arista[i].getN1()].nady++;
	
	if(!aux1){	//que solo lo haga una vez
	//hacer la lista de adyacencia
		listaAdy(naristas,false);
		aux1=true;
	}
	
	crearGrafoConjugado(flag);
	asociarCalleVertice(calles);
}
void grafo::crearGrafo(grafo &G){
	vertice=G.vertice;
	verticeconj=G.verticeconj;
	arista=G.arista;
	aristaconj=G.aristaconj;
	nvertices=G.nvertices;
	naristas=G.naristas;
	nverticesconj=G.nverticesconj;
	naristasconj=G.naristasconj;
	fin=G.fin;
	tempaux=G.tempaux;
	//comprobar si tengo que meter tambien la inicializacion de las matrices FW
}
void grafo::crearGrafoConjugado(bool flag){
	if(flag){
		nverticesconj=naristas;
		for(int i=0; i<naristas;i++){
			for(int j=0;j<naristas;j++){
				if(arista[i].N2==arista[j].N1&&arista[i].N1!=arista[j].N2)
					naristasconj++;
			}
		}
		verticeconj=new Vertice[nverticesconj];
		aristaconj=new Arista[naristasconj];
		//cout<<"\n\nhay "<<naristasconj<<" aristas conjugadas y "<<nverticesconj<<" vertices conjugados \n";
		
		
		int iarista=0;	//indice de aristas
		for(int i=0; i<naristas;i++){
			for(int j=0;j<naristas;j++){
				if(arista[i].N2==arista[j].N1&&arista[i].N1!=arista[j].N2){
					aristaconj[iarista].N1=i;
					aristaconj[iarista].N2=j;
					iarista++;
				}
			}
		}
	}
	//asociar vertices con los indices de las calles
	for(int i=0;i<nverticesconj;i++){
		verticeconj[i].index=arista[i].N3;
		verticeconj[i].ocupacion=arista[i].pesoVariable;
		verticeconj[i].nady=0;
	}
	for(int i=0;i<naristasconj;i++)	//saber cuantos vertices adyacentes hay
		verticeconj[aristaconj[i].getN1()].nady++;
	calcularPesoconj(flag);
	//hacer la lista de adyacencia
	listaAdy(naristasconj,true);
	/*for(int i=0;i<naristasconj;i++)
		cout<<"\nse asignan los valores "<<aristaconj[i].N1<<" "<<aristaconj[i].N2<<" \tpeso fijo "<<aristaconj[i].pesoFijo<<" peso variable "<<aristaconj[i].pesoVariable<<" a la arista conjugada "<<i;*/
	cout<<"\n\nGENERACION DE GRAFOS COMPLETADA\n\n";
}

void grafo::asignarSentidoAristas(listaCalles calles, listaSemaforos semaforos, int filas, int columnas,bool flag){
	int iarista=0;	//indice de aristas
	static int auxx=0;
	for(int i=1;i<=columnas;i++){
		for(int j=1; j<=filas;j++){
			for(int aux=0;aux<4;aux++){
				if(semaforos.ls[(i*j-1+(i-1)*(filas-j))]->getSalida(aux)){
				if(flag){//true todo, false solo el peso variable
				switch(aux){	//posicion relativa al vertice
				case 0:	//arriba
					if(j==filas){ 
					arista[iarista].N1=(i*j-1+(i-1)*(filas-j));
					arista[iarista].N2=(i*1-1+(i-1)*(filas-1));
					arista[iarista].pesoFijo=5;
					}
					else{
					arista[iarista].N1=(i*j-1+(i-1)*(filas-j));
					arista[iarista].N2=(i*(j+1)-1+(i-1)*(filas-(j+1)));
					arista[iarista].pesoFijo=5;
					}
				break;
				case 1:	//derecha
					if(i==columnas){ 
					arista[iarista].N1=(i*j-1+(i-1)*(filas-j));
					arista[iarista].N2=(1*j-1);
					arista[iarista].pesoFijo=5;
					}
					else{
					arista[iarista].N1=(i*j-1+(i-1)*(filas-j));
					arista[iarista].N2=((i+1)*j-1+((i+1)-1)*(filas-j));
					arista[iarista].pesoFijo=5;
					}
				break;
				case 2:	//abajo
					if(j==1){ 
					arista[iarista].N1=(i*j-1+(i-1)*(filas-j));
					arista[iarista].N2=(i*filas-1);
					arista[iarista].pesoFijo=5;
					}
					else{
					arista[iarista].N1=(i*j-1+(i-1)*(filas-j));
					arista[iarista].N2=(i*(j-1)-1+(i-1)*(filas-(j-1)));
					arista[iarista].pesoFijo=5;
					}
				break;
				case 3:	//izquierda
					if(i==1){ 
					arista[iarista].N1=(i*j-1+(i-1)*(filas-j));
					arista[iarista].N2=(columnas*j-1+(columnas-1)*(filas-j));
					arista[iarista].pesoFijo=5;
					}
					else{
					arista[iarista].N1=(i*j-1+(i-1)*(filas-j));
					arista[iarista].N2=((i-1)*j-1+((i-1)-1)*(filas-j));
					arista[iarista].pesoFijo=5;
					}
				break;
				}
				arista[iarista].N3=interaccion::asociarAristaIndex(calles,semaforos.ls[(i*j-1+(i-1)*(filas-j))],aux,filas,columnas,true);
			}	arista[iarista].pesoVariable=interaccion::asociarAristaIndex(calles,semaforos.ls[(i*j-1+(i-1)*(filas-j))],aux,filas,columnas,false);
				//cout<<"\nse asignan los valores "<<arista[iarista].N1<<" "<<arista[iarista].N2<<" \tindice "<<arista[iarista].N3<<" \tpeso fijo "<<arista[iarista].pesoFijo<<"   peso variable "<<arista[iarista].pesoVariable<<" a la arista "<<iarista;
				iarista++;
				auxx++;
				}
			}
		}
	}
	cout<<"\nse han dado valores a "<<auxx<<" aristas\n";
	auxx=0;
}
int funcion(int i, int j, int columnas){
	return (i*j-1+(i-1)*(columnas-j));
}


void grafo::dibujaArista(listaSemaforos semaforos, int i, int j, int giro){//cambiar quitar
	glPushMatrix();
	glColor3f(1,0,0);
	glDisable(GL_LIGHTING);
	glTranslatef(semaforos.ls[i]->getPos().x,semaforos.ls[i]->getPos().y,1);
	glRotatef(90*giro,0,0,1);
	glBegin(GL_POLYGON);			//triangulo (sentido)
		glVertex3d(0,26,0.5f);
		glVertex3d(-6,0,0.5f);
		glVertex3d(6,0,0.5f);
	glEnd();
	glRotatef(-90*giro,0,0,1);
	glTranslatef(-semaforos.ls[i]->getPos().x,-semaforos.ls[i]->getPos().y,-1);
	glPopMatrix();
}

//contarSentidos(calles) sirve para contar el numero de aristas
int grafo::contarSentidos(listaCalles calles){
	int aux=0;
	for(int i=0; i<calles.numero;i++){
		switch(calles.lca[i]->getTipo()){
		case 5:
		case 8:
		case 9:
		case 12:
		case 13:
		case 14:
			if(calles.lca[i]->getPos().x>=0 && calles.lca[i]->getPos().y>=0)
			aux++;	//doble sentido
		case 1:
		case 2:
		case 3:
		case 4:
		case 6:
		case 7:
		case 10:
		case 11:
			if(calles.lca[i]->getPos().x>=0 && calles.lca[i]->getPos().y>=0)
			aux++;	//unico sentido
		break;
		}
	}
	return aux;
}
void grafo::listaAdy(int n, bool conj){	//n es naristas(conj) cambiar conj/////////////////////////////////////////////////
	static bool flag1=false,flag2=false;
	int i=0, aux=0;
	if(!conj){/////////////////////////////////
		if(!flag1){
			for(int aux=0;aux<nvertices;aux++){	//misma cantidad de vertices adyacentes que de pesos a esos vertices
				vertice[aux].ady=new int[vertice[aux].nady];
				vertice[aux].pesoady=new int[vertice[aux].nady];
			}
			flag1=true;
		}
		vertice[arista[i].getN1()].iady=0;
		for(i=0;i<n;i++){	//n=naristas asignar valores
			vertice[arista[i].getN1()].ady[vertice[arista[i].getN1()].iady]=arista[i].getN2();//asignar valor a ady
			vertice[arista[i].getN1()].pesoady[vertice[arista[i].getN1()].iady]=arista[i].getPeso();//asignar peso a ady
			vertice[arista[i].getN1()].iady++;
		}	
		for(int k=0;k<nvertices;k++){
			cout<<"\nEl vertice "<<k<<" se comunica con "<<vertice[k].nady<<" vertices: ";
			for(int x=0;x<vertice[k].nady;x++){
				cout<<vertice[k].ady[x]<<"  "<<vertice[k].pesoady[x]<<"    ";
			}
		}
	}
	else{///////////////////////////////////////
		if(!flag2){
			flag2=true;
		}
		else{
			for(int aux=0;aux<nverticesconj;aux++){	//borrar la informacion en la segunda iteracion
				delete[] verticeconj[aux].ady;
				delete[] verticeconj[aux].pesoady;
				verticeconj[aux].iady=0;
			}
			
		}
		for(int aux=0;aux<nverticesconj;aux++){
			verticeconj[aux].ady=new int[verticeconj[aux].nady];
			verticeconj[aux].pesoady=new int[verticeconj[aux].nady];
		}
		for(i=0;i<n;i++){	//n=naristasconj asignar valores
			verticeconj[aristaconj[i].getN1()].ady[verticeconj[aristaconj[i].getN1()].iady]=aristaconj[i].getN2();//asignar valor a ady
			
			verticeconj[aristaconj[i].getN1()].pesoady[verticeconj[aristaconj[i].getN1()].iady]=aristaconj[i].getPeso();//asignar peso a ady
			
			verticeconj[aristaconj[i].getN1()].iady++;
		}
		for(int k=0;k<nverticesconj;k++){
			cout<<"\nEl verticeconj "<<k<<" se comunica con "<<verticeconj[k].nady<<" vertices: ";
			for(int x=0;x<verticeconj[k].nady;x++){
				cout<<verticeconj[k].ady[x]<<"  "<<verticeconj[k].pesoady[x]<<"    ";
			}
		}
	}
}

void grafo::calcularPesoconj(bool flag){	//flag para calcular solo el peso variable
	if(flag){
		//fijo salida del cruce: 2 si hay muchas salidas, 4 salidas normales, 6 pocas salidas
		int aux=0;
		for(int i=0;i<nverticesconj;i++){
			switch(verticeconj[i].nady){	//heuristica
			case 1:
				aristaconj[aux].pesoFijo=2;
			break;
			case 2:
				aristaconj[aux].pesoFijo=1;
				aristaconj[++aux].pesoFijo=1;
			break;
			case 3:
				aristaconj[aux].pesoFijo=1;
				aristaconj[++aux].pesoFijo=1;
				aristaconj[++aux].pesoFijo=1;
			break;
			default:
				cout<<"\n\n\tERROR AL CALCULAR EL PESO SEGUN SALIDAS";
				int x;
				cin>>x;
			break;
			}
			++aux;
			if(aux==naristasconj) cout<<"\nSe han asignado todos los pesos fijos";
			if(aux>naristasconj) cout<<"\nSe han asignado demasiados pesos fijos";
		}
	}
	//el peso variable se calcula segun la ocupacion de la calle, que coincidirá con la ocupacion del nodo destino
	for(int i=0;i<naristasconj;i++){
		aristaconj[i].pesoVariable=verticeconj[aristaconj[i].N2].ocupacion/2;//para ajustar pesos
	}
}




Vertice::Vertice(){
	ID=index=ocupacion=0;
	terminal=false;
}
Vertice::~Vertice(){
	ID=index=NULL;
	terminal=dv.visitado=false;
	//delete[] ady;
}
Arista::Arista(){
	N1=N2=N3=pesoFijo=pesoVariable=0;
}
Arista::~Arista(){
	N1=N2=N3=pesoFijo=pesoVariable=NULL;
}




void grafo::asociarCalleVertice(listaCalles calles){
	int ivertices=0;
	for(int i=0;i<calles.numero;i++)
		if(calles.lca[i]->getPos().x>=0 && calles.lca[i]->getPos().y>=0)
		switch(calles.lca[i]->getTipo()){
		case 5:
		case 8:
		case 9:
		case 12:
		case 13:
		case 14:	//doble sentido
			calles.lca[i]->setNodo(ivertices,0);
			calles.lca[i]->setNodo(ivertices+1,1);
			ivertices+=2;
		break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 6:
		case 7:
		case 10:
		case 11:	//unico sentido
			calles.lca[i]->setNodo(ivertices,0);
			calles.lca[i]->setNodo(ivertices,1);
			ivertices++;
		break;
		}
	if(ivertices>=naristasconj)
		cout<<"\nERROR en la asociación calle-vertice\t\t"<<ivertices<<"\n";
	else
		cout<<"\nSe han asociado "<<ivertices<<" nodos con "<<calles.numero<<" calles\n";
}


void grafo::resetGrafo(){
	for(int i=0;i<nvertices;i++){
		vertice[i].dv.setDist(0);
		vertice[i].dv.setID(0);
		vertice[i].dv.setIter(0);
		vertice[i].dv.setVisitado(false);
	}
	for(int i=0;i<nverticesconj;i++){
		verticeconj[i].dv.setDist(0);
		verticeconj[i].dv.setID(0);
		verticeconj[i].dv.setIter(0);
		verticeconj[i].dv.setVisitado(false);
	}
}


/*grafo para el semaforo*/
void grafo::crearGrafo(listaCalles& lca, listaCoches& lco, listaSemaforos& ls, int ind, int opt){
	//inicializar los elementos del grafo necesarios, al ser un grafo fijo por cada semaforo comportandose como una maquina de estados, no hace falta pasarle argumentos
	nvertices=4;
	naristas=4;
	vertice=new Vertice[nvertices];
	arista=new Arista[naristas];
	
	//asociar informacion de los vertices
	for(int i=0;i<nvertices;i++){
		vertice[i].ID=i;
	}
	
	//asociar informacion de las aristas
	for(int i=0;i<nvertices;i++){
		arista[i].N1=vertice[i].ID;	//asociar vertice origen
		arista[i].N2=(i==nvertices-1)?(vertice[0].ID):(vertice[i+1].ID);	//asociar vertice destino
	}
	cout<<"\nSe han asociado los vértices y las aristas del grafo de los semaforos";
	
	if(opt) calcularPesoSem(lca,lco,ls.ls[ind],ind); // cambia el peso de los semaforos si hay optimizacion
	cout<<"\nSe han asociado los pesos a las aristas del grafo de los semaforos";
	for(int i=0;i<naristas;i++)
		cout<<"\nSe asignan los valores "<<arista[i].N1<<" "<<arista[i].N2<<" y peso "<<arista[i].getPeso()<<" a la arista "<<i;
}
//////////////////////////
/*calcular el peso variable del semaforo*/
void grafo::calcularPesoSem(listaCalles& lca, listaCoches& lco, Semaforo* s, int ind){	//el peso depende de la ocupacion y del tiempo de espera
	int pesoV=0,pesoH=0;
	for(int i=0;i<4;i++){	//se inicializa la ocupacion de las calles que ve el semaforo
		interaccion::ocupacionCalle(lco,lca.lca[s->getCID(i)]);
	}
	for(int i=0;i<4;i++){	//actualizar los pesos vertical y horizontal con la ocupacion para comparar
		if(!s->getSalida(i)){	//mirar solo las calles de entrada
			if(i%2){	//horizontal
				pesoH+=lca.lca[s->getCID(i)]->getOcupacion();
			}
			else{	//vertical
				pesoV+=lca.lca[s->getCID(i)]->getOcupacion();
			}
		}
	}
	//for(int i=0;i<naristas;i++){
		switch(s->getLuz()){
		case 0:	//vv->av	
			if(pesoV<pesoH ){//|| s->getTemp(1)>(400*50)){	//hay mas coches en horizontal o han pasado mas de 50 segundos en el mismo estado
				//estado 1
				s->setTimer(1,s->getTimer(1),s->getTimer(2),s->getTimer(3));
				s->setTemp(0,1);	//reiniciar contador auxiliar
				s->setTemp(0,0);	//poner contador temp a 0 para que entre a la maquina de estados
			}
			else{	//mantener el estado
				//estado 0
				s->setTimer((40*50),s->getTimer(1),s->getTimer(2),s->getTimer(3));
				if(pesoH)	//aumentar temporizador de espera si hay algun coche en la calle que esta en rojo
					s->setTemp(s->getTemp(1)+1,1);
			}
		break;
		case 1:	//av->vh
			//no dependen de la ocupacion asi que no se cambian los tiempos
			//estado 1
			s->setTimer((40*50),s->getTimer(1),(40*50),s->getTimer(3));
		break;
		case 2:	//vh->ah
			if(pesoV>pesoH ){//|| s->getTemp(1)>(400*50)){	//hay mas coches en vertical o han pasado mas de 50 segundos en el mismo estado
				//estado 3
				s->setTimer(s->getTimer(0),s->getTimer(1),1,s->getTimer(3));
				s->setTemp(0,1);	//reiniciar contador auxiliar
				s->setTemp(0,0);	//poner contador temp a 0 para que entre a la maquina de estados
			}
			else{	//mantener el estado
				//estado 2
				s->setTimer(s->getTimer(0),s->getTimer(1),(40*50),s->getTimer(3));
				if(pesoV)	//aumentar temporizador de espera si hay algun coche en la calle que esta en rojo
					s->setTemp(s->getTemp(1)+1,1);
			}
		break;
		case 3:	//ah->vv
			//no dependen de la ocupacion asi que no se cambian los tiempos
			//estado 3
			s->setTimer((40*50),s->getTimer(1),(40*50),s->getTimer(3));
		break;
		default:
			cout<<"\nERROR ARISTA SEMAFORO FUERA DE RANGO";
			int x;
			cin>>x;
		break;
		}
		cout<<"\n\n\tIndice: "<<ind<<"  PesoV: "<<pesoV<<"  PesoH: "<<pesoH<<"   Temp: "<<s->getTemp(1);
	//}
}
//////////////////////////////////////////














