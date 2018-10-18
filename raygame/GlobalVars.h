#pragma once
#include "raylib.h"

const int screenWidth = 1280;
const int screenHeight = 720;
const int maxPlayerCount = 2;
const int maxShotCount = 100;
const int maxObsticles = 10;
const int shadowLength = 25;
const int shadowIntensity = (int)((20.0f / 100) * 255);
const int deathPopSize = 150;
const float matchTime = 180;
const float swimSpeed = 120;
const float drySwimSpeed = 30;
const Color team1Color = YELLOW;
const Color team2Color = DARKPURPLE;
const Color unclaimed = BLANK;
const Color pitColor = {25, 80, 175, 255};

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

#define BASE_GUN Weapon{ 5.0f, 0.15f, 75.0f, 50.0f, 600.0f, 5.0f, 250.0f, 25, 10, 10 }