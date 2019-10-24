#pragma once
#include "glm/glm.hpp"
#include "Ball.h"

using namespace glm;
struct Paddle {
	float m_width;
	vec2 m_position;
	
	bool intersectBall(Ball const& ball);
	void draw();
};