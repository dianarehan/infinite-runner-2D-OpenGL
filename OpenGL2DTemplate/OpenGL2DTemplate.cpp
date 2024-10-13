#include <stdlib.h>
#include <glut.h>
#include <math.h>	

//sizes of the screen
int xCord = 1000;
int yCord = 500;

//player information
int playerLife = 5;

void drawHeart(float x, float y) {
	glBegin(GL_POLYGON);
	for (float angle = 0; angle < 2 * 3.14159; angle += 0.1) {
		float dx =  16 * sin(angle) * sin(angle) * sin(angle);
		float dy = 13 * cos(angle) - 5 * cos(2 * angle) - 2 * cos(3 * angle) - cos(4 * angle);
		glVertex2f(x + dx, y + dy);
	}
	glEnd();
}

void initializeHealth(int numHearts) {
	float startX = 20.0f; 
	float startY = 40.0f;

	for (int i = 0; i < numHearts; i++) {
		drawHeart(startX + (i * 40.0f), startY);
	}
}

void Display() {
	glClear(GL_COLOR_BUFFER_BIT);

	initializeHealth(playerLife);

	glFlush();
}


void main(int argc, char** argr) {
	glutInit(&argc, argr);

	glutInitWindowSize(xCord, yCord);
	glutInitWindowPosition(150, 150);

	glutCreateWindow("Game");
	glutDisplayFunc(Display);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	gluOrtho2D(0.0, xCord, 0.0, yCord);

	glutMainLoop();
}