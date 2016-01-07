#include "Vector3D.h"
#include <math.h>;

void Sum(t_Vector3D *va, t_Vector3D *vb, t_Vector3D *vc, t_Vector3D *vr)
{
	vr->x  = va->x + vb->x + vc->x;
	vr->y  = va->y + vb->y + vc->y;
	vr->z  = va->z + vb->z + vc->z;
}

float AngleBetweenVectors(t_Vector3D *va, t_Vector3D *vb)
{
	return acos(DotProduct(va, vb)/(AbsValue(va)*AbsValue(vb)));
}

float DotProduct(t_Vector3D *va, t_Vector3D *vb)
{
	return va->x * vb->x + va->y * vb->y + va->z * vb->z;
}

float AbsValue(t_Vector3D *v)
{
	return sqrt(powf(v->x,2.0) + powf(v->y,2.0) + powf(v->z,2.0));
}
