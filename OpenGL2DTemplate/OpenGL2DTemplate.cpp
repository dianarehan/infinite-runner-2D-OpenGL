#include <stdlib.h>
#include <glut.h>
#include <math.h>	
#include<string>

//sizes of the screen
int xCord = 1000;
int yCord = 500;

//player information
int playerLife = 5;
bool isJumping = false;
bool isDucking = false;
bool isVulnerable = false;
int vulnerableTimer = 0;
const int maxVulnerableTime = 10;
int playerScore = 0;

//player position and movement
float playerX = 100.0f;
float playerY = 70.0f;
float jumpHeight = 180.0f;
float gravity = 8.0f;
float jumpVelocity = 16.0f;
float currentJumpHeight = 0.0f;

//player boundaries
int playerWidth = 40;
int playerHeight = 60; 

//transform manipulation
float heartSpacing = 40.0f;
int boundaryW1 = 20;
int boundaryW2 = 40;
int upperHeight = yCord - 70;

//obstacle properties
float obstacleX = xCord;
float obstacleY = 70.0f;
float obstacleWidth = 30.0f;
float obstacleHeight = 30.0f;

//collectibles properties
float collectibleX = xCord;
float collectibleY = playerHeight+jumpHeight;
float collectibleWidth = 20.0f;
float collectibleHeight = 20.0f;
bool isCollectibleActive = true;
float starRotationAngle = 0.0f;

// Colors
const float obstacleColorR = 0.196f;
const float obstacleColorG = 0.659f;
const float obstacleColorB = 0.616f;

//game data
bool isGameOver = false;
bool isTimeUp = false;

// Timer variables
int timer = 60;
int timerFrameCount = 0; // Frame count to update the timer every second
const int framesPerSecond = 60;

void drawStar(float x, float y, float size) {
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 5; ++i) {
		float angle1 = i * 2.0f * 3.14159f / 5.0f;
		float angle2 = (i + 2) * 2.0f * 3.14159f / 5.0f;
		glVertex2f(x, y);
		glVertex2f(x + size * cos(angle1), y + size * sin(angle1));
		glVertex2f(x + size * cos(angle2), y + size * sin(angle2));
	}
	glEnd();
}

void drawCollectible() {
	if (isCollectibleActive) {
		glPushMatrix();
		glTranslatef(collectibleX + collectibleWidth / 2, collectibleY + collectibleHeight / 2, 0.0f);
		glRotatef(starRotationAngle, 0.0f, 0.0f, 1.0f);
		glTranslatef(-(collectibleX + collectibleWidth / 2), -(collectibleY + collectibleHeight / 2), 0.0f);

		glColor3f(1.0f, 1.0f, 0.0f); // Yellow color for the star
		drawStar(collectibleX + collectibleWidth / 2, collectibleY + collectibleHeight / 2, collectibleWidth / 2);

		glPopMatrix();
	}
}

void updateCollectible() {
	if (isCollectibleActive) {
		collectibleX -= 5.0f; // Move collectible to the left

		if (collectibleX + collectibleWidth < 0) {
			collectibleX = xCord;
			collectibleY = 70.0f + (rand() % 2) * playerHeight;
		}
		starRotationAngle += 2.0f;
		if (starRotationAngle >= 360.0f) {
			starRotationAngle -= 360.0f;
		}
	}
}

bool checkCollectibleCollision() {
	float playerLeft = playerX - playerWidth / 2;
	float playerRight = playerX + playerWidth / 2;
	float playerTop = playerY + playerHeight;
	float playerBottom = playerY;

	float collectibleLeft = collectibleX;
	float collectibleRight = collectibleX + collectibleWidth;
	float collectibleTop = collectibleY + collectibleHeight;
	float collectibleBottom = collectibleY;

	return !(playerLeft > collectibleRight || playerRight < collectibleLeft || playerTop < collectibleBottom || playerBottom > collectibleTop);
}

bool checkCollision() {
	float playerLeft = playerX - playerWidth / 2;
	float playerRight = playerX + playerWidth / 2;
	float playerTop = playerY + playerHeight;
	float playerBottom = playerY;

	float obstacleLeft = obstacleX;
	float obstacleRight = obstacleX + obstacleWidth;
	float obstacleTop = obstacleY + obstacleHeight;
	float obstacleBottom = obstacleY;

	return !(playerLeft > obstacleRight || playerRight < obstacleLeft || playerTop < obstacleBottom || playerBottom > obstacleTop);
}

void renderGameOver() {
	glColor3f(1.0f, 0.0f, 0.0f);
	glRasterPos2f(xCord / 2 - 50, yCord / 2);
	const char* message = "Game Lose";
	for (const char* c = message; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
}

void renderTimer() {
	glColor3f(1.0f, 1.0f, 1.0f); // White color for the timer
	glRasterPos2f(900, upperHeight); // Position below the score
	std::string timerText = "Time: " + std::to_string(timer);
	const char* timerCStr = timerText.c_str();
	for (const char* c = timerCStr; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
}

void renderGameEnd() {
	glColor3f(0.1137f, 0.6118f, 0.0980f);
	glRasterPos2f(xCord / 2 - 50, yCord / 2);
	char message[50];
	sprintf(message, "Game End,\n\n with a score of %d", playerScore);
	for (char* c = message; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
}

void renderScore() {
	glColor3f(1.0f, 1.0f, 1.0f); // White color for the score
	glRasterPos2f(450, upperHeight);
	std::string scoreText = "Score: " + std::to_string(playerScore);
	const char* scoreCStr = scoreText.c_str();
	for (const char* c = scoreCStr; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
}

void updateObstacle() {
	obstacleX -= 5.0f; // Move obstacle to the left

	if (obstacleX + obstacleWidth < 0) {
		obstacleX = xCord;
		obstacleY = 70.0f + (rand() % 2) * 60;
	}
}

void drawObstacle() {
	glColor3f(obstacleColorR, obstacleColorG, obstacleColorB);

	glBegin(GL_QUADS);
	glVertex2f(obstacleX, obstacleY);
	glVertex2f(obstacleX + obstacleWidth, obstacleY);
	glVertex2f(obstacleX + obstacleWidth, obstacleY + obstacleHeight);
	glVertex2f(obstacleX, obstacleY + obstacleHeight);
	glEnd();
	glColor3f(0x49 / 255.0f, 0x91 / 255.0f, 0x8d / 255.0f);
	glBegin(GL_TRIANGLES);
	glVertex2f(obstacleX, obstacleY + obstacleHeight);
	glVertex2f(obstacleX + obstacleWidth / 2, obstacleY + obstacleHeight + 20.0f);
	glVertex2f(obstacleX + obstacleWidth, obstacleY + obstacleHeight);
	glEnd();
}

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
	if (isGameOver) return;

	timerFrameCount++;
	if (timerFrameCount >= framesPerSecond) {
		timer--;
		timerFrameCount = 0;
	}

	if (timer <= 0) {
		isTimeUp = true;
		glutPostRedisplay();
		return;
	}

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

	updateObstacle();
	updateCollectible();

	if (!isVulnerable && checkCollision()) {
		playerLife--;
		if (playerLife <= 0) isGameOver = true;

		isVulnerable = true;
		vulnerableTimer = maxVulnerableTime;
		obstacleX += 200.0f; // Move obstacle further to the right
	}

	if (checkCollectibleCollision()) {
		isCollectibleActive = false;
		playerScore ++;
		// Reset collectible position
		collectibleX = xCord;
		collectibleY = 70.0f + (rand() % 2) * 60;
		isCollectibleActive = true;
	}

	// Handle vulnerable state
	if (isVulnerable) {
		vulnerableTimer--;
		if (vulnerableTimer <= 0) {
			isVulnerable = false;
		}
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

	if (isTimeUp) renderGameEnd();
	
	else if (isGameOver) renderGameOver();
	
	else {
		drawUpperBoundary();
		drawLowerBoundary();
		initializeHealth(playerLife);
		drawObstacle();
		drawCollectible();
		drawPlayer(playerX, playerY);
		renderScore();
		renderTimer();
	}
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