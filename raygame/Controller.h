#pragma once
#include "raylib.h"
#include "GlobalVars.h"

class Controller
{
private:
	class Player* playerList[maxPlayerCount];
	class Shot* shotList[maxShotCount];
	int nextShot;
public:
	Controller();
	Controller(class Player** plist, class Shot** slist);
	~Controller();
	void addShot(Rectangle loc, Vector2 dir, float spd, float dmg, float rng, int bsize, int dsize, Color col);
	void paintFloor(Vector2 c, int r, Color col, class FloorTile*** ftile);
	void paintFloorLine(Vector2 p1, Vector2 p2, Color col, class FloorTile*** ftile);
	void paintFloorLineHigh(Vector2 p1, Vector2 p2, Color col, class FloorTile*** ftile);
	void paintFloorLineLow(Vector2 p1, Vector2 p2, Color col, class FloorTile*** ftile);
	Vector2 getScores(class FloorTile*** ftile);
};

