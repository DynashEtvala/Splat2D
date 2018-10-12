#pragma once
#include "raylib.h"

const int screenWidth = 1280;
const int screenHeight = 720;
const int maxPlayerCount = 2;
const int maxShotCount = 100;
const int maxObsticles = 10;
const int shadowLength = 40;
const int shadowIntensity = 75;
const int deathPopSize = 150;
const float matchTime = 20;
const Color team1Color = YELLOW;
const Color team2Color = DARKPURPLE;
const Color unclaimed = BLANK;
const Color pitColor = {25, 80, 175, 255};

struct Weapon
{
	float damage;
	float fireSpeed;
	float walkSpeed;
	float shotSpeed;
	float ammoConsume;
	float range;
	int burstSize;
	int dripSize;
	int accuracy;
};

#define BASE_GUN Weapon{ 5.0f, 0.15f, 75.0f, 600.0f, 5.0f, 250.0f, 25, 10, 10 }