#pragma once

#include "Game.h"

class Sound
{

	private:

		const float MAX_DIS = 10 * CELL_SIZE;

		Mix_Music* sound = nullptr;

	public:

		Sound(const char* filename)
		{
			sound = Mix_LoadMUS(filename);

			setVolume(0);
		}

		~Sound()
		{
			Mix_FreeMusic(sound);
			Mix_Quit();
		}

		void update()
		{

			if (Mix_PlayingMusic() == 0)
			{
				Mix_PlayMusic(sound, -1);
			}

		}

		void setVolume(float dis)
		{

			float volume = std::min(100.0f, ((MAX_DIS - dis) / MAX_DIS) * 100);
			Mix_VolumeMusic((MIX_MAX_VOLUME * volume) / 100);

		}

};