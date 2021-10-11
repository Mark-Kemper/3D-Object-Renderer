#pragma once
#include "Screen.h"
#include <iostream>

class Editor
{
public:
	Screen screen;
public:
	Editor(Screen);
	void drawLine(int*, int*);
};
