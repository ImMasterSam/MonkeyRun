#include "Game.h"

Game* game = nullptr;

int main(int argc, char* argv[])
{	

	// Create Game Object (including windows)
	game = new Game();
	game->init(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, true);

	// FPS Settings
	const int FrameDeley = 1000 / FPS;
	Uint32 FrameStart;
	int FrameTime;

	// Main Loop
	while (game->isRunning())
	{
		FrameStart = SDL_GetTicks();

		game->update();
		game->render();
		game->handleEvent();

		FrameTime = SDL_GetTicks() - FrameStart;

		if (FrameTime < FrameDeley)
		{
			SDL_Delay(FrameDeley - FrameTime);
		}

	}

	game->clear();

	return 0;
}