#pragma once

#include <cstdint>
#include "libs/Chip8.h"


class SDL_Window;
class SDL_Renderer;
class SDL_Texture;
class SDL_Surface;


class canvas
{
public:
	canvas(char const* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight);
	~canvas();

	void Update(uint32_t canvas[],int pitch);
	bool ProcessInput(uint8_t* keys);
	void drawGraphics(int windowWidth, int windowHeight, Chip8 mychip8);
	void draw_chip8_screen(unsigned char canvas[]);

private:
	SDL_Window* window{};
	SDL_Renderer* renderer{};
	SDL_Texture* texture{};

	uint32_t pixels[2048];
};
