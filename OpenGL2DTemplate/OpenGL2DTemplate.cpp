#include <stdlib.h>
#include <glut.h>
#include <math.h>	

//sizes of the screen
int xCord = 1000;
int yCord = 500;

//player information
int playerLife = 5;
bool isJumping = false;
bool isDucking = false;

//player position and movement
float playerX = 100.0f;
float playerY = 70.0f;
float jumpHeight = 150.0f;
float gravity = 5.0f;
float jumpVelocity = 10.0f;
float currentJumpHeight = 0.0f;

//player boundaries
int playerWidth = 40;
int playerHeight = 60; 

//transform manipulation
float heartSpacing = 40.0f;
int boundaryW1 = 20;
int boundaryW2 = 40;

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

void updatePlayer() {
	if (isJumping) {
		playerY += jumpVelocity;
		currentJumpHeight += jumpVelocity;
		if (currentJumpHeight >= jumpHeight) {
			isJumping = false;
		}
	}
	else if (playerY > 70.0f) {
		playerY -= gravity;
		currentJumpHeight -= gravity;
	}

	if (isDucking) {
		playerHeight = 40;
	}
	else {
		playerHeight = 60;
	}
	glutPostRedisplay();
}

void keyDown(unsigned char key, int x, int y) {
	switch (key) {
	case ' ':
		if (playerY == 70.0f) {
			isJumping = true;
			currentJumpHeight = 0;
		}
		break;
	case 's':
		isDucking = true;
		break;
	}
}

void keyUp(unsigned char key, int x, int y) {
	if (key == 's') {
		isDucking = false;
	}
}

void drawPlayer(float x, float y) {
	// Draw player's head (circle)
	drawCircle(x, y + playerHeight - 10, 20, 1.0f, 0.8f, 0.6f); // Head

	// Draw player's body (quad)
	glColor3f(0.0f, 0.0f, 1.0f); // Blue body
	glBegin(GL_QUADS);
	glVertex2f(x - 20, y + playerHeight - 30);
	glVertex2f(x + 20, y + playerHeight - 30);
	glVertex2f(x + 20, y - 20);
	glVertex2f(x - 20, y - 20);
	glEnd();

	// Draw player's left arm (triangle)
	glColor3f(1.0f, 0.0f, 0.0f); // Red arms
	glBegin(GL_TRIANGLES);
	glVertex2f(x - 20, y + playerHeight - 40);
	glVertex2f(x - 40, y + playerHeight - 60);
	glVertex2f(x - 20, y + playerHeight - 70);
	glEnd();

	// Draw player's right arm (triangle)
	glBegin(GL_TRIANGLES);
	glVertex2f(x + 20, y + playerHeight - 40);
	glVertex2f(x + 40, y + playerHeight - 60);
	glVertex2f(x + 20, y + playerHeight - 70);
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
	drawQuad(0, yCord - boundaryW1, 250, yCord - boundaryW2, 1.0f, 0.0f, 0.0f);
	drawQuad(250, yCord - boundaryW1, 500, yCord - boundaryW2, 0.0f, 1.0f, 0.0f);
	drawQuad(500, yCord - boundaryW1, 750, yCord - boundaryW2, 0.0f, 0.0f, 1.0f);
	drawQuad(750, yCord - boundaryW1, 1000, yCord - boundaryW2, 1.0f, 1.0f, 0.0f);
}

void drawLowerBoundary() {
	drawQuad(0, boundaryW1, 250, boundaryW2, 1.0f, 0.5f, 0.0f);
	drawQuad(250, boundaryW1, 500, boundaryW2, 0.0f, 1.0f, 1.0f);
	drawQuad(500, boundaryW1, 750, boundaryW2, 1.0f, 0.0f, 1.0f);
	drawQuad(750, boundaryW1, 1000, boundaryW2, 0.5f, 0.5f, 0.5f);
}

void initializeHealth(int numHearts) {
	float startX = 30.0f;  // Starting x position
	float startY = yCord - 70.0f; // Adjust to be below the upper boundary
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

	drawPlayer(playerX, playerY);
	glFlush();
}

void Timer(int value) {
	updatePlayer();
	glutTimerFunc(16, Timer, 0);
}

void main(int argc, char** argr) {
	glutInit(&argc, argr);

	glutInitWindowSize(xCord, yCord);
	glutInitWindowPosition(150, 150);

	glutCreateWindow("Game");
	glutDisplayFunc(Display);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutTimerFunc(0, Timer, 0);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	gluOrtho2D(0.0, xCord, 0.0, yCord);

	glutMainLoop();
}