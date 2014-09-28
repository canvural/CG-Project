#ifndef MAIN_H
#define MAIN_H

/*============== Global used variables */
int windowHeight, windowWidth;
float windowAspectRatio;
float fov;
int mouseX,
    mouseY;
	
// angle of rotation for the camera direction
float angle;

float xpos, ypos, zpos, xrot, yrot, angle;
float xrotrad, yrotrad;

/*============== Flag variables used globally in our program */
GLenum renderMode;
// holds the id of object if an object selected otherwise its 0
unsigned int selectedObjectId;
int gravity;
int pervaneX, pervaneY, pervaneZ;
int pervaneXe, pervaneYe, pervaneZe;

/*============== Callback Function Declarations  */
void displayFunction();
void reshapeFunction(int w, int h);
void mouseFunction(int button, int state, int x, int y);
void keyboard(unsigned char key,int x, int y);
void specialKeyboard(int key, int x, int y);
void passiveMotion(int x, int y);
void motionFunc(int x, int y);
void camera (void);

/*============== Function Declarations */
void setupRenderSettings();
#endif