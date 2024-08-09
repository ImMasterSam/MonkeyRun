#pragma once

#include "Game.h"
#include "Map.h"
#include "Player.h"
#include "Enemy.h"

class PlayerView
{

	private:

		// constant variables
		const float VIEW_RANGE = 60;
		const float DISTANCE_TO_PROJECTION_PLANE = (WIDTH / 2) / tan((VIEW_RANGE / 2) * M_PI / 180.0);
		const float RESOLUTION = 0.1;

		// Player's info
		float playerDirection = 0;
		float playerXPos = 0, playerYPos = 0;

		// Rendering Datas
		int dataSize = 0;
		std::vector<SDL_FPoint> endPoints = std::vector<SDL_FPoint>();
		std::vector<float> castDistance = std::vector<float>();

		// Private Functions
		float getDisFromPlayer(float xpos, float ypos);
		void castRay(float playerDirection, Map* map, int idx);
		SDL_Texture* loadTexture(const char* filename);

		// Wall Variables
		SDL_Texture* wallTex = nullptr;
		int wallHeight = 0, wallWidth = 0;
		std::vector<float> textureOffset = std::vector<float>();

		// Target Settings;
		const float TARGET_HEIGHT = 40;
		void renderTarget(Enemy* target);

	public:

		PlayerView();
		~PlayerView();

		void init();
		void update(Player* player, Map* map);

		void renderOnMap(Map* map);
		void renderOnScreen(Enemy* target);

};