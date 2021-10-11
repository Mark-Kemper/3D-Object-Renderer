//Incomplete and not in use


#include "Editor.h"
#include <iostream>
#include <stdlib.h>

using namespace std;

Editor::Editor(Screen inputtedScreen) {
	screen = inputtedScreen;
}

void Editor::drawLine(int* firstPoint, int* secondPoint) {

	//first Point and secondPoint are 2 width arrays, first entry being x coord and second being y coord

	//slope = delta y / delta x = secondY - firstY / secondX - firstX

	bool finished = false;
	int lastXCoord = -1;
	int lastYCoord = -1;
	int intDenom = (secondPoint[0] - firstPoint[0]);
	int intSlope;
	int slopeRemain;

	if (intDenom == 0) {
		intSlope = 1000;
		slopeRemain = 0;
	}
	else {
		intSlope = (secondPoint[1] - firstPoint[1]) / intDenom;
		slopeRemain = (secondPoint[1] - firstPoint[1]) % intDenom;
	}
	int horizontalCount = 0;
	bool doVert = false;

	if (intSlope == 0 && slopeRemain != 0) {
		intSlope = 1 / intSlope;
		doVert = true;
	}

	cout << "slope:" << endl;
	cout << intSlope << endl;
	screen.setPixel(firstPoint[0], firstPoint[1], 255, 20, 20);
	screen.setPixel(secondPoint[0], secondPoint[1], 0, 250, 250);

	int horz = 0;
	int vert = 0;

	int num = 50;
	while (!finished) {
		if (horz > num || vert > num)
		{
			finished = true;
		}
		if (doVert) {
			if (horizontalCount >= intSlope) {
				vert++;
				screen.setPixel(firstPoint[0] + horz, firstPoint[1] + vert, 255, 255, 0);
				lastXCoord = firstPoint[0] + horz;
				lastYCoord = firstPoint[1] + vert;
				horizontalCount = 0;
				if (horz % slopeRemain == 0) {
					horizontalCount = -1;
				}
			}
			else {
				horizontalCount++;
				horz++;
				screen.setPixel(firstPoint[0] + horz, firstPoint[1] + vert, 255, 255, 0);
				lastXCoord = firstPoint[0] + horz;
				lastYCoord = firstPoint[1] + vert;
			}

			if (lastXCoord == secondPoint[0] && lastYCoord == secondPoint[1]) {
				finished = true;
			}
		}

		else {
			if (horizontalCount >= intSlope) {
				horz++;
				screen.setPixel(firstPoint[0] + horz, firstPoint[1] + vert, 255, 255, 0);
				lastXCoord = firstPoint[0] + horz;
				lastYCoord = firstPoint[1] + vert;
				horizontalCount = 0;
			}
			else {
				horizontalCount++;
				vert++;
				screen.setPixel(firstPoint[0] + horz, firstPoint[1] + vert, 255, 255, 0);
				lastXCoord = firstPoint[0] + horz;
				lastYCoord = firstPoint[1] + vert;
			}

			if (lastXCoord == secondPoint[0] && lastYCoord == secondPoint[1]) {
				//	screen.setPixel(firstPoint[0] + horz, firstPoint[1] + vert, 0, 250, 255);

				finished = true;
			}
		}

	}
}