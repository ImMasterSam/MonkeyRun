#pragma once

#include "Game.h"
#include "Text.h"

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::steady_clock::time_point Time;

class FPSAnalyzer
{
	private:
		
		int frameNum;
		std::vector<long long> timeBuffer; // us (·L¬í)

		Time startTime, endTime;

		Text* display = nullptr;

	public:

		FPSAnalyzer(int sec)
		{
			frameNum = 1000 / FPS * sec;
			timeBuffer.assign(frameNum, 0);

			display = new Text("minecraft_font.ttf", 30, 0, 0);
		}
		~FPSAnalyzer() {}

		void frameStart()
		{
			startTime = Clock::now();
		}

		void frameEnd()
		{
			endTime = Clock::now();
			long long duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

			timeBuffer[Game::gameTick % frameNum] = duration;

		}

		void render()
		{

			long long total = 0;
			for (long long& t : timeBuffer)
			{
				total += t;
			}

			total = 1000000.0 / ((float)total / frameNum);
			display->setWord(std::to_string(total).c_str());

			display->render();

		}

};