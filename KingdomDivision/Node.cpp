#include "Node.h"
#include <vector>

void Node::addNeighbour(Node* neighbour)
{
	neighbours.push_back(neighbour);
}

Node* Node::getNeighbour(size_t i)
{
	return neighbours[i];
}

size_t Node::getNeighboursSize()
{
	return neighbours.size();
}