#include <stdio.h>
#if defined (__WIN32__)
  #include <windows.h>
#endif
#include <GL/glut.h>

#include "helper.h"
#include "main.h"
#include "world.h"

void createMenus()
{
	rotateMenu = glutCreateMenu(menuHandler);
	glutAddMenuEntry("Toggle X Axis", TOGGLE_X);
	glutAddMenuEntry("Toggle Y Axis", TOGGLE_Y);
	glutAddMenuEntry("Toggle Z Axis", TOGGLE_Z);
	
	colorMenu = glutCreateMenu(menuHandler);
	glutAddMenuEntry("Red", COLOR_RED);
	glutAddMenuEntry("Blue", COLOR_BLUE);
	glutAddMenuEntry("Black", COLOR_BLACK);
	glutAddMenuEntry("Yellow", COLOR_YELLOW);
	
	sizeMenu = glutCreateMenu(menuHandler);
	glutAddMenuEntry("0.5",SIZE_HALF);
	glutAddMenuEntry("1",SIZE_ONE);
	glutAddMenuEntry("2", SIZE_TWO);
	
	objectMenu = glutCreateMenu(menuHandler);
	glutAddSubMenu("Rotate",rotateMenu);
	glutAddSubMenu("Color",colorMenu);
	glutAddMenuEntry("Delete", DELETEOBJ);
	
	mainMenu = glutCreateMenu(menuHandler);
	glutAddMenuEntry("Save World", SAVE_WORLD);
	glutAddMenuEntry("Load World", LOAD_WORLD);
	glutAddSubMenu("Object Ops.",objectMenu);
	glutAddSubMenu("Object Size",sizeMenu);
	glutAddMenuEntry("Exit", EXIT);
	
	//attach main menu to right mouse click
	glutSetMenu (mainMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void menuHandler(int theChoosenOne)
{
	switch(theChoosenOne)
	{
		/*=== Rotation*/
		case TOGGLE_X: {
			if(selectedObjectId) {
				object *obj = getObjectById(selectedObjectId);
				obj->data->rotateArray[0] = !obj->data->rotateArray[0];
			}
			break;
		}
		case TOGGLE_Y: {
			if(selectedObjectId) {
				object *obj = getObjectById(selectedObjectId);
				obj->data->rotateArray[1] = !obj->data->rotateArray[1];
			}
			break;
		}
		case TOGGLE_Z: {
			if(selectedObjectId) {
				object *obj = getObjectById(selectedObjectId);
				obj->data->rotateArray[2] = !obj->data->rotateArray[2];
			}
			break;
		}
		/*=== Color*/
		case COLOR_RED: {
			if(selectedObjectId) {
				object *obj = getObjectById(selectedObjectId);
				obj->data->colorArray[0] = 255;
				obj->data->colorArray[1] = 0;
				obj->data->colorArray[2] = 0;
			}
			break;
		}
		case COLOR_YELLOW: {
			if(selectedObjectId) {
				object *obj = getObjectById(selectedObjectId);
				obj->data->colorArray[0] = 255;
				obj->data->colorArray[1] = 255;
				obj->data->colorArray[2] = 0;
			}
			break;
		}
		case COLOR_BLUE: {
			if(selectedObjectId) {
				object *obj = getObjectById(selectedObjectId);
				obj->data->colorArray[0] = 100;
				obj->data->colorArray[1] = 149;
				obj->data->colorArray[2] = 237;
			}
			break;
		}
		case COLOR_BLACK: {
			if(selectedObjectId) {
				object *obj = getObjectById(selectedObjectId);
				obj->data->colorArray[0] = 131;
				obj->data->colorArray[1] = 139;
				obj->data->colorArray[2] = 131;
			}
			break;
		}
		/*=== Size*/
		case SIZE_HALF: {
			if(selectedObjectId) {
				object *obj = getObjectById(selectedObjectId);
				obj->data->size = 0.5;
			}
			break;
		}
		case SIZE_ONE: {
			if(selectedObjectId) {
				object *obj = getObjectById(selectedObjectId);
				obj->data->size = 1;
			}
			break;
		}
		case SIZE_TWO: {
			if(selectedObjectId) {
				object *obj = getObjectById(selectedObjectId);
				obj->data->size = 2;
			}
			break;
		}
		case LOAD_WORLD: {
			destroyWorld();
			if(loadWorldFromFile("world.txt")) {
				fprintf(stdout, "World could not loaded! Here is why:\n\t");
				exit(-1);
			}
			break;
		}
		case DELETEOBJ: {
			if(selectedObjectId)
				if(deleteObjectById(selectedObjectId)){
					fprintf(stderr, "Something went wrong when deleting an object!");
					exit(EXIT_FAILURE);
				}
			break;
		}
		case EXIT: exit(EXIT_SUCCESS); break;
		default:break;
	}
}

/**
 * Converts the screen coordinates to OpenGL coordinates.
 * 
 * @param int x x coordinate of screen.
 * @param int y y coordinate of screen.
 * @return array x, y, and z coordinates of converted world
 */
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
 
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);
 
    winX = (float)x;
    winY = (float)y;
    glReadPixels(x, (int) winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
 
    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
	
	returnArray[0] = posX;
	returnArray[1] = posY;
	returnArray[2] = posZ;
 
    return returnArray;
}