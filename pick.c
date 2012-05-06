#include <stdio.h>
#if defined (__WIN32__)
  #include <windows.h>
#endif
#include <GL/glut.h>

#include "pick.h"
#include "main.h"
#include "world.h"

/**
 * pick.c
 *
 * Handles the picking an object.
 */
int handlePicking()
{
	renderMode = GL_SELECT; 
	glRenderMode(renderMode);
	glInitNames(); 
	glPushName(-1);
	displayFunction();
	renderMode = GL_RENDER; 
	hits = glRenderMode(renderMode);
	fprintf(stdout, "# pick hits = %d\n", hits);
	
	if(hits > 0)
    {
        int choose = pickBuffer[3];
        int depth = pickBuffer[1];
		int loop;

        for (loop = 1; loop < hits; loop++) {
			// select the closest object
			if (pickBuffer[loop*4+1] < (GLuint)depth)
			{
				choose = pickBuffer[loop*4+3];
				depth = pickBuffer[loop*4+1];
			}
        }
        printf("Selected item id => %d\n\n", choose);
	    object *selectedObject = getObjectById(choose);
	    if(selectedObject->data->selected) {
			selectedObject->data->selected = 0;
			selectedObjectId = 0;
	    }
	    else {
			selectedObject->data->selected = 1;
			selectedObjectId = choose;
		}
	  
		return 0;
    }
	
	return 1;
}