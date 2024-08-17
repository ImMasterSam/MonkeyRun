#pragma once

#include "Game.h"
#include "Map.h"
#include "Player.h"
#include "Sound.h"

class Enemy
{

private:

	// Settings
	const float ENEMY_MAP_RADIUS = 5;
	const int ENEMY_ANGULAR_SPEED = 3;
	const int ENEMY_SPEED = CELL_SIZE / 15;

	// Control Settings
	SDL_Point target = {-1, -1};
	float directionDeg = 0;
	float directionRad = 0;
	int forward = 0;
	int turn = 0;

	SDL_FPoint midpos;
	SDL_FPoint pointer;
	SDL_FRect dstRect;
	SDL_Rect srcRect;

	SDL_Texture* enemyTex = nullptr;
	SDL_Texture* appearanceTex = nullptr;
	SDL_Texture* loadTexture(const char* filename);

	Sound* sound = nullptr;

	void updatePos(Map* map);

	double distance(float x1, float y1, float x2, float y2);
	SDL_Texture* createCircle(int r);


public:

	Enemy();
	~Enemy();

	void init(const char* filename);

	void update(Map* map, Player* player);
	void renderOnMap(Map* map);
	void handleEvent(SDL_Event& event);

	double getXPos() const { return midpos.x; }
	double getYPos() const { return midpos.y; }
	float getDirection() const { return directionDeg; }

	SDL_Texture* getTexture() const { return appearanceTex; }

};