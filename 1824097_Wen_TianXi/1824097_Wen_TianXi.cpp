// Title: Assignment1: Birthday Card
// Student Name: Tianxi Wen
// Student ID: 1824097

#define FREEGLUT_STATIC
#include <math.h>
#include <GL/freeglut.h>
#include <iostream>
#include <windows.h>
#include <stdlib.h>

/*           Global variables                  */
const GLfloat pi = 3.1416f;	//approximation for Pi
GLfloat step = 5;  // declare step 
int time_interval=50;  // declare refresh interval in ms
int ballonX, ballonY;	//used in timer and drawer balloon, simulate balloons flying
int i;		//used in timer, increased one in each time interval 
int cheerCount;		//used in timer, time count for push flower
bool cheerFlag = false;		//flag for push flower
bool candleMode = true;		//flag for candle fire
GLfloat alpha = 0.0;		//value for alpha in glColor4f
float left = 0, right = 400, bottom = 0, top = 400;		//value for gluOthor2d
int MAX_CHAR = 128;		//declear max characters of list that used to draw greatings
int snowCount;

/*                  timer                                       */
void OnTimer(int value) {
	//balloon part
	if (i % 2 == 0) {				//increase / decrease X postion of balloons each time interval
		ballonX += rand() % 3;
		i++;
	}
	else {
		ballonX -= rand() % 3;
		i++;
	}
	ballonY += step;			//increase Y position of balloons each time interval

	//cheer/push flower part
	if (cheerFlag) {			//when cheerFlag change to true, maintains the state for 7 time interval then change to false again
		cheerCount++;
		if (cheerCount > 7) {
			cheerFlag = false;
			cheerCount = 0;
		}
	}

	//color alpha part
	if(i > 75)
		alpha += 0.02;			//let the card gradually appear after balloons flying over

	//snow part
	snowCount -= 5;				//control snow to drop dowm
	if (snowCount <= -200)		//the first row will overlap the third row of snow when refresh
		snowCount = 0;

	glutTimerFunc(time_interval, OnTimer, 1);
}

/*                   keyboard interaction                        */
void keyboard_input(unsigned char key, int x, int y) {
	//press q or Q to exit program
	if (key == 'q' || key == 'Q')	
		exit(0);
	//press c to release pull flower
	if (key == 'c' || key == 'C')	
		cheerFlag = true;
}

/*                   mouse interaction                          */
void mouse_input(int button, int state, int x, int y) {  // mouse interaction    
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON && candleMode == true)
		candleMode = false;
	else if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON && candleMode == false)
		candleMode = true;
} 

/*                  special key interaction                     */
void processSpecialKeys(int key, int xx, int yy) {
	float fraction = 2.0f;
	switch (key) {
		//Camera move left
	case GLUT_KEY_LEFT:
		left -= fraction;
		right -= fraction;
		break;
		//Camera move right
	case GLUT_KEY_RIGHT:
		left += fraction;
		right += fraction;;
		break;
		// Camera move up
	case GLUT_KEY_UP:
		bottom += fraction;
		top += fraction;
		break;
		// Camera move down
	case GLUT_KEY_DOWN:
		bottom -= fraction;
		top -= fraction;
		break;
		// Zoom out
	case GLUT_KEY_ALT_L:
		left += fraction;
		right -= fraction;
		bottom += fraction;
		top -= fraction;
		break;
		// Zoom in
	case GLUT_KEY_ALT_R:
		left -= fraction;
		right == fraction;
		bottom -= fraction;
		top += fraction;
		break;
	}
}

void when_in_mainloop() { // idle callback function
	glutPostRedisplay(); // force OpenGL to redraw the current window
}


/*                  function to draw curves with the shape of heart                      */
void drawCurves() {
	glColor4f(1, 0, 0.4, alpha);
	GLfloat ctrlPoints[7][3] = {										//control points for Bessel Curves
		{147,340,0},
		{117,385,0},{70,150,0},{200,150,0},{280,150,0},{290,385,0},
		{240,340,0}
	};
	glMap1f(GL_MAP1_VERTEX_3, 0.0, 100.0, 3, 7, &ctrlPoints[0][0]);		//draw descender of heart with Bessel Curves 
	glEnable(GL_MAP1_VERTEX_3);
	glLineWidth(3.5);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i <= 100; i++)
	{
		glEvalCoord1f((GLfloat)i);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);			//draw ascender of heart with circle
	for (int j = 210; j <= 330; j++) {
		glVertex2f(195 + 50 * cos(pi / 180 * j), 345 + 40 * sin(pi / 180 * j));
	}
	glEnd();
}


/*                        DDA algorithm to draw lines                                      */
void DDALine(int x0, int y0, int x1, int y1)
{
	int dx, dy, n;
	float xinc, yinc, x, y;
	dx = x1 - x0;
	dy = y1 - y0;
	if (abs(dx) > abs(dy) )
		n = abs(dx);
	else
		n = abs(dy);
	xinc = (float)dx / n;
	yinc = (float)dy / n;
	x = (float)x0;
	y = (float)y0;
	for (int i = 1; i <= n; i++)
	{
		glVertex2f(x, y);
		x += xinc; // Increment in x direction
		y += yinc; // Increment in y direction
	}
}

/*              draw the largest piece of cake in drawCake()                     */
void cakepiece() {
	glColor4f(0.847, 0.439, 0.576, 0.5);
	int fillet = 17;	//the radius for the round corner
	glEnable(GL_DEPTH_TEST);		//enable depth test, so that creme will be displayed in front of cake

	//draw creme
	glBegin(GL_POLYGON);
	glVertex2f(265, 103);
	glColor4f(1, 1, 1, alpha);
	for (float i = 265; i < 385; i += 1) {		//use sine shape to represent creme
		glVertex2f(i, 103 + 7 * sin((i - 265) / 5 * pi));
	}
	glVertex2f(385, 103);
	glVertex2f(385, 113);
	for (int i = 0; i <= 90; i++) {				//the round corner
		glVertex2f(368 + fillet * cos(pi / 180 * i), 113 + fillet * sin(pi / 180 * i));
	}
	glVertex2f(368, 130);
	glVertex2f(282, 130);
	for (int i = 90; i <= 180; i++) {			//the round corner
		glVertex2f(282 + fillet * cos(pi / 180 * i), 113 + fillet * sin(pi / 180 * i));
	}
	glVertex2f(265, 130);
	glEnd();

	//draw cake
	glColor4f(0.847, 0.439, 0.576, alpha);
	glBegin(GL_POLYGON);
	glVertex2f(265, 30);
	glVertex2f(385, 30);
	glVertex2f(385, 113);
	for (int i = 0; i <= 90; i++) {				//the round corner
		glVertex2f(368 + fillet * cos(pi / 180 * i), 113 + fillet * sin(pi / 180 * i));
	}
	glVertex2f(368, 130);
	glVertex2f(282, 130);
	for (int i = 90; i <= 180; i++) {			//the round corner
		glVertex2f(282 + fillet * cos(pi / 180 * i), 113 + fillet * sin(pi / 180 * i));
	}
	glVertex2f(265, 130);
	glEnd();
}


/*                        function to draw a three-tiered cake                           */
void drawCake() {
	//the plane to put cake on it
	glColor4f(0, 0, 0, alpha);
	glPointSize(3);
	glBegin(GL_POINTS);
	DDALine(250, 30, 400, 30);				//use DDA algorithm to draw the line
	glEnd();

	//largest piece of cake
	cakepiece();

	//piece2 of cake
	glPushMatrix();
	glTranslatef(65, 105, 0);	//use translate to put second piece on first piece 
	glScalef(0.8, 0.8, 0);		//use scale to shrink the size
	cakepiece();		//copy the first piece of cake
	glPopMatrix();

	//piece3 of cake
	glPushMatrix();
	glTranslatef(117, 190, 0);
	glScalef(0.64, 0.64, 0);
	cakepiece();
	glPopMatrix();
}


/*                     8 midpoints method to draw circle                          */
void MidEightCircle(int cx, int cy, int r)  //cx, cy is the coordination for the center point
{
	GLdouble x = 0;
	GLdouble y = 0;

	float f;
	x = 0; y = r; f = 1.25 - r;		//x,y is the distance from points on the circle to center point, f is the varible to judge point position
	while (x <= y)
	{
		glPointSize(2);
		glBegin(GL_POINTS);			//draw 8 symmetry points each time
		glVertex2f(cx + x, y + cy);
		glVertex2f(cx + x, cy - y);
		glVertex2f(cx - x, cy + y);
		glVertex2f(cx - x, cy - y);
		glVertex2f(cx + y, x + cy);
		glVertex2f(cx - y, x + cy);
		glVertex2f(cx + y, cy - x);
		glVertex2f(cx - y, cy - x);
		glEnd();
		if (f < 0) f += 0.2 * x + 0.03;						//f < 0 means the next point is inside the circle, vice versa
		else { f += 0.2 * (x - y) + 0.05; y = y - 0.1; }
		x = x + 0.1;
	}
}


/*               function to draw the trees                       */
void drawTrees() {
	glColor4f(0.627, 0.321, 0.176, alpha);
	glPointSize(2);
	glBegin(GL_POINTS);						//DDA to draw lines that represent soil
	DDALine(0, 30, 100, 30);
	DDALine(5, 28, 98, 32);
	DDALine(2, 32, 101, 29);
	DDALine(0, 29, 99, 31);
	glEnd();

	glColor4f(0.87, 0.721, 0.529, alpha);			//tree trunk
	glRectf(55, 30, 65, 100);

	glEnable(GL_DEPTH_TEST);		//allow upper part of tree display in front of lower part 
	glPushMatrix();
	glScalef(0.63, 1, 1);
	glTranslatef(35.5, 0, 0);
	MidEightCircle(60, 345, 15);
	glPopMatrix();
	glColor4f(1, 1, 0, alpha);
	glBegin(GL_POLYGON);
	for (int i = 0; i <= 360; i++) {				//the circle decoration
		glVertex2f(60 + 10 * cos(pi / 180 * i), 345 + 15 * sin(pi / 180 * i));
	}
	glEnd();

	glColor4f(0.6, 1, 0.6, alpha);				//topside of free
	glBegin(GL_TRIANGLES);
	glVertex2f(95, 250);
	glColor4f(0.89, 1, 0.9, alpha);
	glVertex2f(60, 350);
	glVertex2f(25, 250);
	glEnd();

	glColor4f(0.498, 1, 0, alpha);				//midiem of tree
	glBegin(GL_POLYGON);
	glVertex2f(110, 175);
	glVertex2f(80, 255);
	glColor4f(0.6, 1, 0.6, alpha);
	glVertex2f(40, 255);
	glVertex2f(10, 175);
	glEnd();

	glColor4f(0.233, 0.545, 0.333, alpha);		//buttom of tree
	glBegin(GL_POLYGON);
	glVertex2f(120, 100);
	glVertex2f(95, 180);
	glColor4f(0.498, 1, 0, alpha);
	glVertex2f(25, 180);
	glVertex2f(0, 100);
	glEnd();
}


/*               function to draw candles and fire on it, fire interact with mouse click                    */
void drawCandles(bool mode) {
	for (int i = 0; i < 3; i++) {	//draw candles 3 times with the gap of 23
		glColor4f(0.95, 0.1, 0.1, alpha);
		glRectf(298 + i * 23, 273, 308 + i * 23, 326);	//the candles

		glColor4f(0, 0, 0, alpha);
		glLineWidth(0.8);
		glPolygonMode(GL_FRONT, GL_LINE);		//use line mode to draw frame of candles
		glRectf(298 + i * 23, 273, 308 + i * 23, 326);
		glPolygonMode(GL_FRONT, GL_FILL);

		glBegin(GL_LINES);		//the wicks
		glVertex2f(303 + i * 23, 326);
		glVertex2f(303 + i * 23, 346);
		glEnd();

		if (mode) {		//the fire, displayed for default, click left buttom of mouse to blow off / light up
			glBegin(GL_POLYGON);
			glColor4f(1, 0, 0, alpha);
			glVertex2f(303 + i * 23 + rand() % 2, 366 + rand() % 2);	//use rand() to simulating the swaying effect 
			glColor4f(1, 0.8, 0, alpha);										//Gradient color in smooth mode
			glVertex2f(298 + i * 23 + rand() % 2, 351 + rand() % 2);
			glVertex2f(303 + i * 23, 346);
			glVertex2f(308 + i * 23 + rand() % 2, 351 + rand() % 2);
			glEnd();
		}
	}
}


/*                  function to draw cheers and pull flower, interact with keyboard input                 */
void drawCheers(bool cheerFlag) {
	glColor4f(1, 0.521, 0.2, alpha);
	glBegin(GL_POLYGON);
	glVertex2f(130, 40);
	for (int t = 180; t <= 270; t += 1) {			//the cone
		glVertex2f(50 * cos(t * pi / 180) + 190, 50 * sin(t * pi / 180) + 160);
	}
	glEnd();

	glColor4f(1, 0.878, 0.4, alpha);
	glBegin(GL_POLYGON);
	for (int t = 0; t <= 90; t += 1) {				// the shadow of cone
		glVertex2f(50 * cos(t * pi / 180) + 140, 50 * sin(t * pi / 180) + 110);
	}
	for (int t = 180; t <= 270; t += 1) {
		glVertex2f(50 * cos(t * pi / 180) + 190, 50 * sin(t * pi / 180) + 160);
	}
	glEnd();

	if (cheerFlag) {								//when cheerFlag change to true, display push flower
		glColor4f(1, 0.301, 0.651, alpha);
		glRectf(145 + cheerCount * 5, 165 + cheerCount * 8, 152 + cheerCount * 5, 180 + cheerCount * 8);	//rectangle push flower, position change with cheerCount
		glPushMatrix();
		glRotatef(8, 0, 0, -1);																				//use rotation to draw second and third rectangle
		glRectf(145 + cheerCount * 5, 180 + cheerCount * 8, 152 + cheerCount * 5, 195 + cheerCount * 8);
		glRotatef(8, 0, 0, -1);
		glRectf(145 + cheerCount * 5, 170 + cheerCount * 8, 152 + cheerCount * 5, 185 + cheerCount * 8);
		glPopMatrix();
		glColor4f(1, 0, 0, alpha);
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < 3; i++) {		//triangle push flower
			glVertex2f(130 + i * 30 + cheerCount * 4, 168 - i * 5 + cheerCount * 15);
			glVertex2f(139 + i * 30 + cheerCount * 4, 165 - i * 5 + cheerCount * 15);
			glVertex2f(134 + i * 30 + cheerCount * 4, 178 - i * 5 + cheerCount * 15);
		}
		glEnd();
	}
}


/*               function to draw greatings on the screen                               */
void drawGreatings(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;

	// do the initinazation for the first call
	if (isFirstCall) {
		isFirstCall = 0;

		// generate a list with number of MAX_CHAR
		lists = glGenLists(MAX_CHAR);

		// Load the drawing commands of each character into the corresponding display list
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	// Call the display list corresponding to each character and draw each character
	for (; *str != '\0'; ++str)
		glCallList(lists + *str);
}


/*                  function to select the size and font of greatings                 */
void selectFont(int size, int charset, const char* face) {
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}


/*                           function to draw flowers                              */
void drawFlowers() {
	glColor4f(0, 0, 0, alpha);
	glBegin(GL_LINES);	//pedicel for two flowers
	glVertex2f(35, 30);
	glVertex2f(35, 50);
	glVertex2f(15, 30);
	glVertex2f(15, 50);
	glEnd();

	for (int i = 0; i < 2; i++) {	//petals for flowers
		int t;
		glColor4f(1, 0.27, 0, alpha);
		glBegin(GL_LINE_STRIP);
		for (t = 0; t < 1080; t++) {
			int tRad = t * pi / 180;
			glVertex2f((2 + sin(6 * tRad)) * cos(tRad) * 3.5 + 15 + i*20, (2 + sin(6 * tRad)) * sin(tRad) * 3.5+ 50);	//"+15" "+50" for adjusting position
		}																												// "i*20" is the distance between two flowers
		glEnd();
	}
}


/*               function to draw the animation of snow                          */
void drawSnow() {
	glEnable(GL_DEPTH_TEST);
	if (i <= 105)
		return;
	else
		glColor4f(1, 1, 1, 0.3);
	for (int i = 0; i < 3; i++) {					//3 rows
		for (int j = 0; j < 8; j++) {				//8 colomns
			glPushMatrix();
			glTranslatef(50 * j, -200 * i, 0);
			glPointSize(8);
			glBegin(GL_POINTS);						//draw snow
			glVertex2f(25, 600 + snowCount);
			glVertex2f(5, 580 + snowCount);
			glVertex2f(15, 550 + snowCount);
			glVertex2f(40, 515 + snowCount);
			glVertex2f(10, 490 + snowCount);
			glVertex2f(35, 460 + snowCount);
			glVertex2f(3, 445 + snowCount);
			glVertex2f(45, 410 + snowCount);
			glEnd();
			glPopMatrix();
		}
	}
}


/*                   function to draw baloons                                 */
void drawBalloon() {
	double r = 10;
	int n = 100;
	for (int j = 0; j < 10; j++) {		//draw 10 balloons
		glColor3f(1.0, 0, 0);
		glBegin(GL_POLYGON);
		for (int i = 0; i < n; i++)
		{
			glVertex2f(r * cos(2 * pi / n * i) + 30 + 50 * j + ballonX, 2 * r * sin(2 * pi / n * i) + 60 + ballonY);	//draw the circle part for balloon
		}																												//30,60 adjust initial position
		glEnd();																										//50 * j is the gap between two ballons
																														//ballonX, balloonY change with timer, simulating ballon flying
		glColor3f(0, 0, 0);
		GLdouble t;
		GLdouble x, y;
		glLineWidth(1.5);
		glBegin(GL_LINE_STRIP);
		for (t = 0; t <= 20; t += 0.1) {		//use sine to draw rope of balloon 
			x = 3 * sin(t / 10 * 3.1415) + 50 * j + 30 + ballonX;		//30,40 adjust initial position
			y = -t + +40 + ballonY;
			glVertex3f(x, y, 0);
		}
		glEnd();

	}
}

/*                 main display function                                   */
void card()
{
	//initialiazation part
	glClearColor(1, 0.894, 0.8823, 0);		//background color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left, right, bottom, top);		//range for display, can be changed with special key interaction

	drawBalloon();
	if (i > 75) {			//display remaining parts of card after balloon flying over
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			//enable to change alpha
		glEnable(GL_BLEND);
		drawSnow();
		glColor4f(0.576, 0.439, 0.858, alpha);
		selectFont(108, ANSI_CHARSET, "Comic Sans MS");
		glRasterPos2f(150.0f, 330.0f);
		drawGreatings("Happy");
		glRasterPos2f(130.0f, 265.0f);
		drawGreatings("Birthday!");
		drawCurves();
		drawCake();
		drawCandles(candleMode);
		drawTrees();
		drawFlowers();
		drawCheers(cheerFlag);
	}

	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1080, 650);
	glutCreateWindow("birthday card");
	glutDisplayFunc(card);
	glutIdleFunc(when_in_mainloop);
	glutTimerFunc(time_interval, OnTimer, 1);	//timer callback function
	glutKeyboardFunc(keyboard_input);				//keyboard callback function
	glutSpecialFunc(processSpecialKeys);			//
	glutMouseFunc(mouse_input);		//mouse callback function 
	glutMainLoop();
	return 0;
}