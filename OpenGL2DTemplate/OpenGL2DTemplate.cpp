#include <stdlib.h>
#include <glut.h>
#include <math.h>	
#include<string>
#include <iostream>
#include <windows.h>
#include <tchar.h>
using namespace std;

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

//game speed modifier
float speedMultiplier = 1.0f;
const float speedIncreaseRate = 0.06f;
const float initialSpeedMultiplier = 1.0f; //when the player hits smth i want to reset the speed

//aesthetics
const int numClouds = 5;
float cloudPositions[numClouds][2];
float cloudSpeeds[numClouds];

// Power-up properties
float potionX = xCord;
float potionY = 70.0f;
float potionWidth = 20.0f;
float potionHeight = 20.0f;
bool isPotionActive = false;
int potionTimer = 0;
const int potionSpawnInterval = 15 * framesPerSecond;
const int potionEffectDuration = 2;

float shieldX = xCord;
float shieldY = 70.0f;
float shieldWidth = 20.0f;
float shieldHeight = 20.0f;
bool isShieldActive = false;
int shieldTimer = 0;
const int shieldSpawnInterval = 10* framesPerSecond;
const int shieldEffectDuration =2;

//player colors new
float playerColorR = 1.0f;
float playerColorG = 1.0f;
float playerColorB = 1.0f;
const int colorChangeDuration = 5 * framesPerSecond; // 5 seconds
int colorChangeTimer = 0;
bool isColorChanged = false;



bool checkPotionCollision() {
	float playerLeft = playerX - playerWidth / 2;
	float playerRight = playerX + playerWidth / 2;
	float playerTop = playerY + playerHeight;
	float playerBottom = playerY;

	float potionLeft = potionX;
	float potionRight = potionX + potionWidth;
	float potionTop = potionY + potionHeight;
	float potionBottom = potionY;

	return !(playerLeft > potionRight || playerRight < potionLeft || playerTop < potionBottom || playerBottom > potionTop);
}

bool checkShieldCollision() {
	float playerLeft = playerX - playerWidth / 2;
	float playerRight = playerX + playerWidth / 2;
	float playerTop = playerY + playerHeight;
	float playerBottom = playerY;

	float shieldLeft = shieldX;
	float shieldRight = shieldX + shieldWidth;
	float shieldTop = shieldY + shieldHeight;
	float shieldBottom = shieldY;

	return !(playerLeft > shieldRight || playerRight < shieldLeft || playerTop < shieldBottom || playerBottom > shieldTop);
}
void drawCircle(float cx, float cy, float radius, float r, float g, float b);

void initClouds() {
	float minSpacing = xCord / numClouds; // Minimum spacing between clouds
	float lastX = 0.0f; // Track the last x position

	for (int i = 0; i < numClouds; ++i) {
		float xPos = lastX + minSpacing + (rand() % static_cast<int>(minSpacing / 2));
		cloudPositions[i][0] = xPos; // Random x position with spacing
		cloudPositions[i][1] = yCord - (rand() % (yCord / 2)); // Random y position
		cloudSpeeds[i] = 1.0f + static_cast<float>(rand() % 3); // Random speed between 1.0 and 3.0
		lastX = xPos; // Update last x position
	}
}

void drawCloud(float x, float y, float size) {
	glClearColor(211 / 255.0f, 220 / 255.0f, 230 / 255.0f, 1.0f);
	drawCircle(x, y, size, 1.0f, 1.0f, 1.0f);
	drawCircle(x + size * 0.5f, y + size * 0.5f, size * 0.75f, 1.0f, 1.0f, 1.0f);
	drawCircle(x + size, y, size, 1.0f, 1.0f, 1.0f);
}

void drawClouds() {
	for (int i = 0; i < numClouds; ++i) {
		drawCloud(cloudPositions[i][0], cloudPositions[i][1], 50.0f);
	}
}

void updateClouds() {
	for (int i = 0; i < numClouds; ++i) {
		cloudPositions[i][0] -= cloudSpeeds[i] * speedMultiplier; // Move clouds to the left
		if (cloudPositions[i][0] < -100) { // Reset cloud to the right side
			cloudPositions[i][0] = xCord + 100;
			cloudPositions[i][1] = yCord - (rand() % (yCord / 2)); // Random y position
		}
	}
}

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

void drawQuad(float x1, float y1, float x2, float y2, float r, float g, float b) {
	glColor3f(r, g, b);

	glBegin(GL_QUADS);
	glVertex2f(x1, y1);
	glVertex2f(x2, y1);
	glVertex2f(x2, y2);
	glVertex2f(x1, y2);
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
		collectibleX -= 5.0f * speedMultiplier; // Move collectible to the left

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
	glRasterPos2f(xCord / 2 - 100, yCord / 2);
	char message[50];
	sprintf(message, "Game Lose,\n\n with a score of %d", playerScore);
	for (char* c = message; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
}

void renderTimer() {
	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos2f(900, upperHeight); // Position below the score
	std::string timerText = "Time: " + std::to_string(timer);
	const char* timerCStr = timerText.c_str();
	for (const char* c = timerCStr; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
}

void renderGameEnd() {
	glColor3f(0.1137f, 0.6118f, 0.0980f);
	glRasterPos2f(xCord / 2 - 100, yCord / 2);
	char message[50];
	sprintf(message, "Game End,\n\n with a score of %d", playerScore);
	for (char* c = message; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
}

void renderScore() {
	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos2f(450, upperHeight);
	std::string scoreText = "Score: " + std::to_string(playerScore);
	const char* scoreCStr = scoreText.c_str();
	for (const char* c = scoreCStr; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
}

void updateObstacle() {
	obstacleX -= 5.0f  *speedMultiplier; // Move obstacle to the left

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

void drawPotion(float x, float y, float size) {
	if (isPotionActive) {
		float x = potionX;
		float y = potionY;
		float width = potionWidth;
		float height = potionHeight;
		glColor3f(0.0f, 0.5f, 1.0f); // Light blue color
		glBegin(GL_QUADS);
		glVertex2f(x - size / 4, y);
		glVertex2f(x + size / 4, y);
		glVertex2f(x + size / 4, y + size / 2);
		glVertex2f(x - size / 4, y + size / 2);
		glEnd();

		// Draw the bottle neck (quad)
		glColor3f(0.0f, 0.5f, 1.0f); // Light blue color
		glBegin(GL_QUADS);
		glVertex2f(x - size / 8, y + size / 2);
		glVertex2f(x + size / 8, y + size / 2);
		glVertex2f(x + size / 8, y + size * 0.75f);
		glVertex2f(x - size / 8, y + size * 0.75f);
		glEnd();

		// Draw the bottle cap (triangle)
		glColor3f(0.5f, 0.0f, 0.0f); // Dark red color
		glBegin(GL_TRIANGLES);
		glVertex2f(x - size / 8, y + size * 0.75f);
		glVertex2f(x + size / 8, y + size * 0.75f);
		glVertex2f(x, y + size);
		glEnd();

		// Draw the liquid inside the bottle (quad)
		glColor3f(0.0f, 1.0f, 0.0f); // Green color
		glBegin(GL_QUADS);
		glVertex2f(x - size / 4 + 2, y + 2);
		glVertex2f(x + size / 4 - 2, y + 2);
		glVertex2f(x + size / 4 - 2, y + size / 2 - 2);
		glVertex2f(x - size / 4 + 2, y + size / 2 - 2);
		glEnd();
	}
}

void updatePotion() {
	if (isPotionActive) {
		potionX -= 5.0f * speedMultiplier; // Move potion to the left

		if (potionX + potionWidth < 0) {
			isPotionActive = false;
		}
	}
	else {
		potionTimer++;
		if (potionTimer >= potionSpawnInterval) {
			potionX = xCord;
			potionY = 70.0f + (rand() % 2) * playerHeight;
			isPotionActive = true;
			potionTimer = 0;
		}
	}
}

void updateShield() {
	if (isShieldActive) {
		shieldX -= 5.0f * speedMultiplier; // Move shield to the left

		if (shieldX + shieldWidth < 0) {
			isShieldActive = false;
		}
	}
	else {
		shieldTimer++;
		if (shieldTimer >= shieldSpawnInterval) {
			shieldX = xCord;
			shieldY = 70.0f + (rand() % 2) * playerHeight;
			isShieldActive = true;
			shieldTimer = 0;
		}
	}
}

void drawShield() {
	if (isShieldActive) {
		glColor3f(0.0f, 0.0f, 1.0f); // Blue color for the shield
		drawQuad(shieldX, shieldY, shieldX + shieldWidth, shieldY + shieldHeight, 0.0f, 0.0f, 1.0f);
	}
}

void updatePlayer() {
	if (isGameOver || isTimeUp) return;

	timerFrameCount++;
	if (timerFrameCount >= framesPerSecond) {
		timer--;
		timerFrameCount = 0;
		speedMultiplier += speedIncreaseRate;
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
	updateClouds();
	updatePotion();
	updateShield();

	if (!isVulnerable && checkCollision()) {
		playerLife--;
		if (playerLife <= 0) isGameOver = true;

		isVulnerable = true;
		vulnerableTimer = maxVulnerableTime;
		obstacleX += 250.0f; // Move obstacle further to the right
		collectibleX += 100;
		speedMultiplier = initialSpeedMultiplier;
	}

	if (checkCollectibleCollision()) {
		isCollectibleActive = false;
		playerScore ++;
		// Reset collectible position
		collectibleX = xCord+20;
		collectibleY = 80.0f + (rand() % 2) * 60;
		isCollectibleActive = true;
	}
	if (checkPotionCollision()) {
		isPotionActive = false;
		isColorChanged = true;
		colorChangeTimer = colorChangeDuration;
		// Change player colors
		playerColorR = static_cast<float>(rand()) / RAND_MAX;
		playerColorG = static_cast<float>(rand()) / RAND_MAX;
		playerColorB = static_cast<float>(rand()) / RAND_MAX;
	}

	if (checkShieldCollision()) {
		isShieldActive = false;
		isVulnerable = true;
		vulnerableTimer = shieldEffectDuration;
	}

	if (isColorChanged) {
		colorChangeTimer--;
		if (colorChangeTimer <= 0) {
			isColorChanged = false;
			// Reset player colors to default
			playerColorR = 1.0f;
			playerColorG = 1.0f;
			playerColorB = 1.0f;
		}
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
	if (isColorChanged) drawCircle(x, y + playerHeight - 10, 20, playerColorR, playerColorG,playerColorB); // Head
	else drawCircle(x, y + playerHeight - 10, 20, 1.0f, 0.8f, 0.6f); // Head
	
	// Draw player's body (quad)
	if (isColorChanged) glColor3f(playerColorR, playerColorG, playerColorB);
	else glColor3f(0.0f, 0.0f, 1.0f); // Blue body
	glBegin(GL_QUADS);
	glVertex2f(x - 20, y + playerHeight - 30);
	glVertex2f(x + 20, y + playerHeight - 30);
	glVertex2f(x + 20, y - 20);
	glVertex2f(x - 20, y - 20);
	glEnd();

	// Draw player's left arm (triangle)
	if (isColorChanged) glColor3f(playerColorR, playerColorG, playerColorB);
	else
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
	if (isColorChanged) glColor3f(playerColorR, playerColorG, playerColorB);
	else
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
	float startX = 40.0f;
	float startY = yCord - 100.0f;
	float heartWidth = 16.0f;
	float heartHeight = 13.0f;
	float frameWidth = numHearts * heartSpacing;
	float frameHeight = heartHeight * 2;

	glColor3f(237 / 255.0f, 194 / 255.0f, 0 / 255.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(startX - 10, startY - 5);
	glVertex2f(startX + frameWidth -7, startY - 5);
	glVertex2f(startX + frameWidth -7, startY + frameHeight+11);
	glVertex2f(startX - 10, startY + frameHeight +11);
	glEnd();

	// Draw hearts
	for (int i = 0; i < numHearts; i++) {
		drawHeart(startX+10 + (i * heartSpacing), startY+20);
	}
}

void Display() {
	glClearColor(21 / 255.0f, 54 / 255.0f, 82 / 255.0f, 1.0f);//bg color

	glClear(GL_COLOR_BUFFER_BIT);
	drawClouds();
	if (isTimeUp) renderGameEnd();
	
	else if (isGameOver) renderGameOver();
	
	else {
		drawUpperBoundary();
		drawLowerBoundary();
		initializeHealth(playerLife);
		drawObstacle();
		drawCollectible();
		drawPotion(100.0f, 100.0f, 50.0f);
		drawShield();
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
	initClouds();
	glutMainLoop();
}


/*#include <iostream>
#include <windows.h>
#include <tchar.h>
using namespace std;

int main() {
bool played = PlaySound(_T("C:\\Users\\merna\\Downloads\\Eng VII\\Graphics\\OpenGL Ass1\\OpenGL2DTemplate\\file_example_WAV_5MG.wav"), NULL,  SND_ASYNC);

	printf("Success or not %i", played);
	Sleep(5000); // Wait for 2 seconds

	return 0;
}*/