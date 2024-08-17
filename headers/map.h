#pragma once

#include "Game.h"

extern const int MAP_CELL_SIZE;
extern int MAP_WIDTH;
extern int MAP_HEIGHT;

class Map
{

	private:

		enum Cell
		{
			Wall,
			Space
		};

		SDL_Rect cellRect;
		std::vector<std::vector<Cell>> cellArray;

		int x, y;

		void loadIn(const char* filename);

	public:

		Map();
		~Map();

		void init(const char* filename);
		
		void update();
		void render();
		void handleEvent(SDL_Event& event);

		bool iswalkable(float x, float y);
		int getXPos() const { return x; }
		int getYPos() const { return y; }

};