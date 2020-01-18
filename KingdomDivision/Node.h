#ifndef KINGDOM_DIVISION_NODE_H
#define KINGDOM_DIVISION_NODE_H

#include <vector>

class Node
{
public:
    void addNeighbour(Node* neighbour);
    Node* getNeighbour(size_t i);
    size_t getNeighboursSize();
private:
    std::vector<Node*> neighbours;
};

#endif // KINGDOM_DIVISION_NODE_H
