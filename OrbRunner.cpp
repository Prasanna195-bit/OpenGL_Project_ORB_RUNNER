/*
##########
Orb Runner
##########

Introduction
------------
In Orb Runner, the player's aim is to collect as many orbs as possible before the orbs disappear
Each collected orb provides 10 points to the player
However, if the player collects the Red Orb, the game ends

Dependencies
------------
This project uses freeglut.dll from The FreeGLUT Project
*/

// Header Files
#include "Visuals.h"


// Function Prototypes
void init();
void display();
void idle();
void reshape(int, int);
void keyboard(unsigned char key, int x, int y);
void special_keys(int key, int x, int y);
void generateScene();
void gameInstructions();


// Global Variables
Point* player;							// Player Coordinates
float player_startx = 30;				// Player Starting coordinates on X-Axis
float player_starty = 230;				// Player Starting coordinates on Y-axis
float offset;							// Used to control the movement of Environment Objects
float true_offset;						// Same as offset but true_offset does not reset after a certain value
float world_speed = 0.1F;				// Speed of motion of Environment Objects
float gravity = 0.3F;					// Power of gravity that pulls the player down when he jumps
float player_speed = 1;					// Speed of the Player Character
constexpr bool debug_mode = false;		// Enables logging
constexpr bool game_mode = true;		// Enables gameplay messages
constexpr bool enable_orb = true;		// Enables the display of orbs
constexpr int scene = 2;				// Used to select the scene (1 = Day, 2 = Night)


int main(int argc, char** argv) {
	//OpenGL Initialisation
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("UnderDig");
	init();


	//Callback Functions
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special_keys);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}

void init() {
	if (debug_mode) {
		clear_log(); // Clearing old logs
		logger("Orb Runner Debug Log\n\n");
	}

	if (game_mode) {
		gameInstructions();
	}

	//Initial protagonist coords
	offset = 0;
	true_offset = 0;
	player = new Point(player_startx, player_starty);


	initializeVisuals(debug_mode, &world_speed, &player_speed);
	initializeEntropy();
	initializeColors(); // Loads the colors onto the color_set
	initializeTreePoints(); // Loads the tree coordinates onto the vector
	initializeOrbPoints();	// Loads the Orb coordinates onto the vector
	initializeStarPoints();
	initializeCloudPoints();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, WINDOW_HEIGHT, 0, WINDOW_WIDTH);
	glMatrixMode(GL_MODELVIEW);
}
void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	generateScene();
	protagonist(*player);

	glFlush();
	glutSwapBuffers();
}
void idle() {
	bool enable_idle_log = false;

	// Control Overworld motion
	offset = offset - world_speed;
	true_offset -= world_speed;
	if (offset < -600) {
		offset = 0;
		randomizeOrbPoints();
	}

	// Gravity for the player
	if (player->getY() > player_starty) {
		player->decrementY(gravity);
	}

	if (enable_idle_log) {
		logger("Offset = " + to_string(offset) + "\n");
	}

	glutPostRedisplay();
}
void reshape(int w, int h) {
	protagonist(*player);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, WINDOW_HEIGHT, 0, WINDOW_WIDTH);
	glMatrixMode(GL_MODELVIEW);
}
void keyboard(unsigned char key, int, int) {
	float multiplier = 5;

	if (debug_mode) {
		logger("Offset = " + to_string(offset) + "\tPlayer (PX, PY) = " + to_string(player->getX()) + "," + to_string(player->getY()) + "\n");
	}

	// Right = +X Axis
	if ((key == 'd') && (player->getX() < WINDOW_WIDTH)) {
		player->incrementX(player_speed);
		offset = offset - player_speed * multiplier;
	}

	// Left = -X Axis
	if ((key == 'a') && (player->getX() > 0)) {
		player->decrementX(player_speed);
		offset = offset + player_speed * multiplier;
	}

	// Up = +Y Axis
	if (key == 'w') {
		player->incrementY(player_speed);
	}

	// Down = -Y Axis
	if ((key == 's') && (player->getY() > player_starty)) {
		player->decrementY(player_speed);
	}
	// Space bar = Jump
	if ((key == (char)32) && (player->getY() < WINDOW_HEIGHT)) {
		player->incrementY(player_speed * 100);
	}

	glutPostRedisplay();
}
void special_keys(int key, int x, int y) {
	int multiplier = 5;

	if (key == GLUT_KEY_LEFT) {
		x = x - multiplier * player_speed;
		offset = offset + player_speed * multiplier;
	}

	else if (key == GLUT_KEY_RIGHT) {
		x = x + multiplier * player_speed;
		offset = offset - player_speed * multiplier;
	}

	if (debug_mode) {
		logger("Offset = " + to_string(offset) + "\tScreen (X, Y) = " + to_string(x) + ", " + to_string(y) + "\n");
	}
}
void generateScene() {
	// Day Sky
	if (scene == 1) {
		generateBackground(1);
		drawSun(true_offset);
		drawGround();
		drawTree(offset);
		drawClouds(offset);
	}


	// Night Sky
	else if (scene == 2) {
		generateBackground(2);
		drawMoon(true_offset);
		drawStars(offset);
		drawGround();
		drawTree(offset);
	}

	if (enable_orb) {
		drawOrbs(offset);
		checkAllOrbs(*player, offset);
	}
}

void gameInstructions() {
	clear_log();
	logger("\t\t\t\t\t##########\n");
	logger("\t\t\t\t\tOrb Runner\n");
	logger("\t\t\t\t\t##########\n");
	logger("Welcome to Orb Runner! You are a treasure hunter trying to collect the mystical orbs!\n");
	logger("Jump to collect as many orbs as you can to increase your score\n");
	logger("The orbs disappearand reappear every few seconds. So be sure to collect them as soon as you can!\n");
	logger("BEWARE - Avoid the mysterious red orb at all costs! It will paralyze and poison you\n");
	logger("Good luck!\n\n");
}