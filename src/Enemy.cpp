#include "Enemy.h"
#include "PathFinder.h"

Enemy::Enemy() {}
Enemy::~Enemy() {}

// Public

void Enemy::init(const char* filename)
{

	midpos.x = CELL_SIZE * 10.5;
	midpos.y = CELL_SIZE * 10.5;

	target = {(int)midpos.x, (int)midpos.y};

	enemyTex = createCircle(ENEMY_MAP_RADIUS);
	appearanceTex = loadTexture(filename);

	sound = new Sound("You are my Sunshine.mp3");

	srcRect = { 0, 0, (int)ENEMY_MAP_RADIUS * 2, (int)ENEMY_MAP_RADIUS * 2 };
	dstRect = { midpos.x - ENEMY_MAP_RADIUS, midpos.y - ENEMY_MAP_RADIUS, ENEMY_MAP_RADIUS * 2, ENEMY_MAP_RADIUS * 2 };

}

void Enemy::update(Map* map, Player* player)
{

	sound->update();
	sound->setVolume(distance(midpos.x, midpos.y, player->getXPos(), player->getYPos()));

	pointer.x = midpos.x + ENEMY_MAP_RADIUS * cos(directionRad);
	pointer.y = midpos.y - ENEMY_MAP_RADIUS * sin(directionRad);
	
	// Turning

	if (turn != 0)
	{
		directionDeg += turn * ENEMY_ANGULAR_SPEED;

		if (directionDeg < 0)
		{
			directionDeg += 360;
		}
		else if (directionDeg >= 360)
		{
			directionDeg -= 360;
		}

	}

	// Moving

	if (1)
	{
		
		for (int repeat = 0; repeat < ENEMY_SPEED; repeat++)
		{
			
			if (distance(target.x, target.y, midpos.x, midpos.y) <= CELL_SIZE / 10)
			{
				target = PathFinder::Search(midpos.x, midpos.y, player->getXPos(), player->getYPos(), map);

				directionRad = atan((float)(midpos.y - target.y) / (target.x - midpos.x));

				if (target.x < midpos.x)
				{
					directionRad += M_PI;
				}
				if (directionRad <= 0)
				{
					directionRad += 2 * M_PI;
				}

				directionDeg = directionRad / M_PI * 180.0;

				//std::cout << "target: (" << target.x << ",  " << target.y << ")\n";
				//std::cout << directionDeg << "\n";

			}
			
			updatePos(map);
		}
	}

	dstRect.x = midpos.x / CELL_SIZE * MAP_CELL_SIZE - ENEMY_MAP_RADIUS + map->getXPos();
	dstRect.y = midpos.y / CELL_SIZE * MAP_CELL_SIZE - ENEMY_MAP_RADIUS + map->getYPos();
}

void Enemy::renderOnMap(Map* map)
{
	SDL_RenderCopyF(Game::renderer, enemyTex, &srcRect, &dstRect);

	float playerMidX = dstRect.x + ENEMY_MAP_RADIUS;
	float playerMidY = dstRect.y + ENEMY_MAP_RADIUS;

	float pointerX = pointer.x / CELL_SIZE * MAP_CELL_SIZE + map->getXPos();
	float pointerY = pointer.y / CELL_SIZE * MAP_CELL_SIZE + map->getYPos();

	SDL_SetRenderDrawColor(Game::renderer, 255, 0, 0, 255);
	SDL_RenderDrawLineF(Game::renderer, playerMidX, playerMidY, pointerX, pointerY);
}

// Private

void Enemy::handleEvent(SDL_Event& event)
{
	;
}

// Private Functions

SDL_Texture* Enemy::loadTexture(const char* filename)
{
	SDL_Surface* tempSurface = IMG_Load(filename);

	// Error Handling
	if (tempSurface == nullptr)
	{
		std::cout << filename << ": 圖片讀取失敗!\n";
		std::cout << "錯誤訊息: " << SDL_GetError() << "\n\n";
		return nullptr;
	}

	SDL_Texture* res = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	return res;
}

void Enemy::updatePos(Map* map)
{

	float nxtx = midpos.x + cos(directionRad);
	float nxty = midpos.y - sin(directionRad);

	if (map->iswalkable(nxtx, midpos.y))
	{
		midpos.x = nxtx;
	}

	if (map->iswalkable(midpos.x, nxty))
	{
		midpos.y = nxty;
	}

}

double Enemy::distance(float x1, float y1, float x2, float y2)
{
	return std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
}

SDL_Texture* Enemy::createCircle(int r)
{
	SDL_Surface* suf = SDL_CreateRGBSurfaceWithFormat(0, r * 2, r * 2, 32, SDL_PIXELFORMAT_BGRA32);
	SDL_SetSurfaceBlendMode(suf, SDL_BLENDMODE_BLEND);
	SDL_LockSurface(suf);

	std::vector<uint8_t> pixels(suf->h * suf->pitch, 0);
	int max_width = (r * 2) * 4;

	float midx = r - 0.5;
	float midy = r - 0.5;

	for (int i = 0; i < r * 2; i++)
	{
		for (int j = 0; j < r * 2; j++)
		{

			if (distance(midx, midy, j, i) <= r)
			{
				pixels[(j * 4) + (i * suf->pitch)] = 255;
				pixels[(j * 4) + (i * suf->pitch) + 1] = 0;
				pixels[(j * 4) + (i * suf->pitch) + 2] = 0;
				pixels[(j * 4) + (i * suf->pitch) + 3] = 255;
			}

		}
	}

	memcpy(suf->pixels, pixels.data(), (long long)suf->h * suf->pitch);

	SDL_UnlockSurface(suf);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, suf);
	SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(suf);

	return tex;

}