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

  if (sem) {  // apply on original graph
    // Initialize nodes distance
    for (int i = 0; i < graph->nNodes_; i++) {
      if (i == inicio)
        graph->node_[i].dijkstraVariable_.setDistance(0);
      else
        graph->node_[i].dijkstraVariable_.setDistance(MAX_WEIGHT);
      graph->node_[i].dijkstraVariable_.setVisited(false);
    }

    // Process nodes in increasing distance order
    for (int i = 0; i < graph->nNodes_; i++) {
      // Finde closer no visited node
      int u = -1;
      for (int j = 0; j < graph->nNodes_; j++)
        if (!graph->node_[j].dijkstraVariable_.getVisited() && 
            (u == -1 || graph->node_[j].dijkstraVariable_.getDist() 
            < graph->node_[u].dijkstraVariable_.getDist()))
          u = j;
      if (graph->node_[u].dijkstraVariable_.getDist() == MAX_WEIGHT) break;
      graph->node_[u].dijkstraVariable_.setVisited(true);

      // Update adjacent nodes distance
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

    for (int i = 0; i < graph->nNodes_; i++)  // check that all nodes have been visited
      if (!graph->node_[i].dijkstraVariable_.getVisited()) cout << "\n\tThe node: " << i << " has yet to be visited";
    DOUT << "\n\tAll nodes visited" << endl;
    for (int i = 0; i < graph->nNodes_; i++)
      DOUT << "\nNode=" << i << "   Distance=" << graph->node_[i].getDijkstraVariable_().getDist()
           << "   predecessorID=" << graph->node_[i].getDijkstraVariable_().getID() << "   iteration=" 
           << graph->node_[i].getDijkstraVariable_().getIteration();
  } else {  // apply on conjugate graph
    // Initialize nodes distance
    for (int i = 0; i < graph->nConjugateNodes_; i++) {
      if (i == inicio)
        graph->conjugateNode_[i].dijkstraVariable_.setDistance(0);
      else
        graph->conjugateNode_[i].dijkstraVariable_.setDistance(MAX_WEIGHT);
      graph->conjugateNode_[i].dijkstraVariable_.setVisited(false);
    }

    // Process nodes in increasing distance order
    for (int i = 0; i < graph->nConjugateNodes_; i++) {
      // Finde closer no visited node
      int u = -1;
      for (int j = 0; j < graph->nConjugateNodes_; j++)
        if (!graph->conjugateNode_[j].dijkstraVariable_.getVisited() &&
            (u == -1 || graph->conjugateNode_[j].dijkstraVariable_.getDist() < graph->conjugateNode_[u].dijkstraVariable_.getDist()))
          u = j;
      if (graph->conjugateNode_[u].dijkstraVariable_.getDist() == MAX_WEIGHT) break;
      graph->conjugateNode_[u].dijkstraVariable_.setVisited(true);

      // Update adjacent nodes distance
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

    for (int i = 0; i < graph->nConjugateNodes_; i++)  // check that all nodes have been visited
      if (!graph->conjugateNode_[i].dijkstraVariable_.getVisited()) cout << "\n\tThe node: " << i << " has yet to be visited";
    DOUT << "\n\tAll nodes visited" << endl;
    for (int i = 0; i < graph->nConjugateNodes_; i++)
      cout << "\nNode=" << i << "   Distance=" << graph->conjugateNode_[i].getDijkstraVariable_().getDist()
           << "   predecessorID=" << graph->conjugateNode_[i].getDijkstraVariable_().getID() << "   iteration=" << graph->conjugateNode_[i].getDijkstraVariable_().getIteration();
  }
}

void Algorithm::floydWarshall(Graph* graph) {
  // Weight matrix to save distance between nodes, origin matrix to save previous node
  // Initialize distances and origins of the nodes
  for (int i = 0; i < graph->nConjugateNodes_; i++) {
    for (int j = 0; j < graph->nConjugateNodes_; j++) {
      graph->weightFloydWarshall_[i][j] = MAX_WEIGHT;
      graph->originFloydWarshall_[i][j] = -1;
    }
    graph->weightFloydWarshall_[i][i] = 0;  // The distance to itself is 0
  }

  // Update distance to initial graph values
  for (int i = 0; i < graph->nConjugateNodes_; i++) {
    for (int j = 0; j < graph->conjugateNode_[i].numberAdjacentNodes_; j++) {
      // Distance between origin node i and destination node are assigned with a matrix
      // graph.conjugateNode_[i]->adjacentNode_[j]
      graph->weightFloydWarshall_[i][graph->conjugateNode_[i].adjacentNode_[j]] = graph->conjugateNode_[i].adjacentNodeWeight_[j];
      graph->originFloydWarshall_[i][graph->conjugateNode_[i].adjacentNode_[j]] =
          i;  // check the origin and do not change anything on the first iteration
    }
  }

  // Apply algorithm
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

  // Obtain the result
  cout << "\nMinimum distances:" << endl;
  for (int i = 0; i < graph->nConjugateNodes_; i++) {
    for (int j = 0; j < graph->nConjugateNodes_; j++) {
      if (graph->weightFloydWarshall_[i][j] == MAX_WEIGHT)
        cout << "Do not connect ";
      else
        cout << graph->weightFloydWarshall_[i][j] << ((graph->weightFloydWarshall_[i][j] > 9) ? ("  ") : ("   "));
    }
    cout << endl;
  }

  cout << "\nOrigin matrix:" << endl;
  for (int i = 0; i < graph->nConjugateNodes_; i++) {
    for (int j = 0; j < graph->nConjugateNodes_; j++) {
      if (graph->originFloydWarshall_[i][j] == MAX_WEIGHT)
        cout << "Do not connect ";
      else
        cout << graph->originFloydWarshall_[i][j] << ((graph->originFloydWarshall_[i][j] > 9) ? ("  ") : ("   "));
    }
    cout << endl;
  }
}

void Algorithm::greenWave(Graph* graph, int*& streetlightRoute, int& streetlightNumber,
                          StreetList& streetList) {  // loop to apply the algorithm
  int ini1 = rand() % graph->nNodes_, ini2 = rand() % graph->nNodes_,
      ini3 = rand() % graph->nNodes_;         // using the initial graph, not the congugated
  bool aux1 = true;                         // flag to obtain ini3 that does not repeat
  int n1 = 0, n2 = 0, n3 = 0;               // nodes number in each route
  int route = 0, *r1 = nullptr, *r2 = nullptr, *r3 = nullptr;  // 3 routes
  int *streets, *streets1, *streets2, *streets3;

  while (ini1 == ini2)  // avoid initial nodes repetition
    ini2 = rand() % graph->nNodes_;

  DOUT << "\nThey should not repeat" << endl;
  auxFunction(ini1, ini2, graph, n1, r1, streets1);

  while (aux1) {  // TODO tambien se podria aplicar usando std::find metiendole una lista de exclusion
    ini3 = rand() % graph->nNodes_;
    aux1 = 0;
    for (int i = 0; i < n1; i++)
      if (ini3 == r1[i]) aux1 = 1;
    if (ini3 == ini2) aux1 = 1;
  }
  graph->resetGraph();  // reset dijkstra variables
  auxFunction(ini2, ini3, graph, n2, r2, streets2);
  graph->resetGraph();  // reset dijkstra variables
  auxFunction(ini3, ini1, graph, n3, r3, streets3);
  graph->resetGraph();  // reset dijkstra variables

  DOUT << "\n\tRoute information: n1: " << n1 << ", n2: " << n2 << ", n3: " << n3 << ", initial nodes: " << ini1 << ", "
       << ini2 << ", " << ini3 << endl;

  DOUT << "\n\t\tRoute 1: " << endl;
  for (int i = 0; i < n1; ++i) 
    DOUT << r1[i] << " ";

  DOUT << "\n\t\tRoute 2: " << endl;
  for (int i = 0; i < n2; ++i) 
    DOUT << r2[i] << " ";

  DOUT << "\n\t\tRoute 3: " << endl;
  for (int i = 0; i < n3; ++i) 
    DOUT << r3[i] << " ";









  // ----- Ruta completa de nodos -----
  DOUT << "\n\t\tRoute completed: " << endl;
  // Ruta 1 completa
  for (int i = 0; i < n1; ++i) DOUT << r1[i] << " ";
  // Ruta 2 desde el segundo nodo
  for (int i = 1; i < n2; ++i) DOUT << r2[i] << " ";
  // Ruta 3 desde el segundo nodo
  for (int i = 1; i < n3; ++i) DOUT << r3[i] << " ";
  DOUT << endl;

  // ----- Calles usadas (ciclo completo) -----
  std::vector<int> streetsUsed;

  // 1) Calles internas de ruta 1
  for (int i = 0; i < n1 - 1; ++i) {
    streetsUsed.push_back(streets1[i]);  // (r1[i], r1[i+1]) -> (11,9)
  }

  // 2) Calle entre fin de ruta 1 y inicio de ruta 2: (r1[n1-1], r2[0]) -> (9,6)
  auto findStreet = [&](int a, int b) -> int {
    for (int j = 0; j < graph->nLinks_; ++j) {
      int o = graph->link_[j].getOrigin();
      int d = graph->link_[j].getDestination();
      if ((o == a && d == b) || (o == b && d == a)) {
        return graph->link_[j].getIndex();
      }
    }
    return -1;
  };

  int s = findStreet(r1[n1 - 1], r2[0]);
  streetsUsed.push_back(s);

  // 3) Calles internas de ruta 2: (6,3), (3,5)
  for (int i = 0; i < n2 - 1; ++i) {
    streetsUsed.push_back(streets2[i]);
  }

  // 4) Calle entre fin de ruta 2 y inicio de ruta 3: (r2[n2-1], r3[0]) -> (5,4)
  s = findStreet(r2[n2 - 1], r3[0]);
  streetsUsed.push_back(s);

  // 5) Calles internas de ruta 3: (4,7), (7,8)
  for (int i = 0; i < n3 - 1; ++i) {
    streetsUsed.push_back(streets3[i]);
  }

  // 6) Calle entre fin de ruta 3 y inicio de ruta 1: (r3[n3-1], r1[0]) -> (8,11)
  s = findStreet(r3[n3 - 1], r1[0]);
  streetsUsed.push_back(s);

  // Ahora ya tienes exactamente 8 calles en el ejemplo
  streetlightNumber = static_cast<int>(streetsUsed.size());
  streetlightRoute = new int[streetlightNumber];
  streets = new int[streetlightNumber];

  // streetlightRoute: nodos destino de cada calle del ciclo
  // (11,9) -> 9, (9,6) -> 6, (6,3) -> 3, ..., (8,11) -> 11
  int currentNode = r1[0];  // 11
  for (int i = 0; i < streetlightNumber; ++i) {
    streets[i] = streetsUsed[i];

    // deducir nodo destino a partir de la calle y el nodo actual
    int idx = streetsUsed[i];
    if (idx >= 0 && idx < graph->nLinks_) {
      int o = graph->link_[idx].getOrigin();
      int d = graph->link_[idx].getDestination();
      int nextNode = (o == currentNode) ? d : (d == currentNode ? o : -1);
      streetlightRoute[i] = nextNode;
      currentNode = (nextNode != -1 ? nextNode : currentNode);
    } else {
      streetlightRoute[i] = -1;
    }
  }

  // Debug
  DOUT << "\n\t\tStreets used: " << endl;
  for (int i = 0; i < streetlightNumber; ++i) {
    DOUT << streets[i] << " ";
  }
  DOUT << endl;

  // Aplicar color solo a índices válidos
  for (int i = 0; i < streetlightNumber; ++i) {
    if (streets[i] >= 0 && streets[i] < streetList.number_) {
      streetList.streetList_[streets[i]]->setColor(1, 0, 1);
    } else {
      DOUT << "\n[WARN] Invalid street index in greenWave: " << streets[i] << endl;
    }
  }

  // Liberar solo lo que viene de auxFunction
  delete[] r1;
  delete[] r2;
  delete[] r3;
  delete[] streets1;
  delete[] streets2;
  delete[] streets3;

  cin >> n1;  // TODO borrar esto cuando se compruebe que esta bien

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

void Algorithm::auxFunction(int inia, int inib, Graph*& graph, int& n, int*& route, int*& streets) {
  DOUT << "****************************************************************************" << endl;
  dijkstra(inia, graph, true);  // Find best route from node a to each other node
  DOUT << "****************************************************************************" << endl;

  // adapted Interaction::setRoute() to get ini2 to ini3 route
  int aux = inib;
  n = graph->node_[inib].dijkstraVariable_.getIteration();
  route = new int[n];  // array to save first route

  for (int i = n - 1; i >= 0; i--) {
    route[i] = graph->node_[aux].dijkstraVariable_.getID();
    aux = graph->node_[aux].dijkstraVariable_.getID();
  }


  int m = (n > 0) ? n - 1 : 0;
  streets = new int[m];
  for (int i = 0; i < m; ++i) {
    int a = route[i];
    int b = route[i + 1];
    bool found = false;

    for (int j = 0; j < graph->nLinks_; ++j) {
      int o = graph->link_[j].getOrigin();
      int d = graph->link_[j].getDestination();

      if ((o == a && d == b) || (o == b && d == a)) {
        graph->link_[j].setVariableWeight(1000);
        streets[i] = graph->link_[j].getIndex();
        found = true;
        break;
      }
    }

    if (!found) {
      // Fallback seguro: marca como inválido
      streets[i] = -1;
    }
  }
  
  
  
  
  // TODO aqui igual hay que aumentar el peso de las aristas que conectan los vertices de la route_ para que no se repitan
  // vertices, luego habria que bajar el peso para que se usen en setRuta
  /*for (int i = 0; i < n; i++) {
    if (i < (n - 1))  // first index of partial route
      for (int j = 0; j < graph->nLinks_; j++) {
        if (graph->link_[j].getOrigin() == route[i]) {  // if previous or next node is in the route, 
                                                        //link weight is increased to exclude it from the next route
          graph->link_[j].setVariableWeight(1000);
          streets[i] = graph->link_[j].getIndex();
        }
        if (graph->link_[j].getDestination() == route[i]) graph->link_[j].setVariableWeight(1000);
      }
    else  // last node of partial route
      for (int j = 0; j < graph->nLinks_; j++) {
        if (graph->link_[j].getOrigin() ==
            route[i]) {  // just the origin, if previous or next node is in the route, 
                         //link weight is increased to exclude it from the next route
          graph->link_[j].setVariableWeight(1000);
          streets[i] = graph->link_[j].getIndex();
        }
      }
  }*/
  DOUT << "\n\t\t***** Exit route auxFunction *****";
  for (int i = 0; i < n; ++i) DOUT << "\t" << route[i] << endl;
}
