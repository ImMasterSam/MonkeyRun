#include "PlayerView.h"

// Public: 

#define min(a, b) (a<b ? a : b)
#define max(a, b) (a>b ? a : b)

PlayerView::PlayerView() {}
PlayerView::~PlayerView() {}

void PlayerView::init()
{

	dataSize = (int)(VIEW_RANGE / RESOLUTION);

	endPoints.assign(dataSize, { 0, 0 });
	castDistance.assign(dataSize, 0);
	textureOffset.assign(dataSize, 0);

	wallTex = loadTexture("wall_2.jpg");

}

void PlayerView::update(Player* player, Map* map)
{

	playerDirection = player->getDirection();
	playerXPos = player->getXPos();
	playerYPos = player->getYPos();

	int idx = 0;
	for (float i = -VIEW_RANGE/2; i <= VIEW_RANGE / 2; i += RESOLUTION)
	{
		castRay(i, map, idx++);
	}

}

void PlayerView::renderOnMap(Map *map)
{

	SDL_SetRenderDrawColor(Game::renderer, 255, 0, 255, 255);

	float playerMidX = playerXPos / CELL_SIZE * MAP_CELL_SIZE + map->getXPos();
	float playerMidY = playerYPos / CELL_SIZE * MAP_CELL_SIZE + map->getYPos();

	for (SDL_FPoint& pos : endPoints)
	{
		SDL_RenderDrawLineF(Game::renderer, playerMidX, playerMidY, pos.x, pos.y);
	}

}

void PlayerView::renderOnScreen(Enemy* target)
{

	SDL_Rect srcRect = {-1, -1, -1, -1};
	SDL_FRect dstRect = { -1, -1, -1, -1 };

	// Green Ground

	SDL_Rect grdRect = { 0, 540, 1920, 540 };
	SDL_SetRenderDrawColor(Game::renderer, 0, 150, 0, 255);
	SDL_RenderFillRect(Game::renderer, &grdRect);

	float barWidth = (float)WIDTH / dataSize;
	float targetDistance = getDisFromPlayer(target->getXPos(), target->getYPos());

	/*
	1. Render the tiles behind the target
	2. Render the target
	3. Render the tiles in front of the target
	*/


	// Behind Target
	for (int it=0;it< dataSize;it++)
	{

		float distance = castDistance[it];
		if (distance < targetDistance)
		{
			continue;
		}

		float barHeight = (CELL_HEIGHT / distance) * DISTANCE_TO_PROJECTION_PLANE;

		dstRect.x = WIDTH - barWidth * (it + 1);
		dstRect.y = (HEIGHT - barHeight) / 2;
		dstRect.w = barWidth;
		dstRect.h = barHeight;

		srcRect.w = 1;
		srcRect.h = wallHeight;
		srcRect.x = (int)(textureOffset[it] * wallWidth / CELL_SIZE);
		srcRect.y = 0;
		
		SDL_RenderCopyF(Game::renderer, wallTex, &srcRect, &dstRect);

		// Long Distance Shader;
		SDL_SetRenderDrawColor(Game::renderer, 50, 50, 50, min((int)(distance / CELL_SIZE * 10), 255));
		SDL_RenderFillRectF(Game::renderer, &dstRect);

	}

	// Render the Target
	renderTarget(target);

	// In Front of the Target
	for (int it = 0; it < dataSize; it++)
	{

		float distance = castDistance[it];
		if (distance >= targetDistance)
		{
			continue;
		}

		float barHeight = (CELL_HEIGHT / distance) * DISTANCE_TO_PROJECTION_PLANE;

		dstRect.x = WIDTH - barWidth * (it + 1);
		dstRect.y = (HEIGHT - barHeight) / 2;
		dstRect.w = barWidth;
		dstRect.h = barHeight;

		srcRect.w = 1;
		srcRect.h = wallHeight;
		srcRect.x = (int)(textureOffset[it] * wallWidth / CELL_SIZE);
		srcRect.y = 0;

		SDL_RenderCopyF(Game::renderer, wallTex, &srcRect, &dstRect);

		// Long Distance Shader;
		SDL_SetRenderDrawColor(Game::renderer, 50, 50, 50, min((int)(distance / CELL_SIZE * 10), 255));
		SDL_RenderFillRectF(Game::renderer, &dstRect);

	}

}

// Private:

float PlayerView::getDisFromPlayer(float xpos, float ypos)
{
	return sqrt(pow(xpos - playerXPos, 2) + pow(ypos - playerYPos, 2));
}

void PlayerView::castRay(float viewdiff, Map* map, int idx)
{
	/*
		DDA method
	*/

	float tempDirection = playerDirection + viewdiff;

	if (tempDirection < 0.0)
	{
		tempDirection += 360.0;
	}
	else if (tempDirection >= 360.0)
	{
		tempDirection -= 360.0;
	}

	// Look for X axis:

	float Xaxis_TX = (floorf(playerXPos / CELL_SIZE) + (tempDirection < 90 || tempDirection > 270)) * CELL_SIZE;
	float Xaxis_TY = playerYPos - tan(tempDirection / 180.0 * M_PI) * (Xaxis_TX - playerXPos);

	while (map->iswalkable(Xaxis_TX, Xaxis_TY) && map->iswalkable(Xaxis_TX - 1, Xaxis_TY))
	{
		if (tempDirection < 90 || tempDirection > 270)
		{
			Xaxis_TX += CELL_SIZE;
		}
		else
		{
			Xaxis_TX -= CELL_SIZE;
		}

		Xaxis_TY = playerYPos - tan(tempDirection / 180.0 * M_PI) * (Xaxis_TX - playerXPos);
	}

	// Look for Y axis

	float Yaxis_TY = (floorf(playerYPos / CELL_SIZE) + (tempDirection > 180)) * CELL_SIZE;
	float Yaxis_TX = playerXPos - (1.0 / tan(tempDirection / 180.0 * M_PI)) * (Yaxis_TY - playerYPos);

	while (map->iswalkable(Yaxis_TX, Yaxis_TY) && map->iswalkable(Yaxis_TX, Yaxis_TY - 1))
	{
		if (tempDirection > 180)
		{
			Yaxis_TY += CELL_SIZE;
		}
		else
		{
			Yaxis_TY -= CELL_SIZE;
		}

		Yaxis_TX = playerXPos - (1.0 / tan(tempDirection / 180.0 * M_PI)) * (Yaxis_TY - playerYPos);
	}

	SDL_FPoint endPos = { -1, -1 };
	SDL_FPoint renderPos = {-1, -1};

	// Look for the closest point
	
	if (getDisFromPlayer(Xaxis_TX, Xaxis_TY) < getDisFromPlayer(Yaxis_TX, Yaxis_TY))
	{
		endPos.x = Xaxis_TX;
		endPos.y = Xaxis_TY;

		textureOffset[idx] = Xaxis_TY - (floorf(Xaxis_TY / CELL_SIZE) * CELL_SIZE);
	}
	else
	{
		endPos.x = Yaxis_TX;
		endPos.y = Yaxis_TY;

		textureOffset[idx] = Yaxis_TX - (floorf(Yaxis_TX / CELL_SIZE) * CELL_SIZE);
	}

	renderPos.x = endPos.x / CELL_SIZE * MAP_CELL_SIZE + map->getXPos();
	renderPos.y = endPos.y / CELL_SIZE * MAP_CELL_SIZE + map->getYPos();

	endPoints[idx] = renderPos;

	float distance = getDisFromPlayer(endPos.x, endPos.y);
	distance *= cos(fabs(viewdiff) / 180.0 * M_PI);

	castDistance[idx] = distance;

}

// Private Fuctions

SDL_Texture* PlayerView::loadTexture(const char* filename)
{

	SDL_Surface* tempSurface = IMG_Load(filename);

	// Error Handling
	if (tempSurface == nullptr)
	{
		std::cout << filename << ": 圖片讀取失敗!\n";
		std::cout << "錯誤訊息: " << SDL_GetError() << "\n\n";
		return nullptr;
	}

	wallHeight = tempSurface->h;
	wallWidth = tempSurface->w;

	SDL_Texture* res = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	return res;

}

void PlayerView::renderTarget(Enemy* target)
{

	SDL_Texture* tex = target->getTexture();
	int texWidth = 0, texHeight = 0;
	SDL_QueryTexture(tex, nullptr, nullptr, &texWidth, &texHeight);

	// Decide the Size of Texture According to distance;

	SDL_FRect dstRect = { 0, 0, 0, 0 };
	float distance = getDisFromPlayer(target->getXPos(), target->getYPos());
	
	dstRect.h = (TARGET_HEIGHT / distance) * DISTANCE_TO_PROJECTION_PLANE;
	dstRect.w = dstRect.h * ((float)texWidth / texHeight);
	dstRect.y = (HEIGHT - dstRect.h) / 2;

	// Then Calculate the Direction
	float targetDirection = atan((float)(playerYPos - target->getYPos()) / (target->getXPos() - playerXPos));
	float playerRad = (playerDirection / 180.0 * M_PI);

	if (target->getXPos() < playerXPos)
	{
		targetDirection += M_PI;
	}
	if (targetDirection <= 0)
	{
		targetDirection += 2 * M_PI;
	}

	float directionDifference = playerDirection - ((targetDirection) / M_PI * 180.0);
	if (fabs(directionDifference) > 180.0)
	{
		directionDifference += 360 * (directionDifference > 0 ? -1 : 1); // Difference Correction
	}

	float barwidth = (float)WIDTH / dataSize;
	dstRect.x = ((float)WIDTH / 2) + (directionDifference / RESOLUTION) * barwidth;

	//std::cout << "Target Direction: " << ((targetDirection) / M_PI * 180.0) << "\n";
	//std::cout << "Difference: " << directionDiffence << "\n";

	SDL_RenderCopyF(Game::renderer, tex, nullptr, &dstRect);

}