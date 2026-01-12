#pragma once
#include "algoritmos.h"



Dijkstra::Dijkstra(){
	distancia=IDant=iter=0;
	visitado=false;
}
Dijkstra::~Dijkstra(){
	distancia=IDant=iter=visitado=NULL;
}

void Algoritmo::Dijkstra(int inicio, grafo *G, bool sem){
	if (inicio == -1) {
		cout << "\n\nError en el inicio del grafo" << endl;
		return;
	}
	else
		cout << "\nEl inicio del grafo es " << inicio << endl;

	if(sem){	//aplicar sobre el grafo original
		//Inicializar las distancias de los vertices
		//inicio=2;
		for (int i=0;i<G->nvertices;i++) {
			if (i == inicio) G->vertice[i].dv.setDist(0);
			else G->vertice[i].dv.setDist(PESO_MAX);
			G->vertice[i].dv.setVisitado(false);
		}

		//Procesar los nodos en orden creciente de distancia
		for (int i=0; i<G->nvertices;i++) {
		//Encontrar el nodo más cercano no visitado
			int u=-1;
			for (int j=0;j<G->nvertices;j++)
				if (!G->vertice[j].dv.getVisitado() && (u==-1 || G->vertice[j].dv.getDist()<G->vertice[u].dv.getDist()))
					u = j;
			if (G->vertice[u].dv.getDist()==PESO_MAX) break;
			G->vertice[u].dv.setVisitado(true);

			// Actualizar las distancias de los nodos adyacentes
			for (int j=0;j<G->naristas;j++)
				if (G->arista[j].getN1()==u) {
					int v=G->arista[j].getN2();
					int w=G->arista[j].getPeso();
					if (G->vertice[u].dv.getDist()+w<G->vertice[v].dv.getDist()) {
						G->vertice[v].dv.setDist(G->vertice[u].dv.getDist()+w);
						G->vertice[v].dv.setID(u);
						G->vertice[v].dv.setIter(G->vertice[u].dv.getIter()+1);
					}
				}
		}

		for(int i=0;i<G->nvertices;i++)		//comprobar que se han visitado todos los nodos
			if(!G->vertice[i].dv.getVisitado())
				cout<<"\n\tFalta por visitar el vertice "<<i;
		cout<<"\n\tSe han visitado todos los nodos"<<endl;
		for(int i=0;i<G->nvertices;i++)
			cout<<"\nVertice="<<i<<"   Distancia="<<G->vertice[i].getdv().getDist()<<"   IDant="<<G->vertice[i].getdv().getID()<<"   iter="<<G->vertice[i].getdv().getIter();
	}
	else{	//aplicar sobre el grafo conjugado
		//Inicializar las distancias de los vertices
		//inicio=2;
		for (int i=0;i<G->nverticesconj;i++) {
			if (i == inicio) G->verticeconj[i].dv.setDist(0);
			else G->verticeconj[i].dv.setDist(PESO_MAX);
			G->verticeconj[i].dv.setVisitado(false);
		}

		//Procesar los nodos en orden creciente de distancia
		for (int i=0; i<G->nverticesconj;i++) {
		//Encontrar el nodo más cercano no visitado
			int u=-1;
			for (int j=0;j<G->nverticesconj;j++)
				if (!G->verticeconj[j].dv.getVisitado() && (u==-1 || G->verticeconj[j].dv.getDist()<G->verticeconj[u].dv.getDist()))
					u = j;
			if (G->verticeconj[u].dv.getDist()==PESO_MAX) break;
			G->verticeconj[u].dv.setVisitado(true);

			// Actualizar las distancias de los nodos adyacentes
			for (int j=0;j<G->naristasconj;j++)
				if (G->aristaconj[j].getN1()==u) {
					int v=G->aristaconj[j].getN2();
					int w=G->aristaconj[j].getPeso();
					if (G->verticeconj[u].dv.getDist()+w<G->verticeconj[v].dv.getDist()) {
						G->verticeconj[v].dv.setDist(G->verticeconj[u].dv.getDist()+w);
						G->verticeconj[v].dv.setID(u);
						G->verticeconj[v].dv.setIter(G->verticeconj[u].dv.getIter()+1);
					}
				}
		}

		for(int i=0;i<G->nverticesconj;i++)		//comprobar que se han visitado todos los nodos
			if(!G->verticeconj[i].dv.getVisitado())
				cout<<"\n\tFalta por visitar el vertice "<<i;
		cout<<"\n\tSe han visitado todos los nodos"<<endl;
		for(int i=0;i<G->nverticesconj;i++)
			cout<<"\nVertice="<<i<<"   Distancia="<<G->verticeconj[i].getdv().getDist()<<"   IDant="<<G->verticeconj[i].getdv().getID()<<"   iter="<<G->verticeconj[i].getdv().getIter();
	}
	
}


void Algoritmo::FloydWarshall(grafo* G){
	//matriz dist para las distancias entre todos los nodos, matriz origen para conocer el vertice previo 
	//Inicializar las distancias y los origenes de los vertices
	for (int i=0;i<G->nverticesconj;i++) {
		for (int j=0;j<G->nverticesconj;j++) {
			G->distFW[i][j]=PESO_MAX;
			G->origenFW[i][j]=-1;
		}
		G->distFW[i][i]=0;	//la distancia a si mismo es 0
	}
	
	//Actualizar las distancias a los valores iniciales del grafo
	for (int i=0;i<G->nverticesconj;i++) {
		for (int j=0;j<G->verticeconj[i].nady;j++) {
			//Se asignan las distancias con una matriz donde se ve el peso entre el vertice origen i y el vertice destino G.verticeconj[i]->ady[j]
			G->distFW[i][G->verticeconj[i].ady[j]]=G->verticeconj[i].pesoady[j];
			G->origenFW[i][G->verticeconj[i].ady[j]]=i;	//se marca el origen y no se toca lo demas para la primera iteracion
		}
	}
	
	//Se aplica el algoritmo
	for (int k=0;k<G->nverticesconj;k++) {
		for (int i=0;i<G->nverticesconj;i++) {
			for (int j=0;j<G->nverticesconj;j++) {
				if (G->distFW[i][k]!=PESO_MAX && G->distFW[k][j]!=PESO_MAX && G->distFW[i][j]>G->distFW[i][k]+G->distFW[k][j]){
					G->distFW[i][j]=G->distFW[i][k]+G->distFW[k][j];
					G->origenFW[i][j]=G->origenFW[k][j];
				}
			}
		}
	}
	
	//Se obtiene el resultado
	cout<<"\nDistancias mínimas:"<<endl;
	for (int i=0;i<G->nverticesconj;i++) {
		for (int j=0;j<G->nverticesconj;j++) {
			if (G->distFW[i][j]==PESO_MAX) cout<<"No conectan ";
			else cout<<G->distFW[i][j]<<((G->distFW[i][j]>9)?("  "):("   "));
		}
		cout<<endl;
	}
	
	cout<<"\nMatriz de origenes:"<<endl;
	for (int i=0;i<G->nverticesconj;i++) {
		for (int j=0;j<G->nverticesconj;j++) {
			if (G->origenFW[i][j]==PESO_MAX) cout<<"No conectan ";
			else cout<<G->origenFW[i][j]<<((G->origenFW[i][j]>9)?("  "):("   "));
		}
		cout<<endl;
	}
}


void Algoritmo::greenWave(grafo* G){	//ciclo en el que aplicar el algoritmo
	int ini1=rand()%G->nvertices,ini2=rand()%G->nvertices,ini3=rand()%G->nvertices;	//se hace sobre el grafo inicial, no el conjugado
	bool aux1=true;	//flag para obtener un ini3 que no se repita
	int n1=0,n2=0,n3=0;	//numero de vértices por cada ruta
	int r=0, *r1=&r,*r2 = &r,*r3 = &r;	//las 3 rutas
	
	while(ini1==ini2)	//evitar que los indices desde los que se busca la mejor ruta sean los mismos
		ini2=rand()%G->nvertices;
	
	
	
	
	cout<<"\nesto no deberia coincidir ninguno";
	funcionAux(ini1,ini2,G,n1,r1);
	
	while(aux1){	//tambien se podria aplicar usando std::find metiendole una lista de exclusion
		ini3=rand()%G->nvertices;
		aux1=0;
		for(int i=0;i<n1;i++)
			if(ini3==r1[i])
				aux1=1;
		if(ini3==ini2)
			aux1=1;
	}
	
	funcionAux(ini2,ini3,G,n2,r2);
	funcionAux(ini3,ini1,G,n3,r3);
	cout << "\n\t\tRuta 1: " << endl;
	for (int i=0;i<n1;i++)
		cout << r1[i] << " ";
	cout << "\n\t\tRuta 2: " << endl;
	for (int i=0;i<n2;i++)
		cout << r2[i] << " ";
	cout << "\n\t\tRuta 3: " << endl;
	for (int i=0;i<n3;i++)
		cout << r3[i] << " ";
		
	cin>>n1;//borrar esto cuando se compruebe que esta bien
	
	/*
	Dijkstra(ini1,G,true);	//encontrar la ruta optima desde el primer vertice hasta todos los demás
	
	//hacer un pseudo interaccion::setRuta() para conocer la ruta de ini1 a ini2
	int aux=ini2;
	n1=G->vertice[ini2].dv.getIter();
	r1=new int[n1];	//vector para almacenar la primera ruta
	for(int i=n1-1;i>=0;i--){
		r1[i]=G->vertice[aux].dv.getID();
		aux=G->vertice[aux].dv.getID();
	}
	//aqui igual hay que aumentar el peso de las aristas que conectan los vertices de la ruta para que no se repitan vertices, luego habria que bajar el peso para que se usen en setRuta
	for(int i=0;i<n1;i++){
		if(i<(n1-1))//los primeros indices de la ruta parcial
			for(int j=0;j<G->naristas;j++){
				if(G->arista[j].N1==r1[i] || G->arista[j].N2==r1[i])//si el vertice anterior o el siguiente está en la ruta, se aumenta el peso de la arista que los une para excluirla de la siguiente ruta
					G->arista[j].setPesoVariable(1000);
			}
		else//el ultimo indice de la ruta parcial
			for(int j=0;j<G->naristas;j++){
				if(G->arista[j].N1==r1[i])// solo el origen, si el vertice anterior o el siguiente está en la ruta, se aumenta el peso de la arista que los une para excluirla de la siguiente ruta
					G->arista[j].setPesoVariable(1000);
			}
	}
	
	
	
	///////repetir para el segundo vertice
	Dijkstra(ini2,G,true);	//encontrar la ruta optima desde el segundo vertice hasta todos los demás
	
	//hacer un pseudo interaccion::setRuta() para conocer la ruta de ini2 a ini3
	int aux=ini3;
	n2=G->vertice[ini3].dv.getIter();
	r2=new int[n2];	//vector para almacenar la primera ruta
	for(int i=n2-1;i>=0;i--){
		r2[i]=G->vertice[aux].dv.getID();
		aux=G->vertice[aux].dv.getID();
	}
	//aqui igual hay que aumentar el peso de las aristas que conectan los vertices de la ruta para que no se repitan vertices, luego habria que bajar el peso para que se usen en setRuta
	for(int i=0;i<n1;i++){
		if(i<(n2-1))//los primeros indices de la ruta parcial
			for(int j=0;j<G->naristas;j++){
				if(G->arista[j].N1==r2[i] || G->arista[j].N2==r2[i])//si el vertice anterior o el siguiente está en la ruta, se aumenta el peso de la arista que los une para excluirla de la siguiente ruta
					G->arista[j].setPesoVariable(1000);
			}
		else//el ultimo indice de la ruta parcial
			for(int j=0;j<G->naristas;j++){
				if(G->arista[j].N1==r2[i])// solo el origen, si el vertice anterior o el siguiente está en la ruta, se aumenta el peso de la arista que los une para excluirla de la siguiente ruta
					G->arista[j].setPesoVariable(1000);
			}
	}
	*/
	
}


void Algoritmo::funcionAux(int inia, int inib, grafo *G, int n, int* r){
	Dijkstra(inia,G,true);	//encontrar la ruta optima desde el segundo vertice hasta todos los demás
	
	//hacer un pseudo interaccion::setRuta() para conocer la ruta de ini2 a ini3
	int aux=inib;
	n=G->vertice[inib].dv.getIter();
	r=new int[n];	//vector para almacenar la primera ruta
	for(int i=n-1;i>=0;i--){
		r[i]=G->vertice[aux].dv.getID();
		aux=G->vertice[aux].dv.getID();
	}
	//aqui igual hay que aumentar el peso de las aristas que conectan los vertices de la ruta para que no se repitan vertices, luego habria que bajar el peso para que se usen en setRuta
	for(int i=0;i<n;i++){
		if(i<(n-1))//los primeros indices de la ruta parcial
			for(int j=0;j<G->naristas;j++){
				if(G->arista[j].getN1()==r[i] || G->arista[j].getN2()==r[i])//si el vertice anterior o el siguiente está en la ruta, se aumenta el peso de la arista que los une para excluirla de la siguiente ruta
					G->arista[j].setPesoVariable(1000);
			}
		else//el ultimo indice de la ruta parcial
			for(int j=0;j<G->naristas;j++){
				if(G->arista[j].getN1()==r[i])// solo el origen, si el vertice anterior o el siguiente está en la ruta, se aumenta el peso de la arista que los une para excluirla de la siguiente ruta
					G->arista[j].setPesoVariable(1000);
			}
	}
	for(int i=0;i<n;i++)
	cout<<"\t"<<r[i];
	cout<<"\n";
}


















































