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