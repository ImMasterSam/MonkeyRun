#pragma once

#include "Game.h"
#include "Map.h"
#include <queue>

class PathFinder
{


	private:

		struct node
		{

			SDL_Point pos;

			float gValue;
			float hValue;
			float fValue;

			void calculate()
			{
				fValue = gValue + hValue;
			}
		};

		struct cmp
		{

			bool operator () (const node& a, const node& b) {

				if (a.fValue != b.fValue)
				{
					return a.fValue > b.fValue;
				}
				else
				{
					return a.hValue > b.hValue;
				}

			}

		};

		using PriorityQ = std::priority_queue<node, std::vector<node>, cmp>;

		static std::vector<std::vector<SDL_Point>> previous;
		static std::vector<std::vector<bool>> visited;
		static PriorityQ pq;

		static int sx, sy;
		static int ex, ey;

		static float distanceWithEnd(int xpos, int ypos);
		static SDL_Point traceNextPos(int xpos, int ypos);
		static SDL_Point getGoalPos(int xpos, int ypos);

	public:

		static SDL_Point Search(int EnemyXPos, int EnemyYPos, int PlayerXPos, int PlayerYPos, Map* map);

};