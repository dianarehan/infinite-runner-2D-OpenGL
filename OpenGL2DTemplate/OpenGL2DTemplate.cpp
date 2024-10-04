#include <glut.h>

int xCord = 500;
int yCord = 250;

int passX = 0;
int passY = 0;

int actX = 0;
int actY = 0;

int keyX = 0;
int keyY = 0;
int keyC = 0;

int speX = 0;
int speY = 0;
int speC = 0;

double backRed = 0;
double backGreen = 0;
double backBlue = 0.5;

int redD = 1;
int greenD = 1;
int blueD = 1;

void Display() {

	glClearColor(backRed, backGreen, backBlue, 0.0f);//the background color is changed in the timer function after 1 sec by changing these parameters.
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();//draws a red pixel at the origin
	glColor3f(1, 0, 0);
	glTranslated(passX, passY, 0);//updates mouse's motion coordinates 
	glBegin(GL_POINTS);
	glVertex3d(0, 0, 0);
	glEnd();
	glPopMatrix();//bn7ot push w pop 3shan nefsel between kol object and their properties 3shan translation t2sar fe awel point bas

	glPushMatrix(); //draws a yellow pixel @ 10,20
	glColor3f(1, 1, 0);
	glTranslated(actX, actY, 0);
	glBegin(GL_POINTS);
	glVertex3d(10, 20, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();  //draws a magenta pixel @ 10,120
	glColor3f(1, keyC, 1);
	glTranslated(keyX, keyY, 0);
	glBegin(GL_POINTS);
	glVertex3d(10, 120, 0);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, speC, 1); //draws a blue pixel @ 10,220
	glTranslated(speX, speY, 0);
	glBegin(GL_POINTS);
	glVertex3d(10, 220, 0);
	glEnd();
	glPopMatrix();

	glFlush();
}

void passM(int x, int y)//passive motion function takes 2 parameters the x and y positions of the mouse
//it allows the user to make use of the mouse motion without clicking on the mouse buttons
{
	passX = x;//it moves the red square center to the mouse location.
	passY = yCord-y;
	glutPostRedisplay();//ay haga ablaha btget computed abl ma yerga3 lel display method call
}

void actM(int button, int state, int x, int y)//mouse function takes 4 parameters: button: which button has been clicked (GLUT_RIGHT_BUTTON or GLUT_LEFT_BUTTON),
//state wether the button is clicked or released (GLUT_UP or GLUT_DOWN)
// x and y are the position of the mouse cursor
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)//if the left button has been clicked then translate the square to the mouse position
	{
		actX = x - 10;
		actY = 250 - y - 20;
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)//if the right button has been clicked, translate  the square to the origin (0,0)
	{
		actX = 0;
		actY = 0;
	}

	glutPostRedisplay();//redisplay to update the screen with the new paraeters
}

void key(unsigned char k, int x, int y)//keyboard function, takes 3 parameters
// k is the key pressed from the keyboard
// x and y are mouse postion when the key was pressed.
//you may need to use the position of the mouse when pressing the key when you aim or smth
{
	if (k == 'd')//if the letter d is pressed, then the object will be translated in the x axis by 10 (moving to the right).
		keyX += 10;
	if (k == 'a')//if the letter a is pressed, then the object will be translated in the x axis by -10 (moving to the left).
		keyX -= 10;
	if (k == 'w')//if the letter w is pressed, then the object will be translated in the y axis by 10. (moving upwords)
		keyY += 10;
	if (k == 's')//if the letter w is pressed, then the object will be translated in the y axis by -10. (moving downwords)
		keyY -= 10;
	if (k == 'q')//if the letter q is pressed, then the object color is changed
		keyC = 1;
	glutPostRedisplay();//redisplay to update the screen with the changes
}

void keyUp(unsigned char k, int x, int y)//keyboard up function is called whenever the keyboard key is released
{
	if (k == 'q')//if the letter q is pressed, then the object will go back to it's original color.
		keyC = 0;
	glutPostRedisplay();//redisplay to update the screen with the changed
}

void spe(int k, int x, int y)// keyboard special key function takes 3 parameters
// int k: is the special key pressed such as the keyboard arrows the f1,2,3 and so on
{
	if (k == GLUT_KEY_RIGHT)//if the right arrow is pressed, then the object will be translated in the x axis by 10. (moving right)
		speX += 10;
	if (k == GLUT_KEY_LEFT)//if the left arrow is pressed, then the object will be translated in the x axis by -10. (moving left)
		speX -= 10;
	if (k == GLUT_KEY_UP)//if the up arrow is pressed, then the object will be translated in the y axis by 10. (moving upwords)
		speY += 10;
	if (k == GLUT_KEY_DOWN)//if the down arrow is pressed, then the object will be translated in the y axis by -10. (moving downwords)
		speY -= 10;
	if (k == GLUT_KEY_F1)//if the F1 key is pressed, then the object color will be changed
		speC = 1;
	glutPostRedisplay();//redisplay to update the screen with the changes
}

void speUp(int k, int x, int y)// keyboard special key function is called whenever the special key is released.
{
	if (k == GLUT_KEY_F1)//if the F1 key is released, then the object will return back to it's original color.
		speC = 0;
	glutPostRedisplay();//redisplay to update the screen with the changes
}

void time(int val)//timer animation function, allows the user to pass an integer valu to the timer function.
{
	backRed += 0.1 * redD;							//--
	if (backRed < 0 || backRed>1)					//	|
		redD *= -1;								//	|
	//	|
	backGreen += 0.1 * greenD;						//	|
	if (backGreen < 0 || backGreen>0.5)				//	|		
		greenD *= -1;								//	|this code is called every 1000 ms "ever 1 sec"
	//	|		
	backBlue += 0.1 * blueD;						//	|
	if (backBlue < 0.5 || backBlue>0.75)				//	|
		blueD *= -1;								//	|
	//--
	glutPostRedisplay();						// redraw 		
	glutTimerFunc(1000, time, 0);					//recall the time function after 1000 ms and pass a zero value as an input to the time func.
}

void main(int argc, char** argr) {
	glutInit(&argc, argr);

	glutInitWindowSize(xCord, yCord);
	glutInitWindowPosition(150, 150);

	glutCreateWindow("Control");//window title
	glutDisplayFunc(Display);

	glutPassiveMotionFunc(passM);	//call the passive motion function hovering
	glutMouseFunc(actM);			//call the mouse function detect mouse click

	glutKeyboardFunc(key);			//call the keyboard function
	glutKeyboardUpFunc(keyUp);		//call the keyboard up function

	glutSpecialFunc(spe);			//call the keyboard special keys function
	glutSpecialUpFunc(speUp);		//call the keyboard special keys up function

	glutTimerFunc(0, time, 0);		//call the timer function
	glPointSize(25);				//change the point size to be 25 vertex size is 25*25 pixels


	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	gluOrtho2D(0.0, xCord, 0.0, yCord);

	glutMainLoop();//don't call any method after this line as it will not be reached.
}
