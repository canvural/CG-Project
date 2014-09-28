#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#if defined (__WIN32__)
  #include <windows.h>
#endif
#include <GL/glut.h>

#include "draw.h"
#include "world.h"
#include "main.h"

void drawWorld()
{
	object *iterator = NULL;
	
	// rotating speed
	static float rotateBy = 0;
	rotateBy += 10;
	if(rotateBy > 360)
		rotateBy = 0;
	
	// iterate all the objects and draw them
	iterator = firstObject;
	while(iterator) {
		glPushMatrix();
		if(gravity) {
			if(iterator->data->translateArray[1] > 0)
				iterator->data->translateArray[1] -= 0.1;
		}
		if(pervaneX && floor(abs(iterator->data->translateArray[1])) == 0) {
			iterator->data->rotateArray[0] = 0;
			iterator->data->rotateArray[1] = 0;
			iterator->data->rotateArray[2] = -1;
				
			iterator->data->translateArray[0] += 0.2;
		}
		if(pervaneY && iterator->data->translateArray[1] <= 10) {
			iterator->data->rotateArray[0] = 0;
			iterator->data->rotateArray[1] = 0;
			iterator->data->rotateArray[2] = -1;
				
			iterator->data->translateArray[1] += 0.2;
		}
		if(pervaneZ) {
			iterator->data->rotateArray[0] = 0;
			iterator->data->rotateArray[1] = 0;
			iterator->data->rotateArray[2] = -1;
				
			iterator->data->translateArray[2] += 0.2;
		}
		
		// minus
		if(pervaneXe && floor(abs(iterator->data->translateArray[1])) == 0) {
			iterator->data->rotateArray[0] = 0;
			iterator->data->rotateArray[1] = 0;
			iterator->data->rotateArray[2] = -1;
				
			iterator->data->translateArray[0] -= 0.2;
		}
		
		if(pervaneYe && floor(abs(iterator->data->translateArray[1])) > 0) {
			iterator->data->rotateArray[0] = 0;
			iterator->data->rotateArray[1] = 0;
			iterator->data->rotateArray[2] = -1;
				
			iterator->data->translateArray[1] -= 0.2;
		}
		
		if(pervaneZe) {
			iterator->data->rotateArray[0] = 0;
			iterator->data->rotateArray[1] = 0;
			iterator->data->rotateArray[2] = -1;
				
			iterator->data->translateArray[2] -= 0.2;
		}
		
		glTranslatef(iterator->data->translateArray[0], iterator->data->translateArray[1], iterator->data->translateArray[2]);
		// if the object is selected draw a yellow wire sphere around it. 
		if(iterator->data->selected) {
			glColor3f(255, 255, 0);
			glutWireSphere(1.8 * iterator->data->size, 20, 20);
		}
		// if a rotate array is specified in object properties, rotate the object
		if((iterator->data->rotateArray[0] + iterator->data->rotateArray[1] + iterator->data->rotateArray[2]) != 0) {
			glRotatef(rotateBy, iterator->data->rotateArray[0], iterator->data->rotateArray[1], iterator->data->rotateArray[2]);
		}
		glColor3ubv(iterator->data->colorArray);
		glLoadName(iterator->data->id);
		
		// draw the object based on its 'shape' attribute
		/**
		 * 1 - Solid Teapot
		 * 2 - Solid Cube
		 * 3 - Solid Cone
		 */
		switch(iterator->data->shape) {
			case 1: (iterator->data->solidOrWire) ? glutSolidIcosahedron() : glutWireTeapot(iterator->data->size); break;
			case 2: (iterator->data->solidOrWire) ? glutSolidCube(iterator->data->size) : glutWireCube(iterator->data->size); break;
			case 3: (iterator->data->solidOrWire) ? glutSolidCone(0.8, iterator->data->size, 20, 20) : glutWireCone(0.8, iterator->data->size, 20, 20); break;
			
			default: glutSolidTeapot(1);
		}
		glPopMatrix();
		iterator = iterator->nextObject;
	}
}
