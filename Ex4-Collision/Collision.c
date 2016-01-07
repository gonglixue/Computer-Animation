/*
 */

#include <stdio.h>
#include "Collision.h"   
#include <stdlib.h> 
#include <time.h>
#include <math.h>
#include "glut.h"	// OpenGL Graphics Utility Library
#include "Vector3D.h";

#define PARTICLES_NUMBER 100
#define PARTICLE_RADIUS 1

int BOX_SIZE = 200;

static GLenum spinMode = GL_TRUE;
static GLenum singleStep = GL_FALSE;

static struct particle particles[PARTICLES_NUMBER];

float eye[] = { 0, 0, 200 * 2 };
float center[] = { 0, 0, 0 };

// glutKeyboardFunc is called below to set this function to handle
//		all normal key presses.  
static void KeyPressFunc( unsigned char Key, int x, int y )
{
	switch ( Key ) {
	case 'R':
	case 'r':
		Key_r();
		break;
	case 's':
	case 'S':
		Key_s();
		break;
	case 27:	// Escape key
		exit(1);

	case 'i':
	case 'I':
		eye[1] += 10;
		break;
	case 'k':
	case 'K':
		eye[1] -= 10;
		break;
	case 'j':
	case 'J':
		eye[0] -= 10;
		break;
	case 'l':
	case 'L':
		printf("l");
		eye[0] += 10;
		break;
	case 'm':
	case 'M':{
				 eye[2] += 10;
				 center[2] += 10;
				 break;
	}
	case 'n':
	case 'N':{
				 eye[2] -= 10;
				 center[2] -= 10;
				 break;
	}
	case '+':
		BOX_SIZE += 10;
		break;
	case '-':
		BOX_SIZE -= 10;
		break;
	case ' ':{
				 CreateParticles();
				 break;
	}
	}


}

static void Key_r(void)
{
	if ( singleStep ) {			// If ending single step mode
		singleStep = GL_FALSE;
		spinMode = GL_TRUE;		// Restart animation
	}
	else {
		spinMode = !spinMode;	// Toggle animation on and off.
	}
}

static void Key_s(void)
{
	singleStep = GL_TRUE;
	spinMode = GL_TRUE;
}

static int existsParticleInPos(int i, float x, float y, float z)
{
	int result = 0;
	int j;
	for (j = 0; j < PARTICLES_NUMBER; j++)
		{
			if (i != j)
			{
				float d = sqrt(pow(particles[i].x - particles[j].x, 2) + 
					pow(particles[i].y - particles[j].y, 2) +
					pow(particles[i].z - particles[j].z, 2));
				if (d < particles[i].radius + particles[j].radius)
				{
					result = 1;
					break;
				}
			}
		}
	return result;
}

static void CreateParticles()
{
	int i;
	srand((unsigned)(time(0))); 
	for (i = 0; i < PARTICLES_NUMBER; i++)
	{
		int size = BOX_SIZE;
		particles[i].x = (gen_rand(size)-(size/2))/1.0;
		particles[i].y = (gen_rand(size)-(size/2))/1.0;
		particles[i].z = (gen_rand(size)-(size/2))/1.0;
		while (existsParticleInPos(i, particles[i].x, particles[i].y, particles[i].z))
		{
			particles[i].x = (gen_rand(size)-(size/2))/1.0;
			particles[i].y = (gen_rand(size)-(size/2))/1.0;
			particles[i].z = (gen_rand(size)-(size/2))/1.0;
		}
		//particles[i].radius = PARTICLE_RADIUS;
		particles[i].radius = gen_rand(PARTICLE_RADIUS * 5);
		particles[i].slices = 20;
		particles[i].stacks = 20;
		particles[i].r = (gen_rand(10))/10.0;
		particles[i].g = (gen_rand(10))/10.0;
		particles[i].b = (gen_rand(10))/10.0;
		particles[i].vx = (gen_rand(10)-5)/10.0;
		particles[i].vy = (gen_rand(10)-5)/10.0;
		particles[i].vz = (gen_rand(10)-5)/10.0;
	}
}

/*
 * Draw the particles container.
 */
static void DrawBox()
{
	glColor3f(255, 255, 255);
	glutWireCube(BOX_SIZE);
}

/*
 * Detects collision between all particles.
 */
static void DetectCollision()
{
	int i;
	int j;
	//Temporary array to store the result velocity (a vector) for each particle.
	struct Vector3D velocities[PARTICLES_NUMBER];

	//Calculates the collision between all particles. It's naive because it's a very simplistic approach. 
	//We could somehow store the particles near each other and only calculate the collision between them.
	for (i = 0; i < PARTICLES_NUMBER; i++)
	{
		//Acumulates the velocities sums for each axis. 
		//The result velocity of an axis for a given particle is the sum of this axis velocity with the velocities of the same axis for each colliding particle.
		float sumvx = 0.0;
		float sumvy = 0.0;
		float sumvz = 0.0;
		
		//Iterates through all particles.
		for (j = 0; j < PARTICLES_NUMBER; j++)
		{
			//If it's not the current particle.
			if (i != j)
			{
				//Calculates the distance between particles centers.
				float d = sqrt(pow(particles[i].x - particles[j].x, 2) + 
					pow(particles[i].y - particles[j].y, 2) +
					pow(particles[i].z - particles[j].z, 2));
				//If the distance is equal or less than the sum of the radius, we have a collision.
				if (d <= particles[i].radius + particles[j].radius)
				{
					//Accumulates the velocities sum for each axis.
					//sumvx += particles[j].vx;
					//sumvy += particles[j].vy;
					//sumvz += particles[j].vz;

					//float t;  // exchange velocity
					//t = particles[i].vx;
					//particles[i].vx = particles[j].vx;
					//particles[j].vx = t;

					//t = particles[i].vy;
					//particles[i].vy = particles[j].vy;
					//particles[j].vy = t;

					//t = particles[i].vz;
					//particles[i].vz = particles[j].vz;
					//particles[j].vz = t;
				
					float m1 = particles[i].radius;
					float m2 = particles[j].radius;
					float v1 = particles[i].vx;
					float v2 = particles[j].vx;

					float v1_new = ((m1 - m2)*v1 + 2 * m2*v2) / (m1 + m2);
					float v2_new = ((m2 - m1)*v2 + 2 * m1*v1) / (m1 + m2);
					particles[i].vx = v1_new;
					particles[j].vx = v2_new;

					v1 = particles[i].vy;
					v2 = particles[j].vy;
					v1_new = ((m1 - m2)*v1 + 2 * m2*v2) / (m1 + m2);
					 v2_new = ((m2 - m1)*v2 + 2 * m1*v1) / (m1 + m2);
					particles[i].vy = v1_new;
					particles[j].vy = v2_new;

					v1 = particles[i].vz;
					v2 = particles[j].vz;
					v1_new = ((m1 - m2)*v1 + 2 * m2*v2) / (m1 + m2);
					v2_new = ((m2 - m1)*v2 + 2 * m1*v1) / (m1 + m2);
					particles[i].vz = v1_new;
					particles[j].vz = v2_new;


				}
			}
		}
		//Stores the results in the temporary array.
		if (sumvx > 0)
			velocities[i].x = sumvx;
		else
			velocities[i].x = particles[i].vx;
		if (sumvy > 0)
			velocities[i].y = sumvy;
		else
			velocities[i].y = particles[i].vy;
		if (sumvz > 0)
			velocities[i].z = sumvz;
		else
			velocities[i].z = particles[i].vz;

	}

	//Stores the results in the temporary array back in the particles array.
	//Then tests if the particles hit a box face, preventing it from "escaping" from inside the box.
	for (i = 0; i < PARTICLES_NUMBER; i++)
	{
		//Stores calculated velocities back in the particles array.
		particles[i].vx = velocities[i].x;
		particles[i].vy = velocities[i].y;
		particles[i].vz = velocities[i].z;

		//Tests if a particle is about to extrapolate the box limits.
		if (particles[i].x + particles[i].vx - particles[i].radius < -BOX_SIZE/2 || particles[i].x + particles[i].vx + particles[i].radius > BOX_SIZE/2)
		{
			//If so, tests wether it's a positive or negative limit.
			if (particles[i].x + particles[i].vx - particles[i].radius < -BOX_SIZE/2)
			{
				float offset = 0;
				//Adjusts the particle position, so it doesn't get out of the box.
				particles[i].x = -BOX_SIZE/2 + particles[i].radius + offset;
				//Tests if there are other particles in the adjusted position.
				while (existsParticleInPos(i, particles[i].x, particles[i].y, particles[i].z))
				{
					//If so, we add an offset to the adjusted position.
					offset += particles[i].radius / 10;
					particles[i].x = -BOX_SIZE/2 + particles[i].radius + offset;
				}
			}
			else
			if (particles[i].x + particles[i].vx + particles[i].radius > BOX_SIZE/2)
			{
				float offset = 0;
				particles[i].x = BOX_SIZE/2 - particles[i].radius - offset;
				while (existsParticleInPos(i, particles[i].x, particles[i].y, particles[i].z))
				{
					offset += particles[i].radius / 10;
					particles[i].x = BOX_SIZE/2 - particles[i].radius - offset;
				}
			}
			//The particle hit one of the faces, so we need to invert its orientation.
			particles[i].vx = - particles[i].vx;
		}
		else
		{
			//The particle didn't hit any faces, so we increase its position according to its velocity.
			particles[i].x += particles[i].vx;
		}
		//The same for the y axis.
		if (particles[i].y + particles[i].vy - particles[i].radius < -BOX_SIZE/2 || particles[i].y + particles[i].vy + particles[i].radius > BOX_SIZE/2)
		{
			if (particles[i].y + particles[i].vy - particles[i].radius < -BOX_SIZE/2)
			{
				float offset = 0;
				particles[i].y = -BOX_SIZE/2 + particles[i].radius + offset;
				while (existsParticleInPos(i, particles[i].x, particles[i].y, particles[i].z))
				{
					offset += particles[i].radius / 10;
					particles[i].y = -BOX_SIZE/2 + particles[i].radius + offset;
				}
			}
			else
			if (particles[i].y + particles[i].vy + particles[i].radius > BOX_SIZE/2)
			{
				float offset = 0;
				particles[i].y = BOX_SIZE/2 - particles[i].radius - offset;
				while (existsParticleInPos(i, particles[i].x, particles[i].y, particles[i].z))
				{
					offset += particles[i].radius / 10;
					particles[i].y = BOX_SIZE/2 - particles[i].radius - offset;
				}
			}

			particles[i].vy = - particles[i].vy;
		}
		else
		{
			particles[i].y += particles[i].vy;
		}
		//And for the z axis.
		if (particles[i].z + particles[i].vz - particles[i].radius < -BOX_SIZE/2 || particles[i].z + particles[i].vz + particles[i].radius > BOX_SIZE/2)
		{
			if (particles[i].z + particles[i].vz - particles[i].radius < -BOX_SIZE/2)
			{
				float offset = 0;
				particles[i].z = -BOX_SIZE/2 + particles[i].radius + offset;
				while (existsParticleInPos(i, particles[i].x, particles[i].y, particles[i].z))
				{
					offset += particles[i].radius / 10;
					particles[i].z = -BOX_SIZE/2 + particles[i].radius + offset;
				}
			}
			else
			if (particles[i].z + particles[i].vz + particles[i].radius > BOX_SIZE/2)
			{
				float offset = 0;
				particles[i].z = BOX_SIZE/2 - particles[i].radius - offset;
				while (existsParticleInPos(i, particles[i].x, particles[i].y, particles[i].z))
				{
					offset += particles[i].radius / 10;
					particles[i].z = BOX_SIZE/2 - particles[i].radius - offset;
				}
			}

			particles[i].vz = - particles[i].vz;
		}
		else
		{
			particles[i].z += particles[i].vz;
		}
	}
}

/*
 * Render a string onto screen.
 */
static void renderBitmapString(
		float x,
		float y,
		float z,
		void *font,
		char *string) {

  char *c;
  glRasterPos3f(x, y,z);
  for (c=string; *c != '\0'; c++) {
    glutBitmapCharacter(font, *c);
  }
}

/*
 * Draw all particles.
 */
static void DrawParticles()
{
	char text [50];
	int i;
	float sumSpeedx = 0;
	for (i = 0; i < PARTICLES_NUMBER; i++)
	{
		//sums all velocities from all particles. This sum should always be constant.
		sumSpeedx += 0.5*particles[i].radius*particles[i].vx*particles[i].vx
			+ 0.5*particles[i].radius*particles[i].vy*particles[i].vy
			+ 0.5*particles[i].radius*particles[i].vz*particles[i].vz;
		glPushMatrix();
		glTranslatef(particles[i].x, particles[i].y, particles[i].z);
		glColor3f(particles[i].r, particles[i].g, particles[i].b);
		glutSolidSphere(particles[i].radius, particles[i].slices, particles[i].stacks);
		glPopMatrix();
	}
	glColor3f(255.0, 255.0, 255.0);
	sprintf(text, "Speeds sum: %3.2f", sumSpeedx);
	renderBitmapString(BOX_SIZE, BOX_SIZE - 50, 0, GLUT_BITMAP_HELVETICA_10, text);
}

/*
 * Generate a random int.
 */
static int gen_rand(int max)
{
	int n;
	n = max*rand()/(RAND_MAX+1.0); 
	return(n);
}

/*
 * Animate() handles the animation and the redrawing of the
 *		graphics window contents.
 */
static void Animate(void)
{
	// Clear the redering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Clear the current matrix (Modelview)
    glLoadIdentity();

	gluLookAt(eye[0], eye[1], eye[2],
		center[0], center[1], center[2],
		0, 1, 0
		);

	// Back off eight units to be able to view from the origin.
    glTranslatef ( 0.0, 0.0, 0.0);

	// Rotate the plane of the elliptic
	// (rotate the model's plane about the x axis by fifteen degrees)
	glRotatef( 0.0, 0.0, 0.0, 0.0 );
	
	DrawBox();
	DetectCollision();
    DrawParticles();

	// Flush the pipeline, and swap the buffers
    glFlush();
    glutSwapBuffers();

	if ( singleStep ) {
		spinMode = GL_FALSE;
	}

	glutPostRedisplay();		// Request a re-draw for animation purposes

}

// Initialize OpenGL's rendering modes
void OpenGLInit(void)
{
    glShadeModel( GL_FLAT );
    glClearColor( 0.0, 0.0, 0.0, 0.0 );
    glClearDepth( 1.0 );
    glEnable( GL_DEPTH_TEST );
	CreateParticles();
}

// ResizeWindow is called when the window is resized
static void ResizeWindow(int w, int h)
{
    float aspectRatio;
	h = (h == 0) ? 1 : h;
	w = (w == 0) ? 1 : w;
	glViewport( 0, 0, w, h );	// View port uses whole window
	aspectRatio = (float)w/(float)h;

	// Set up the projection view matrix (not very well!)
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 50.0, aspectRatio, 1.0, 10000.0 );
	gluLookAt(0.0, 0.0, BOX_SIZE * 2,
          0.0, 0.0, 0.0,
          0.0, 1.0, 0.0);
	// Select the Modelview matrix
    glMatrixMode( GL_MODELVIEW );
}


// Main routine
// Set up OpenGL, hook up callbacks, and start the main loop
int main( int argc, char** argv )
{
	// Need to double buffer for animation
	glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

	// Create and position the graphics window
    glutInitWindowPosition( 0, 0 );
    glutInitWindowSize( 1280, 1024 );
    glutCreateWindow( "Collision Detector" );

	// Initialize OpenGL.
    OpenGLInit();

	// Set up callback functions for key presses
	glutKeyboardFunc( KeyPressFunc );

	// Set up the callback function for resizing windows
    glutReshapeFunc( ResizeWindow );

	// Callback for graphics image redrawing
    glutDisplayFunc( Animate );
	
	// Start the main loop.  glutMainLoop never returns.
	glutMainLoop(  );

    return(0);			// Compiler requires this to be here. (Never reached)
}
