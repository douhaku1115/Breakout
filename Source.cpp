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
#define PADDLE_DEFAULT_WIDTH 48
#define BLOCK_COLUMN_MAX 14
#define BLOCK_ROW_MAX  8
#define BALL_X_MAX_SPEED .5f
#define FONT_HEIGHT 32
#define FONT_WEIGHT 4
using namespace glm;
#define BALL_MAX 2
#define BLOCK_HEIGHT 12
#define SE_WAIT_MAX 6

enum {
	LEVEL_DEFAULT,
	LEVEL_HIT_4,
	LEVEL_HIT_12,
	LEVEL_HIT_ORANGE,
	LEVEL_HIT_RED,
	LEVEL_HIT_MAX
};
ivec2 windowSize = { 800, 600 };
Rect blocks[BLOCK_ROW_MAX][BLOCK_COLUMN_MAX];
bool keys[256];
Ball balls[BALL_MAX];
Rect rect1 = Rect(vec2(100, 100), vec2(100, 200));
Rect rect2 = Rect(vec2(windowSize.x/2, windowSize.y / 2), vec2(200, 100));
Rect field;
Ball ball = { 8 };
Paddle paddle = { PADDLE_DEFAULT_WIDTH };
int turn = 1;
int score = 0;
int seCount;
int seWait;
int level;
float powerTbl[] = {
	2,	//LEVEL_DEFAULT,
	3,	//LEVEL_HIT_12,
	4,	//LEVEL_HIT_ORANGE,
	5,	//LEVEL_HIT_RED,
	6	//LEVEL_HIT_MAX

};

int getBlockCount() {
	int n= 0;
	for (int i = 0; i < BLOCK_ROW_MAX; i++)
		for (int j = 0; j <BLOCK_COLUMN_MAX; j++) {
			if (!blocks[i][j].isDead)
				n++;
	}
	return n;
		
}
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

	glColor3ub(0x00, 0x00, 0x00);//GLubyte red, GLubyte green, GLubyte blue);
	field.draw();

	unsigned char colors[][3] = {
		{0xff,0x00,0x00},
		{0xff,0x80,0x00},
		{0x00,0xff,0x00},
		{0xff,0xff,0x00}
	};

	for (int i = 0; i < BLOCK_ROW_MAX; i++)
		for (int j = 0; j < BLOCK_COLUMN_MAX; j++) {
			if (blocks[i][j].isDead)
				continue;		
			int colorId = i / 2;
			unsigned char* color = colors[colorId];
			glColor3ub(color[0], color[1], color[2]);
			//glColor3ub(0xff, 0xff, 0xff);
			//blocks[i][j].draw();
			glRectfv(
				(GLfloat*) & (blocks[i][j].m_position + vec2(1, 1)),//const GLfloat *v1, 
				(GLfloat*) & (blocks[i][j].m_position + blocks[i][j].m_size - vec2(1, 1))//const GLfloat *v2);
			);
		};

	glColor3ub(0xff, 0xff, 0xff);
	ball.draw();

	glColor3ub(0x00, 0x80, 0xff);
	paddle.draw();

	glColor3ub(0xff, 0xff, 0xff);
	float x = field.m_position.x,
		y = field.m_position.y;

	fontBegin();
	fontSetHeight(FONT_HEIGHT);
	fontSetWeight(FONT_WEIGHT);
	fontSetPosition(x,y );
	fontDraw("1");
	fontSetPosition(x + field.m_size.x / 2, y);
	fontDraw("%d",turn);
	y += fontGetHeight() + fontGetWeight();
	x += fontGetWidth();
	fontSetPosition(x, y);
	{
		static unsigned int frame;
		
		if((++frame/15)%2==0)
			fontDraw("%03d", score);
	}
	fontSetPosition(x+field.m_size.x/2, y);
	fontDraw("000");
	y += fontGetHeight() + fontGetWeight();

	fontSetHeight(16);
	fontSetWeight(2);
	x = field.m_position.x;
	y += BLOCK_HEIGHT * BLOCK_ROW_MAX;
	fontSetPosition(x, y);
	fontDraw("seCount:%d\n",seCount);
	fontDraw("seWait:%d\n", seWait);
	fontDraw("level:%d\n", level);
	fontDraw("blockCout:%d\n", getBlockCount());
	fontEnd();

	glutSwapBuffers();
};

void idle(void) {
	if (seCount > 0) {
		if (--seWait <= 0) {
			seCount--;
			seWait = SE_WAIT_MAX;

			audioStop();
			audioFreq(440 / 2);
			audioPlay();
		}
	}
	audioUpdate();
	

	//level = 1;
	ball.m_power = powerTbl[level];
	ball.update();

	if ((ball.m_position.x < field.m_position.x)
		|| (ball.m_position.x >= field.m_position.x + field.m_size.x)) {
		audioStop();
		audioFreq(440);
		audioPlay();
		ball.m_position = ball.m_lastPosition;
		ball.m_speed.x *= -1;
	}
	if ((ball.m_position.y < field.m_position.y)
		|| (ball.m_position.y >= field.m_position.y + field.m_size.y)) {
		audioStop();
		audioFreq(440);
		audioPlay();
		ball.m_position = ball.m_lastPosition;
		ball.m_speed.y *= -1;

		
	}

	if (paddle.intersectBall(ball)) {
		audioStop();
		audioFreq(440 * 2);
		audioPlay();
		ball.m_position = ball.m_lastPosition;
		ball.m_speed.y *= -1;

		float padleCenterX = paddle.m_position.x + paddle.m_width / 2;
		float sub = ball.m_position.x - padleCenterX;
		float subMax = paddle.m_width / 2;
		ball.m_speed.x = sub / subMax * 1;

	}

	for (int i = 0; i < BLOCK_ROW_MAX; i++)
		for (int j = 0; j < BLOCK_COLUMN_MAX; j++) {
			if (blocks[i][j].isDead)
				continue;
			if (blocks[i][j].intersect(ball.m_position)) {
				blocks[i][j].isDead = true;
				ball.m_speed.y *= -1;

				int colorId = 3 - (i / 2);
				int s= 1 + 2 * colorId;

				audioStop();
				audioFreq(440/2);
				audioPlay();

				seCount += s - 1;
				seWait = SE_WAIT_MAX;
				score += s;
				{
					int n = getBlockCount();
					int blockCountMax = BLOCK_COLUMN_MAX * BLOCK_ROW_MAX;
					if ((n <= blockCountMax - 4) && (level < LEVEL_HIT_4))
						level = LEVEL_HIT_4;
					if ((n <= blockCountMax - 12) && (level < LEVEL_HIT_12))
						level = LEVEL_HIT_12;
					if ((colorId == 2) && (level < LEVEL_HIT_ORANGE))
						level = LEVEL_HIT_ORANGE;
				}if ((colorId == 3) && (level < LEVEL_HIT_RED))
					level = LEVEL_HIT_RED;
			}
	}
	/*for (int i = 0; i < BALL_MAX; i++){
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
	if (keys['w']) rect1.m_position.y -= f;
	if (keys['s']) rect1.m_position.y += f;
	if (keys['a']) rect1.m_position.x -= f;
	if (keys['d']) rect1.m_position.x += f;
	*/
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

	ball.m_lastPosition = ball.m_position = vec2(field.m_position.x, field.m_position.y + field.m_size.y / 2);
	ball.m_speed = vec2(1, 1)*3.f;
	ball.m_power = 1;

	paddle.m_position = vec2(field.m_position.x+ field.m_size.x/2, field.m_position.y + field.m_size.y - 48);

	vec2 blocksSize = vec2(field.m_size.x / BLOCK_COLUMN_MAX, BLOCK_HEIGHT);
	float y = field.m_position.y+(FONT_HEIGHT+FONT_WEIGHT)*2;
	for(int i=0;i<BLOCK_ROW_MAX;i++)
		for (int j = 0; j < BLOCK_COLUMN_MAX; j++) {
			blocks[i][j].m_position = vec2(
				field.m_position.x+field.m_size.x * j / BLOCK_COLUMN_MAX,
				y+i * blocksSize.y);
			blocks[i][j].m_size = blocksSize;
		}

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
	paddle.m_position.x = x;
	paddle.m_position.x = max(paddle.m_position.x,field.m_position.x);
	paddle.m_position.x = max(paddle.m_position.x, field.m_position.x);
	paddle.m_position.x = min(paddle.m_position.x, field.m_position.x+field.m_size.x-paddle.m_width);

	printf("passoveMotion::x:%d y:%d\n",x,y);
}
int main(int argc, char* argv[]) {
	audioInit();
	audioDecay(.9f);
	audioWaveform(AUDIO_WAVEFORM_PULSE_50);
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
	reshape(windowSize.x,windowSize.y);
	glutMainLoop();
}