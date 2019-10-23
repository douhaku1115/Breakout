#include "Paddle.h"
#include "glut.h"

bool Paddle::intersectBall(Ball const& _ball) {
	if (
		(
		(_ball.m_position.x >= m_position.x)
			&& (_ball.m_lastPosition.x < m_position.x))
		||
		(
		(_ball.m_position.x < m_position.x)
			&& (_ball.m_lastPosition.x >= m_position.x)
			)
		)
	{
		if (
			(_ball.m_position.y >= m_position.y)
			&& (_ball.m_position.y < m_position.y + m_height)
			)return true;
	}return false;
}


void Paddle::draw() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	GLfloat range[2];
	
	glGetFloatv(
		GL_LINE_WIDTH_RANGE,//GLenum pname,
		range); //GLfloat *params);
	glLineWidth(range[1]);//GLfloat width);
	glBegin(GL_LINES);
	{
		glVertex2f(m_position.x, m_position.y);
		vec2 v = m_position + vec2(0, m_height);
		glVertex2f(v.x, v.y);
	}
	glEnd();

	glPopAttrib();
}