#pragma once
#include "raylib.h"

const int screenWidth = 1280;
const int screenHeight = 720;
const int maxPlayerCount = 2;
const int maxShotCount = 100;
const Color team1Color = YELLOW;
const Color team2Color = DARKPURPLE;
const Color unclaimed = BLANK;
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

#define BASE_GUN Weapon{ 10.0f, 0.15f, 75.0f, 600.0f, 5.0f, 250.0f, 25, 10, 10 }