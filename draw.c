#include <stdio.h>
#include <stdlib.h>
#if defined (__WIN32__)
  #include <windows.h>
#endif
#include <GL/glut.h>

#include "draw.h"
#include "world.h"

void drawWorld()
{
	object *iterator = NULL;
	static float rotateBy = 0;
	rotateBy+=0.5;
		
	iterator = firstObject;
	while(iterator) {
		glPushMatrix();
		glTranslatef(iterator->data->translateArray[0], iterator->data->translateArray[1], iterator->data->translateArray[2]);
		if(iterator->data->selected) {
			glColor3f(0, 0, 0);
			glutWireSphere(1.8,20,20);
		}
		glRotatef(rotateBy,1,0,0);
		glColor3ubv(iterator->data->colorArray);
		glLoadName(iterator->data->id);
		switch(iterator->data->shape) {
			case 1: glutSolidTeapot(1); break;
			default: glutSolidTeapot(1); break;
		}
		glTranslatef(0-iterator->data->translateArray[0], 0-iterator->data->translateArray[1], 0-iterator->data->translateArray[2]);
		glPopMatrix();
		iterator = iterator->nextObject;
	}
}