#include <stdio.h>
#include <stdlib.h>
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
	rotateBy += 0.2;
	
	// iterate all the objects and draw them
	iterator = firstObject;
	while(iterator) {
		glPushMatrix();
		glTranslatef(iterator->data->translateArray[0], iterator->data->translateArray[1], iterator->data->translateArray[2]);
		// if the object is selected draw a yellow wire sphere around it. 
		if(iterator->data->selected) {
			glColor3f(255, 255, 0);
			glutWireSphere(1.8 * iterator->data->size, 20, 20);
			// focus the camera on object (not correctly working right now)
			/*eyeX = iterator->data->translateArray[0];
			eyeY = iterator->data->translateArray[1];
			eyeZ = iterator->data->translateArray[2];
			cameraX = iterator->data->translateArray[0] - 5;
			cameraY = iterator->data->translateArray[1] - 5;
			cameraZ = iterator->data->translateArray[2] - 5;*/
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
			case 1: (iterator->data->solidOrWire) ? glutSolidTeapot(iterator->data->size) : glutWireTeapot(iterator->data->size); break;
			case 2: (iterator->data->solidOrWire) ? glutSolidCube(iterator->data->size) : glutWireCube(iterator->data->size); break;
			case 3: (iterator->data->solidOrWire) ? glutSolidCone(0.8, iterator->data->size, 20, 20) : glutWireCone(0.8, iterator->data->size, 20, 20); break;
			
			default: glutSolidTeapot(1);
		}
		glPopMatrix();
		iterator = iterator->nextObject;
	}
}