#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace AIForGames
{
    // ZORA: Default struct
    struct Node;

    // ZORA: Default struct for an edge of a directed flow graph (target node and cost to get there)
    struct Edge {
        Node* targetNode;
        float cost;

        // Default constructor
        Edge();

        // Overloaded constructor
        // "provide an explicit default constructor to replace the implicit one that gets created if you don’t specify any constructors."
        // Define an overloaded constructor with the arguments _target and _cost, and assign _target to 'target', and _cost' to cost
        Edge(Node* _target, float _cost) : targetNode(_target), cost(_cost) {}

        // Default destructor
        ~Edge();
    };


    // ZORA: Overloaded struct wth 2D position and vector of connecting edges
    struct Node {
        // Node variables 
        glm::vec2 position;
        std::vector<Edge> connections;
        int gScore;
        Node* previousNode;

        // Default constructor
        Node();

        // ZORA: Struct overloaded constructor to take floats instead of single vec2
        Node(float x, float y);  

        // Default destructor
        ~Node();

        // A function to add an Edge connecting to a given Node with a given cost
        void ConnectTo(Node* other, float cost);
    };
}
