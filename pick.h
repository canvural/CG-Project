#ifndef PICK_H
#define PICK_H

#define PICK_TOL   0.5 
#define PICK_BUFFER_SIZE 256 

unsigned int pickBuffer[PICK_BUFFER_SIZE];
int hits;
int i, j, nitems, zmin, zmax, indeks, item;

void handlePicking();

#endif