#pragma once
#include <fstream>
#include "Mundo.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
void Mundo::crearmatriz(int filas, int columnas){
	for(float i=0;i<columnas;i++)
		for(float j=0;j<filas;j++){
			semaforos.agregar(new Semaforo(((calleaux->getLong())*i),((calleaux->getLong())*j),rand()%3));
			for(int k=0;k<2;k++)
				if(i==(columnas-1)&&k || k==0&&j==(filas-1)){
					interaccion::callesExteriores(pcalles,i,j,k,calleaux->getLong());
				}
				else
				calles.agregar(new Calle({(calleaux->getLong())*i+(calleaux->getLong())/2*k,(calleaux->getLong())*j-(calleaux->getLong())/2*(k-1)},k));//se envian asi las coordenadas para que las posiciones de las calles sean los puntos centrales de estas y se construya bien tanto en horizontal como en vertical
			if(i==0) calles.agregar(new Calle({-(calleaux->getLong())+(calleaux->getLong())/2,(calleaux->getLong())*j},1));
			if(j==0) calles.agregar(new Calle({(calleaux->getLong())*i,-(calleaux->getLong())+(calleaux->getLong())/2},0));
		}
	//coches.agregar(new Coche({-10,60},3));//cambiar quitar coche en 6, tiene que ir a 0
	//coches.agregar(new Coche(calles.lca[16]->getPos(),3));
	//coches.agregar(new Coche(calles.lca[27]->getPos(),2));
	//interaccion::callesemsize(calles,semaforos);
	cout<<"numero de calles: "<<calles.numero<<"\tnumero de semaforos: "<<semaforos.numero<<"ff";
}

Mundo::Mundo() {
	//Init();
}

Mundo::~Mundo() {
	delete[] Gsem;
}

void Mundo::Init() {
	/*float xxx=0;
	for(int i=0;i<40;i++)
		xxx+=3*cos(0.0125f*i*3)*0.025;
		cout<<xxx<<endl;*/
	cout<<"Introduce las filas: "<<endl;
	cin>>filas;
	cout<<"Introduce las columnas: "<<endl;
	cin>>columnas;
	pox=(calleaux->getLong())/2*(columnas-1);
	poy=(calleaux->getLong())/2*(filas-1);
	poz=(pox>poy ? ((pox+(2*(calleaux->getLong())))/tan(PI/6)):((poy+(3*(calleaux->getLong())))/tan(PI/6)));
	/*pox=(calleaux->getLong()+12)/2*(columnas-1);
	poy=(calleaux->getLong()+12)/2*(filas-1);
	poz=(pox>poy ? ((pox+(2*(calleaux->getLong()+12)))/tan(PI/6)):((poy+(3*(calleaux->getLong()+12)))/tan(PI/6)));*/
	
	crearmatriz(filas,columnas);
	//calles.lca[0]->setTipo(1);//cambiar quitar
	interaccion::callesem(calles,semaforos);
	interaccion::comprobarInalcanzable(semaforos, calles);
	for(int i=0;i<semaforos.numero;i++)
		semaforos.ls[i]->iniEstadoLuces();
	
	interaccion::crearcoche(pcoches, calles);
	interaccion::ocupacionCalle(coches, calles);
	iteraciones=10*coches.numero;
	for(int i=0;i<coches.numero;i++)
		coches.lco[i]->setTIni(clock());	//se empieza el contador de tiempo para la primera iteracion
	cout<<"\n\nIntroduce las optimizaciones:";
	while(optRuta<0){
		cout<<"\nRutas:\n\t0) Ninguna\n\t1) Dijkstra\n\t2) Floyd-Warshall\n";
		cin>>optRuta;
		if(optRuta<0||optRuta>2){
			cout<<"\n\nEl valor introducido es erróneo, vuelva a elegir: ";
			optRuta=-1;
		}
		else{
			G.crearGrafo(filas, columnas, calles, semaforos,true);
			cout<<"\nha creado el grafo";
			if(optRuta==2){
				double tini = clock();//tfin;
				Algoritmo::FloydWarshall(pg);
				tiempo=(double)(clock()-tini)/CLOCKS_PER_SEC;
				cout<<"\n\n******************** Algoritmo Floyd-Warshall ********************\n******************** Tiempo alg: "<<tiempo<<" segundos ********************\n\n";
			}
			interaccion::setRuta(coches,pg,calles,optRuta);
		}
	}
	while(optSem<0){
		cout<<"\nSemáforos:\n\t0) Ninguna\n\t1) Local\n\t2) General\n\t\t";
		cin>>optSem;
		switch(optSem){
		case 0:	//ninguna
			Gsem = new grafo[semaforos.numero];
			for (int i = 0; i < semaforos.numero; i++)
				Gsem[i].crearGrafo(calles, coches, semaforos, i, optSem);
			cout << "\nSe ha creado el grafo de la maquina de estados para " << semaforos.numero << " semaforos";
		break;
		case 1:	//local
			Gsem=new grafo[semaforos.numero];
			for(int i=0;i<semaforos.numero;i++)
				Gsem[i].crearGrafo(calles,coches,semaforos,i, optSem);
			cout<<"\nSe ha creado el grafo local para "<<semaforos.numero<<" semaforos";
		break;
		case 2:	//general/green wave
			cout<<"\ninicializa grafoGsem";
			//G2.crearGrafo(filas,columnas,calles,semaforos,true);	//cambiar mirar si se puede poner un constructor copia
			//Gsem=new grafo;
			cout<<"\nle mete valor a Gsem";
			cout<<"\ncosas";
			//Gsem=new grafo[1];
			//Gsem[0].crearGrafo(G2);	//cambiar aqui peta
			cout<<"\nSe ha creado el grafo para la green wave";
		break;
		default:
			cout<<"\n\nEl valor introducido es erróneo, vuelva a elegir: ";
			optSem=-1;
		break;
		//se hace la asignacion de pesos en tiempo real en la funcion OnTimer del semaforo
		}
	}
	
	
}

void Mundo::InitGL() {
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	glMatrixMode(GL_PROJECTION);
	gluPerspective(45.0, 2000 / 1500.0f, 0.1, 500);
}

void Mundo::OnKeyboardDown(unsigned char key, int x, int y) {
	static int i=0;
	// teclas usadas
	// a s d w e ' ' v b i h k m p z 1 2 3 4 .
	if(editor::getEnable()){
		//edit.cargaDatos(coches, calles, semaforos);
		editor::funcionesEditor(key, coches, calles, semaforos);	
	}
	switch(key){		//a,s,d,w,e, ,v,b,i,h,k,m,p,1,2,3,4,.,
	case 'a':
		coches.lco[0]->setGiro(-1);
		break;
	/*case 's':
		i++;
		break;*/
	case 'd':
		coches.lco[0]->setGiro(1);
		break;
	case 'w':
		coches.lco[0]->setGiro(0);
		coches.lco[0]->setColor(1,0.5f,1);
		coches.lco[0]->setVel(1);
		coches.lco[0]->setVelDir();
		break;
	case 'e':
		editor::toggleEnable();
		break;
	case ' ':
		cout<<"\n";
		for(int i=0;i<semaforos.numero;i++){
			cout<<"\nel semaforo "<<i<<" tiene los tiempos: ";
			for(int j=0;j<4;j++)
				cout<<semaforos.ls[i]->getTimer(j)<<" ";
			cout<<" temp y tempaux: "<<semaforos.ls[i]->getTemp(0)<<" "<<semaforos.ls[i]->getTemp(1);
		}
		/*for(int i=0; i<coches.numero; i++)
			cout<<"\nel coche "<<i<<" tiene direccion "<<coches.lco[i]->getDir()<<"   stop "<<coches.lco[i]->getStop();
		cout<<"\nel proximo giro es: "<<coches.lco[0]->getGiro();
		//cout<<interaccion::comprobarDesaparecidos(coches);*/
		break;
	case 'v':
		semaforos.ls[0]->setLuz(0);
		break;
	/*case 'b':
		for(int i=0; i<calles.numero; i++)
		calles.lca[i]->setTipo(calles.lca[0]->getTipo()+1);
		interaccion::cambiarTipoCalle(calles);
		interaccion::comprobarInalcanzable(semaforos, calles);
		interaccion::callesem(calles,semaforos);
		break;*/
	case 'i':
		for(int i=0; i<calles.numero; i++)
		calles.lca[i]->setTipo(calles.lca[i]->getTipo()+1);
		interaccion::cambiarTipoCalle(calles);
		interaccion::comprobarInalcanzable(semaforos, calles);
		interaccion::callesem(calles,semaforos);
		break;
	case 'h':
		*piteraciones=0;
		cout<<"\n\tSe finaliza la simulación al terminar la próxima ruta\n\n";
		break;
	case 'k':
		break;
	case 'm':
		for (int i = 0; i < semaforos.numero; i++) {
			cout << "\nEl semaforo " << i<< " tiene las salidas: ";
			for (int j = 0; j < 4; j++)
				cout << semaforos.ls[i]->getSalida(j) << " ";
		}
		break;
	case '1':
		coches.lco[0]->setVel(0,3);
		break;
	case '2':
		coches.lco[0]->setVel(3,0);
		break;
	case '3':
		coches.lco[0]->setVel(0,-3);
		break;
	case '4':
		coches.lco[0]->setVel(-3,0);
		break;
	case 'p':
		char aux;
		cout << "\n\npausado\n\n";
		cin >> aux;
		break;
	case 'z':
		cout << "\n Los semaforos tienen los tiempos: tvv tav tvh tah";
		for (int i =0;i<semaforos.numero;i++){
			cout << "\nsemaforo " <<i;
			for (int j=0;j<4;j++)
				cout << "\t" << semaforos.ls[i]->getTimer(j) << " ";
		}
		break;
	case '.': //debug
		int auxgiro=8;
		for(int i=0;i<coches.numero;i++){
			auxgiro = coches.lco[i]->getGiro();
			coches.lco[i]->flagdebug=false;
			cout << "\nEl coche " << i << " tiene el giro: " << auxgiro;
		}
		break;
	}
	
}

void Mundo::OnTimer() {
	float t=0.025;
	if(!editor::getEnable()){//cambiar quitar esto
	coches.mueve(t);
	interaccion::OnTimerSem(calles,coches,semaforos,optSem,Gsem);
		interaccion::cruce(coches,semaforos,calles,pg,&tiempo,iteraciones,filas,columnas,optRuta);
	interaccion::salirMundo(coches, filas, columnas, calles.getLong());
	interaccion::parcheSetDir(coches,calles);	//antes de distSeg porque se cambia el stop
	interaccion::distSeg(coches);
	interaccion::cambiarCarril(coches, calles);
	}
	//cout<<" x  "<<coches.lco[0]->getPos().x<<" y  "<<coches.lco[0]->getPos().y;
	//cout<<"\ncarril inicio "<<coches.lco[0]->n[0]<<" carril destino "<<coches.lco[0]->n[1]<<"\t";
	//printf("\nvx=%f\tvy=%f\tpx=%f\tpy=%f\nangulo=%f\t%f",coches.lco[0]->getVel().x,coches.lco[0]->getVel().y,coches.lco[0]->getPos().x, coches.lco[0]->getPos().y, coches.lco[0]->getAngRad(), coches.lco[0]->getAngD());
}

void Mundo::OnDraw() {
	//Borrado de la pantalla	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Para definir el punto de vista
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(pox, poy, poz,  	// posicion del ojo
		pox, poy, 0.0,      	// hacia que punto mira  (0,0,0) 
		0.0, 1.0, 0.0);      	// definimos hacia arriba (eje Y)    

	/////////////////
	///////////
	//		AQUI EMPIEZA MI DIBUJO
	
	
	coches.dibuja();
	calles.dibuja();
	semaforos.dibuja();
	editor::dibuja();
	
	/////////////////
	///////////
	//		AQUI TERMINA MI DIBUJO
	////////////////////////////

	//Al final, cambiar el buffer
	glutSwapBuffers();
}
