/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/


/*
AIE Pathfinding tutorial #1
In this tutorial series we’ll look at creating an implementation of Djikstra’s Shortest Path algorithm.

There are three tutorials:

Creating a Node Graph
Calculating a Path
Creating a Pathing Agent

*/


#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#include "raygui.h"
#include "Pathfinding.h"
#include <string>;
//#include "memory.h"
#include "NodeMap.h"
#include <iostream>
#include "PathAgent.h"

using namespace std;
using namespace AIForGames;

int main(int argc, char* argv[])
{
	// Initialization
	//--------------------------------------------------------------------------------------
	int screenWidth = 800;
	int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "Zora Jane Kerr: Artificial Intelligence for Games (Assessment 2 - Other Pathfinding Algorithms [Dijkstra's algorithm]) AIE, 2023 (student year 1)");

	SetTargetFPS(60);
	//--------------------------------------------------------------------------------------

	// ZORA: Tutorial requests making an ASCII art map to visualise the node map
	// Create a vector of strings, with each string representing a row line of the map.
	// "We can use a code such as 0 = solid wall, 1 =  navigable space, and set up anything from a simple test shape to a complex maze for pathfinding to take place in"


	// 12x8 grid of chars denoting whether or not a cell is navigable (1) or impassable (0) /// Original map from tutorial
	vector<string> asciiMap;
	//asciiMap.push_back("000000000000");     // row 1
	//asciiMap.push_back("010111011100");     // row 2
	//asciiMap.push_back("010101110110");     // row 3
	//asciiMap.push_back("010100000000");     // row 4
	//asciiMap.push_back("010111111110");     // row 5
	//asciiMap.push_back("010000001000");     // row 6
	//asciiMap.push_back("011111111110");     // row 7
	//asciiMap.push_back("000000000000");     // row 8

	// 14x8 grid of chars denoting whether or not a cell is navigable (1) or impassable (0) /// ALTERNATE MAP
	asciiMap.push_back("00000000000000");     // row 1
	asciiMap.push_back("01011101110000");     // row 2
	asciiMap.push_back("01010111011110");     // row 3
	asciiMap.push_back("01010000000010");     // row 4
	asciiMap.push_back("01011111111010");     // row 5
	asciiMap.push_back("01000000100010");     // row 6
	asciiMap.push_back("01111111111110");     // row 7
	asciiMap.push_back("00000000000000");     // row 8

	// Create a NodeMap class with a width, height and cell size, ie the spacing in pixels between consecutive squares in the grid. We’ll give it a function to initialize its data from the ASCII map declared above.
	NodeMap* map = new NodeMap();
	map->Initialise(asciiMap, 50);

	// Set the starting node for the Dijkstra search equal to the Node* in column index 1, row index 1 (in the ascii map)
	Node* start = map->GetNode(1, 1);
	// Set the target point (the end destination) equal to the Node* in column index 10, row index 2
	Node* end = map->GetNode(10, 2);
	// Find the vector of nodes that constitute the Dijkstra path between (1, 1) and (10, 2)
	vector<Node*> nodeMapPath = map->DijkstraSearch(start, end);
	cout << "The Dijkstra path consists of " << nodeMapPath.size() << " nodes." << endl;

	PathAgent agent;
	agent.SetNode(start);
	agent.SetSpeed(64);

	// map->Print(nodeMapPath);
	
	// Time at commencement of pathfinding
	float time = (float)GetTime();
	float deltaTime;

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Time at moment of update
		float fTime = (float)GetTime();

		// Change in time since last update
		deltaTime = fTime - time;

		// Reset timer for this update
		time = fTime;

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(BLACK);

		map->Draw();

		if (IsMouseButtonPressed(0)) {
			Vector2 mousePos = GetMousePosition();
			end = map->GetClosestNode(glm::vec2(mousePos.x, mousePos.y));
			// On mouse click, set the agent's current node = the nearest node to its current world position
			agent.GoToNode(end);
		}

		// ----- This code is just for demonstrating moving the path's origin -----
		// If left mouse is clicked...
		/*if (IsMouseButtonPressed(0)) {
			Vector2 mousePos = GetMousePosition();
			start = map->GetClosestNode(glm::vec2(mousePos.x, mousePos.y));
			nodeMapPath = map->DijkstraSearch(start, end);
			nodeMapPath = NodeMap::DijkstraSearch(start, end);
		}*/

		// ----- This code is just for demonstrating moving the path's destination -----
		//// If right mouse is clicked...
		//if (IsMouseButtonPressed(1)) {
		//	Vector2 mousePos = GetMousePosition();
		//	end = map->GetClosestNode(glm::vec2(mousePos.x, mousePos.y));
		//	//nodeMapPath = map->DijkstraSearch(start, end);
		//	nodeMapPath = NodeMap::DijkstraSearch(start, end);
		//}

		map->DrawPath(agent.GetPath());
		agent.Update(deltaTime);
		agent.Draw();

		EndDrawing();

		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	delete map;
	map = nullptr;	

	return 0;
}