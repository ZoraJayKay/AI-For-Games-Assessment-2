#pragma once
#include "Pathfinding.h"
#include <string>


// Use the same namespace as the one set up by the tutorial
namespace AIForGames {
	// Create a new class within the namespace to hold the map of nodes
	class NodeMap
	{
		// Map variables
		int m_width;
		int m_height;

		// The size of every separate 'cell' within the map
		float m_cellSize;

		// From the tute: "The Node** variable nodes is essentially a dynamically allocated one dimensional array of Node pointers."
		Node** m_nodes;

	public:
		// Default constructor
		NodeMap();

		// Destructor
		~NodeMap();

		void GetMapSize();

		// A function to set the start/end position of the node map depending on which mouse button is pressed
		Node* GetClosestNode(glm::vec2 worldPos);

		// A function for the purposes of setting up a node map according to a vector of strings, called 'asciiMap', and a given size for each node to be
		// From the tute: "In the Initialise function we will allocate this array to match the width and height of the map (determined by the vector of strings passed in) and fill it with either newly allocated Nodes or null pointers for each square on the grid."
		void Initialise(std::vector<std::string> asciiMap, int cellSize);

		// A function to return the Node* for a given pair of coordinates
		Node* GetNode(int x, int y);

		// A function for drawing the best path calculated by a Dijkstra search
		void DrawPath(std::vector<Node*> dijkstraPath);

		// A function to draw the map to the screen
		void Draw();

		void Print(std::vector<Node*> path);

		static std::vector<Node*> DijkstraSearch(Node* startNode, Node* endNode);
	};
}