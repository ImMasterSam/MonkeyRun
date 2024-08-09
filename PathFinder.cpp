#include "PathFinder.h"

const int dir[8][2] = { {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };

PathFinder::PriorityQ PathFinder::pq;
std::vector<std::vector<SDL_Point>> PathFinder::previous;
std::vector<std::vector<bool>> PathFinder::visited;

int PathFinder::sx, PathFinder::sy;
int PathFinder::ex, PathFinder::ey;

SDL_Point PathFinder::Search(int EnemyXPos, int EnemyYPos, int PlayerXPos, int PlayerYPos, Map* map)
{

	// Reset Variables
	decltype(pq)().swap(pq);
	previous.assign(ROW, std::vector<SDL_Point>(COL, { -1, -1 }));
	visited.assign(ROW, std::vector<bool>(COL, false));

	sx = EnemyYPos / CELL_SIZE;
	sy = EnemyXPos / CELL_SIZE;

	ex = PlayerYPos / CELL_SIZE;
	ey = PlayerXPos / CELL_SIZE;

	node cur{}, nxt{};

	cur.pos.x = sx;
	cur.pos.y = sy;
	cur.gValue = 0;
	cur.hValue = distanceWithEnd(sx, sy);
	cur.calculate();

	visited[sx][sy] = true;
	pq.push(cur);

	while (!pq.empty())
	{
		cur = pq.top();
		pq.pop();

		if (cur.pos.x == ex && cur.pos.y == ey)
		{	
			return traceNextPos(ex, ey);
		}

		for (int d = 0; d < 8; d++)
		{
			nxt = cur;

			nxt.pos.x += dir[d][0];
			nxt.pos.y += dir[d][1];

			if ((nxt.pos.x >= 0 && nxt.pos.x < ROW) &&
				(nxt.pos.y >= 0 && nxt.pos.y < COL) &&
				map->iswalkable(nxt.pos.y * CELL_SIZE + (CELL_SIZE / 2), nxt.pos.x * CELL_SIZE + (CELL_SIZE / 2)) &&
				!visited[nxt.pos.x][nxt.pos.y])
			{

				nxt.gValue += 1;
				nxt.hValue = distanceWithEnd(nxt.pos.x, nxt.pos.y);
				nxt.calculate();
				previous[nxt.pos.x][nxt.pos.y] = cur.pos;

				visited[nxt.pos.x][nxt.pos.y] = true;
				pq.push(nxt);

			}


		}
	}

	return {0, 0};

}

// Private Functions

float PathFinder::distanceWithEnd(int xpos, int ypos) {

	return sqrt(pow(xpos - ex, 2) + pow(ypos - ey, 2));

}

SDL_Point PathFinder::traceNextPos(int xpos, int ypos)
{

	if ((previous[xpos][ypos].x == sx && previous[xpos][ypos].y == sy) ||
		(previous[xpos][ypos].x == -1 && previous[xpos][ypos].y == -1))
	{
		//std::cout << "[" << xpos << ", " << ypos << "]\n";
		return getGoalPos(xpos, ypos);
	}
	else
	{
		//std::cout << "[" << xpos << ", " << ypos << "]\n";
		return traceNextPos(previous[xpos][ypos].x, previous[xpos][ypos].y);
	}

}

SDL_Point PathFinder::getGoalPos(int xpos, int ypos)
{
	SDL_Point res{};

	res.y = xpos * CELL_SIZE + (CELL_SIZE / 2);
	res.x = ypos * CELL_SIZE + (CELL_SIZE / 2);

	return res;

}