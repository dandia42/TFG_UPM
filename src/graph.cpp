#pragma once
#include "graph.h"

int function(int, int, int); // TODO quesesto

Graph::Graph() {
  nNodes_ = nLinks_ = nConjugateNodes_ = nConjugateLinks_ = 0;
}
Graph::~Graph() {
  for (int i = 0; i < nNodes_; i++) {
    delete[] node_[i].adjacentNode_;
    delete[] node_[i].adjacentNodeWeight_;
  }
  for (int i = 0; i < nConjugateNodes_; i++) {
    delete[] conjugateNode_[i].adjacentNode_;
    delete[] conjugateNode_[i].adjacentNodeWeight_;
  }
  // Delete Floyd-Warshall matrix
  for (int i = 0; i < nConjugateNodes_; i++) {
    delete[] weightFloydWarshall_[i];
    delete[] originFloydWarshall_[i];
  }
  for (int i = 0; i < nConjugateNodes_; i++) {
    delete[] weightFloydWarshall_;
    delete[] originFloydWarshall_;
  }

  delete[] link_;
  delete[] node_;
  delete[] conjugateLink_;
  delete[] conjugateNode_;
  cout << endl << endl << "Manage memory" << endl;
}

void Graph::buildGraph(int rows, int columns, StreetList streets, TrafficlightList trafficlights, bool flag) {
  static bool aux1 = false;  
  if (flag) { // Flag used by green wave to calculate adjacency list just the first time
    int i, j, iNodes = 0;  // iNodes: indice de vertices
    // Create nodes
    nNodes_ = rows * columns;
    node_ = new Node[nNodes_];  // rows*columns=nNodes
    for (j = 1; j <= rows; j++) {
      cout << endl;
      for (i = 1; i <= columns; i++) {
        node_[iNodes].id_ = i * j - 1 + (i - 1) * (rows - j);
        cout << node_[iNodes].id_ << "\t";
        iNodes++;
      }
    }

    // Link creation 0 for horizontal 1 for vertical
    nLinks_ = countDirections(streets);
    link_ = new Link[nLinks_];
    cout << "\nThere are " << nLinks_ << " links\n";
  }
  assignLinkDirection(streets, trafficlights, rows, columns, flag);  // All true, false only for variable weight
  for (int i = 0; i < nNodes_; i++) node_[i].numberAdjacentNodes_ = 0;
  for (int i = 0; i < nLinks_; i++)  // Used to know how many adjacent nodes are there
    node_[link_[i].getOrigin()].numberAdjacentNodes_++;

  if (!aux1) {  // only once
    adjacencyList(nLinks_, false);
    aux1 = true;
  }

  buildConjugateGraph(flag);
  asociateStreetNode(streets);
  asociateStreetLink(streets, trafficlights);
}
void Graph::buildGraph(Graph& graph) {
  node_ = graph.node_;
  conjugateNode_ = graph.conjugateNode_;
  link_ = graph.link_;
  conjugateLink_ = graph.conjugateLink_;
  nNodes_ = graph.nNodes_;
  nLinks_ = graph.nLinks_;
  nConjugateNodes_ = graph.nConjugateNodes_;
  nConjugateLinks_ = graph.nConjugateLinks_;
  end_ = graph.end_;
  auxiliarTimer_ = graph.auxiliarTimer_;
  // TODO comprobar si tengo que meter tambien la inicializacion de las matrices FW
}
void Graph::buildConjugateGraph(bool flag) {
  if (flag) {
    nConjugateNodes_ = nLinks_;
    for (int i = 0; i < nLinks_; i++) {
      for (int j = 0; j < nLinks_; j++) {
        if (link_[i].destination_ == link_[j].origin_ && link_[i].origin_ != link_[j].destination_) nConjugateLinks_++;
      }
    }
    conjugateNode_ = new Node[nConjugateNodes_];
    conjugateLink_ = new Link[nConjugateLinks_];
    // cout<<"\n\nhay "<<nConjugateLinks_<<" aristas conjugadas y "<<nConjugateNodes_<<" vertices conjugados \n";

    int iLink = 0;  // Link index
    for (int i = 0; i < nLinks_; i++) {
      for (int j = 0; j < nLinks_; j++) {
        if (link_[i].destination_ == link_[j].origin_ && link_[i].origin_ != link_[j].destination_) {
          conjugateLink_[iLink].origin_ = i;
          conjugateLink_[iLink].destination_ = j;
          iLink++;
        }
      }
    }
  }
  // Asociate nodes and street index
  for (int i = 0; i < nConjugateNodes_; i++) {
    conjugateNode_[i].index_ = link_[i].index_;
    conjugateNode_[i].occupancy_ = link_[i].variableWeight_;
    conjugateNode_[i].numberAdjacentNodes_ = 0;
  }
  for (int i = 0; i < nConjugateLinks_; i++)  // Used to know how many adjacent nodes are there
    conjugateNode_[conjugateLink_[i].getOrigin()].numberAdjacentNodes_++;
  calculateConjugateWeight(flag);
  adjacencyList(nConjugateLinks_, true);
  /*for(int i=0;i<nConjugateLinks_;i++)
          cout<<"\nse asignan los valores "<<conjugateLink_[i].origin_<<" "<<conjugateLink_[i].destination_<<" \tpeso fijo
     "<<conjugateLink_[i].constantWeight_<<" peso variable "<<conjugateLink_[i].variableWeight_<<" a la link_ conjugada "<<i;*/
  cout << "\n\nGRAPH GENERATION COMPLETED\n\n";
}

void Graph::assignLinkDirection(StreetList streets, TrafficlightList trafficlights, int rows, int columns, bool flag) {
  int iLink = 0;  // link index
  static int aux = 0;
  for (int i = 1; i <= columns; i++) {
    for (int j = 1; j <= rows; j++) {
      for (int aux = 0; aux < 4; aux++) {
        if (trafficlights.trafficlightList_[(i * j - 1 + (i - 1) * (rows - j))]->getOutDirection(aux)) {
          if (flag) {       // All true, false only for variable weight
            switch (aux) {  // Position relative to node
              case 0:       // Up
                if (j == rows) {
                  link_[iLink].origin_ = (i * j - 1 + (i - 1) * (rows - j));
                  link_[iLink].destination_ = (i * 1 - 1 + (i - 1) * (rows - 1));
                  link_[iLink].constantWeight_ = 5;
                } else {
                  link_[iLink].origin_ = (i * j - 1 + (i - 1) * (rows - j));
                  link_[iLink].destination_ = (i * (j + 1) - 1 + (i - 1) * (rows - (j + 1)));
                  link_[iLink].constantWeight_ = 5;
                }
                break;
              case 1:  // Right
                if (i == columns) {
                  link_[iLink].origin_ = (i * j - 1 + (i - 1) * (rows - j));
                  link_[iLink].destination_ = (1 * j - 1);
                  link_[iLink].constantWeight_ = 5;
                } else {
                  link_[iLink].origin_ = (i * j - 1 + (i - 1) * (rows - j));
                  link_[iLink].destination_ = ((i + 1) * j - 1 + ((i + 1) - 1) * (rows - j));
                  link_[iLink].constantWeight_ = 5;
                }
                break;
              case 2:  // Down
                if (j == 1) {
                  link_[iLink].origin_ = (i * j - 1 + (i - 1) * (rows - j));
                  link_[iLink].destination_ = (i * rows - 1);
                  link_[iLink].constantWeight_ = 5;
                } else {
                  link_[iLink].origin_ = (i * j - 1 + (i - 1) * (rows - j));
                  link_[iLink].destination_ = (i * (j - 1) - 1 + (i - 1) * (rows - (j - 1)));
                  link_[iLink].constantWeight_ = 5;
                }
                break;
              case 3:  // Left
                if (i == 1) {
                  link_[iLink].origin_ = (i * j - 1 + (i - 1) * (rows - j));
                  link_[iLink].destination_ = (columns * j - 1 + (columns - 1) * (rows - j));
                  link_[iLink].constantWeight_ = 5;
                } else {
                  link_[iLink].origin_ = (i * j - 1 + (i - 1) * (rows - j));
                  link_[iLink].destination_ = ((i - 1) * j - 1 + ((i - 1) - 1) * (rows - j));
                  link_[iLink].constantWeight_ = 5;
                }
                break;
            }
            // link_[iLink].index_=Interaction::associateLinkIndex(streets_,trafficlights_.trafficlightList_[(i*j-1+(i-1)*(rows_-j))],aux,rows_,columns_,true);
          }
          link_[iLink].variableWeight_ = Interaction::associateLinkIndex(
              streets, trafficlights.trafficlightList_[(i * j - 1 + (i - 1) * (rows - j))], aux, rows, columns, false);
          // cout<<"\nse asignan los valores "<<link_[iLink].origin_<<" "<<link_[iLink].destination_<<" \tindice
          // "<<link_[iLink].index_<<" \tpeso fijo "<<link_[iLink].constantWeight_<<"   peso variable
          // "<<link_[iLink].variableWeight_<<" a la link_ "<<iLink;
          iLink++;
          // aux++;  //TODO que hace esto aqui
        }
      }
    }
  }
  nLinks_ = iLink;
  cout << "\nValues assigned to " << nLinks_ << " links\n";
  aux = 0;
}
int function(int i, int j, int columns) {
  return (i * j - 1 + (i - 1) * (columns - j));
}

void Graph::drawLink(TrafficlightList trafficlights, int i, int j, int turning) {  // TODO quitar
  glPushMatrix();
  glColor3f(1, 0, 0);
  glDisable(GL_LIGHTING);
  glTranslatef(trafficlights.trafficlightList_[i]->getPosition().x, trafficlights.trafficlightList_[i]->getPosition().y, 1);
  glRotatef(90 * turning, 0, 0, 1);
  glBegin(GL_POLYGON);  // triangle (direction)
  glVertex3d(0, 26, 0.5f);
  glVertex3d(-6, 0, 0.5f);
  glVertex3d(6, 0, 0.5f);
  glEnd();
  glRotatef(-90 * turning, 0, 0, 1);
  glTranslatef(-trafficlights.trafficlightList_[i]->getPosition().x, -trafficlights.trafficlightList_[i]->getPosition().y, -1);
  glPopMatrix();
}

// Count link number
int Graph::countDirections(StreetList streets) {
  int aux = 0;
  for (int i = 0; i < streets.number_; i++) {
    switch (streets.streetList_[i]->getType()) {
      case 5:
      case 8:
      case 9:
      case 12:
      case 13:
      case 14:
        if (streets.streetList_[i]->getPosition().x >= 0 && streets.streetList_[i]->getPosition().y >= 0)
          aux++;  // Both ways
      case 1:
      case 2:
      case 3:
      case 4:
      case 6:
      case 7:
      case 10:
      case 11:
        if (streets.streetList_[i]->getPosition().x >= 0 && streets.streetList_[i]->getPosition().y >= 0)
          aux++;  // One way
        break;
    }
  }
  return aux;
}
void Graph::adjacencyList(int n, bool conj) {  // n es nLinks(conj) TODO no se hacer con esto conj/////////
  static bool flag1 = false, flag2 = false;
  int i = 0, aux = 0;
  if (!conj) {  /////////////////////////////////
    if (!flag1) {
      for (int aux = 0; aux < nNodes_; aux++) {  // Same adjacent nodes and weight of these nodes
        node_[aux].adjacentNode_ = new int[node_[aux].numberAdjacentNodes_];
        node_[aux].adjacentNodeWeight_ = new int[node_[aux].numberAdjacentNodes_];
      }
      flag1 = true;
    }
    node_[link_[i].getOrigin()].currentAdjacentNode_ = 0;
    for (i = 0; i < n; i++) {  // n=nLinks_ assign values
      node_[link_[i].getOrigin()].adjacentNode_[node_[link_[i].getOrigin()].currentAdjacentNode_] = 
        link_[i].getDestination(); // Assign value to adjacentNode
      node_[link_[i].getOrigin()].adjacentNodeWeight_[node_[link_[i].getOrigin()].currentAdjacentNode_] = 
        link_[i].getWeight();  // Assign weight to adjacentNode
      node_[link_[i].getOrigin()].currentAdjacentNode_++;
    }
    for (int k = 0; k < nNodes_; k++) {
      cout << "\nNode " << k << " sees " << node_[k].numberAdjacentNodes_ << " nodes: ";
      for (int x = 0; x < node_[k].numberAdjacentNodes_; x++) {
        cout << node_[k].adjacentNode_[x] << "  " << node_[k].adjacentNodeWeight_[x] << "    ";
      }
    }
    for (int i = 0; i < nLinks_; i++) {
      cout << "\nLink " << i << " between nodes: " << link_[i].getOrigin() << " and "
        << link_[i].getDestination();
    }
  } else {  ///////////////////////////////////////
    if (!flag2) {
      flag2 = true;
    } else {
      for (int aux = 0; aux < nConjugateNodes_; aux++) {  // Delete information before iteration
        delete[] conjugateNode_[aux].adjacentNode_;
        delete[] conjugateNode_[aux].adjacentNodeWeight_;
        conjugateNode_[aux].currentAdjacentNode_ = 0;
      }
    }
    for (int aux = 0; aux < nConjugateNodes_; aux++) {
      conjugateNode_[aux].adjacentNode_ = new int[conjugateNode_[aux].numberAdjacentNodes_];
      conjugateNode_[aux].adjacentNodeWeight_ = new int[conjugateNode_[aux].numberAdjacentNodes_];
    }
    for (i = 0; i < n; i++) {  // n=nConjugateLinks_ assign values
      conjugateNode_[conjugateLink_[i].getOrigin()].adjacentNode_[conjugateNode_[conjugateLink_[i].getOrigin()].currentAdjacentNode_] =
          conjugateLink_[i].getDestination();  // assign value a adjacentNode_

      conjugateNode_[conjugateLink_[i].getOrigin()].adjacentNodeWeight_[conjugateNode_[conjugateLink_[i].getOrigin()].currentAdjacentNode_] =
          conjugateLink_[i].getWeight();  // asignar peso a adjacentNode_

      conjugateNode_[conjugateLink_[i].getOrigin()].currentAdjacentNode_++;
    }
    for (int k = 0; k < nConjugateNodes_; k++) {
      cout << "\nConjugateNode " << k << " sees " << conjugateNode_[k].numberAdjacentNodes_ << " nodes: ";
      for (int x = 0; x < conjugateNode_[k].numberAdjacentNodes_; x++) {
        cout << conjugateNode_[k].adjacentNode_[x] << "  " << conjugateNode_[k].adjacentNodeWeight_[x] << "    ";
      }
    }
  }
}

void Graph::calculateConjugateWeight(bool flag) {  // Flag to calculate only variable weight
  if (flag) {
    // Constant weight for out crossroad: 2 if there are a lot of exits, 4 normal exits, 6 just a few exits
    int aux = 0;
    for (int i = 0; i < nConjugateNodes_; i++) {
      switch (conjugateNode_[i].numberAdjacentNodes_) {  // heuristic
        case 1:
          conjugateLink_[aux++].constantWeight_ = 2;
          break;
        case 2:
          conjugateLink_[aux++].constantWeight_ = 1;
          conjugateLink_[aux++].constantWeight_ = 1;
          break;
        case 3:
          conjugateLink_[aux++].constantWeight_ = 1;
          conjugateLink_[aux++].constantWeight_ = 1;
          conjugateLink_[aux++].constantWeight_ = 1;
          break;
        default:
          cout << "\n\n\tERROR WHEN CALCULATING WEIGHT BASED ON EXITS";
          int x;
          cin >> x;
          break;
      }
    }
    if (aux == nConjugateLinks_) cout << "\nConstant weights have been assigned";
    if (aux > nConjugateLinks_) {
      cout << "\nERROR when constant weights are being assigned, too much weights";
      int x;
      cin >> x;
    }
  }
  // Variable weight calculated using street occupancy, is the same than destiny node weight
  for (int i = 0; i < nConjugateLinks_; i++) {
    conjugateLink_[i].variableWeight_ = conjugateNode_[conjugateLink_[i].destination_].occupancy_ / 2;  // to adjust weights
  }
}

Node::Node() {
  id_ = index_ = occupancy_ = 0;
}
Node::~Node() {
  id_ = index_ = NULL;
  dijkstraVariable_.visited_ = false;
  // delete[] adjacentNode_;
}
Link::Link() {
  origin_ = destination_ = index_ = constantWeight_ = variableWeight_ = 0;
}
Link::~Link() {
  origin_ = destination_ = index_ = constantWeight_ = variableWeight_ = NULL;
}

void Graph::asociateStreetNode(StreetList streets) {
  int iNodes = 0;
  for (int i = 0; i < streets.number_; i++)
    if (streets.streetList_[i]->getPosition().x >= 0 && streets.streetList_[i]->getPosition().y >= 0) switch (streets.streetList_[i]->getType()) {
        case 5:
        case 8:
        case 9:
        case 12:
        case 13:
        case 14:  // Both ways
          streets.streetList_[i]->setNode(iNodes, 0);
          streets.streetList_[i]->setNode(iNodes + 1, 1);
          iNodes += 2;
          break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 6:
        case 7:
        case 10:
        case 11:  // One way
          streets.streetList_[i]->setNode(iNodes, 0);
          streets.streetList_[i]->setNode(iNodes, 1);
          iNodes++;
          break;
      }
  if (iNodes >= nConjugateLinks_) {
    cout << "\nERROR in street-node association\t\t" << iNodes << "\n";
    int x;
    cin >> x;
  }
  else
    cout << "\nThere are " << iNodes << " nodes associated with " << streets.number_ << " streets\n";
}

void Graph::asociateStreetLink(StreetList streets, TrafficlightList trafficlights) {  // TODO revisar por que se le pasan calles
  int iLinks = 0;
  for (int i = 0; i < trafficlights.number_; i++) {
    for (int j = 0; j < 4; j++) {
      if (trafficlights.trafficlightList_[i]->getOutDirection(j)) {
        link_[iLinks].setIndex(trafficlights.trafficlightList_[i]->getCID(j));
        cout << "Link " << iLinks << " associated with street " << link_[iLinks].getIndex() << endl;
        iLinks++;
      }
    }
  }
}

void Graph::resetGraph() {
  for (int i = 0; i < nNodes_; i++) {
    node_[i].dijkstraVariable_.setDistance(0);
    node_[i].dijkstraVariable_.setID(0);
    node_[i].dijkstraVariable_.setIteration(0);
    node_[i].dijkstraVariable_.setVisited(false);
  }
  for (int i = 0; i < nConjugateNodes_; i++) {
    conjugateNode_[i].dijkstraVariable_.setDistance(0);
    conjugateNode_[i].dijkstraVariable_.setID(0);
    conjugateNode_[i].dijkstraVariable_.setIteration(0);
    conjugateNode_[i].dijkstraVariable_.setVisited(false);
  }
}

// Traffic light graph 
void Graph::buildGraph(StreetList& streetList, CarList& carList, 
                      TrafficlightList& trafficlightList, int index, int optimization) {
  // Initialize graph elements, there is a fixed graph for each traffic light used as a state machine, 
  //no arguments are needed
  nNodes_ = 4;
  nLinks_ = 4;
  node_ = new Node[nNodes_];
  link_ = new Link[nLinks_];

  // Associate nodes information
  for (int i = 0; i < nNodes_; i++) {
    node_[i].id_ = i;
  }

  // Associate links information
  for (int i = 0; i < nNodes_; i++) {
    link_[i].origin_ = node_[i].id_; // Node origin
    link_[i].destination_ = (i == nNodes_ - 1) ? (node_[0].id_) : (node_[i + 1].id_); // Node destination
  }
  cout << "\nNodes and links configurated";

  if (optimization) // Change traffic light weight if there is optimization
    calculateTrafficlightWeight(streetList, carList, trafficlightList.trafficlightList_[index], index);
  cout << "\nLink weights have been configurated in traffic lights graphs";
  for (int i = 0; i < nLinks_; i++)
    cout << "\nOrigin: " << link_[i].origin_ << " Destination: " << link_[i].destination_ << " and weight: " << link_[i].getWeight()
         << " assigned to link " << i;
}
//////////////////////////
// Calculate variable weight
void Graph::calculateTrafficlightWeight(StreetList& streetList, CarList& carList, Trafficlight* trafficlight, int index) {
  // The weight depends on occupancy and waiting time
  int verticalWeight = 0, horizontalWeight = 0;
  for (int i = 0; i < 4; i++) { // Initialize occupancy of streets of the traffic light
    Interaction::streetOccupancy(carList, streetList.streetList_[trafficlight->getCID(i)]);
  }
  for (int i = 0; i < 4; i++) { // Update vertical and horizontal weight with occupancy to compare
    if (!trafficlight->getOutDirection(i)) { // Just use entry streets
      if (i % 2) { // horizontal
        horizontalWeight += streetList.streetList_[trafficlight->getCID(i)]->getOccupancy();
      } else { // vertical
        verticalWeight += streetList.streetList_[trafficlight->getCID(i)]->getOccupancy();
      }
    }
  }
  // for(int i=0;i<nLinks_;i++){
  switch (trafficlight->getState()) {
    case 0: // vertical green -> vertical amber
      if (verticalWeight < horizontalWeight) {  //|| trafficlight->getTemp(1)>(400*50)){	
        // There are more horizontal cars or it has been in the same state for more than 50 seconds
        // state 1
        trafficlight->setTimer(1, trafficlight->getTimer(1), trafficlight->getTimer(2), trafficlight->getTimer(3));
        trafficlight->setTemp(0, 1);  // restart auxiliar counter
        trafficlight->setTemp(0, 0);  // set timer to 0 to enter the state machine
      } else {             // maintain the state
        // state 0
        trafficlight->setTimer((40 * 50), trafficlight->getTimer(1), trafficlight->getTimer(2), trafficlight->getTimer(3));
        if (horizontalWeight)  // add to the waiting timer if there are any cars waiting
          trafficlight->setTemp(trafficlight->getTemp(1) + 1, 1);
      }
      break;
    case 1: // vertical amber -> horizontal green
      // this does not depends on occupancy so there are no time changes
      // state 1
      trafficlight->setTimer((40 * 50), trafficlight->getTimer(1), (40 * 50), trafficlight->getTimer(3));
      break;
    case 2: // horizontal green -> horizontal amber
      if (verticalWeight > horizontalWeight) {  //|| trafficlight->getTemp(1)>(400*50)){	
        // There are more vertical cars or it has been in the same state for more than 50 seconds
        // state 3
        trafficlight->setTimer(trafficlight->getTimer(0), trafficlight->getTimer(1), 1, trafficlight->getTimer(3));
        trafficlight->setTemp(0, 1);  // restart auxiliar counter
        trafficlight->setTemp(0, 0);  // set timer to 0 to enter the state machine
      } else {             // maintain the state
        // state 2
        trafficlight->setTimer(trafficlight->getTimer(0), trafficlight->getTimer(1), (40 * 50), trafficlight->getTimer(3));
        if (verticalWeight)  // add to the waiting timer if there are any cars waiting
          trafficlight->setTemp(trafficlight->getTemp(1) + 1, 1);
      }
      break;
    case 3:  // ah->vv
      // this does not depends on occupancy so there are no time changes
      // state 3
      trafficlight->setTimer((40 * 50), trafficlight->getTimer(1), (40 * 50), trafficlight->getTimer(3));
      break;
    default:
      cout << "\nERROR TRAFFIC LIGHT LINK OUT OF RANGE";
      int x;
      cin >> x;
      break;
  }
  cout << "\n\n\tIndex: " << index << "  verticalWeight: " << verticalWeight << "  horizontalWeight: "
    << horizontalWeight << "   Temp: " << trafficlight->getTemp(1);
  //}
}
//////////////////////////////////////////
