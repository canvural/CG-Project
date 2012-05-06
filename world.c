#include <stdio.h>
#include <stdlib.h>
#if defined (__WIN32__)
  #include <windows.h>
#endif
#include <GL/glut.h>

#include "world.h"
#include "main.h"

void makeSolidObjects()
{
	object *iterator = firstObject;
	
	while(iterator) {
		iterator->data->solidOrWire = 1;
		
		iterator = iterator->nextObject;
	}
}

void makeWiredObjects()
{
	object *iterator = firstObject;
	
	while(iterator) {
		iterator->data->solidOrWire = 0;
		
		iterator = iterator->nextObject;
	}
}

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


/**
 * Creates a new default object using the parameter given.
 *
 * @param int shape Valid shape id of object.
 * @return int Returns zero on success, non-zero otherwise.
 */
/*int createNewObject(int shape)
{
	objectData *data    = NULL;
	object *objectToAdd = NULL;
	
	data = malloc(sizeof(objectData));
	objectToAdd = malloc(sizeof(object));
	if(data == NULL || objectToAdd == NULL){
		fprintf(stderr, "Memory colud not allocated for object!\n");
		return -1;
	}
	
	// place the data read from file into objects data
	data->id = ++numberOfObjects;
	
	data->size = 1;
	
	data->translateArray = malloc(sizeof(float) * 3);
	memcpy(data->translateArray, translateArray, 3 * sizeof(float));
	
	data->colorArray = malloc(sizeof(unsigned char) * 3);
	memcpy(data->colorArray, colorArray, 3 * sizeof(unsigned char));
	
	data->rotateArray = malloc(sizeof(int) * 3);
	memcpy(data->rotateArray, rotateArray, 3 * sizeof(int));
	
	data->shape = objectShape;
	data->solidOrWire = solidOrWire;
	data->selected = 0;
	
	int objectAdded = addObjectToWorld(objectToAdd, data);
	if(objectAdded) {
		fprintf(stderr, "Something went wrong when adding new object to list!\n");
		return -1;
	}
}*/

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

int deleteObjectById(int id)
{
	object *iterator = firstObject;
	object *previous = NULL;
	
	while(iterator) {
		if(iterator->data->id == id) {
			break;
		}
		previous = iterator;
		iterator = iterator->nextObject;
	}
	
	if(iterator) {
		if(previous == NULL) {
			if(firstObject->data->id == lastObject->data->id) {
				firstObject = NULL;
				lastObject = NULL;
			}
			else {
				firstObject = firstObject->nextObject;
			}
		}
		else {
			previous->nextObject = iterator->nextObject;
			if(previous->nextObject == NULL)
				lastObject = previous;
		}
		free(iterator->data);
		free(iterator);
		
		return 0;
	}
	
	return 1;
}

int destroyWorld()
{
	object *iterator = firstObject;
	object *nextObject = NULL;
	
	while(iterator) {
		nextObject = iterator->nextObject;
		
		free(iterator->data->translateArray);
		free(iterator->data->rotateArray);
		free(iterator->data->colorArray);
		free(iterator->data);
		free(iterator);
		iterator = nextObject;
	}
	firstObject = lastObject = NULL;
	
	return 0;
}

/**
 * Loads a world from, file given in parameter.
 * 
 * File format should be like this:
 *     x <= Number of objects
 *     (Id of object) (x, y, z coordinate of object separately) (RGB color codes of object) (x, y, z of rotate vector) (a valid object shape id) (will the object solid or wire)
 * 
 */
int loadWorldFromFile(char *fileName)
{
	float translateArray[3];
	unsigned char colorArray[3];
	int rotateArray[3];
	int objectShape;
	int id, solidOrWire, size;
	
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
		fscanf(file, "%d %d %f %f %f %u %u %u %d %d %d %d %d",
			   &id, &size, &translateArray[0], &translateArray[1], &translateArray[2], &colorArray[0], &colorArray[1], &colorArray[2],
			   &rotateArray[0], &rotateArray[1], &rotateArray[2], &objectShape, &solidOrWire
		) == 13
	)
	{
		#if defined (DEBUG)
			printf("\n=====DEBUG INFO=====\n");
			fprintf(stdout, "\nId = %d\nTranslate Array = %f, %f, %f\nColor Array = %u, %u, %u\nRotateArray = %d %d %d\nShape = %d\nSolidOrWire = %d\n========================",
			        id, translateArray[0], translateArray[1], translateArray[2], colorArray[0], colorArray[1], colorArray[2], 
					rotateArray[0], rotateArray[1], rotateArray[2], objectShape, solidOrWire
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
		
		data->size = size;
		
		data->translateArray = malloc(sizeof(float) * 3);
		memcpy(data->translateArray, translateArray, 3 * sizeof(float));
		
		data->colorArray = malloc(sizeof(unsigned char) * 3);
		memcpy(data->colorArray, colorArray, 3 * sizeof(unsigned char));
		
		data->rotateArray = malloc(sizeof(int) * 3);
		memcpy(data->rotateArray, rotateArray, 3 * sizeof(int));
		
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