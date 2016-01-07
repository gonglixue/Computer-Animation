#pragma once;

#include "Vector3D.h";

typedef struct particle {
	float radius;
	float slices;
	float stacks;
	float x;
	float y;
	float z;
	float vx;
	float vy;
	float vz;
	float r;
	float g;
	float b; 
} t_Particle;

void Direction(t_Vector3D *vector);
