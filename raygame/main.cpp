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
#include "SpawnPad.h"


int main()
{
	// Initialization
	//--------------------------------------------------------------------------------------

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	FloorTile*** floorTiles = new FloorTile**[screenHeight];

	float gameTimer = 0;

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

	SpawnPad pads[2];

	pads[0] = SpawnPad{ 25, 25, team1Color };
	pads[1] = SpawnPad{ 1175, 575, team2Color };

	Player* playerList[maxPlayerCount];

	for (int i = 0; i < maxPlayerCount; i++)
	{
		if (i % 2 == 0)
		{
			playerList[i] = new Player(pads[0].spawnSpaces[i / 2], BASE_GUN, team1Color, team2Color, i);
		}
		else
		{
			playerList[i] = new Player(pads[1].spawnSpaces[3 - (i / 2)], BASE_GUN, team2Color, team1Color, i);
		}
	}

	Rectangle walls[maxObsticles];
	int wallCount = 6;

	walls[0] = { -20, -20, screenWidth + 40, 20 };
	walls[1] = { -20, -20, 20, screenHeight + 40 };
	walls[2] = { screenWidth + 1, -20, 20, screenHeight + 40 };
	walls[3] = { -20, screenHeight - 100, screenWidth + 40, 100 };
	walls[4] = { 400, 200, 100, 300 };
	walls[5] = { 800, 200, 100, 300 };

	Rectangle pits[maxObsticles];
	int pitCount = 3;

	pits[0] = { 500, 300, 300, 100 };
	pits[1] = { 600, -100, 100, 300 };
	pits[2] = { 600, 500, 100, 200 };

	Shot* shotList[maxShotCount];

	for (int i = 0; i < maxShotCount; i++)
	{
		shotList[i] = new Shot();
	}

	Controller controller{ playerList, shotList };

	SetTargetFPS(60);
	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------
		if (gameTimer <= matchTime)
		{
			for (int i = 0; i < maxPlayerCount; i++)
			{
				playerList[i]->Update(&controller, floorTiles, pads, walls, pits);
			}

			gameTimer += GetFrameTime();
		}

		for (int i = 0; i < maxShotCount; i++)
		{
			if (shotList[i]->active)
			{
				shotList[i]->Update(&controller, floorTiles, walls, pits, playerList);
			}
		}

		controller.paintFloor(pads[0].getCenter(), pads[0].getRadius() - 1, team1Color, floorTiles);
		controller.paintFloor(pads[1].getCenter(), pads[1].getRadius() - 1, team2Color, floorTiles);

		controller.clearFromObsticle(floorTiles, pits);
		controller.clearFromObsticle(floorTiles, walls);

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
			Vector2 shadowpoints[6] =
			{
				{ pits[i].x, pits[i].y },
				{ pits[i].x, pits[i].y + pits[i].height },
				{ pits[i].x + shadowLength, pits[i].y + pits[i].height },
				{ pits[i].x + shadowLength, pits[i].y + shadowLength },
				{ pits[i].x + pits[i].width, pits[i].y + shadowLength },
				{ pits[i].x + pits[i].width, pits[i].y },
			};
			DrawRectangle(pits[i].x, pits[i].y, pits[i].width, pits[i].height, pitColor);
			DrawPolyEx(shadowpoints, 6, Color{ 0, 0, 0, shadowIntensity });
		}

		pads[0].Draw();
		pads[1].Draw();

		for (int i = 0; i < maxPlayerCount; i++)
		{
			playerList[i]->Draw();
		}

		for (int i = 0; i < maxShotCount; i++)
		{
			if (shotList[i]->active)
			{
				shotList[i]->Draw();
			}
		}

		for (int i = 3; i < wallCount; i++)
		{
			Vector2 shadowpoints[6] =
			{
				Vector2{ walls[i].x + walls[i].width + shadowLength, walls[i].y + walls[i].height + shadowLength },
				Vector2{ walls[i].x + walls[i].width + shadowLength, walls[i].y + shadowLength },
				Vector2{ walls[i].x + walls[i].width, walls[i].y },
				Vector2{ walls[i].x + walls[i].width, walls[i].y + walls[i].height },
				Vector2{ walls[i].x, walls[i].y + walls[i].height },
				Vector2{ walls[i].x + shadowLength, walls[i].y + walls[i].height + shadowLength },
			};
			DrawPolyEx(shadowpoints, 6, Color{ 0, 0, 0, shadowIntensity });
			DrawRectangle(walls[i].x, walls[i].y, walls[i].width, walls[i].height, GRAY);
		}
		for (int i = 0; i * 50 < screenHeight; i++)
		{
			DrawLine(0, i * 50, screenWidth, i * 50, LIME);
			DrawLine(0, i * 50 - 1, screenWidth, i * 50 - 1, LIME);
		}
		for (int i = 0; i * 50 < screenWidth; i++)
		{
			DrawLine(i * 50, 0, i * 50, screenHeight, LIME);
			DrawLine(i * 50 - 1, 0, i * 50 - 1, screenHeight, LIME);
		}
		if (gameTimer > matchTime)
		{
			DrawRectangle(0, screenHeight - 20, screenWidth, 20, team2Color);
			DrawRectangle(0, screenHeight - 20, (scores.x / (scores.x + scores.y)) * screenWidth, 20, team1Color);
		}

		

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