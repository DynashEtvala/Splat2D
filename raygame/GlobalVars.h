#pragma once
#include "raylib.h"

const int screenWidth = 1300;
const int screenHeight = 800;
const int maxPlayerCount = 4;
const int maxShotCount = 100;
const int maxObsticles = 10;
const int shadowLength = 14;
const int shadowIntensity = (int)((20.0f / 100) * 255);
const int deathPopSize = 150;
const float matchTime = 180;
const float swimSpeed = 120;
const float drySwimSpeed = 30;
const float enemyInkSpeed = 20;
const Color team1Color = PINK;
const Color team2Color = GREEN;
const Color unclaimed = BLANK;
const Color pitColor = {25, 80, 175, 255};

enum Direction{ UP, DOWN, LEFT, RIGHT, TOPLEFT, TOPRIGHT, BOTTOMLEFT, BOTTOMRIGHT, CENTER };

struct Weapon
{
	float damage;
	float fireRate;
	float walkSpeed;
	float shootingSpeed;
	float bulletSpeed;
	float ammoConsume;
	float range;
	int burstSize;
	int dripSize;
	int accuracy;
};

#define BASE_GUN Weapon{ 5.0f, 0.01f, 75.0f, 50.0f, 600.0f, 5.0f, 150.0f, 25, 10, 10 }