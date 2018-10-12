#include "Controller.h"

#include "Shot.h"
#include "Player.h"
#include "FloorTile.h"
#include "Math.h"
#include <math.h>

Controller::Controller()
{
	nextShot = 0;
}

Controller::Controller(Player** plist, Shot** slist)
{
	for (int i = 0; i < maxPlayerCount; i++)
	{
		playerList[i] = plist[i];
	}
	for (int i = 0; i < maxShotCount; i++)
	{
		shotList[i] = slist[i];
	}
	nextShot = 0;
}

Controller::~Controller()
{}

void Controller::addShot(Rectangle loc, Vector2 dir, float spd, float dmg, float rng, int bsize, int dsize, Color col)
{
	shotList[nextShot]->SetNewVals(loc, dir, spd, dmg, rng, bsize, dsize, col);
	nextShot++;
	if (nextShot >= 100)
	{
		nextShot = 0;
	}
}

void Controller::paintFloor(Vector2 c, int r, Color col, FloorTile*** ftile)
{
	int x = r;
	int y = 0;
	int err = 0;

	while (x >= y)
	{
		paintFloorLine({ c.x - x, c.y + y }, { c.x + x, c.y + y }, col, ftile);
		paintFloorLine({ c.x + y, c.y - x }, { c.x + y, c.y + x }, col, ftile);
		paintFloorLine({ c.x - y, c.y - x }, { c.x - y, c.y + x }, col, ftile);
		paintFloorLine({ c.x + x, c.y - y }, { c.x - x, c.y - y }, col, ftile);

		if (err <= 0)
		{
			y += 1;
			err += 2 * y + 1;
		}

		if (err > 0)
		{
			x -= 1;
			err -= 2 * x + 1;
		}
	}
}

void Controller::paintFloorLine(Vector2 p1, Vector2 p2, Color col, FloorTile*** ftile)
{
	if (abs((int)(p2.y - p1.y)) < abs((int)(p2.x - p1.x)))
	{
		if (p1.x > p2.x)
		{
			paintFloorLineLow({ p2.x, p2.y }, { p1.x, p1.y }, col, ftile);
		}
		else
		{
			paintFloorLineLow({ p1.x, p1.y }, { p2.x, p2.y }, col, ftile);
		}
	}
	else
	{
		if (p1.y > p2.y)
		{
			paintFloorLineHigh({ p2.x, p2.y }, { p1.x, p1.y }, col, ftile);
		}
		else
		{
			paintFloorLineHigh({ p1.x, p1.y }, { p2.x, p2.y }, col, ftile);
		}
	}
}

void Controller::paintFloorLineHigh(Vector2 p1, Vector2 p2, Color col, class FloorTile*** ftile)
{
	int dx = p2.x - p1.x;
	int dy = p2.y - p1.y;
	int xi = 1;
	if (dx < 0)
	{
		xi = -1;
		dx = -dx;
	}
	int D = 2 * dx - dy;
	int x = p1.x;

	for (int y = p1.y; y <= p2.y; y++)
	{
		if (x >= 0 && x < screenWidth && y >= 0 && y < screenHeight)
		{
			ftile[y][x]->color = col;
		}
		if (D > 0)
		{
			x = x + xi;
			D = D - 2 * dy;
		}
		D = D + 2 * dx;
	}
}

void Controller::paintFloorLineLow(Vector2 p1, Vector2 p2, Color col, class FloorTile*** ftile)
{
	int dx = p2.x - p1.x;
	int dy = p2.y - p1.y;
	int yi = 1;
	if (dy < 0)
	{
		yi = -1;
		dy = -dy;
	}
	int D = 2 * dy - dx;
	int y = p1.y;

	for (int x = p1.x; x <= p2.x; x++)
	{
		if (x >= 0 && x < screenWidth && y >= 0 && y < screenHeight)
		{
			ftile[y][x]->color = col;
		}
		if (D > 0)
		{
			y = y + yi;
			D = D - 2 * dx;
		}
		D = D + 2 * dy;
	}
}

Vector2 Controller::getScores(class FloorTile*** ftile)
{
	float t1count = 1;
	Color t1color = team1Color;
	float t2count = 1;
	Color t2color = team2Color;

	for (int i = 0; i < screenHeight; i++)
	{
		for (int j = 0; j < screenWidth; j++)
		{
			if (ftile[i][j]->color == t1color)
			{
				t1count++;
			}
			else if (ftile[i][j]->color == t2color)
			{
				t2count++;
			}
		}
	}
	t1count = (t1count / (screenHeight * screenWidth)) * 100;
	t2count = (t2count / (screenHeight * screenWidth)) * 100;
	return { t1count, t2count };
}