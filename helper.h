#ifndef HELPER_H
#define HELPER_H

/**========== define constants to use in menus */
// Main Menu
#define SAVE_WORLD 1
#define LOAD_WORLD 2
#define EXIT 3
// Object Menu
#define TOGGLE_X 4
#define TOGGLE_Y 5
#define TOGGLE_Z 6
#define COLOR_RED 7
#define COLOR_YELLOW 8
#define COLOR_BLUE 9
#define COLOR_BLACK 10
#define SIZE_HALF 11
#define SIZE_ONE 12
#define SIZE_TWO 13
#define DELETEOBJ 14

// menu ids
int mainMenu,
	objectMenu,
	rotateMenu,
	colorMenu,
	sizeMenu;

GLdouble *convertScreenToWorld(int x, int y);
void createMenus();
void menuHandler(int theChoosenOne);
#endif