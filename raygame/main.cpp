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

	Player* playerList[maxPlayerCount];

	for (int i = 0; i < maxPlayerCount; i++)
	{
		if (i % 2 == 0)
		{
			playerList[i] = new Player({ 50.0f, 50.0f }, BASE_GUN, team1Color, team2Color, i);
		}
		else
		{
			playerList[i] = new Player({ 1180.0f, 620.0f }, BASE_GUN, team2Color, team1Color, i);
		}
	}

	Rectangle obsticles[maxObsticles];
	int obsticleCount = 6;

	obsticles[0] = { 0, -2, screenWidth + 2, 2 };
	obsticles[1] = { -2, 0, 2, screenHeight + 2 };
	obsticles[2] = { 0, screenHeight + 1, screenWidth + 2, 2 };
	obsticles[3] = { screenWidth + 1, 0, 2, screenHeight + 2 };
	obsticles[4] = { 340, 210, 100, 300 };
	obsticles[5] = { 840, 210, 100, 300 };

	Rectangle pits[maxObsticles];
	int pitCount = 3;

	pits[0] = { 440, 310, 400, 100 };
	pits[1] = { 590, -100, 100, 300 };
	pits[2] = { 590, 520, 100, 200 };

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
				playerList[i]->Update(&controller, floorTiles);
			}

			gameTimer += GetFrameTime();
		}

		for (int i = 0; i < maxShotCount; i++)
		{
			if (shotList[i]->active)
			{
				shotList[i]->Update(&controller, floorTiles, obsticles, pits, playerList);
			}
		}

		controller.clearFromObsticle(floorTiles, pits);
		controller.clearFromObsticle(floorTiles, obsticles);

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

		for (int i = 0; i < maxPlayerCount; i++)
		{
			DrawCircle(playerList[i]->getCenter().x, playerList[i]->getCenter().y, playerList[i]->getRadius(), BLACK);
			DrawCircle(playerList[i]->getCenter().x, playerList[i]->getCenter().y, playerList[i]->getRadius() - 1, playerList[i]->enemyColor);
			DrawCircle(playerList[i]->getCenter().x, playerList[i]->getCenter().y, ((playerList[i]->getRadius() - 1) / 100) * playerList[i]->GetHealth(), playerList[i]->teamColor);
		}

		for (int i = 0; i < maxShotCount; i++)
		{
			if (shotList[i]->active)
			{
				DrawCircle(shotList[i]->getCenter().x, shotList[i]->getCenter().y, shotList[i]->getRadius(), BLACK);
				DrawCircle(shotList[i]->getCenter().x, shotList[i]->getCenter().y, shotList[i]->getRadius() - 1, shotList[i]->color);
			}
		}

		for (int i = 4; i < obsticleCount; i++)
		{
			Vector2 shadowpoints[6] =
			{
				Vector2{ obsticles[i].x + obsticles[i].width + shadowLength, obsticles[i].y + obsticles[i].height + shadowLength },
				Vector2{ obsticles[i].x + obsticles[i].width + shadowLength, obsticles[i].y + shadowLength },
				Vector2{ obsticles[i].x + obsticles[i].width, obsticles[i].y },
				Vector2{ obsticles[i].x + obsticles[i].width, obsticles[i].y + obsticles[i].height },
				Vector2{ obsticles[i].x, obsticles[i].y + obsticles[i].height },
				Vector2{ obsticles[i].x + shadowLength, obsticles[i].y + obsticles[i].height + shadowLength },
			};
			DrawPolyEx(shadowpoints, 6, Color{ 0, 0, 0, shadowIntensity });
			DrawRectangle(obsticles[i].x, obsticles[i].y, obsticles[i].width, obsticles[i].height, GRAY);
		}

		//DrawRectangle(0, 0, screenWidth, 20, team2Color);
		//DrawRectangle(0, 0, (scores.x / (scores.x + scores.y)) * screenWidth, 20, team1Color);

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