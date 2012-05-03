#include <stdio.h>
#include <stdlib.h>
#if defined (__WIN32__)
  #include <windows.h>
#endif
#include <GL/glut.h>

#include "world.h"
#include "main.h"


object *getObjectById(int id)
{
	object *iterator = firstObject;
	
	while(iterator) {
		if(iterator->data->id == id) {
			return iterator;
		}
		iterator = iterator->nextObject;
	}
	
	return NULL;
}

int addObjectToWorld(object *objectToAdd, objectData *data)
{
	if(firstObject != NULL) {
		objectToAdd->data = data;
		lastObject->nextObject = objectToAdd;
		lastObject = objectToAdd;
		lastObject->nextObject = NULL;
	}
	else {
		objectToAdd->data = data;
		firstObject = objectToAdd;
		lastObject = objectToAdd;
		lastObject->nextObject = NULL;
	}
	
	return 0;
}

int loadWorldFromFile(char *fileName)
{
	float translateArray[3];
	unsigned char colorArray[3];
	int objectShape;
	int id, solidOrWire;
	
	objectData *data    = NULL;
	object *objectToAdd = NULL;
	firstObject = NULL;
	lastObject = NULL;
	
	
	FILE *file = fopen(fileName, "r");
	if(file == NULL) {
		fprintf(stderr, "\"%s\" could not found!", fileName);
		return -1;
	}
	
	if(fscanf(file, "%d", &numberOfObjects) != 1) {
		fprintf(stderr, "%s is not a valid world file!", fileName);
		return -1;
	}
	
	if(numberOfObjects == 0) {
		fprintf(stderr, "World is empty!");
		return -1;
	}
	
	// 
	while(
		fscanf(file, "%d %f %f %f %u %u %u %d %d",
			   &id, &translateArray[0], &translateArray[1], &translateArray[2], &colorArray[0], &colorArray[1], &colorArray[2], &objectShape, &solidOrWire
		) == 9
	)
	{
		#if defined (DEBUG)
			printf("\n=====DEBUG INFO=====\n");
			fprintf(stdout, "\nId = %d\nTranslate Array = %f, %f, %f\nColor Array = %u, %u, %u\nShape = %d\nSolidOrWire = %d\n========================",
			        id, translateArray[0], translateArray[1], translateArray[2], colorArray[0], colorArray[1], colorArray[2], objectShape, solidOrWire
			);
		#endif
		
		// allocate space for new object
		data = malloc(sizeof(objectData));
		objectToAdd = malloc(sizeof(object));
		if(data == NULL || objectToAdd == NULL){
			fprintf(stderr, "Memory colud not allocated for object!\n");
			return -1;
		}
		
		// place the data read from file into objects data
		data->id = id;
		data->translateArray = malloc(sizeof(float) * 3);
		memcpy(data->translateArray,translateArray,3*sizeof(float));
		data->colorArray = malloc(sizeof(unsigned char) * 3);
		memcpy(data->colorArray,colorArray,3*sizeof(unsigned char));
		data->shape = objectShape;
		data->solidOrWire = solidOrWire;
		data->selected = 0;
		
		int objectAdded = addObjectToWorld(objectToAdd, data);
		if(objectAdded) {
			fprintf(stderr, "Something went wrong when adding new object to list!\n");
			return -1;
		}
	}// end while
	
	fclose(file);
	
	return 0;
}

void goForward()
{
	cameraZ += 0.2;
}
void goBackward()
{
	cameraZ -= 0.2;
}
void goLeft()
{
	cameraY += 0.2;
}
void goRight()
{
	cameraY -= 0.2;
}