#include "Pathfinding.h"

namespace AIForGames {
	// 
	Node::Node() {};

	// Overloaded struct constructor
	Node::Node(float x, float y) {
		position.x = x;
		position.y = y;
		previousNode = nullptr;
		gScore = 0;
	};

	// Default destructor
	Node::~Node() {};

	void Node::ConnectTo(Node* other, float cost) {
		connections.push_back(Edge(other, cost));
	};


	// Default constructor
	Edge::Edge() {
		targetNode = nullptr;
		cost = 0;
	};

	// Default destructor
	Edge::~Edge() {};
}