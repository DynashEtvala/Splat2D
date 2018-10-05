/*******************************************************************************************
*
*   raylib [core] example - basic window
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2016 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "Player.h"
#include "Shot.h"
#include "Controller.h"

int main()
{
	// Initialization
	//--------------------------------------------------------------------------------------
	int screenWidth = 800;
	int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	int playerCount = 1;
	Player** playerList = new Player*[playerCount];

	for (int i = 0; i < playerCount; i++)
	{
		playerList[i] = new Player();
	}

	int shotCount = 1;
	Shot** shotList = new Shot*[shotCount];

	Controller controller{playerList, playerCount, shotList, shotCount};

	SetTargetFPS(60);
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------

		for (int i = 0; i < playerCount; i++)
		{
			playerList[i]->Update(controller);
		}
		
		if (controller.playerChange)
		{
			delete[] playerList;
			playerList = controller.getPList();
			playerCount = controller.getPCount();
		}
		if (controller.shotChange)
		{
			delete[] shotList;
			shotList = controller.getSList();
			shotCount = controller.getSCount();
		}

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(DARKGRAY);

		for (int i = 0; i < playerCount; i++)
		{
			DrawCircle(playerList[i]->getCenter().x, playerList[i]->getCenter().y, playerList[i]->getRadius(), playerList[i]->color);
		}

		for (int i = 0; i < shotCount; i++)
		{
			DrawCircle(shotList[i]->getCenter().x, shotList[i]->getCenter().y, shotList[i]->getRadius(), shotList[i]->color);
		}

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}