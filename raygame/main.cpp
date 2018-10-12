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
#include "GlobalVars.h"
#include "FloorTile.h"
#include "Math.h"


int main()
{
	// Initialization
	//--------------------------------------------------------------------------------------
	
	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
	
	FloorTile*** floorTiles = new FloorTile**[screenHeight];

	Vector2 scores;
	int scoretimer = 0;

	for (int i = 0; i < screenHeight; i++)
	{
		floorTiles[i] = new FloorTile*[screenWidth];
	}

	for (int i = 0; i < screenHeight; i++)
	{
		for (int j = 0; j < screenWidth; j++)
		{
			floorTiles[i][j] = new FloorTile(BLANK);
		}
	}

	Player* playerList[maxPlayerCount];

	for (int i = 0; i < maxPlayerCount; i++)
	{
		if (i % 2 == 0)
		{
			playerList[i] = new Player({ 50.0f, i * 200.0f + 50.0f }, BASE_GUN, team1Color);
		}
		else
		{
			playerList[i] = new Player({ 50.0f, i * 200.0f + 50.0f }, BASE_GUN, team2Color);
		}
	}

	Rectangle obsticles[10];
	int obsticleCount = 0;

	Rectangle pits[10];
	int pitCount = 0;

	Shot* shotList[maxShotCount];

	for (int i = 0; i < maxShotCount; i++)
	{
		shotList[i] = new Shot();
	}

	Controller controller{playerList, shotList};

	SetTargetFPS(60);
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------

		for (int i = 0; i < maxPlayerCount; i++)
		{
			playerList[i]->Update(&controller);
		}
		
		for (int i = 0; i < maxShotCount; i++)
		{
			if (shotList[i]->active)
			{
				shotList[i]->Update(&controller, floorTiles);
			}
		}

		if (scoretimer >= 10)
		{
			scores = controller.getScores(floorTiles);
			scoretimer = 0;
		}
		else
		{
			scoretimer++;
		}

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(DARKGRAY);

		for (int i = 0; i < screenHeight; i++)
		{
			for (int j = 0; j < screenWidth; j++)
			{
				if (floorTiles[i][j]->color != unclaimed)
				{
					DrawPixel(j, i, floorTiles[i][j]->color);
				}
			}
		}

		for (int i = 0; i < pitCount; i++)
		{
			DrawRectangle(pits[i].x, pits[i].y, pits[i].width, pits[i].height, DARKBLUE);
		}

		for (int i = 0; i < maxPlayerCount; i++)
		{
			DrawCircle(playerList[i]->getCenter().x, playerList[i]->getCenter().y, playerList[i]->getRadius(), BLACK);
			DrawCircle(playerList[i]->getCenter().x, playerList[i]->getCenter().y, playerList[i]->getRadius() - 1, playerList[i]->color);
		}

		for (int i = 0; i < maxShotCount; i++)
		{
			if (shotList[i]->active)
			{
				DrawCircle(shotList[i]->getCenter().x, shotList[i]->getCenter().y, shotList[i]->getRadius(), BLACK);
				DrawCircle(shotList[i]->getCenter().x, shotList[i]->getCenter().y, shotList[i]->getRadius() - 1, shotList[i]->color);
			}
		}

		for (int i = 0; i < obsticleCount; i++)
		{
			DrawRectangle(obsticles[i].x, obsticles[i].y, obsticles[i].width, obsticles[i].height, LIGHTGRAY);
		}

		DrawRectangle(0, 0, screenWidth, 20, team2Color);
		DrawRectangle(0, 0, (scores.x / (scores.x + scores.y)) * screenWidth, 20, team1Color);

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	for (int i = 0; i < screenHeight; i++)
	{
		for (int j = 0; j < screenWidth; j++)
		{
			delete floorTiles[i][j];
		}
	}
	for (int i = 0; i < maxPlayerCount; i++)
	{
		delete playerList[i];
	}
	for (int i = 0; i < maxShotCount; i++)
	{
		delete shotList[i];
	}
	//--------------------------------------------------------------------------------------
	
	return 0;
}