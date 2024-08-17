#pragma once

#include "Game.h"

class Text
{

	private:

		TTF_Font* font = nullptr;
		SDL_Texture* fontTex = nullptr;
		SDL_Color fontColor = { 255, 255, 255, 255 };
		SDL_Rect dstRect = { 0, 0, 0, 0 };

		char* displayWord = nullptr;
		int fontSize = 0;
		

	public:

		Text(const char* filename, int Size, int x, int y, const char* word = "###")
		{
			font = TTF_OpenFont(filename, Size);
			fontSize = Size;

			if (font == nullptr)
			{
				std::cout << filename << ": ¦rÅéÅª¨ú¥¢±Ñ\n";
				return;
			}

			dstRect.x = x;
			dstRect.y = y;
			dstRect.h = fontSize;
			dstRect.w = (int)strlen(word) * (fontSize / 2);

			setWord(word);

		}
		~Text() {}

		void setWord(const char* word)
		{
			displayWord = (char*)word;

			dstRect.w = (int)strlen(displayWord) * (fontSize / 2);
			
			SDL_Surface* temp = TTF_RenderText_Blended(font, displayWord, fontColor);
			SDL_DestroyTexture(fontTex);
			fontTex = SDL_CreateTextureFromSurface(Game::renderer, temp);
			SDL_FreeSurface(temp);
			
		}

		void render()
		{
			SDL_RenderCopy(Game::renderer, fontTex, nullptr, &dstRect);
		}

};