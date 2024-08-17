#include "Game.h"
#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "PlayerView.h"
#include "FPS_Analyzer.h"

// Global Settings 
const char* TITLE = "MonkeyRun";
const int FPS = 60;
const bool SHOW_FPS = true;
const bool SHOW_CURSOR = true;

const int CELL_SIZE = 50;
const int CELL_HEIGHT = 50;
const int CELL_MARGIN = 0;
int ROW = 5;
int COL = 7;

const int WIDTH = 1920;
const int HEIGHT = 1080;

// Game's Variable (Pointers)
long long Game::gameTick = 0;
SDL_Renderer* Game::renderer = nullptr;
Map* map = nullptr;
Player* player = nullptr;
Enemy* enemy = nullptr;
PlayerView* view = nullptr;
FPSAnalyzer* analyzer = nullptr;

Game::Game() {}
Game::~Game() {}

void Game::init(const char* title, int x, int y, int w, int h, bool fullscreen)
{	
	// Toggle Fullscreen
	int flag = 0;
	if (fullscreen)
	{
		flag = SDL_WINDOW_FULLSCREEN_DESKTOP;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0 && TTF_Init() == 0)
	{
		
		// Mixer Init
		Mix_Init(MIX_INIT_MP3 || MIX_INIT_WAVPACK);
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

		// Create
		window = SDL_CreateWindow(title, x, y, w, h, flag);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

		// Renderer's Settings
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		// Other Settings
		SDL_SetRelativeMouseMode(SDL_TRUE);

		running = true;
	}
	else
	{
		std::cout << TITLE << ": 遊戲啟動失敗!\n";
		std::cout << "錯誤訊息: " << SDL_GetError() << "\n\n";
		return;
	}

	// Create map object;
	map = new Map();
	map->init("map.png");

	// Create Player object;
	player = new Player();
	player->init();

	// Create Player object;
	enemy = new Enemy();
	enemy->init("Creeper.png");

	// Create View object;
	view = new PlayerView();
	view->init();

	// Create FPS Analyzer object
	analyzer = new FPSAnalyzer(5);
	analyzer->frameStart();

}

void Game::clear()
{
	
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	SDL_Quit();

}

void Game::update()
{	
	
	analyzer->frameEnd();
	
	map->update();
	player->update(map);
	enemy->update(map, player);
	view->update(player, map);
	
	gameTick++;
	
	analyzer->frameStart();
	
}

void Game::render()
{

	SDL_SetRenderDrawColor(renderer, 100, 150, 255, 255);
	SDL_RenderClear(renderer);
	
	view->renderOnScreen(enemy);

	map->render();
	view->renderOnMap(map);
	player->renderOnMap(map);
	enemy->renderOnMap(map);
	
	if (SHOW_FPS)
	{
		analyzer->render();
	}
	
	SDL_RenderPresent(renderer);

}

void Game::handleEvent()
{

	while (SDL_PollEvent(&event))
	{
		map->handleEvent(event);
		player->handleEvent(event);
		enemy->handleEvent(event);

		switch (event.type)
		{	
			// Quit
			case SDL_QUIT:
				running = false;
				break;

			// KeyBoard
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{	
					case SDLK_ESCAPE:
						running = false;
						break;

					default:
						break;
				}

			default:
				break;
		}

	}
	
}