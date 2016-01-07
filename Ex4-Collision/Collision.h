
#include "Particle.h"   
#include "Vector3D.h";


void OpenGLInit(void);

static void Animate(void );
static void Key_r(void );
static void Key_s(void );
static void ResizeWindow(int w, int h);

static void KeyPressFunc( unsigned char Key, int x, int y );
static void CalcCollision(t_Vector3D *va, t_Vector3D *vb);
static void CreateParticles();
static int gen_rand(int);
