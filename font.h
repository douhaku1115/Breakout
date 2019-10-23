#pragma once
#include <stdlib.h>

#include "glut.h"
#include "glm/glm.hpp"
#define FONT_DEFAULT_WIDTH (80.f)
#define FONT_DEFAULT_HEIGHT (100.f)
void fontBegin();
void fontEnd();

void fontSetPosition(float _x, float _y);
void fontSetHeight(float _size);
float fontGetHeight();
float fontGetWeight();

float fontGetWeightMin();
float fontGetWeightMax();
void fontSetWeight(float _weight);
//void fontSetColor(unsigned char _red, unsigned char _green, unsigned char _blue);
void fontDraw(const char *_format,...);

