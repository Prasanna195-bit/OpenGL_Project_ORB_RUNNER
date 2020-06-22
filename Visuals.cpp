#include "Visuals.h"
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <iterator>
#include <random>


// Global Variables
constexpr GLfloat sun_offset = 0.1F;
constexpr GLfloat moon_offset = 0.05F;
constexpr GLfloat star_size_offset = 5.0F;
constexpr GLfloat scene_reset_distance = 600.0F;
constexpr GLfloat orb_size = 20;
constexpr GLfloat player_size = 15; // Used to control the overall size of the character
constexpr int game_over_orb = 3;
bool enable_logger = true;
int score;

// 
GLfloat* world_speed_ptr;
GLfloat* player_speed_ptr;
GLfloat color_set[20][3] = { {} };
vector<Point> orb_list;
vector<Point> tree_list;
vector<Point> star_list;
vector<Point> cloud_list;
vector<bool> orb_contact;
minstd_rand my_rand;

// Colors List
GLfloat BrightLavender[] = { 0.7490F, 0.5804F, 0.8941F };
GLfloat RussianViolet[] = { 0.1961F, 0.0902F, 0.3020F };
GLfloat LightKhaki[] = { 0.9412F, 0.9020F, 0.5490F };
GLfloat DurianYellow[] = { 0.8824F, 0.7412F, 0.1529F };
GLfloat VividRaspberry[] = { 1.0F, 0.0F, 0.4235F };
GLfloat MildGreen[] = { 0.0F, 0.5F, 0.1F };
GLfloat Brown[] = { 0.9F, 0.2F, 0.0F };
GLfloat White[] = { 1.0F, 1.0F, 1.0F };
GLfloat Kuchinashi[] = { 1.00F, 0.7255F, 0.3529F };
GLfloat PastelRed[] = { 1.00F, 0.4118F, 0.3804F };
GLfloat FluorescentBlue[] = { 0.8240F, 0.9569F, 0.9333F };
GLfloat MardiGras[] = { 0.5569F, 0.588F, 0.5647F };
GLfloat Canary[] = { 1.00F, 1.00F, 0.600F };
GLfloat MintGreen[] = { 0.749F, 0.7529F, 0.9333F };
GLfloat MangoTango[] = { 1.00F, 0.5098F, 0.2627F };
GLfloat Red[] = { 1.0F, 0.0F, 0.0F };
GLfloat AcidGreen[] = { 0.6902F, 0.749F, 0.102F };


// Object Generator Functions
void initializeVisuals(bool debug_mode, GLfloat* world_speed, GLfloat* player_speed) {
	world_speed_ptr = world_speed;
	player_speed_ptr = player_speed;
	enable_logger = debug_mode;
	score = 0;
}
void generateBackground(int option) {
	// Day-Sky Background
	if (option == 1) {
		glColor3f(0.0F, 0.9F, 0.9F); // Very slightly light cyan
		glBegin(GL_POLYGON);
		glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
		glVertex2f(WINDOW_WIDTH, -WINDOW_HEIGHT);
		glVertex2f(-WINDOW_WIDTH, WINDOW_HEIGHT);
		glVertex2f(-WINDOW_WIDTH, -WINDOW_HEIGHT);
		glEnd();
	}

	// Night Sky Background
	if (option == 2) {
		glColor3f(0, 0, 0); // Completely Black
		glBegin(GL_POLYGON);
		glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
		glVertex2f(WINDOW_WIDTH, -WINDOW_HEIGHT);
		glVertex2f(-WINDOW_WIDTH, WINDOW_HEIGHT);
		glVertex2f(-WINDOW_WIDTH, -WINDOW_HEIGHT);
		glEnd();
	}
}
void generateFilledCircle(Point p, GLfloat radius) {
	generateFilledCircle(p, 0.0F, radius);
}
void generateFilledCircle(Point p, GLfloat offset, GLfloat radius) {
	/*
	This function draws a number of triangles in order to finally generate a circle
	The color of the circle is the last selected color
	*/
	int triangles = 20; // Number of triangles used to draw the circle
	GLfloat twoPi = 3.14F * 2.0F;
	GLfloat point_x;
	GLfloat point_y;

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(p.getX() + offset, p.getY());
	for (int i = 0; i <= triangles; i++) {
		point_x = p.getX() + offset + (radius * cos(i * twoPi / triangles));
		point_y = p.getY() + (radius * sin(i * twoPi / triangles));
		glVertex2f(point_x, point_y);
	}
	glEnd();
}
void generateFilledCircle(GLfloat x, GLfloat y, GLfloat radius) {
	int triangles = 20; // Number of triangles used to draw the circle
	GLfloat twoPi = 3.14F * 2.0F;
	GLfloat point_x;
	GLfloat point_y;

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y);
	for (int i = 0; i <= triangles; i++) {
		point_x = x + (radius * cos(i * twoPi / triangles));
		point_y = y + (radius * sin(i * twoPi / triangles));
		glVertex2f(point_x, point_y);
	}
	glEnd();
}
void generateTree(Point p, GLfloat offset, int leaves_level, int trunk_width, int trunk_height, GLfloat leaf_block_size, const GLfloat leaf_color[]) {
	GLfloat level_gap_modifier = 1.25;
	GLfloat x = p.getX() + offset;
	GLfloat y = p.getY();

	// Generating the Tree Trunk
	glColor3fv(Brown);
	glBegin(GL_POLYGON);
	glVertex2f(x, y);
	glVertex2f(x, y + trunk_height);
	glVertex2f(x + trunk_width, y + trunk_height);
	glVertex2f(x + trunk_width, y);
	glEnd();


	glColor3fv(leaf_color);
	y = y + trunk_height;
	// Positioning at lower left end of leaves generation
	x = x - (leaves_level / 2 * leaf_block_size);
	// Adjusting the position based on even or odd number of levels of leaves
	if (leaves_level % 2 == 0) {
		x = x + leaf_block_size;
	}
	else {
		x = x + leaf_block_size / 2;
	}
	// Generating the Tree Leaves
	for (int i = leaves_level; i > 0; i--) {
		for (int j = 0; j < i; j++) {
			generateFilledCircle(x, y, leaf_block_size);
			x = x + leaf_block_size;
		}
		// Positioning at the left end for the next layer of leaves
		x = x - i * leaf_block_size + leaf_block_size / 2;
		y = y + leaf_block_size * level_gap_modifier;
	}
}
void generateTree(Point p, GLfloat offset, int leaves_level, int trunk_width, int trunk_height, GLfloat leaf_block_size) {
	generateTree(p, offset, leaves_level, trunk_width, trunk_height, leaf_block_size, MildGreen);
}
void generateClouds(Point p, GLfloat offset, int length) {
	GLfloat cloud_size = 25;
	GLfloat x = p.getX() + offset;
	GLfloat y = p.getY();

	// Generate the "head" or "front" part of the cloud
	glColor3f(0.9F, 0.9F, 0.9F);
	generateFilledCircle(x, y, cloud_size);

	// Generate the middle part of the cloud
	for (int i = length - 3; i >= 0; i--) {
		x = x + cloud_size * 1.5F;
		generateFilledCircle(x, y, cloud_size + 10);
	}

	// Generate the tail part of the cloud
	x = x + cloud_size * 1.5F;
	generateFilledCircle(x, y, cloud_size);
}
void generateStar(Point p, GLfloat offset, const GLfloat color[]) {
	GLfloat x;
	GLfloat y;

	x = p.getX() + offset;
	y = p.getY();
	glColor3fv(color);
	glBegin(GL_TRIANGLES);
	glVertex2f(x, y);
	glVertex2f(x - star_size_offset, y - 8);
	glVertex2f(x + star_size_offset, y - 8);
	glVertex2f(x, y - 10);
	glVertex2f(x - star_size_offset, y - 3);
	glVertex2f(x + star_size_offset, y - 3);
	glEnd();
}
void generateStar(Point p, GLfloat offset) {
	generateStar(p, offset, White);
}
void generateOrb(Point p, GLfloat offset, const GLfloat color[]) {
	glColor3fv(color);
	generateFilledCircle(p, offset, orb_size);
}
void protagonist(Point player) {
	GLfloat x = player.getX();
	GLfloat y = player.getY();
	// Dimensions of the Character
	GLfloat width = 1 * player_size;
	GLfloat height = 2 * player_size;


	// Please refer to the documentation for a visual representation of the character, including the coordinates
	// Head
	glColor3fv(LightKhaki);
	glBegin(GL_POLYGON);
	glVertex2f(x - width / 2, y + height + width);
	glVertex2f(x + width / 2, y + height + width);
	glVertex2f(x + width / 2, y + height);
	glVertex2f(x - width / 2, y + height);
	glEnd();


	//Torso
	glColor3fv(RussianViolet);
	glBegin(GL_POLYGON);
	glVertex2f(x - width, y + height);
	glVertex2f(x + width, y + height);
	glVertex2f(x + width, y - height);
	glVertex2f(x - width, y - height);
	glEnd();

	//Left Hand
	glColor3fv(LightKhaki);
	glBegin(GL_POLYGON);
	glVertex2f(x - 2 * width, y + height / 2);
	glVertex2f(x - width, y + height / 2);
	glVertex2f(x - width, y);
	glVertex2f(x - 2 * width, y);
	glEnd();

	// Right Hand
	glColor3fv(LightKhaki);
	glBegin(GL_POLYGON);
	glVertex2f(x + width, y + height / 2);
	glVertex2f(x + 2 * width, y + height / 2);
	glVertex2f(x + 2 * width, y);
	glVertex2f(x + width, y);
	glEnd();

	// Left Leg
	glColor3fv(DurianYellow);
	glBegin(GL_POLYGON);
	glVertex2f(x - width, y - height);
	glVertex2f(x - width / 2, y - height);
	glVertex2f(x - width / 2, y - height - width);
	glVertex2f(x - width, y - height - width);
	glEnd();

	// Right Leg
	glColor3fv(DurianYellow);
	glBegin(GL_POLYGON);
	glVertex2f(x + width / 2, y - height);
	glVertex2f(x + width, y - height);
	glVertex2f(x + width, y - height - width);
	glVertex2f(x + width / 2, y - height - width);
	glEnd();
}
void initializeColors() {
	//TODO - fill out the rest of the colors. Need 12

	// Color 0 - Bright Lavender
	color_set[0][0] = BrightLavender[0];
	color_set[0][1] = BrightLavender[1];
	color_set[0][2] = BrightLavender[2];

	// Color 1 - Acid Green
	color_set[1][0] = AcidGreen[0];
	color_set[1][1] = AcidGreen[1];
	color_set[1][2] = AcidGreen[2];

	// Color 2 - Kuchinashi
	color_set[2][0] = Kuchinashi[0];
	color_set[2][1] = Kuchinashi[1];
	color_set[2][2] = Kuchinashi[2];

	// Color 3 - Red - GAME OVER
	color_set[3][0] = Red[0];
	color_set[3][1] = Red[1];
	color_set[3][2] = Red[2];

	// Color 4 - Canary
	color_set[4][0] = Canary[0];
	color_set[4][1] = Canary[1];
	color_set[4][2] = Canary[2];

	// Color 5 - Fluoroscent Blue
	color_set[5][0] = FluorescentBlue[0];
	color_set[5][1] = FluorescentBlue[1];
	color_set[5][2] = FluorescentBlue[2];

	// Color 6 - Pastel Red
	color_set[6][0] = PastelRed[0];
	color_set[6][1] = PastelRed[1];
	color_set[6][2] = PastelRed[2];

	//Color 7


	// Color 8 - Mango Tango
	color_set[8][0] = MangoTango[0];
	color_set[8][1] = MangoTango[1];
	color_set[8][2] = MangoTango[2];

	// Color 9 - Vivid Raspberry
	color_set[9][0] = VividRaspberry[0];
	color_set[9][1] = VividRaspberry[1];
	color_set[9][2] = VividRaspberry[2];

}
void initializeOrbPoints() {
	orb_list.clear();
	orb_list.push_back(*(new Point(200, 300)));
	orb_list.push_back(*(new Point(300, 250)));
	orb_list.push_back(*(new Point(400, 200)));
	orb_list.push_back(*(new Point(450, 700)));
	orb_list.push_back(*(new Point(500, 400)));
	orb_list.push_back(*(new Point(600, 700)));
	orb_list.push_back(*(new Point(700, 350)));
	orb_list.push_back(*(new Point(900, 500)));
	orb_list.push_back(*(new Point(1100, 300)));
	orb_list.push_back(*(new Point(1200, 600)));
	orb_list.push_back(*(new Point(1300, 550)));

	if (enable_logger) {
		vector<Point>::iterator iter;
		logger("Orb List:-\n");
		for (iter = orb_list.begin(); iter != orb_list.end(); iter++) {
			logger(to_string(iter->getX()) + ", " + to_string(iter->getY()) + "\n");
		}
	}
	initializeOrbContactsList();
}
void initializeOrbContactsList() {
	// Initializing orb_contact vector
	orb_contact.clear();
	for (unsigned int i = 0; i < orb_list.size(); i++) {
		orb_contact.push_back(false);
	}
}
void initializeTreePoints() {
	GLfloat distance = scene_reset_distance;

	tree_list.push_back(*(new Point(-400, 100)));
	tree_list.push_back(*(new Point(-500, 135)));
	tree_list.push_back(*(new Point(-700, 120)));
	tree_list.push_back(*(new Point(100, 135)));
	tree_list.push_back(*(new Point(100 + distance, 135)));
	tree_list.push_back(*(new Point(280, 185)));
	tree_list.push_back(*(new Point(280 + distance, 185)));
	tree_list.push_back(*(new Point(400, 100)));
	tree_list.push_back(*(new Point(400 + distance, 100)));
	tree_list.push_back(*(new Point(1300, 135)));
	tree_list.push_back(*(new Point(1300 + distance, 135)));
	tree_list.push_back(*(new Point(1400, 120)));
	tree_list.push_back(*(new Point(1400 + distance, 120)));

	if (enable_logger) {
		vector<Point>::iterator iter;
		logger("Tree List:-\n");
		for (iter = tree_list.begin(); iter != tree_list.end(); iter++) {
			logger(to_string(iter->getX()) + ", " + to_string(iter->getY()) + "\n");
		}
	}
}
void initializeStarPoints() {
	GLfloat distance = scene_reset_distance;

	star_list.push_back(*new Point(75, 550));
	star_list.push_back(*new Point(75 + distance, 550));
	star_list.push_back(*new Point(200, 650));
	star_list.push_back(*new Point(200 + distance, 650));
	star_list.push_back(*new Point(300, 450));
	star_list.push_back(*new Point(300 + distance, 450));
	star_list.push_back(*new Point(375, 600));
	star_list.push_back(*new Point(375 + distance, 600));
	star_list.push_back(*new Point(500, 550));
	star_list.push_back(*new Point(500 + distance, 550));
	star_list.push_back(*new Point(575, 650));
	star_list.push_back(*new Point(575 + distance, 650));
	star_list.push_back(*new Point(1100, 550));
	star_list.push_back(*new Point(1100 + distance, 550));
	star_list.push_back(*new Point(1275, 550));
	star_list.push_back(*new Point(1275 + distance, 550));

	if (enable_logger) {
		vector<Point>::iterator iter;
		for (iter = star_list.begin(); iter != star_list.end(); iter++) {
			logger(to_string(iter->getX()) + ", " + to_string(iter->getY()) + "\n");
		}
	}
}
void initializeCloudPoints() {
	GLfloat distance = scene_reset_distance;

	cloud_list.push_back(*new Point(-370 - distance, 505));
	cloud_list.push_back(*new Point(-370, 505));
	cloud_list.push_back(*new Point(-370 + distance, 505));
	cloud_list.push_back(*new Point(300, 625));
	cloud_list.push_back(*new Point(300 + distance, 625));
	cloud_list.push_back(*new Point(550, 725));
	cloud_list.push_back(*new Point(550 + distance, 725));
	cloud_list.push_back(*new Point(1200, 550));
	cloud_list.push_back(*new Point(1200 + distance, 550));

}
bool checkContact(Point player, Point orb, GLfloat offset) {
	GLfloat px = player.getX();
	GLfloat py = player.getY();
	GLfloat ox = orb.getX() + offset;
	GLfloat oy = orb.getY();
	GLfloat test_x = 0;
	GLfloat test_y = 0;
	GLfloat dist_x;
	GLfloat dist_y;
	GLfloat distance;

	// For simplicity purposes, we will be considering the player character as a rectangle rather than complex geometrical shape
	// Please reference the 'Chracter Design Model' for more details
	GLfloat player_height = player_size; // 2h + 2w
	GLfloat player_width = player_size * 1.5;  // 4w


	// Depending which side the orb collision occurs, we test for that edge
	if (px > ox) {
		// Left Side/Edge
		test_x = px;
	}
	else if (px + player_width < ox) {
		// Right Side/Edge
		test_x = px + player_width;
	}

	if (py > oy) {
		// Top Side/Edge
		test_y = py;
	}
	else if (py + player_height < oy) {
		// Bottom Side/Edge
		test_y = py + player_height;
	}

	// Now that we know which two edges to check, we use the Pythogoras theorem to compute the distance (hypotenuse)
	dist_x = ox - test_x;
	dist_y = oy - test_y;
	distance = sqrt((dist_x * dist_x) + (dist_y + dist_y));

	// We now compare this distance with the radius of the orb
	if (orb_size >= distance) {
		if (enable_logger) {
			logger("Contact Made! Orb(X, Y) = (" + to_string(ox) + ", " + to_string(oy) + ")\n");
		}
		score += 10;
		logger("Orb collected! Score = " + to_string(score) + "\n");
		return true;
	}
	else {
		return false;
	}
}
void checkAllOrbs(Point player, GLfloat offset) {
	// TODO - Change to foreach loop
	for (unsigned int i = 0; i < orb_list.size(); i++) {
		if (orb_contact[i] == false) {
			orb_contact[i] = checkContact(player, orb_list[i], offset);
			if ((i == game_over_orb) && (orb_contact[i] == true)) {
				endGame();
			}
		}
	}
}
void initializeEntropy() {
	// Seeding the Random Number Generator
	my_rand.seed(14);
}
void randomizeOrbPoints() {
	// Used for randomized and varied Orb generator
	int random_x;
	int random_y;
	int seed = 14;
	int deviation = 50;

	// Resetting the existing Orb Points
	initializeOrbPoints();

	vector<Point>::iterator iter;
	for (iter = orb_list.begin(); iter != orb_list.end(); iter++) {
		random_x = my_rand() % deviation - my_rand() % deviation; // [-deviation, +deviation]
		random_y = my_rand() % deviation - my_rand() % deviation;
		iter->setX(iter->getX() + random_x);
		iter->setY(iter->getY() + random_y);
	}

	if (enable_logger) {
		logger("Orb Points randomized!\n");
	}

}
void endGame() {
	*world_speed_ptr = 0.0F;
	*player_speed_ptr = 0.0F;

	logger("\n\t\tGAME OVER\n\n");
	logger("Your score was " + to_string(score) + "\n");
	//TODO - Display Game Over on the OpenGL Screen
}




// Drawing Objects at quasi-specific location Functions
void drawGround()
{
	//Ground
	glColor3f(0.0F, 0.3F, 0.0F);
	glBegin(GL_POLYGON);
	glVertex2f(-600, 0);
	glVertex2f(-600, 185);
	glVertex2f(1100, 185);
	glVertex2f(1100, 0);

	glVertex2f(WINDOW_WIDTH, 0);
	glVertex2f(250, -WINDOW_HEIGHT);
	glVertex2f(250, 0);
	glVertex2f(-WINDOW_WIDTH, 0);
	glEnd();
}
void drawSun(GLfloat offset)
{
	GLfloat sun_size = 50;
	glPointSize(1);
	glColor3f(1.0F, 0.9F, 0.0F);
	generateFilledCircle(200 + sun_offset * offset, 625, sun_size);

}
void drawMoon(GLfloat offset) {
	GLfloat moon_size = 35; //radius of the moon

	glPointSize(1);
	glColor3f(1.0, 1.0, 1.0);
	generateFilledCircle(100 + moon_offset * offset, 625, moon_size);
}
void drawClouds(GLfloat offset)
{
	vector<Point>::iterator iter;

	for (iter = cloud_list.begin(); iter != cloud_list.end(); iter++) {
		generateClouds(*iter, offset, 5);
	}
}
void drawTree(GLfloat offset) {
	/*
	The scene is rendered in a cyclic fashion
	This is done by reducing the offset by a constant value (scene_reset_distance) once it exceeds that value
	This essentially causes the scene to "reset"
	To prevent clipping and teleportation effects for the objects, each object needs to be in a pair
	The second member of the pair is placed at a constant distance from the first member
	This ensures that when the scene reset occurs, clipping effect does not occur
	*/
	generateTree(tree_list[0], offset, 6, 15, 70, 25);
	generateTree(tree_list[1], offset, 5, 40, 150, 40);
	generateTree(tree_list[2], offset, 4, 20, 95, 20);
	// Set A. First visible tree.
	generateTree(tree_list[3], offset, 5, 40, 150, 40);
	generateTree(tree_list[4], offset, 5, 40, 150, 40); // Scene reload occurs near here
	// Set B
	generateTree(tree_list[5], offset, 3, 15, 70, 25, BrightLavender);
	generateTree(tree_list[6], offset, 3, 15, 70, 25, BrightLavender);
	// Set C
	generateTree(tree_list[7], offset, 6, 15, 70, 25);
	generateTree(tree_list[8], offset, 6, 15, 70, 25);
	// Set D
	generateTree(tree_list[9], offset, 5, 40, 150, 40);
	generateTree(tree_list[10], offset, 5, 40, 150, 40);
	// Set E
	//generateTree(tree_list[11], offset, 5, 40, 150, 40);
	generateTree(tree_list[12], offset, 5, 40, 150, 40);
}
void drawStars(GLfloat offset) {
	/*
	Star generation, similar to tree generation makes use of scene resetting by reducing the value of offset
	For this to work smoothly, each object on the screen should have a corresponding pair at a constant distance
	This constant distance is stored in 'scene_reset_distance' variable
	*/
	vector<Point>::iterator iter;

	for (iter = star_list.begin(); iter != star_list.end(); iter++) {
		generateStar(*iter, offset);
	}
}
void drawOrbs(GLfloat offset) {
	vector<Point>::iterator iter;
	int i = 0;

	for (iter = orb_list.begin(); iter != orb_list.end(); iter++) {
		if (orb_contact[i] == false) {
			generateOrb(*iter, offset, color_set[i]);
		}
		i++;
	}
}


//TODO - Add a main menu


// Logger functions
void logger(const string& message) {
	ofstream log_file;

	cout << message;
	log_file.open("log.txt", ios::app);
	log_file << message;
	log_file.close();
}
void clear_log() {
	ofstream log_file;

	log_file.open("log.txt", ios::out);
	log_file.clear();
	log_file.close();
}
