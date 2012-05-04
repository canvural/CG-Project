#include <stdio.h>
#if defined (__WIN32__)
  #include <windows.h>
#endif
#include <GL/glut.h>

#include "helper.h"

/**
 * Rotates the whole world to up by 10 degrees.
 */
void rotateUp()
{
	glRotatef(10, 1, 0, 0);
}

/**
 * Rotates the whole world to down by 10 degrees.
 */
void rotateDown()
{
	glRotatef(-10, 1, 0, 0);
}

/**
 * Rotates the whole world to left by 10 degrees.
 */
void rotateLeft()
{
	glRotatef(10, 0, 1, 0);
}

/**
 * Rotates the whole world to right by 10 degrees.
 */
void rotateRight()
{
	glRotatef(-10, 0, 1, 0);
}

GLdouble *convertScreenToWorld(int x, int y)
{
	GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;
	GLdouble *returnArray = malloc(sizeof(GLdouble) * 3);
	if(returnArray == NULL) {
		fprintf(stderr, "Memory could not allocated!");
		exit(EXIT_FAILURE);
	}
 
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );
 
    winX = (float)x;
    winY = (float)y;
    glReadPixels( x, (int) winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
 
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	
	returnArray[0] = posX;
	returnArray[1] = posY;
	returnArray[2] = posZ;
 
    return returnArray;
}