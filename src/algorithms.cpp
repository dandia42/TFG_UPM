#pragma once
#include "algorithms.h"

Dijkstra::Dijkstra() {
  distance_ = predecessorID_ = iteration_ = 0;
  visited_ = false;
}
Dijkstra::~Dijkstra() {
  distance_ = predecessorID_ = iteration_ = visited_ = NULL;
}

void Algorithm::dijkstra(int inicio, Graph* graph, bool sem) {
  if (inicio == -1) {
    cout << "\n\nError en el inicio del graph" << endl;
    return;
  } else
    cout << "\nEl inicio del graph es " << inicio << endl;

  if (sem) {  // aplicar sobre el graph original
    // Inicializar las distancias de los vertices
    // inicio=2;
    for (int i = 0; i < graph->nNodes_; i++) {
      if (i == inicio)
        graph->node_[i].dijkstraVariable_.setDistance(0);
      else
        graph->node_[i].dijkstraVariable_.setDistance(MAX_WEIGHT);
      graph->node_[i].dijkstraVariable_.setVisited(false);
    }

    // Procesar los nodos en orden creciente de distance_
    for (int i = 0; i < graph->nNodes_; i++) {
      // Encontrar el nodo más cercano no visited_
      int u = -1;
      for (int j = 0; j < graph->nNodes_; j++)
        if (!graph->node_[j].dijkstraVariable_.getVisited() && 
            (u == -1 || graph->node_[j].dijkstraVariable_.getDist() 
            < graph->node_[u].dijkstraVariable_.getDist()))
          u = j;
      if (graph->node_[u].dijkstraVariable_.getDist() == MAX_WEIGHT) break;
      graph->node_[u].dijkstraVariable_.setVisited(true);

      // Actualizar las distancias de los nodos adyacentes
      for (int j = 0; j < graph->nLinks_; j++)
        if (graph->link_[j].getOrigin() == u) {
          int v = graph->link_[j].getDestination();
          int w = graph->link_[j].getWeight();
          if (graph->node_[u].dijkstraVariable_.getDist() + w < graph->node_[v].dijkstraVariable_.getDist()) {
            graph->node_[v].dijkstraVariable_.setDistance(graph->node_[u].dijkstraVariable_.getDist() + w);
            graph->node_[v].dijkstraVariable_.setID(u);
            graph->node_[v].dijkstraVariable_.setIteration(graph->node_[u].dijkstraVariable_.getIteration() + 1);
          }
        }
    }

    for (int i = 0; i < graph->nNodes_; i++)  // comprobar que se han visited_ todos los nodos
      if (!graph->node_[i].dijkstraVariable_.getVisited()) cout << "\n\tFalta por visitar el node_ " << i;
    DOUT << "\n\tSe han visited_ todos los nodos" << endl;
    for (int i = 0; i < graph->nNodes_; i++)
      DOUT << "\nVertice=" << i << "   Distancia=" << graph->node_[i].getDijkstraVariable_().getDist()
           << "   predecessorID_=" << graph->node_[i].getDijkstraVariable_().getID() << "   iteration_=" 
           << graph->node_[i].getDijkstraVariable_().getIteration();
  } else {  // aplicar sobre el graph conjugado
    // Inicializar las distancias de los vertices
    // inicio=2;
    for (int i = 0; i < graph->nConjugateNodes_; i++) {
      if (i == inicio)
        graph->conjugateNode_[i].dijkstraVariable_.setDistance(0);
      else
        graph->conjugateNode_[i].dijkstraVariable_.setDistance(MAX_WEIGHT);
      graph->conjugateNode_[i].dijkstraVariable_.setVisited(false);
    }

    // Procesar los nodos en orden creciente de distance_
    for (int i = 0; i < graph->nConjugateNodes_; i++) {
      // Encontrar el nodo más cercano no visited_
      int u = -1;
      for (int j = 0; j < graph->nConjugateNodes_; j++)
        if (!graph->conjugateNode_[j].dijkstraVariable_.getVisited() &&
            (u == -1 || graph->conjugateNode_[j].dijkstraVariable_.getDist() < graph->conjugateNode_[u].dijkstraVariable_.getDist()))
          u = j;
      if (graph->conjugateNode_[u].dijkstraVariable_.getDist() == MAX_WEIGHT) break;
      graph->conjugateNode_[u].dijkstraVariable_.setVisited(true);

      // Actualizar las distancias de los nodos adyacentes
      for (int j = 0; j < graph->nConjugateLinks_; j++)
        if (graph->conjugateLink_[j].getOrigin() == u) {
          int v = graph->conjugateLink_[j].getDestination();
          int w = graph->conjugateLink_[j].getWeight();
          if (graph->conjugateNode_[u].dijkstraVariable_.getDist() + w < graph->conjugateNode_[v].dijkstraVariable_.getDist()) {
            graph->conjugateNode_[v].dijkstraVariable_.setDistance(graph->conjugateNode_[u].dijkstraVariable_.getDist() + w);
            graph->conjugateNode_[v].dijkstraVariable_.setID(u);
            graph->conjugateNode_[v].dijkstraVariable_.setIteration(graph->conjugateNode_[u].dijkstraVariable_.getIteration() + 1);
          }
        }
    }

    for (int i = 0; i < graph->nConjugateNodes_; i++)  // comprobar que se han visited_ todos los nodos
      if (!graph->conjugateNode_[i].dijkstraVariable_.getVisited()) cout << "\n\tFalta por visitar el node_ " << i;
    cout << "\n\tSe han visited_ todos los nodos" << endl;
    for (int i = 0; i < graph->nConjugateNodes_; i++)
      cout << "\nVertice=" << i << "   Distancia=" << graph->conjugateNode_[i].getDijkstraVariable_().getDist()
           << "   predecessorID_=" << graph->conjugateNode_[i].getDijkstraVariable_().getID() << "   iteration_=" << graph->conjugateNode_[i].getDijkstraVariable_().getIteration();
  }
}

void Algorithm::floydWarshall(Graph* graph) {
  // matriz dist para las distancias entre todos los nodos, matriz origen para conocer el node_ previo
  // Inicializar las distancias y los origenes de los vertices
  for (int i = 0; i < graph->nConjugateNodes_; i++) {
    for (int j = 0; j < graph->nConjugateNodes_; j++) {
      graph->weightFloydWarshall_[i][j] = MAX_WEIGHT;
      graph->originFloydWarshall_[i][j] = -1;
    }
    graph->weightFloydWarshall_[i][i] = 0;  // la distance_ a si mismo es 0
  }

  // Actualizar las distancias a los valores iniciales del graph
  for (int i = 0; i < graph->nConjugateNodes_; i++) {
    for (int j = 0; j < graph->conjugateNode_[i].numberAdjacentNodes_; j++) {
      // Se asignan las distancias con una matriz donde se ve el peso entre el node_ origen i y el node_ destino
      // graph.conjugateNode_[i]->adjacentNode_[j]
      graph->weightFloydWarshall_[i][graph->conjugateNode_[i].adjacentNode_[j]] = graph->conjugateNode_[i].adjacentNodeWeight_[j];
      graph->originFloydWarshall_[i][graph->conjugateNode_[i].adjacentNode_[j]] =
          i;  // se marca el origen y no se toca lo demas para la primera iteracion
    }
  }

  // Se aplica el algoritmo
  for (int k = 0; k < graph->nConjugateNodes_; k++) {
    for (int i = 0; i < graph->nConjugateNodes_; i++) {
      for (int j = 0; j < graph->nConjugateNodes_; j++) {
        if (graph->weightFloydWarshall_[i][k] != MAX_WEIGHT && graph->weightFloydWarshall_[k][j] != MAX_WEIGHT &&
            graph->weightFloydWarshall_[i][j] > graph->weightFloydWarshall_[i][k] + graph->weightFloydWarshall_[k][j]) {
          graph->weightFloydWarshall_[i][j] = graph->weightFloydWarshall_[i][k] + graph->weightFloydWarshall_[k][j];
          graph->originFloydWarshall_[i][j] = graph->originFloydWarshall_[k][j];
        }
      }
    }
  }

  // Se obtiene el resultado
  cout << "\nDistancias mínimas:" << endl;
  for (int i = 0; i < graph->nConjugateNodes_; i++) {
    for (int j = 0; j < graph->nConjugateNodes_; j++) {
      if (graph->weightFloydWarshall_[i][j] == MAX_WEIGHT)
        cout << "No conectan ";
      else
        cout << graph->weightFloydWarshall_[i][j] << ((graph->weightFloydWarshall_[i][j] > 9) ? ("  ") : ("   "));
    }
    cout << endl;
  }

  cout << "\nMatriz de origenes:" << endl;
  for (int i = 0; i < graph->nConjugateNodes_; i++) {
    for (int j = 0; j < graph->nConjugateNodes_; j++) {
      if (graph->originFloydWarshall_[i][j] == MAX_WEIGHT)
        cout << "No conectan ";
      else
        cout << graph->originFloydWarshall_[i][j] << ((graph->originFloydWarshall_[i][j] > 9) ? ("  ") : ("   "));
    }
    cout << endl;
  }
}

void Algorithm::greenWave(Graph* graph, int*& rsem, int& ncompleta,
                          StreetList& lca) {  // ciclo en el que aplicar el algoritmo
  int ini1 = rand() % graph->nNodes_, ini2 = rand() % graph->nNodes_,
      ini3 = rand() % graph->nNodes_;         // se hace sobre el graph inicial, no el conjugado
  bool aux1 = true;                         // flag para obtener un ini3 que no se repita
  int n1 = 0, n2 = 0, n3 = 0;               // number de nodos en cada route_
  int r_ = 0, *r1 = &r_, *r2 = &r_, *r3 = &r_;  // las 3 rutas
  int *streets_, *calles1, *calles2, *calles3;

  while (ini1 == ini2)  // evitar que los indices desde los que se busca la mejor route_ sean los mismos
    ini2 = rand() % graph->nNodes_;

  DOUT << "\nesto no deberia coincidir ninguno" << endl;
  auxFunction(ini1, ini2, graph, n1, r1, calles1);

  while (aux1) {  // tambien se podria aplicar usando std::find metiendole una lista de exclusion
    ini3 = rand() % graph->nNodes_;
    aux1 = 0;
    for (int i = 0; i < n1; i++)
      if (ini3 == r1[i]) aux1 = 1;
    if (ini3 == ini2) aux1 = 1;
  }
  graph->resetGraph();  // se inicializan a 0 las variables de dijkstra
  auxFunction(ini2, ini3, graph, n2, r2, calles2);
  graph->resetGraph();  // se inicializan a 0 las variables de dijkstra
  auxFunction(ini3, ini1, graph, n3, r3, calles3);
  graph->resetGraph();  // se inicializan a 0 las variables de dijkstra

  DOUT << "\n\tInfo rutas: n1: " << n1 << ", n2: " << n2 << ", n3: " << n3 << ", nodos iniciales: " << ini1 << ", "
       << ini2 << ", " << ini3 << endl;
  DOUT << "\n\t\tRuta 1: " << endl;
  for (int i = 0; i < n1; i++) DOUT << r1[i] << " ";
  DOUT << "\n\t\tRuta 2: " << endl;
  for (int i = 0; i < n2; i++) DOUT << r2[i] << " ";
  DOUT << "\n\t\tRuta 3: " << endl;
  for (int i = 0; i < n3; i++) DOUT << r3[i] << " ";

  // se agrupan las 3 rutas en 1
  ncompleta = n1 + n2 + n3;
  rsem = new int[ncompleta];
  streets_ = new int[ncompleta];
  int pos = 0;
  for (int i = 0; i < n1; i++) {
    rsem[pos] = r1[i];
    streets_[pos++] = calles1[i];
  }
  for (int i = 0; i < n2; i++) {
    rsem[pos] = r2[i];
    streets_[pos++] = calles2[i];
  }
  for (int i = 0; i < n3; i++) {
    rsem[pos] = r3[i];
    streets_[pos++] = calles3[i];
  }

  DOUT << "\n\t\tRuta Completa: " << endl;
  for (int i = 0; i < ncompleta; i++) DOUT << rsem[i] << " ";

  DOUT << "\n\t\tCalles usadas: " << endl;
  for (int i = 0; i < ncompleta; i++) {
    DOUT << streets_[i] << " ";
    lca.streetList_[streets_[i]]->setColor(1, 0, 1);
  }

  cin >> n1;  // borrar esto cuando se compruebe que esta bien

  /*
  dijkstra(ini1,graph,true);	//encontrar la route_ optima desde el primer node_ hasta todos los demás

  //hacer un pseudo Interaction::setRoute() para conocer la route_ de ini1 a ini2
  int aux=ini2;
  n1=graph->node_[ini2].dijkstraVariable_.getIteration();
  r1=new int[n1];	//vector para almacenar la primera route_
  for(int i=n1-1;i>=0;i--){
          r1[i]=graph->node_[aux].dijkstraVariable_.getID();
          aux=graph->node_[aux].dijkstraVariable_.getID();
  }
  //aqui igual hay que aumentar el peso de las aristas que conectan los vertices de la route_ para que no se repitan
  vertices, luego habria que bajar el peso para que se usen en setRuta for(int i=0;i<n1;i++){ if(i<(n1-1))//los primeros
  indices de la route_ parcial for(int j=0;j<graph->nLinks_;j++){ if(graph->link_[j].origin_==r1[i] || graph->link_[j].destination_==r1[i])//si el
  node_ anterior o el siguiente está en la route_, se aumenta el peso de la link_ que los une para excluirla de la
  siguiente route_ graph->link_[j].setVariableWeight(1000);
                  }
          else//el ultimo indice de la route_ parcial
                  for(int j=0;j<graph->nLinks_;j++){
                          if(graph->link_[j].origin_==r1[i])// solo el origen, si el node_ anterior o el siguiente está en la
  route_, se aumenta el peso de la link_ que los une para excluirla de la siguiente route_
                                  graph->link_[j].setVariableWeight(1000);
                  }
  }



  ///////repetir para el segundo node_
  dijkstra(ini2,graph,true);	//encontrar la route_ optima desde el segundo node_ hasta todos los demás

  //hacer un pseudo Interaction::setRoute() para conocer la route_ de ini2 a ini3
  int aux=ini3;
  n2=graph->node_[ini3].dijkstraVariable_.getIteration();
  r2=new int[n2];	//vector para almacenar la primera route_
  for(int i=n2-1;i>=0;i--){
          r2[i]=graph->node_[aux].dijkstraVariable_.getID();
          aux=graph->node_[aux].dijkstraVariable_.getID();
  }
  //aqui igual hay que aumentar el peso de las aristas que conectan los vertices de la route_ para que no se repitan
  vertices, luego habria que bajar el peso para que se usen en setRuta for(int i=0;i<n1;i++){ if(i<(n2-1))//los primeros
  indices de la route_ parcial for(int j=0;j<graph->nLinks_;j++){ if(graph->link_[j].origin_==r2[i] || graph->link_[j].destination_==r2[i])//si el
  node_ anterior o el siguiente está en la route_, se aumenta el peso de la link_ que los une para excluirla de la
  siguiente route_ graph->link_[j].setVariableWeight(1000);
                  }
          else//el ultimo indice de la route_ parcial
                  for(int j=0;j<graph->nLinks_;j++){
                          if(graph->link_[j].origin_==r2[i])// solo el origen, si el node_ anterior o el siguiente está en la
  route_, se aumenta el peso de la link_ que los une para excluirla de la siguiente route_
                                  graph->link_[j].setVariableWeight(1000);
                  }
  }
  */
}

void Algorithm::auxFunction(int inia, int inib, Graph*& graph, int& n, int*& r_, int*& streets_) {
  DOUT << "****************************************************************************" << endl;
  dijkstra(inia, graph, true);  // encontrar la route_ optima desde el segundo node_ hasta todos los demás
  DOUT << "****************************************************************************" << endl;

  // hacer un pseudo Interaction::setRoute() para conocer la route_ de ini2 a ini3
  int aux = inib;
  n = graph->node_[inib].dijkstraVariable_.getIteration();
  r_ = new int[n];  // vector para almacenar la primera route_
  streets_ = new int[n];

  for (int i = n - 1; i >= 0; i--) {
    r_[i] = graph->node_[aux].dijkstraVariable_.getID();
    aux = graph->node_[aux].dijkstraVariable_.getID();
  }
  // aqui igual hay que aumentar el peso de las aristas que conectan los vertices de la route_ para que no se repitan
  // vertices, luego habria que bajar el peso para que se usen en setRuta
  for (int i = 0; i < n; i++) {
    if (i < (n - 1))  // los primeros indices de la route_ parcial
      for (int j = 0; j < graph->nLinks_; j++) {
        if (graph->link_[j].getOrigin() == r_[i]) {  // si el node_ anterior o el siguiente está en la route_, se aumenta el
                                             // peso de la link_ que los une para excluirla de la siguiente route_
          graph->link_[j].setVariableWeight(1000);
          streets_[i] = graph->link_[j].getIndex();
        }
        if (graph->link_[j].getDestination() == r_[i]) graph->link_[j].setVariableWeight(1000);
      }
    else  // el ultimo indice de la route_ parcial
      for (int j = 0; j < graph->nLinks_; j++) {
        if (graph->link_[j].getOrigin() ==
            r_[i]) {  // solo el origen, si el node_ anterior o el siguiente está en la route_, se aumenta el peso de la
                     // link_ que los une para excluirla de la siguiente route_
          graph->link_[j].setVariableWeight(1000);
          streets_[i] = graph->link_[j].getIndex();
        }
      }
  }
  DOUT << "\n\t\t***** Salida route_ funcion aux *****";
  for (int i = 0; i < n; i++) DOUT << "\t" << r_[i] << endl;
}
