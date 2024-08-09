#include "Player.h"

Player::Player() {}
Player::~Player() {}

// Public

void Player::init()
{	

	midpos.x = CELL_SIZE * 1.5;
	midpos.y = CELL_SIZE * 1.5;
	
	playerTex = createCircle(PLAYER__MAP_RADIUS);
	srcRect = { 0, 0, (int)PLAYER__MAP_RADIUS * 2, (int)PLAYER__MAP_RADIUS * 2 };
	dstRect = { midpos.x - PLAYER__MAP_RADIUS, midpos.y - PLAYER__MAP_RADIUS, PLAYER__MAP_RADIUS * 2, PLAYER__MAP_RADIUS * 2 };

}

void Player::update(Map* map)
{	

	directionRad = directionDeg / 180.0 * M_PI;
	pointer.x = midpos.x + PLAYER__MAP_RADIUS * cos(directionRad);
	pointer.y = midpos.y - PLAYER__MAP_RADIUS * sin(directionRad);

	// Turning

	if (turn != 0)
	{
		directionDeg += turn * DPI;

		if (directionDeg < 0)
		{
			directionDeg += 360;
		}
		else if (directionDeg >= 360)
		{
			directionDeg -= 360;
		}

		turn = 0;

	}

	// Moving

	if (forward)
	{
		for (int repeat = 0; repeat < PLAYER_SPEED; repeat++)
		{
			updatePos(map, directionRad, forward);
		}
	}
	if (sideway)
	{
		for (int repeat = 0; repeat < PLAYER_SPEED; repeat++)
		{
			updatePos(map, directionRad - (M_PI / 2), sideway * 0.5);
		}
	}


	dstRect.x = midpos.x / CELL_SIZE * MAP_CELL_SIZE - PLAYER__MAP_RADIUS + map->getXPos();
	dstRect.y = midpos.y / CELL_SIZE * MAP_CELL_SIZE - PLAYER__MAP_RADIUS + map->getYPos();
}

void Player::renderOnMap(Map* map)
{	
	SDL_RenderCopyF(Game::renderer, playerTex, &srcRect, &dstRect);

	float playerMidX = dstRect.x + PLAYER__MAP_RADIUS;
	float playerMidY = dstRect.y + PLAYER__MAP_RADIUS;

	float pointerX = pointer.x / CELL_SIZE * MAP_CELL_SIZE + map->getXPos();
	float pointerY = pointer.y / CELL_SIZE * MAP_CELL_SIZE + map->getYPos();

	SDL_SetRenderDrawColor(Game::renderer, 0, 0, 255, 255);
	SDL_RenderDrawLineF(Game::renderer, playerMidX, playerMidY, pointerX, pointerY);
}

// Private

void Player::handleEvent(SDL_Event& event)
{	

	switch (event.type)
	{	
		// Mouse
		case SDL_MOUSEMOTION:
			turn = std::pow(-event.motion.xrel, 2) * (event.motion.xrel > 0 ? -1 : 1);
			break;

		// KeyBoard
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
				case SDLK_w:
					forward = 1;
					break;
				case SDLK_s:
					forward = -1;
					break;
				case SDLK_d:
					sideway = 1;
					break;
				case SDLK_a:
					sideway = -1;
					break;
				default:
					break;
			}
			break;

		case SDL_KEYUP:
			switch (event.key.keysym.sym)
			{
				case SDLK_w:
				case SDLK_s:
					forward = 0;
					break;
				case SDLK_d:
				case SDLK_a:
					sideway = 0;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}

void Player::updatePos(Map* map, float tempDirection, float move)
{

	float nxtx = midpos.x + cos(tempDirection) * move;
	float nxty = midpos.y - sin(tempDirection) * move;

	if (map->iswalkable(nxtx, midpos.y))
	{
		midpos.x = nxtx;
	}

	if (map->iswalkable(midpos.x, nxty))
	{
		midpos.y = nxty;
	}	

}

double Player::distance(float x1, float y1, float x2, float y2)
{
	return std::sqrt(std::pow(x1 - x2, 2) + std::pow(y1 - y2, 2));
}

SDL_Texture* Player::createCircle(int r)
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
				pixels[(j * 4) + (i * suf->pitch)] = 0;
				pixels[(j * 4) + (i * suf->pitch) + 1] = 0;
				pixels[(j * 4) + (i * suf->pitch) + 2] = 255;
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