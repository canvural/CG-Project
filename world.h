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
	unsigned int id; // unique integer to identify object
	float size; // requried for some objects
	float *translateArray; // x, y, z coordinates of object relative to origin
	int *rotateArray;// x, y, z rotate vector
	unsigned char *colorArray;// RGB color values of object
	int shape;// indicates the shape of object. see draw.c for valid values
	int solidOrWire;// indicates the object is solid or wired
	int selected;// flag for if the object is selected or not
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
int deleteObjectById(int id);
int destroyWorld();
void makeSolidObjects();
void makeWiredObjects();

#endif