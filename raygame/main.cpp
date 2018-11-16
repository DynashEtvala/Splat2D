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
#include "Gamestate.h"

int main()
{
	// Initialization
	//--------------------------------------------------------------------------------------

	InitWindow(screenWidth, screenHeight, "Splat2d");
	
	FloorTile*** floorTiles = new FloorTile**[screenHeight];

	float gameTimer = 0;

	Vector2 scores;
	int scoretimer = 0;

	RenderTexture2D fTiles = LoadRenderTexture(screenWidth, screenHeight);

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
	int wallCount = 8;

	walls[0] = { -20, -20, screenWidth + 40, 20 };
	walls[1] = { -20, -20, 20, screenHeight + 40 };
	walls[2] = { screenWidth + 1, -20, 20, screenHeight + 40 };
	walls[3] = { -20, screenHeight - 100, screenWidth + 40, 100 };
	walls[6] = { 400, 200, 100, 300 };
	walls[5] = { 800, 200, 100, 300 };
	walls[4] = { 200, 200, 200, 100 };
	walls[7] = { 900, 400, 200, 100 };

	Rectangle pits[maxObsticles];
	int pitCount = 8;

	pits[0] = { 500, 300, 100, 100 };
	pits[7] = { 700, 300, 100, 100 };
	pits[1] = { 600, -100, 100, 300 };
	pits[2] = { 600, 500, 100, 200 };
	pits[3] = { 100, 400, 200, 200 };
	pits[4] = { 1000, 100, 200, 200 };
	pits[5] = { 400, 500, 100, 100 };
	pits[6] = { 800, 100, 100, 100 };

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
		switch (GameState::getState())
		{
		default:
		case Splash:
			if (gameTimer <= 5)
			{
				gameTimer += GetFrameTime();
			}
			else
			{
				gameTimer = 0;
				GameState::setState(MainMenu);
			}
			if (GetGamepadButtonPressed() == GAMEPAD_XBOX_BUTTON_START)
			{
				gameTimer = 0;
				GameState::setState(InGame);
			}
			break;
		case MainMenu:
			break;
		case PlayerSelect:
			break;
		case InGame:
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
					shotList[i]->Update(&controller, floorTiles, walls, pits, pads, playerList);
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
			break;
		case GameOver:
			break;
		case Options:
			break;
		}
		

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);

		switch (GameState::getState())
		{
		default:
		case Splash:
			break;
		case MainMenu:
			ClearBackground(Color{ 190, 190, 25, 255 });
			DrawRectangle(250, 250, 200, 200, RAYWHITE);
			DrawText("PLAY", 250, 250, 65, BLACK);
			break;
		case PlayerSelect:
			break;
		case InGame:
			ClearBackground(DARKGRAY);

			if (scoretimer % 5 == 0)
			{
				BeginTextureMode(fTiles);
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
				EndTextureMode();
			}

			DrawTextureRec(fTiles.texture, Rectangle{ 0, 0, (float)fTiles.texture.width, (float)(-fTiles.texture.height) }, Vector2{ 0, 0 }, WHITE);

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

			for (int i = 0; i < maxShotCount; i++)
			{
				if (shotList[i]->active)
				{
					shotList[i]->Draw();
				}
			}

			for (int i = 0; i < maxPlayerCount; i++)
			{
				playerList[i]->Draw();
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
			if (gameTimer > matchTime)
			{
				DrawRectangle(0, screenHeight - 20, screenWidth, 20, team2Color);
				DrawRectangle(0, screenHeight - 20, (scores.x / (scores.x + scores.y)) * screenWidth, 20, team1Color);
			}
			break;
		case GameOver:
			break;
		case Options:
			break;
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