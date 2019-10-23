#pragma once
#include "glm/glm.hpp"
#include "Ball.h"

using namespace glm;
struct Paddle {
	vec2 m_position;
	float m_height;
	
	bool intersectBall(Ball const& ball);
	void draw();
};