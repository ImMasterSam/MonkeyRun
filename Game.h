#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include <iostream>
#include <algorithm>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <chrono>

extern const char* TITLE;
extern const int FPS;
extern const bool SHOW_FPS;
extern const bool SHOW_CURSOR;

extern const int CELL_SIZE;
extern const int CELL_HEIGHT;
extern const int CELL_MARGIN;
extern int ROW;
extern int COL;

extern const int WIDTH;
extern const int HEIGHT;

class Game
{

	private:

		SDL_Window* window = nullptr;
		SDL_Event event;

		SDL_Rect mousePos = {-1, -1, 0, 0};

		bool running = false;

	public:

		Game();
		~Game();

		void init(const char* title, int x, int y, int w, int h, bool fullscreen);
		void clear();

		void update();
		void render();
		void handleEvent();

		bool isRunning() const { return running; }

		static SDL_Renderer* renderer;
		static long long gameTick;

};