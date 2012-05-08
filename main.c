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
#include "helper.h"


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
	//glutIdleFunc(displayFunction);
	glutMouseFunc(mouseFunction);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);
	glutPassiveMotionFunc(passiveMotion);
	
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
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
	
	renderMode = GL_RENDER;
	
	fov = 45.0f;
	
	// light stuff
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	//glColorMaterial(GL_FRONT,GL_DIFFUSE);
	glEnable(GL_LIGHT0);
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
	GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat position[] = { -1.5f, 1.0f, -4.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	
	// create menus
	createMenus();
	
	int worldLoaded = loadWorldFromFile("world.txt");
	if(worldLoaded) {
		fprintf(stdout, "World could not loaded! Here is why:\n\t");
		exit(-1);
	}
	
	// angle of rotation for the camera direction
	angle=0.0;
	// actual vector representing the camera's direction
	lx=0.0f,lz=-1.0f;
	// XZ position of the camera
	x=0.0f,z=20.0f;
	
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
	
	/*gluLookAt(
		cameraX, cameraY, cameraZ,
		eyeX, eyeY, eyeZ-0.1,
		0, 1, 0
	);*/
	gluLookAt(	x, 1.0f, z,
			x+lx, 1.0f,  z+lz,
			0.0f, 1.0f,  0.0f);
	
	drawWorld();
	
	/**
	 * Yerdeki kareler.
	 */
	glPushMatrix();
		GLfloat fExtent = 30.0f;
		GLfloat fStep = 1.0f;
		GLfloat y = -1.0f;
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
	
	if(renderMode == GL_RENDER)
		glutPostRedisplay();
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
		fprintf(stdout, "\nYou pressed left mouse button at [%d,%d] [%f %f %f]\n", x, y, worldCoords[0], worldCoords[1], worldCoords[2]);
		// make mouse location global for picking
		mouseX = x;
		mouseY = y;
		handlePicking();
	}
	else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		fprintf(stdout, "\nYou pressed right mouse button at [%d,%d]\n", x, y);
	}
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 't':
		case 'T': 
			makeWiredObjects();
			break;
		case 'k':
		case 'K': 
			makeSolidObjects();
			break;
		case 'q':
			exit(EXIT_SUCCESS);
			break;
		default: break;
	}
}

void specialKeyboard(int key, int x, int y)
{
     float fraction = 0.3f;

	switch (key) {
		case GLUT_KEY_LEFT :
			angle -= 0.05f;
			lx = sin(angle);
			lz = -cos(angle);
			break;
		case GLUT_KEY_RIGHT :
			angle += 0.05f;
			lx = sin(angle);
			lz = -cos(angle);
			break;
		case GLUT_KEY_UP :
			x += lx * fraction;
			z += lz * fraction;
			break;
		case GLUT_KEY_DOWN :
			x -= lx * fraction;
			z -= lz * fraction;
			break;
	}
}

void passiveMotion(int x, int y)
{
	mouseX = x;
	mouseY = windowHeight - y;
	//printf("%d, %d\n", mouseX, mouseY);
}