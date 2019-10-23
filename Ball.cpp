#include "Ball.h"
#include "glut.h"

void Ball::update() {
	m_lastPosition = m_position;
	m_position += m_speed;

}
void Ball::draw() {
	glPushMatrix();
	{
		glTranslatef(m_position.x, m_position.y, 0);//GLfloat x, GLfloat y, GLfloat z);
		glScalef(m_radius,m_radius,0);//GLfloat x, GLfloat y, GLfloat z);
		glutSolidSphere(
			1,	//GLdouble radius, 
			16,	//GLint slices,
			16);	//GLint stacks);
	}
	glPopMatrix();

}