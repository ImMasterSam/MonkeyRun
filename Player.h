#pragma once

#include "Game.h"
#include "Map.h"

class Player
{

	private:

		// Settings
		const float PLAYER__MAP_RADIUS = 5;
		const int PLAYER_SPEED = CELL_SIZE / 10;
		const float DPI = 0.05f;

		// Control Settings
		float directionDeg = 0;
		float directionRad = 0;
		int forward = 0;
		int sideway = 0;
		float turn = 0;

		SDL_FPoint midpos;
		SDL_FPoint pointer;
		SDL_FRect dstRect;
		SDL_Rect srcRect;

		SDL_Texture* playerTex = nullptr;

		void updatePos(Map* map, float tempDirection, float move);

		double distance(float x1, float y1, float x2, float y2);
		SDL_Texture* createCircle(int r);


	public:

		Player();
		~Player();

		void init();

		void update(Map *map);
		void renderOnMap(Map* map);
		void handleEvent(SDL_Event &event);

		double getXPos() const { return midpos.x; }
		double getYPos() const { return midpos.y; }
		float getDirection() const { return directionDeg; }
	
};