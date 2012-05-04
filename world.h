#ifndef WORLD_H
#define WORLD_H
#if defined (__WIN32__)
  #include <windows.h>
#endif
#include <GL/glut.h>

/**
 * world.h
 * 
 * 
 */
 
 
//DEBUG MODE
#define DEBUG
 
typedef struct {
	unsigned int id;
	float *translateArray;
	int *rotateArray;
	unsigned char *colorArray;
	int shape;
	int solidOrWire;
	int selected;
} objectData;

typedef struct _object{
	objectData *data;
	struct _object *nextObject;
} object;

object *firstObject,
       *lastObject;
// Holds the number of objects in the world at any time.	   
int numberOfObjects;

/*========= Function Declarations*/
int addObjectToWorld(object *objectToAdd, objectData *data);
int loadWorldFromFile(char *fileName);
object *getObjectById(int id);
void goForward();
void goBackward();
void goLeft();
void goRight();

#endif