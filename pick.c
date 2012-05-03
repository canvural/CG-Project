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
 
 void handlePicking()
 {
	renderMode = GL_SELECT; 
	glRenderMode(renderMode);
	glInitNames(); 
	glPushName(-1);
	displayFunction();
	renderMode = GL_RENDER; 
	hits = glRenderMode(renderMode);
	fprintf(stderr, "# pick hits = %d\n", hits);
	
	if(hits > 0)
    {
        int choose = pickBuffer[3];
        int depth = pickBuffer[1];
		int loop;

        for (loop = 1; loop < hits; loop++) {
            // If This Object Is Closer To Us Than The One We Have Selected
            if (pickBuffer[loop*4+1] < (GLuint)depth)
            {
                choose = pickBuffer[loop*4+3];
                depth = pickBuffer[loop*4+1];
            }
        }
      printf("Selected item id => %d", choose);
	  object *selectedObject = getObjectById(choose);
	  selectedObject->data->selected = !selectedObject->data->selected;
    }
}