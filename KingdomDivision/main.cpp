#include "Node.h"
#include <iostream>
#include <fstream>
#include <vector>

typedef unsigned long long ULL;

std::vector<Node*> nodes; // Vector of all nodes
Node* root;               // Pointer to the root node

// Calculate combinations for a subtree with given root 'r', parent of which is 'parent'.
std::pair<ULL, ULL> calcForSubtree(Node* r, Node* parent);
// Recurrent function to add up multiplications of all possibilities for all children (see below).
ULL mul(ULL offset, std::vector<ULL> combinationsWhenChildWhite, std::vector<ULL> combinationsWhenChildBlack);

int main(int argc, char* argv[])
{
	//std::ifstream file;
	//std::string filepath;
	size_t treeSize;

	if (argc < 3 || strcmp(argv[1], "-m") != 0)
	{
		std::cout << "usage: kingdom-division -m MODE_NUM [params]" << std::endl;
		return 1;
	}
	
	char mode = *argv[2];

	switch (mode)
	{
	case '1':
		/* Read the tree from the standard input */
		std::cin >> treeSize;

		for (size_t i = 0; i < treeSize; ++i)
		{
			nodes.push_back(new Node);
		}

		// Read connections
		size_t first, second;
		while (true)
		{
			std::cin >> first;
			if (std::cin.eof()) break;
			std::cin >> second;

			if (first > treeSize || second > treeSize)
			{
				std::cerr << "Nodes numbers must be between 1 and " << treeSize << std::endl;
				return 1;
			}
			nodes[first - 1]->addNeighbour(nodes[second - 1]);
			nodes[second - 1]->addNeighbour(nodes[first - 1]);
		}
		break;
	case 2:
		if (strcmp(argv[3], "-n") != 0)
		{
			std::cerr << "usage: kingdom-division -m 2 -n N_VERTICES" << std::endl;
			return 1;
		}

		treeSize = static_cast<int>(*argv[4]);

		//generateTree(treeSize);
		break;
	case 3:
		break;
	default:
		std::cerr << "Mode number must be 1, 2 or 3" << std::endl;
		return 1;
	}

	std::cout << "Tree size: " << treeSize << std::endl;
	return 0;
//	file.open(filepath);
	//if (!file)
	//{
		std::cerr << "Unable to open the file." << std::endl;
		return 1;
	//}

	// Read the number of nodes
	//size_t treeSize;
	//file >> treeSize;


	if (treeSize == 1)
	{
		std::cout << 2 << std::endl;
		return 0;
	}
	if (treeSize == 2)
	{
		if (nodes[0]->getNeighboursSize() == 1)
		{
			std::cout << 2 << std::endl;
			return 0;
		}
		else
		{
			std::cerr << "A tree is connected by definition!" << std::endl;
			return 1;
		}
	}
	// Choose the root. Any non-leaf can be the root.
	size_t i = 0;
	do {
		if (i == treeSize)
		{
			// We didn't find any non-leaf.
			std::cerr << "A tree is connected by definition!" << std::endl;
			return 1;
		}
		root = nodes[i++];
	} while (root->getNeighboursSize() <= 1);

	// Calculate and display the number of possible combinations
	std::pair <ULL, ULL> result = calcForSubtree(root, nullptr);
	std::cout << 2 * result.second << std::endl; // We only count cases when at least one child of the whole tree root
									             // has the same color as the root.

	// Clean up
	for (auto node : nodes)
	{
		delete node;
	}

	return 0;
}

// Calculate possible combinations for a subtree with given root 'r', parent of which is 'parent'.
// We calculate assuming that r is white. This is why at the very root of the whole tree we will have to multiply
// all combinations by two.
std::pair<ULL, ULL> calcForSubtree(Node* r, Node* parent)
{
	ULL returnAllChildrenBlack;    // Number of combinations for this subtree given that all children of r are black
	ULL returnNotAllChildrenBlack; // Number of combinations for this subtreee given that at least 1 child of r is white

	std::vector <ULL> combinationsWhenChildBlack; // Numbers of combinations for every child's subtree given that the child is black
	std::vector <ULL> combinationsWhenChildWhite; // Numbers of combinations for every child's subtree given that the child is white

	// Calculate possibilities for all children
	for (size_t i = 0; i < r->getNeighboursSize(); ++i)
	{
		// The only neighbour which is not a child is the parent - omit it.
		if (r->getNeighbour(i) == parent)
		{
			continue;
		}

		// If the child is a leaf
		if (r->getNeighbour(i)->getNeighboursSize() == (size_t) 1)
		{
			// It must have the same color as its parent (i.e. white, assuming that r is white).
			combinationsWhenChildBlack.push_back(0);
			combinationsWhenChildWhite.push_back(1);
		}
		// If the child is not a leaf
		else
		{
			// Calculate in the deeper subtree
			std::pair<ULL, ULL> result = calcForSubtree(r->getNeighbour(i), r);

			// Case: r white, child black
			// The children of the child can't be all in different color than the child.
			// This is why we multiply 1 (just for transparentness - one combination: r white, child black)
			// by number of combinations when not all children of the child are black, assuming that the child is white.
			// In our case we assume the child is black, but it makes no difference - number of combinations is the same
			// (all colors are flipped).
			combinationsWhenChildBlack.push_back(1 * result.second);

			// Case: r white, child white
			// The children of the child can be all in different color than the child, but they don't have to.
			// This is why we multiply 1 (just for transparentness - one combination: r white, child white)
			// by all possible combinations for the child's subtree: all its children are black + not all are black.
			combinationsWhenChildWhite.push_back(1 * (result.first + result.second));
		}
	}

	// Calculate combinations when all children are black.
	returnAllChildrenBlack = 1;
	for (ULL i : combinationsWhenChildBlack)
	{
		returnAllChildrenBlack *= i;
	}

	// Calculate combinations when at least one child is white.
	// For all possible choices of color for every child multiply possibilities for them
	// and at the end subtract the case when all children are black.
	returnNotAllChildrenBlack = combinationsWhenChildWhite[0] * mul(1, combinationsWhenChildWhite, combinationsWhenChildBlack)
		+ combinationsWhenChildBlack[0] * mul(1, combinationsWhenChildWhite, combinationsWhenChildBlack);
	returnNotAllChildrenBlack -= returnAllChildrenBlack;

	return std::pair<ULL, ULL>(returnAllChildrenBlack, returnNotAllChildrenBlack);
}

// Recurrent function to add up multiplications of all possibilites for all children.
// Example: if we have 3 children, each can be black or white and we want to multiply all posibilities
// (and add them). We will perform 2^3 multiplications.
ULL mul(ULL offset, std::vector<ULL> combinationsWhenChildWhite, std::vector<ULL> combinationsWhenChildBlack)
{
	if (offset == combinationsWhenChildWhite.size())
	{
		return 1;
	}
	return (combinationsWhenChildWhite[offset] * mul(offset + 1, combinationsWhenChildWhite, combinationsWhenChildBlack) 
	          + combinationsWhenChildBlack[offset] * mul(offset + 1, combinationsWhenChildWhite, combinationsWhenChildBlack));
}
