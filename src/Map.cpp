#include "Map.h"
#include "Player.h"

int default_map[5][7] = { {1, 1, 1, 1, 1, 1, 1},
						  {1, 0, 0, 0, 0, 0, 1},
						  {1, 0, 1, 0, 1, 0, 1},
						  {1, 0, 0, 0, 0, 0, 1},
						  {1, 1, 1, 1, 1, 1, 1} };

const int MAP_CELL_SIZE = 15;
int MAP_WIDTH = 0;
int MAP_HEIGHT = 0;

Map::Map() {}
Map::~Map() {}

void Map::init(const char* filename)
{

	loadIn(filename);

	x = WIDTH - MAP_CELL_SIZE * COL;
	y = 0;

	MAP_WIDTH = MAP_CELL_SIZE * COL;
	MAP_HEIGHT = MAP_CELL_SIZE * ROW;
	
	cellRect.w = cellRect.h = MAP_CELL_SIZE - CELL_MARGIN * 2;

}

void Map::update()
{

}

void Map::render()
{
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{	

			cellRect.x = j * MAP_CELL_SIZE + CELL_MARGIN * 2 + x;
			cellRect.y = i * MAP_CELL_SIZE + CELL_MARGIN * 2 + y;

			switch (cellArray[i][j])
			{
				case Space:
					SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 255);
					break;
				case Wall:
					SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
					break;
			
				default:
					break;
			}

			SDL_RenderFillRect(Game::renderer, &cellRect);

		}
	}
}

void Map::handleEvent(SDL_Event& event)
{

}

bool Map::iswalkable(float xpos, float ypos)
{

	if (xpos < 0 || xpos >= CELL_SIZE * COL || ypos < 0 || ypos >= CELL_SIZE * ROW)
	{	
		//std::cout << "Boudary!\n";
		return false;
	}
	else
	{	

		int row = (int)(ypos) / CELL_SIZE;
		int col = (int)(xpos) / CELL_SIZE;

		return cellArray[row][col] == Space;
	}

}

void Map::loadIn(const char* filename)
{

	SDL_Surface* suf = IMG_Load(filename);

	// Error Handling
	if (suf == nullptr)
	{
		std::cout << filename << ": 圖片讀取失敗!\n";
		std::cout << "錯誤訊息: " << SDL_GetError() << "\n\n";
		return;
	}

	Uint8* pixels = (Uint8*)suf->pixels;

	ROW = suf->h;
	COL = suf->w;

	cellArray.assign(ROW, std::vector<Cell>(COL, Space));

	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{	
			if (pixels[(j * 4) + (i * (COL * 4))] == 255)
			{
				cellArray[i][j] = Wall;
			}
			else
			{
				cellArray[i][j] = Space;
			}
		}
	}

	SDL_FreeSurface(suf);

}