#include<GL/freeglut.h>
#include<iostream>
#include<fstream>
#include<string>
#include<Windows.h>

#ifndef VISUALS_H
#define VISUALS_H


using namespace std;


#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 800
#define GRID_ROW WINDOW_WIDTH
#define GRID_COL WINDOW_HEIGHT


class Point {
	GLfloat x;
	GLfloat y;

public:
	Point(GLfloat xval, GLfloat yval) {
		x = xval;
		y = yval;
	}

	// Getter & Setter Functions
	void setX(GLfloat xval) {
		x = xval;
	}
	void setY(GLfloat yval) {
		y = yval;
	}
	GLfloat getX() {
		return x;
	}
	GLfloat getY() {
		return y;
	}
	void incrementX(GLfloat val) {
		x = x + val;
	}
	void incrementY(GLfloat val) {
		y = y + val;
	}
	void decrementX(GLfloat val) {
		x = x - val;
	}
	void decrementY(GLfloat val) {
		y = y - val;
	}
};


// Object Generator Functions
void initializeVisuals(bool debug_mode, GLfloat* world_speed, GLfloat* player_speed);
void generateBackground(int);
void generateFilledCircle(Point p, GLfloat offset, GLfloat radius);
void generateFilledCircle(GLfloat x, GLfloat y, GLfloat radius);
void generateTree(Point p, GLfloat offset, int leaves_level, int trunk_width, int trunk_height, GLfloat leaf_block_size, const GLfloat leaf_color[]);
void generateTree(Point p, GLfloat offset, int leaves_level, int trunk_width, int trunk_height, GLfloat leaf_block_size);
void generateClouds(Point p, GLfloat offset, int length);
void generateStar(Point p, GLfloat offset, const GLfloat color[]);
void generateStar(Point p, GLfloat offset);
void generateOrb(Point p, GLfloat offset, const GLfloat color[]);
void protagonist(Point player);
void initializeColors();
void initializeOrbPoints();
void initializeOrbContactsList();
void initializeTreePoints();
void initializeStarPoints();
void initializeCloudPoints();
bool checkContact(Point player, Point orb,GLfloat offset);
void checkAllOrbs(Point player, GLfloat offset);
void randomizeOrbPoints();
void initializeEntropy();
void endGame();

// Objects that populate the environment
void drawGround();
void drawSun(GLfloat offset);
void drawMoon(GLfloat offset);
void drawClouds(GLfloat offset);
void drawTree(GLfloat offset);
void drawStars(GLfloat offset);
void drawOrbs(GLfloat offset);


// Logger functions
void logger(const string& message);
void clear_log();

#endif