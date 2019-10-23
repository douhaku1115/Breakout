#include <stdio.h>
#include <stdlib.h>
#include "glut.h"
#include "glm/glm.hpp"
#include "font.h"
#include "Rect.h"
#include "Ball.h"
#include <time.h>
#include "Paddle.h"
#include "audio.h"

using namespace glm;
#define BALL_MAX 2
ivec2 windowSize = { 800, 600 };

bool keys[256];
Paddle paddle;
Ball balls[BALL_MAX];
Rect rect1 = Rect(vec2(100, 100), vec2(100, 200));
Rect rect2 = Rect(vec2(windowSize.x/2, windowSize.y / 2), vec2(200, 100));
Rect field;

void display(void) {
	
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);//(GLenum mode);
	glLoadIdentity();
	gluOrtho2D(
		0,//GLdouble left,
		windowSize.x,//GLdouble right,
		windowSize.y,	//GLdouble bottom,
		0);	//GLdouble top);
	glMatrixMode(GL_MODELVIEW);//GLenum mode
	glLoadIdentity();
	
	glColor3ub(0xff, 0xff, 0xff);
	glRectf(
		field.m_position.x - 8, 0, //GLfloat x1, GLfloat y1,
		field.m_position.x + field.m_size.x + 8, windowSize.y//GLfloat x2, GLfloat y2);
	); 
	
	glColor3ub(0x00,0x00,0x00);//GLubyte red, GLubyte green, GLubyte blue);
	field.draw();

	fontBegin();
	fontSetHeight(FONT_DEFAULT_HEIGHT);
	fontSetPosition(0, 0 );
	//fontDraw("A");
	fontEnd();

	glutSwapBuffers();
};

void idle(void){
	audioUpdate();
	for (int i = 0; i < BALL_MAX; i++){
		balls[i].update();

		if (paddle.intersectBall(balls[i])) {
			balls[i].m_position = balls[i].m_lastPosition;
			balls[i].m_speed.x *= -1;
		}

		if (balls[i].m_position.y < 0) {
			balls[i].m_position = balls[i].m_lastPosition;
			balls[i].m_speed.y = fabs(balls[i].m_speed.y);
		}
		if (balls[i].m_position.y >= windowSize.y) {
			balls[i].m_position = balls[i].m_lastPosition;
			balls[i].m_speed.y = -fabs(balls[i].m_speed.y);
		}
		if (balls[i].m_position.x >= windowSize.x) {
			balls[i].m_position = balls[i].m_lastPosition;
			balls[i].m_speed.x = -fabs(balls[i].m_speed.x);
		}
		if (balls[i].m_position.x < 0) {
			balls[i].m_position = balls[i].m_lastPosition;
			balls[i].m_speed.x = fabs(balls[i].m_speed.x);
		}
	}
	float f = 2;
	if (keys['w']) rect1.m_position.y -= f;
	if (keys['s']) rect1.m_position.y += f;
	if (keys['a']) rect1.m_position.x -= f;
	if (keys['d']) rect1.m_position.x += f;
	
	glutPostRedisplay();
}
void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, 0);
}
void reshape(int width, int height) {
	printf("rehape:width:%d height:%d\n", width, height);
	glViewport(0, 0, width, height);
	//GLint x, GLint y, GLsizei width, GLsizei height);
	windowSize = ivec2(width, height);

	float frameHeight = 16;
	field.m_size.y = windowSize.y - frameHeight;
	field.m_size.x = field.m_size.y;
	field.m_position.x = (windowSize.x - field.m_size.x) / 2;
	field.m_position.y = frameHeight;
}

void keyboard(unsigned char key, int x, int y) {
	if (key == 0x1b)
		exit(0);
	printf("keyboard:\'%c\'(%#x)\n", key, key);
	keys[key] = true;
	
}
void keyboardUp(unsigned char key, int x, int y) {
	printf("keyboardUp:\'%c\'(%#x)\n", key, key);
	keys[key] = false;
}
void passiveMotion(int x, int y) {
	paddle.m_position = vec2(x, y);
	printf("passoveMotion::x:%d y:%d\n",x,y);
}
int main(int argc, char* argv[]) {
	audioInit();
	srand(time(NULL));

	

	glutInit(&argc, argv);

	glutInitDisplayMode(GL_DOUBLE);
	glutInitWindowPosition(640,0);
	glutInitWindowSize(windowSize.x, windowSize.y);
	glutCreateWindow("a");
	glutDisplayFunc(display);
	//glutTimerFunc(0, timer, 0);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);//void (GLUTCALLBACK *func)(int width, int height));
	glutKeyboardFunc(keyboard);//GLUTCALLBACK *func)(unsigned char key, int x, int y));
	glutKeyboardUpFunc(keyboardUp);//void (GLUTCALLBACK *func)(unsigned char key, int x, int y));
	//glutPassiveMotionFunc(passiveMotion); //void (GLUTCALLBACK * func)(int x, int y));
    //glutMotionFunc(motion); void (GLUTCALLBACK * func)(int x, int y));
	glutIgnoreKeyRepeat(GL_TRUE);//int ignore
	glutPassiveMotionFunc(passiveMotion);//void (GLUTCALLBACK *func)(int x, int y));
	glutMainLoop();
}