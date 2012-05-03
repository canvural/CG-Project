#ifndef MAIN_H
#define MAIN_H

/*============== Global used variables */
int windowHeight, windowWidth;
float windowAspectRatio;
float fov;
// Coordinates of camera at any time
float cameraX,
	  cameraY,
	  cameraZ;
float eyeX,
	  eyeY;
int mouseX,
	mouseY;

/*============== Flag variables used globally in our program */
GLenum renderMode;

/*============== Callback Function Declarations  */
void displayFunction();
void reshapeFunction(int w, int h);
void mouseFunction(int button, int state, int x, int y);
void keyboard(unsigned char key,int x, int y);
void specialKeyboard(int key, int x, int y);

/*============== Function Declarations */
void setupRenderSettings();
#endif