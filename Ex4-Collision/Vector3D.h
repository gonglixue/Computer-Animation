#pragma once;

typedef struct Vector3D {
	float x;
	float y;
	float z;
} t_Vector3D;

void Sum(t_Vector3D *va, t_Vector3D *vb, t_Vector3D *vc, t_Vector3D *vr);

float AngleBetweenVectors(t_Vector3D *va, t_Vector3D *vb);

float DotProduct(t_Vector3D *va, t_Vector3D *vb);

float AbsValue(t_Vector3D *v);
