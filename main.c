#include "maze.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <GL/glut.h>

#define PI 3.14159265

/**
 * struct for kruskal maze
 * @map: 2d array of maze
 * @size: size of maze
 */
typedef struct {
	int** map;
	int size;
} Kruskal;

/**
 * struct for transversal maze
 * @map: 2d array of maze
 * @mazeSize: size of maze
 * @transMap: 2d array of transversal maze
 */
typedef struct {
	int** map;
	int mazeSize;
	int** transMap;
} Transversal;

	Kruskal myMaze;
	Transversal myTran;

	int** Map, ** TransMap;
	int xPlayer, yPlayer, xEnemy, yEnemy, playerType = 0;
	int inDoor, outDoor, xNim, yNim, rotBy = 1;
	float xRot = -20, yRot, xNimRot, yNimRot, zNimRot;
	int viewMode = 0;
	int isTrans, isMorning, isEnemyRun = 1;
	int diff, spec, amb;
	float pos[] = {1, 10.5, 0};
	float ver[] = {0, -1, 0};
	float alpha = 4.71;
	int xRoad, yRoad;

/**
 * placeDoor - place door in maze
 * Return: void
 */
void placeDoor() {
	inDoor = myMaze.map[0][rand() % myMaze.size];
	outDoor = myMaze.map[myMaze.size * 2][rand() % myMaze.size];
	Map[0][inDoor] = 0;
	Map[myMaze.size * 2][outDoor] = 0;
}

/**
 * doTransit - transversal maze
 * @size: size of maze
 * Return: void
 */
void settingUp(int size) {
	myMaze.size = size;
	myMaze.map = (int**)malloc(sizeof(int*) * size);

	for (int i = 0; i < size; i++) {
		myMaze.map[i] = (int*)malloc(sizeof(int) * size);
	}

	myTran.mazeSize = size;
	myTran.transMap = (int**)malloc(sizeof(int*) * (size * 2 + 1));

	for (int i = 0; i < size * 2 + 1; i++) {
		myTran.transMap[i] = (int*)malloc(sizeof(int) * (size * 2 - 1));
	}

/**
 * doGenerate - declares a function called doGenerate with no parameters
 * @map - assigns the maze map to the variable Map
 * @xNim - generates the x coordinates for the player
 * @yNim - generates the y coordinates for the player
 * placeDoor - call function to place a door
 * doTransit - handle player transit
 * @transmap - Assign the transformation map from myTran to the variable TransMap
 * @pos - Set the initial position of the player
 * @size -  Check if size is even, and set the appropriate x position for the player
 *  and enemy in the maze
 * Return: void
 */
void doGenerate();
	Map = myMaze.map;
	xNim = (rand() % myMaze.size) * 2 + 1;
	yNim = (rand() % myMaze.size) * 2 + 1;
	placeDoor();
	doTransit();
	TransMap = myTran.transMap;
	pos[1] = size + 2.5;
	size % 2 == 0 ? pos[0] = 1 : pos[0] = 0;

	xPlayer = outDoor;
	yPlayer = myMaze.size * 2;
	xEnemy = size * 2 - 1;
	yEnemy = size * 2 - 1;
}
/**
 * myinit - initialize the OpenGL settings
 * @isMorning - Set the clear color based on whether it's morning or not
 * @glMatrixMode - Set the matrix mode to projection
 * @glLoadIdentity - load the identity matrix
 * Return: void
 */
void myinit() {
	isMorning == 1 ? glClearColor(0.5, 0.5, 1, 0.5) : glClearColor(0.0, 0.0, 0.0, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (viewMode == 2) {
		gluPerspective(45.0, 1, 1, 50);
		gluLookAt(pos[0], pos[1], pos[2], pos[0] + ver[0], pos[1] + ver[1], pos[2] + ver[2], 0, 0, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	} else {
		glOrtho(-myMaze.size - 2.5, myMaze.size + 2.5, -myMaze.size - 2.5, myMaze.size + 2.5, -myMaze.size - 4.5,
			myMaze.size + 4.5);
		glMatrixMode(GL_MODELVIEW);
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
}

/**
 * drawBox - draw coloured boxin 3D space
 * @glColor4f - set the color of the box
 * @zPlus: depth of box
 * @zMin: depth of box
 * @left: left of box
 * @down: down of box
 * @right: right of box
 * @top: top of box
 * @colR: color of box red
 * @colG: color of box green
 * @colB: color of box blue
 * @colAlpha: color of box alpha
 * Return: void
 */
void drawBox(float zPlus, float zMin, float left, float down, float right, float top, float colR, float colG,
		float colB, float colAlpha) {
	glColor4f(colR, colG, colB, colAlpha);

	/* Draw front face*/
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, 1);
	glVertex3f(left, down, zPlus);
	glVertex3f(right, down, zPlus);
	glVertex3f(right, top, zPlus);
	glVertex3f(left, top, zPlus);
	glEnd();

	/*Draw back face*/
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, -1);
	glVertex3f(left, down, zMin);
	glVertex3f(right, down, zMin);
	glVertex3f(right, top, zMin);
	glVertex3f(left, top, zMin);
	glEnd();

	/* Increase color intensity for side faces*/
	glColor4f(colR + 0.2, colG + 0.2, colB + 0.2, colAlpha);

	/*Draw left face*/
	glBegin(GL_POLYGON);
	glNormal3f(-1, 0, 0);
	glVertex3f(left, down, zPlus);
	glVertex3f(left, top, zPlus);
	glVertex3f(left, top, zMin);
	glVertex3f(left, down, zMin);
	glEnd();

	/*Draw right face*/
	glBegin(GL_POLYGON);
	glNormal3f(1, 0, 0);
	glVertex3f(right, down, zPlus);
	glVertex3f(right, down, zMin);
	glVertex3f(right, top, zMin);
	glVertex3f(right, top, zPlus);
	glEnd();

	/*Draw bottom face*/
	glBegin(GL_POLYGON);
	glNormal3f(0, -1, 0);
	glVertex3f(left, down, zPlus);
	glVertex3f(left, down, zMin);
	glVertex3f(right, down, zMin);
	glVertex3f(right, down, zPlus);
	glEnd();

	/*Draw top face*/
	glBegin(GL_POLYGON);
	glNormal3f(0, 1, 0);
	glVertex3f(left, top, zPlus);
	glVertex3f(right, top, zPlus);
	glVertex3f(right, top, zMin);
	glVertex3f(left, top, zMin);
	glEnd();
}
/**
 * drawNim - Draw the nim character using a sequence of colored boxes
 * Return: void
 */
void drawNim() {
	drawBox(0.1, -0.1, 0.1 + xNim, 0.1 + yNim, 0.18 + xNim, 0.9 + yNim, 1, 0, 0.4, 1);
	drawBox(0.1, -0.1, 0.21 + xNim, 0.1 + yNim, 0.3 + xNim, 0.9 + yNim, 1, 0, 0.4, 1);
	drawBox(0.1, -0.1, 0.3 + xNim, 0.1 + yNim, 0.45 + xNim, 0.19 + yNim, 1, 0, 0.4, 1);
	drawBox(0.1, -0.1, 0.45 + xNim, 0.1 + yNim, 0.54 + xNim, 0.9 + yNim, 1, 0, 0.4, 1);
	drawBox(0.1, -0.1, 0.3 + xNim, 0.81 + yNim, 0.45 + xNim, 0.9 + yNim, 1, 0, 0.4, 1);
	drawBox(0.1, -0.1, 0.57 + xNim, 0.1 + yNim, 0.66 + xNim, 0.9 + yNim, 1, 0, 0.4, 1);
	drawBox(0.1, -0.1, 0.57 + xNim, 0.1 + yNim, 0.82 + xNim, 0.19 + yNim, 1, 0, 0.4, 1);
	drawBox(0.1, -0.1, 0.82 + xNim, 0.1 + yNim, 0.91 + xNim, 0.9 + yNim, 1, 0, 0.4, 1);
	drawBox(0.1, -0.1, 0.66 + xNim, 0.81 + yNim, 0.91 + xNim, 0.9 + yNim, 1, 0, 0.4, 1);
	drawBox(0.1, -0.1, 0.66 + xNim, 0.46 + yNim, 0.91 + xNim, 0.55 + yNim, 1, 0, 0.4, 1);
}

/**
 * putMaterial - put material in maze
 * @r: red
 * @g: green
 * @b: blue
 * @a: alpha
 * Return: void
 */
void putMaterial(float r, float g, float b, float a) {
	GLfloat mat_specular[] = {r, g, b, a};
	GLfloat mat_shininess[] = {10.0};
	GLfloat mat_diffuse[] = {r, g, b, a};
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
}

/**
 * drawPlayer - draw player in maze
 * Return: void
 */
void drawPlayer() {
	putMaterial(0.0, 1.0, 0.0, 1.0);
	glPushMatrix();
	
	if (playerType == 0) {
		glTranslatef(xPlayer + 0.5, yPlayer + 0.5, 0);
		glutSolidSphere(0.5, 20, 10);
	} else {
		glTranslatef(xPlayer + 0.5, yPlayer + 0.5, -0.5);
		glutSolidCone(0.4, 1, 50, 50);
	}
	glPopMatrix();
}

/**
 * drawEnemy - draw enemy in maze
 * Return: void
 */
void drawEnemy() {
	putMaterial(0.0, 1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(xEnemy + 0.5, yEnemy + 0.5, -0.5);
	glutSolidCone(0.5, 1, 50, 50);
	glPopMatrix();
}

/**
 * setNim - set nim in maze
 * Return: void
 */
void setNim() {
	putMaterial(1.0, 0.0, 0.0, 1.0);
	glPushMatrix();
	
	if (viewMode != 0) {
		glTranslated(0.5 + xNim, 0.5 + yNim, 0);
		glRotatef(xNimRot, 1, 0, 0);
		glRotatef(yNimRot, 0, 1, 0);
		glRotatef(zNimRot, 0, 0, 1);
		glTranslated(-0.5 - xNim, -0.5 - yNim, 0);
	}
	drawNim();
	glPopMatrix();
}

/**
 * setDisplay - set display object
 * Return: void
 */
void setDisplay() {
	glPushMatrix();
	
	if (viewMode == 1) {
		glRotatef(yRot, 0, 1, 0);
		glRotatef(xRot, 1, 0, 0);
	}
	glTranslatef(-myMaze.size - 0.5, -myMaze.size - 0.5, 0);
}

/**
 * helperRoad - helper function for road
 * @xRoad: The x coordinate of the road 
 * @yRoad: The y coordinate of the road
 * Return: void
 */
void helperRoad(int xRoad, int yRoad) {
	glBegin(GL_POLYGON);
	glColor3f(0, 1, 0);
	glVertex3f(xRoad, yRoad, 0.0);
	glVertex3f(xRoad + 1, yRoad, 0.0);
	glVertex3f(xRoad + 1, yRoad + 1, 0.0);
	glVertex3f(xRoad, yRoad + 1, 0.0);
	glEnd();
}

/**
 * display - display the maze
 * Return: void
 */
void display() {
	// TODO: drawing the maze player nim and transparent floor
	myinit();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	setDisplay();
	isTrans == 1 ? putMaterial(0.0, 0.0, 0.8, 0.3) : putMaterial(0.0, 0.0, 0.8, 1.0);
	
	for (int i = 0; i < myMaze.size; i++) {
		for (int j = 0; j < myMaze.size; j++) {
			if (Map[i][j] == 1) {
				drawBox(0.5, -0.5, j, i, j + 1, i + 1, 0.1, 0.1, 0.1, 1);
			}
		}
	}
	setNim();
	// player
	drawPlayer();
	// enemy
	drawEnemy();
	// floor
	putMaterial(1, 1, 1, 0.5);
	drawBox(-0.51, -0.61, 0, 0, myMaze.size * 2 + 1, myMaze.size * 2 + 1, 0.6, 0.6, 0.6, 0.4);
	glutSwapBuffers();
}

/**
 * Timer - timer for enemy
 * @iUnused: unused variable for timer
 * Return: void
 */
void Timer(int iUnused) {
	if (isEnemyrun && yEnemy != 0) {
		if (TransMap[yEnemy][xEnemy - 1] == 2) {
			TransMap[yEnemy][xEnemy] = 0;
			xEnemy -= 1;
		} else if (TransMap[yEnemy][xEnemy + 1] == 2) {
			TransMap[yEnemy][xEnemy] = 0;
			xEnemy += 1;
		} else if (TransMap[yEnemy + 1][xEnemy] == 2) {
			TransMap[yEnemy][xEnemy] = 0;
			yEnemy += 1;
		} else {
			TransMap[yEnemy][xEnemy] = 0;
			yEnemy -= 1;
		}
		glutPostRedisplay();
	
		if (xEnemy == inDoor && yEnemy == 0) {
			isEnemyrun = 0;
		}
		glutTimerFunc(500, Timer, 0);
	}
}

/**
 * nimAnimation - animation for nim
 * Return: void
 */
void nimAnimation() {
	// CHANGED: change angle size of rotation (animation)
	if (rotBy == 0) {
		xNimRot += 0.1;
	}
	if (rotBy == 1) {
		yNimRot += 0.1;
	}
	if (rotBy == 2) {
		zNimRot += 0.1;
	}
	glutPostRedisplay();
}

/**
 * input - input for keyboard
 * @key: key pressed
 * @x: x coordinate
 * @y: y coordinate
 * Return: void 
 */
void input(unsigned char key, int x, int y) {
	if (key == 'c' || key == 'C') {
		settingUp(mazeSize);
	}
	if (key == '1') {
		if (amb == 1) {
			GLfloat ambient_light[] = {0.5, 0.5, 0.5, 1.0};
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
			amb = 0;
		} else {
			GLfloat ambient_light[] = {0.9, 0.9, 0.9, 1.0};
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
			amb = 1;
		}
	}
	if (key == '2') {
		if (diff == 1) {
			glDisable(GL_LIGHT0);
			diff = 0;
		} else {
			glEnable(GL_LIGHT0);
			diff = 1;
		}
	}
	if (key == '3') {
		if (spec == 1) {
			glDisable(GL_LIGHT1);
			spec = 0;
		} else {
			glEnable(GL_LIGHT1);
			spec = 1;
		}
	}
	if (key == '4') {
		isMorning == 1 ? isMorning = 0 : isMorning = 1;
	}
	if (key == '5') {
		isTrans == 1 ? isTrans = 0 : isTrans = 1;
	}
	if (key == 32) {
		isEnemyrun == 1 ? isEnemyrun = 0 : isEnemyrun = 1;
		Timer(0);
	}
	if (key == '+' || key == '-') {
		key == '+' ? mazeSize += 1 : mazeSize -= 1;
		settingUp(mazeSize);
	}
	if (key == 'p' || key == 'P') {
		if (playerType == 0) {
			playerType = 1;
		} else {
			playerType = 0;
		}
	}
	if (key == 'v' || key == 'V') {
		// FIXME: rotation
		if (viewMode == 0) {
			glutIdleFunc(nimAnimation);
			viewMode = 1;
		} else if (viewMode == 1) {
			viewMode = 2;
		} else {
			glutIdleFunc(NULL);
			viewMode = 0;
		}
	}
	// TODO: to control player walk inside maze & collision
	if ((key == 'w' || key == 'W') && yPlayer < myMaze.length * 2 && Map[yPlayer + 1][xPlayer] != 1) {
		yPlayer += 1;
	}
	if ((key == 'a' || key == 'A') && Map[yPlayer][xPlayer - 1] != 1) {
		xPlayer -= 1;
	}
	if ((key == 's' || key == 'S') && yPlayer > 0 && Map[yPlayer - 1][xPlayer] != 1) {
		yPlayer -= 1;
	}
	if ((key == 'd' || key == 'D') && Map[yPlayer][xPlayer + 1] != 1) {
		xPlayer += 1;
	}
	// TODO: rotation of maze y or x
	if ((key == 'i' || key == 'I') && viewMode == 1) {
		xRot -= 1;
	}
	if ((key == 'j' || key == 'J') && viewMode == 1) {
		yRot -= 1;
	}
	if ((key == 'k' || key == 'K') && viewMode == 1) {
		xRot += 1;
	}
	if ((key == 'l' || key == 'L') && viewMode == 1) {
		yRot += 1;
	}
	display();
}

/**
 * mouse - mouse function for rotation of nim
 * @button: button pressed
 * @state: state of button
 * @x: x coordinate
 * @y: y coordinate
 * Return: void
 */
void mouse(int button, int state, int x, int y) {
	// CHANGED: change direction of nim rotation
	switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN && viewMode != 0) {
				rotBy = 0;
			}
			break;
		case GLUT_MIDDLE_BUTTON:
			if (state == GLUT_DOWN && viewMode != 0) {
				rotBy = 2;
			}
			break;
		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN && viewMode != 0) {
				rotBy = 1;
			}
			break;
		default:
			break;
	}
}

/**
 * settingLight - setting light in maze
 * Return: void
 */
void settingLight() {
	GLfloat light_position_diff[] = {10.0, 10.0, 5.0, 0.0};
	GLfloat light_position_spec[] = {10.0, -10.0, 5.0, 0.0};
	GLfloat diffuse_light[] = {0.8, 0.8, 1.0, 0.5};
	GLfloat specular_light[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat ambient_light[] = {0.9, 0.9, 0.9, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position_diff);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position_spec);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular_light);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light);
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
}

/**
 * specKey - special key for rotation of maze 
 * @key: key pressed to rotate maze
 * @x: x coordinate
 * @y: y coordinate
 */
void specKey(int key, int x, int y) {
	if (viewMode == 2) {
		if (key == GLUT_KEY_RIGHT) {
			alpha -= 0.1;
			ver[0] = cos(alpha);
			ver[1] = sin(alpha);
		} else if (key == GLUT_KEY_LEFT) {
			alpha += 0.1;
			ver[0] = cos(alpha);
			ver[1] = sin(alpha);
		} else if (key == GLUT_KEY_UP) {
			pos[0] += 0.05 * ver[0];
			pos[1] += 0.05 * ver[1];
			pos[2] += 0.05 * ver[2];
		} else if (key == GLUT_KEY_DOWN) {
			pos[0] -= 0.05 * ver[0];
			pos[1] -= 0.05 * ver[1];
			pos[2] -= 0.05 * ver[2];
		}
		display();
	}
}

/**
 * main - main function 
 * @argc: argument count
 * @argv: argument vector/array
 * Return 0 if success
 */
int main(int argc, char *argv[]) {
	srand(time(NULL));
	srand((unsigned)time(0));
	settingUp(8);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Maze Game || DSA is Fun");
	Timer(0);
	glutDisplayFunc(display);
	glutSpecialFunc(specKey);
	glutKeyboardFunc(input);
	glutMouseFunc(mouse);
	myinit();
	settingLight();
	glutMainLoop();
	return (0);
}



