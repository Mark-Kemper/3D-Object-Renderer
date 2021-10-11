#include <iostream>
#include <SDL.h>
#include "Screen.h"
#include <stdlib.h>
#include "Renderer.h"
#include "Editor.h"
#include "vector"

using namespace std;
using namespace renderNameSpace;

int main(int argc, char* argv[])
{
	Screen screen;
	if (screen.init() == false)
	{
		cout << "Failed to initialize" << endl;
	}

	int screenSizeX = screen.xSize;
	int screenSizeY = screen.ySize;
	vector<double> camPos = { -15, 6, 2 };
	vector<double> camOrient = { 0, 0, 0 };
	RenderEngine renderEngine(screenSizeX, screenSizeY, screen, camPos, camOrient);
	int divider = 250;
	int divider2 = 250;
	double xCoord = 11;

	//vector<double> sphere1Pos = { -7, 2.5, 6};
	vector<double> sphere2Pos = { -5, 4, 2 };
	vector<double> sphere3Pos = { -2, 1, 0 };
	vector<double> light1Pos = { -2, 4, 3 };
	vector<double> light2Pos = { -12, 3, 1 };
	vector<double> planeP1 = { xCoord, 2.5 + (double)screenSizeY / divider2, 2 - (double)screenSizeX / divider }; //new double[3];
	vector<double> planeP2 = { xCoord, 2.5 + (double)screenSizeY / divider2, 2 + (double)screenSizeX / divider }; //new double[3];
	vector<double> planeP3 = { xCoord, 2.5 - (double)screenSizeY / divider2, 2 - (double)screenSizeX / divider }; //new double[3];
	vector<double> planeP4 = { xCoord, 2.5 - (double)screenSizeY / divider2, 2 + (double)screenSizeX / divider }; //new double[3];
	//renderEngine.addSphere(sphere1Pos, 2.5, 255, 50, 50); //red
	renderEngine.addSphere(sphere2Pos, 4, 50, 255, 40);
	renderEngine.addSphere(sphere3Pos, 4, 255, 255, 255);
	//renderEngine.addPlane(planeP1, planeP2, planeP3, planeP4, 255, 255, 255);
	//renderEngine.addLightEmitter(light1Pos);
	renderEngine.addLightEmitter(light2Pos);
	//renderEngine.addPlane(planeP1, planeP2, planeP3, planeP4, 200, 200, 50);

	bool continueRendering = true;
	screen.setEntireScreenBlack();
	screen.update();
	int cycleCount = 0;
	int maxCycles = 2000;

	while (continueRendering) {
		if (cycleCount % 5 == 0) { //gradually display progress
			screen.update();
		}

		if (cycleCount >= maxCycles) {
			continueRendering = false;
		}

		for (int j = 0; j < 10000; j++) {
			renderEngine.emitAndRenderLight();
		}

		cycleCount++;

		if (screen.processEvents() == false)
		{
			break;
		}
	}

	screen.close();
	return 0;
}