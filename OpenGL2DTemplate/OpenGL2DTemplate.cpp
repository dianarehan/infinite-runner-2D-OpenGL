#include <stdlib.h>
#include <glut.h>
#include <math.h>	

//sizes of the screen
int xCord = 1000;
int yCord = 500;

//player information
int playerLife = 5;


//transform manipulation
float heartSpacing = 40.0f;
int boundaryW1 = 50;
int boundaryW2 = 70;

void drawCircle(float cx, float cy, float radius, float r, float g, float b) {
	glColor3f(r, g, b);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++) {
		float theta = i * (3.14159 / 180.0f);
		float x = radius * cos(theta);
		float y = radius * sin(theta);
		glVertex2f(cx + x, cy + y);
	}
	glEnd();
}

void drawPlayer(float x, float y) {
	// Draw player's head (circle)
	drawCircle(x, y + 50, 20, 1.0f, 0.8f, 0.6f); // Head

	// Draw player's body (quad)
	glColor3f(0.0f, 0.0f, 1.0f); // Blue body
	glBegin(GL_QUADS);
	glVertex2f(x - 20, y + 30);
	glVertex2f(x + 20, y + 30);
	glVertex2f(x + 20, y - 20);
	glVertex2f(x - 20, y - 20);
	glEnd();

	// Draw player's left arm (triangle)
	glColor3f(1.0f, 0.0f, 0.0f); // Red arms
	glBegin(GL_TRIANGLES);
	glVertex2f(x - 20, y + 20);
	glVertex2f(x - 40, y);
	glVertex2f(x - 20, y - 10);
	glEnd();

	// Draw player's right arm (triangle)
	glBegin(GL_TRIANGLES);
	glVertex2f(x + 20, y + 20);
	glVertex2f(x + 40, y);
	glVertex2f(x + 20, y - 10);
	glEnd();

	// Draw player's legs (quad for simplicity)
	glColor3f(0.0f, 1.0f, 0.0f); // Green legs
	glBegin(GL_QUADS);
	glVertex2f(x - 15, y - 20);
	glVertex2f(x - 5, y - 20);
	glVertex2f(x - 5, y - 40);
	glVertex2f(x - 15, y - 40);

	glVertex2f(x + 5, y - 20);
	glVertex2f(x + 15, y - 20);
	glVertex2f(x + 15, y - 40);
	glVertex2f(x + 5, y - 40);
	glEnd();
}

void drawQuad(float x1, float y1, float x2, float y2, float r, float g, float b) {
	glColor3f(r, g, b);

	glBegin(GL_QUADS);
	glVertex2f(x1, y1);
	glVertex2f(x2, y1);
	glVertex2f(x2, y2);
	glVertex2f(x1, y2);
	glEnd();
}

void drawHeart(float x, float y) {
	glColor3f(1.0f, 0.0f, 0.0f);//red hearts
	glBegin(GL_POLYGON);
	for (float angle = 0; angle < 2 * 3.14159; angle += 0.1) {
		float dx =  16 * sin(angle) * sin(angle) * sin(angle);
		float dy = 13 * cos(angle) - 5 * cos(2 * angle) - 2 * cos(3 * angle) - cos(4 * angle);
		glVertex2f(x + dx, y + dy);
	}
	glEnd();
}

void drawUpperBoundary() {
	drawQuad(50, yCord - boundaryW1, 200, yCord - boundaryW2, 1.0f, 0.0f, 0.0f);
	drawQuad(250, yCord - boundaryW1, 400, yCord - boundaryW2, 0.0f, 1.0f, 0.0f);
	drawQuad(450, yCord - boundaryW1, 600, yCord - boundaryW2, 0.0f, 0.0f, 1.0f);
	drawQuad(650, yCord - boundaryW1, 800, yCord - boundaryW2, 1.0f, 1.0f, 0.0f);
}

void drawLowerBoundary() {
	drawQuad(50, boundaryW1, 200, boundaryW2, 1.0f, 0.5f, 0.0f);
	drawQuad(250, boundaryW1, 400, boundaryW2, 0.0f, 1.0f, 1.0f);
	drawQuad(450, boundaryW1, 600, boundaryW2, 1.0f, 0.0f, 1.0f);
	drawQuad(650, boundaryW1, 800, boundaryW2, 0.5f, 0.5f, 0.5f);
}

void initializeHealth(int numHearts) {
	float startX = 50.0f;  // Starting x position
	float startY = yCord - 100.0f; // Adjust to be below the upper boundary
	glClearColor(1, 0, 0, 0.0f);
	for (int i = 0; i < numHearts; i++) {
		drawHeart(startX + (i * heartSpacing), startY);
	}
}

void Display() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);//bg color

	glClear(GL_COLOR_BUFFER_BIT);
	drawUpperBoundary();
	drawLowerBoundary();
	initializeHealth(playerLife);

	drawPlayer(500, 250);
	glFlush();
}


void main(int argc, char** argr) {
	glutInit(&argc, argr);

	glutInitWindowSize(xCord, yCord);
	glutInitWindowPosition(150, 150);

	glutCreateWindow("Game");
	glutDisplayFunc(Display);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	gluOrtho2D(0.0, xCord, 0.0, yCord);

	glutMainLoop();
}