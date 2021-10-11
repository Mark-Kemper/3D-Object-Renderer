#pragma once
#include <SDL.h>

class Screen
{
public:
	const static int xSize = 800;
	const static int ySize = 600;
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	Uint32* pixelBuffer;
	double* doubleBufferR;
	double* doubleBufferG;
	double* doubleBufferB;

public:
	Screen(); //constructor
	bool init();
	bool processEvents();
	void close();
	void update();
	void setPixel(int, int, double, double, double);
	void incrementPixel(int, int, double, double, double);
	void setEntireScreenBlack();
};