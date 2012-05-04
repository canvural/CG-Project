/**
 * Computer Graphics Assigment
 * 
 * @author Can Vural, Engin Culhaci
 */

#include <stdio.h>
#if defined (__WIN32__)
  #include <windows.h>
#endif
#include <GL/glut.h>
#include <math.h>

// include our header files
#include "main.h"
#include "pick.h"


/**
 * Main entrance point to application.
 * 
 * First initialize the application, create the window then assign the callbacks and loop forever.
 * 
 * @return int
 */
int main(int argc, char** argv)
{
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	windowHeight = 500;windowWidth = 500;
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(50, 50);
    glutInit(&argc, argv);
	
	glutCreateWindow("Computer Graphics Project");
	
	// Callback functions
	glutDisplayFunc(displayFunction);
	glutReshapeFunc(reshapeFunction);
	glutIdleFunc(displayFunction);
	glutMouseFunc(mouseFunction);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);
	
	setupRenderSettings();
	
	glutMainLoop();
	
	return 0;
}

void setupRenderSettings()
{
	glShadeModel(GL_SMOOTH);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
	
	renderMode = GL_RENDER;
	
	fov = 45.0f;
	
	int worldLoaded = loadWorldFromFile("world.txt");
	if(worldLoaded) {
		fprintf(stdout, "World could not loaded! Here is why:\n\t");
		exit(-1);
	}
	
	// initalize the camera
	cameraX = cameraY = cameraZ = 0;
	eyeX = eyeY = 0;
	
	// select buffer
	glSelectBuffer( PICK_BUFFER_SIZE, pickBuffer );
}

void displayFunction()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	int viewport[4];
	if(renderMode == GL_SELECT) {
		glMatrixMode( GL_PROJECTION ); 
		glPushMatrix();
			glLoadIdentity();
			glGetIntegerv(GL_VIEWPORT, viewport);
			gluPickMatrix( (double) mouseX, (double) mouseY, PICK_TOL, PICK_TOL, viewport );
			gluPerspective(45, windowAspectRatio, 0.1, 100.0);
	}
	 
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	
	gluLookAt(
		cameraX, cameraY, cameraZ,
		eyeX, eyeY, cameraZ-0.1,
		0, 1, 0
	);
	
	drawWorld();
	
	/**
	 * Yerdeki kareler.
	 */
	glPushMatrix();
		GLfloat fExtent = 60.0f;
		GLfloat fStep = 1.0f;
		GLfloat y = -0.9f;
		GLint iLine;
		glColor3i(0, 0, 0);
		glBegin(GL_LINES);
			for(iLine = -fExtent; iLine <= fExtent; iLine += fStep)
			{
				glVertex3f(iLine, y, fExtent);
				glVertex3f(iLine, y, -fExtent);

				glVertex3f(fExtent, y, iLine);
				glVertex3f(-fExtent, y, iLine);
			}
		glEnd();
	glPopMatrix();
	if(renderMode == GL_SELECT) {
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	}
	glFlush();
	glutSwapBuffers();
}

void reshapeFunction(int width, int height)
{
    //make global the new window height and width
	windowWidth = width;
	windowHeight = height;
	windowAspectRatio = (GLfloat)windowWidth / (GLfloat)windowHeight;
	
	glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, windowAspectRatio, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void mouseFunction(int button, int state, int x, int y)
{
	y = windowHeight - y;
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
	    GLdouble *worldCoords = convertScreenToWorld(x, y);
		fprintf(stdout, "\nYou pressed left mouse button at [%d,%d] [%f %f]\n", x, y, worldCoords[0], worldCoords[1]);
		// make mouse location global for picking
		mouseX = x;
		mouseY = y;
		handlePicking();
	}
	else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		fprintf(stdout, "\nYou pressed right mouse button at [%d,%d]\n", x, y);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'd':
		case 'D':
			glTranslatef(-1, 0.0, 0.0);
			break;
		case 'a':
		case 'A':
			glTranslatef(1, 0.0, 0.0);
			break;
		case 'w':
		case 'W':
			glTranslatef(0.0, 0.0, 1);
			break;
		case 's':
		case 'S':
			glTranslatef(0.0, 0.0, -1);
			break;
		case 'q':
		case 'Q':
			glTranslatef(0.0, 1, 0.0);
			break;
		case 'e':
		case 'E':
			glTranslatef(0.0, -1, 0.0);
			break;
		case 27:            
			  exit (0);
			  break;
	}
}

void specialKeyboard(int key, int x, int y)
{
     switch(key)
     {
        case(GLUT_KEY_UP):
            goForward();
            break;
        case(GLUT_KEY_DOWN):
            goBackward();
            break;
        case(GLUT_KEY_LEFT):
            goLeft();
            break;
        case(GLUT_KEY_RIGHT):
            goRight();
            break;
     }
}