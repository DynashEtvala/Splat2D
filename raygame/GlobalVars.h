#pragma once
#include "raylib.h"

const int screenWidth = 1300;
const int screenHeight = 800;
const int maxPlayerCount = 4;
const int maxShotCount = 100;
const int maxObsticles = 10;
const int shadowLength = 20;
const int shadowIntensity = (int)((20.0f / 100) * 255);
const int deathPopSize = 100;
const float matchTime = 180;
const float swimSpeed = 120;
const float drySwimSpeed = 30;
const float enemyInkSpeed = 20;
const Color team1Color = GREEN;
const Color team2Color = DARKPURPLE;
const Color unclaimed = BLANK;
const Color pitColor = {25, 60, 135, 255};

enum Direction{ UP, DOWN, LEFT, RIGHT, TOPLEFT, TOPRIGHT, BOTTOMLEFT, BOTTOMRIGHT, CENTER };

struct Weapon{ float damage; float fireRate; float walkSpeed; float shootingSpeed; float bulletSpeed; float ammoConsume; float range; int burstSize; int dripSize; int accuracy;};

//					 		damage		firerate	walkspeed	shootingspeed	bulletspeed		ammoconsume		range			burstsize	dripsize	accuracy
#define BASE_GUN Weapon   {	15.0f,		0.15f,		75.0f,		50.0f,			600.0f,			2.5f,			150.0f,			25,			12,			20 }


struct SubWeapon { float damage; float bombSpeed; float ammoConsume; float bombTime; int burstSize; };

//							damage		bombSpeed	ammoconsume		bombTime	burstsize
#define BASE_SUB SubWeapon{  }