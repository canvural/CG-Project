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
#include "world.h"


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
	glutPassiveMotionFunc(passiveMotion);
	glutMotionFunc(motionFunc);
	
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
	
	xpos = 0; ypos = 0; zpos = 0; xrot = 0; yrot = 90; angle=0.0;
	
	fov = 45.0f;
	gravity = 0;
	pervaneX = 0;
	pervaneY = 0;
	pervaneZ = 0;
	pervaneXe = 0;
	pervaneYe = 0;
	pervaneZe = 0;
	
	// light stuff
	glEnable(GL_LIGHTING);
	
	glEnable(GL_COLOR_MATERIAL);
	//glColorMaterial(GL_FRONT,GL_DIFFUSE);
	glEnable(GL_LIGHT0);
	GLfloat ambientLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
	GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat position[] = { 5.0f, 5.0f, 9.0f, 0.0f };

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
	
	gluLookAt(0, 1.0f, 20,
		  0, 1.0f,  19,
		  0.0f, 1.0f,  0.0f);
	camera();
	
	drawWorld();
	
	/**
	 * Yerdeki kareler.
	 */
	glPushMatrix();
		GLfloat fExtent = 30.0f;
		GLfloat fStep = 1.0f;
		GLfloat y = -0.2f;
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
	
	// ust taraftaki kafes
	glPushMatrix();
		fExtent = 30.0f;
		fStep = 1.0f;
		y = 10.0f;
		iLine = 0;
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

void camera (void) {
    glRotatef(xrot,1.0,0.0,0.0); 
    glRotatef(yrot,0.0,1.0,0.0); 
    glTranslated(-xpos,-ypos,-zpos); 
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
			destroyWorld();
			glutLeaveGameMode();
			exit(EXIT_SUCCESS);
			break;
		case 'o':
			glEnable(GL_LIGHT0);
			break;
		case 'p':
			glDisable(GL_LIGHT0);
			break;
		case 'g':
			gravity = !gravity;
			break;
		case 'x':
			pervaneX = 1;
			pervaneY = 0;
			pervaneZ = 0;
			pervaneXe = 0;
			pervaneYe = 0;
			pervaneZe = 0;
			break;
		case 'y':
			pervaneX = 0;
			pervaneY = 1;
			pervaneZ = 0;
			pervaneXe = 0;
			pervaneYe = 0;
			pervaneZe = 0;
			break;
		case 'z':
			pervaneX = 0;
			pervaneY = 0;
			pervaneZ = 1;
			pervaneXe = 0;
			pervaneYe = 0;
			pervaneZe = 0;
			break;
		// minus
		case 'v':
			pervaneXe = 1;
			pervaneYe = 0;
			pervaneZe = 0;
			pervaneX = 0;
			pervaneY = 0;
			pervaneZ = 0;
			break;
		case 'u':
			pervaneXe = 0;
			pervaneYe = 1;
			pervaneZe = 0;
			pervaneX = 0;
			pervaneY = 0;
			pervaneZ = 0;
			break;
		case 'c':
			pervaneXe = 0;
			pervaneYe = 0;
			pervaneZe = 1;
			pervaneX = 0;
			pervaneY = 0;
			pervaneZ = 0;
			break;
		case 'b':
			pervaneX = 0;
			pervaneY = 0;
			pervaneZ = 0;
			pervaneXe = 0;
			pervaneYe = 0;
			pervaneZe = 0;
			break;
		/*======== CAMERA ROTATIONS*/
		// move forward
		case 'w':
			yrotrad = (yrot / 180 * 3.141592654f);
			xrotrad = (xrot / 180 * 3.141592654f); 
			xpos += (float)(sin(yrotrad)) ;
			zpos -= (float)(cos(yrotrad)) ;
			ypos -= (float)(sin(xrotrad)) ;
			break;
		// turn left
		case 'a':
			yrot += 1;
			if (yrot >360)
				yrot -= 360;
			break;
		//move backward
		case 's':
			yrotrad = (yrot / 180 * 3.141592654f);
			xrotrad = (xrot / 180 * 3.141592654f); 
			xpos -= (float)(sin(yrotrad));
			zpos += (float)(cos(yrotrad)) ;
			ypos += (float)(sin(xrotrad));
			break;
		// turn right
		case 'd':
			yrot -= 1;
			if (yrot < -360)
				yrot += 360;
			break;
		default: break;
	}
}

void specialKeyboard(int key, int x, int y)
{
    switch (key) {
		case GLUT_KEY_UP :
			xrot += 1;
			if (xrot >360)
				xrot -= 360;
			break;
		case GLUT_KEY_DOWN :
			xrot -= 1;
			if (xrot < -360)
				xrot += 360;
			break;
	}
}

void passiveMotion(int x, int y)
{
	mouseX = x;
	mouseY = windowHeight - y;
	//printf("%d, %d\n", mouseX, mouseY);
}

void motionFunc(int x, int y)
{
	GLdouble *worldCoords = convertScreenToWorld(x, windowHeight - y);
	
	// if an object selected and dragging with mouse, move the objectwith the mouse
	if(selectedObjectId) {
		object *obj = getObjectById(selectedObjectId);
		
		obj->data->translateArray[0] =  worldCoords[0];
		obj->data->translateArray[1] =  worldCoords[1];
		obj->data->translateArray[2] =  worldCoords[2];
	}
}