#pragma once
#include "glm/glm.hpp"
#include "glut.h"

using namespace glm;



struct Rect {
	vec2 m_position;
	vec2 m_size;
	bool isDead
		;
	Rect();
	Rect(vec2 const& _position, vec2 const& _size);
	void draw();
	bool intersect(vec2 const& _point);
	bool intersect(Rect const& _rect);
};