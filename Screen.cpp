#include "Screen.h"
#include <memory.h>

//SDL setup was done following Cave of Programming tutorial

Screen::Screen() :
	window(NULL), renderer(NULL), texture(NULL), pixelBuffer(NULL), doubleBufferR(NULL), doubleBufferG(NULL), doubleBufferB(NULL) {
}

bool Screen::init() {

	if (SDL_Init(SDL_INIT_VIDEO)) {
		return false;
	}

	window = SDL_CreateWindow("3D Object Renderer",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, xSize, ySize, SDL_WINDOW_SHOWN);

	if (window == NULL) {
		SDL_Quit();
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	texture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, xSize, ySize);

	if (renderer == NULL) {
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	if (texture == NULL) {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}

	pixelBuffer = new Uint32[xSize * ySize];
	doubleBufferR = new double[xSize * ySize];
	doubleBufferG = new double[xSize * ySize];
	doubleBufferB = new double[xSize * ySize];

	return true;
}

bool Screen::processEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			return false;
		}
	}
	return true;
}

void Screen::close() {
	delete[] pixelBuffer;
	delete[] doubleBufferR;
	delete[] doubleBufferG;
	delete[] doubleBufferB;

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Screen::update() {
	SDL_UpdateTexture(texture, NULL, pixelBuffer, xSize * sizeof(Uint32));
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void Screen::setPixel(int xCoord, int yCoord, double R, double G, double B) {
	Uint32 color = 0;
	color += (Uint8)R;
	color <<= 8;
	color += (Uint8)G;
	color <<= 8;
	color += (Uint8)B;
	color <<= 8;
	color += 0xFF;

	pixelBuffer[yCoord * xSize + xCoord] = color;
	doubleBufferR[yCoord * xSize + xCoord] = R;
	doubleBufferG[yCoord * xSize + xCoord] = G;
	doubleBufferB[yCoord * xSize + xCoord] = B;
}

void Screen::incrementPixel(int xCoord, int yCoord, double R, double G, double B)
{
	double initialPixelColorR = doubleBufferR[yCoord * xSize + xCoord];
	double initialPixelColorG = doubleBufferG[yCoord * xSize + xCoord];
	double initialPixelColorB = doubleBufferB[yCoord * xSize + xCoord];
	Uint32 color = 0;

	//overflow checking
	double overflowR = initialPixelColorR + R;
	double overflowG = initialPixelColorG + G;
	double overflowB = initialPixelColorB + B;


	if (overflowR > 255) {
		overflowR = 255;
	}
	if (overflowG > 255) {
		overflowG = 255;
	}
	if (overflowB > 255) {
		overflowB = 255;
	}

	doubleBufferR[yCoord * xSize + xCoord] = overflowR;
	doubleBufferG[yCoord * xSize + xCoord] = overflowG;
	doubleBufferB[yCoord * xSize + xCoord] = overflowB;


	color += (Uint8)overflowR;
	color <<= 8;
	color += (Uint8)overflowG;
	color <<= 8;
	color += (Uint8)overflowB;
	color <<= 8;
	color += 0xFF;

	pixelBuffer[yCoord * xSize + xCoord] = color;
}

void Screen::setEntireScreenBlack()
{
	memset(pixelBuffer, 0, xSize * ySize * sizeof(Uint32));
	memset(doubleBufferR, 0, xSize * ySize * sizeof(double));
	memset(doubleBufferG, 0, xSize * ySize * sizeof(double));
	memset(doubleBufferB, 0, xSize * ySize * sizeof(double));
}