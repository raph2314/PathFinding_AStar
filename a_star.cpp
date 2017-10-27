#include <iostream>
#include <math.h>
#include <sstream>
#include <intrin.h>

#include "a_star.hpp"

using namespace std;

// Calculates and returns the fcost
int AStar::getFCost(Node* currentNode) {
	return currentNode->gCost + currentNode->hCost;
}

// Computes the Euclidean distance between two nodes and scales it by 10 to avoid floats
int AStar::getHDistance(Node* A, Node* B) {
	return sqrt(pow(abs(A->posX - B->posX), 2) + pow(abs(A->posY - B->posY), 2)) * 10;
}

// Iterates through the node's parents and creates a list of the shortest path to take
list<Node*> AStar::retracePath (Node* startNode, Node* endNode) {
	list<Node*> path;	
	Node* currentNode = endNode;

	while (currentNode != startNode) {
		path.push_front(currentNode);
		currentNode = currentNode->parent;
	}
	return path;
}

// For the current node, cnode, discovers all walkable neighbours, and adds them to the neighbours list
list<Node*> AStar::getNeighbours(Node* cnode, Node* grid[Y_LENGTH][X_LENGTH]) {
	list<Node*> neighbours;	
	
	// TODO Step3. Add walkable neighbours to the neighbours list
	for(int x = cnode->posX-1; x <= cnode->posX+1 ;x++){
		for(int y = cnode->posY-1; y <= cnode->posY+1; y++){
			if(y < Y_LENGTH && y >= 0 && x < X_LENGTH && x >= 0 && grid[y][x]->walkable)
			neighbours.push_back(grid[y][x]);
		}
	}

	return neighbours;
}

// Outputs the discovered path as a string
string AStar::getPathString(list<Node*> path) {
	stringstream outputStream;
	for (list<Node*>::iterator it = path.begin(); it != path.end(); ++it) {
		outputStream << "(" << ((*it))->posX << "," << ((*it))->posY << ")" << endl;
	}
	return outputStream.str();
}

// Outputs path distance
int AStar::getPathDistance(list<Node*> path) {
	return path.size() > 0 ? path.back() -> fCost : 0;
}

// Finds shortest path between startNode and endNode using A* algorithm
list<Node*> AStar::findPath(Node* world[Y_LENGTH][X_LENGTH], Node* startNode, Node* endNode) {
	list<Node*> openSet; // list of nodes not yet evaluated
	list<Node*> closedSet; // list of nodes already evaluated
	Node* currentNode;

	startNode->gCost = 0;
	startNode->hCost = getHDistance(startNode, endNode);
	startNode->fCost = startNode->hCost;
	
	openSet.push_back(startNode); // insert the starting node at the beginning of the open set
	while(!openSet.empty()) {	
		//break; // REMOVE this line once the following code has been implemented
		
		// TODO Step4. Find a node in the openSet that has the smallest fCost
		currentNode = openSet.front();
		int smallestF = getFCost(openSet.front());

		list<Node*>::iterator listIterator = openSet.begin();
		for(list<Node*>::iterator it = openSet.begin(); it != openSet.end(); it++){
			if(getFCost((*it))< smallestF){
				smallestF = getFCost((*it));
				currentNode = (*it);
			}
			else if(getFCost((*it)) == smallestF){
				if((*it)->hCost < currentNode->hCost){
					currentNode = (*it);
				}
			}
		}
		// If there is a conflict, select the node with the smaller hCost
		// Use <list> iterator to iterate through the list; see sample iterator code below

		// TODO Step5. Remove the found node from the open list and insert it into closed
		openSet.remove(currentNode);
		closedSet.push_back(currentNode);

		
		
		// TODO Step6. Get a list of walkable neighbours for the current node		
		list<Node*> neighbours = getNeighbours(currentNode, world);

//        vector<Node*> debugOpen{std::begin(neighbours), std::end(neighbours)};
//        std::cout << "OPEN SET ---------------------------------\n";
//        for(int i = 0; i < debugOpen.size(); i ++){
//            std::cout << "(" << debugOpen[i]->posX << "," << debugOpen[i]->posY << "," <<
//                      debugOpen[i]->walkable << ")" << std::endl;
//        }
		
		// TODO Step7. Iterate through the neighbours list and add matching neighbours to the open list		
		for(list<Node*>::iterator it = neighbours.begin(); it != neighbours.end(); it++) {
		
			// Step7.1. Check if the current neighbour is already in the closed list; if it is, skip it
            bool skip = false;
            for(list<Node*>::iterator it2 = closedSet.begin(); it2 != closedSet.end(); it2++){
                if((*it2)->posX == (*it)->posX && (*it2)->posY == (*it)->posY ){
                    skip = true;
                    break;
                }
            }
            if(skip) continue;
            Node* pushNode = (*it);
            openSet.push_front(pushNode);

			// Step7.2. Compute gCost from the start node for the curent neighbour
			// If that cost is less than previously computed gCost, update the neighbour's parent to the current node, and 
			// update gCost, hCost, and fCost values for the neighbour to match the current node
			// Use getHDistance to get the cost from the current node to the current neighour

            if((*it)->parent == NULL || (*it)->gCost > getHDistance(currentNode, (*it)) + currentNode->gCost){

                (*it)->parent = currentNode;
                (*it)->hCost = getHDistance((*it), endNode);
                (*it)->gCost = getHDistance(currentNode, (*it)) + currentNode->gCost;
                (*it)->fCost = getFCost((*it));
            }

 		}
        // TODO Step8. Check if the current node is the end node; if it is, return the retraced path from start to end
        if (currentNode->posX == endNode->posX && currentNode->posY == endNode->posY){
            return retracePath(startNode, currentNode);
        }

	}
	
	list<Node*> empty;
	return empty;
}

