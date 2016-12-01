/*
 * Balloons.cpp: A class for drawing the balloons.
 *
 * (c) 2001-2002: Stephen Chenney, University of Wisconsin at Madison.
 */


#include "Balloons.h"
#include "libtarga.h"
#include <stdio.h>

// Destructor
Balloons::~Balloons(void)
{
    if ( initialized )
    {
		gluDeleteQuadric(qobj1);
		gluDeleteQuadric(qobjtop1);
		gluDeleteQuadric(qobj2);
		gluDeleteQuadric(qobjtop2);
		gluDeleteQuadric(qobj3);
		gluDeleteQuadric(qobjtop3);
    }
}


// Initializer. Returns false if something went wrong, like not being able to
// load the texture.
bool
Balloons::Initialize(void)
{
	qobj1 = gluNewQuadric();
	qobjtop1 = gluNewQuadric();
	gluQuadricNormals(qobj1, GLU_SMOOTH);
	gluQuadricNormals(qobjtop1, GLU_SMOOTH);

	qobj2 = gluNewQuadric();
	qobjtop2 = gluNewQuadric();
	gluQuadricNormals(qobj2, GLU_SMOOTH);
	gluQuadricNormals(qobjtop2, GLU_SMOOTH);

	
	qobj3 = gluNewQuadric();
	qobjtop3 = gluNewQuadric();
	gluQuadricNormals(qobj3, GLU_SMOOTH);
	gluQuadricNormals(qobjtop3, GLU_SMOOTH);
	
    // We only do all this stuff once, when the GL context is first set up.
    initialized = true;

    return true;
}


// Draw just calls the display lists we set up earlier.
void
Balloons::Draw(void)
{
	glPushMatrix();
		glTranslatef(20.0, -20.0, height1);
		glRotatef(rotation, 0, 0, 1);
		gluQuadricDrawStyle(qobj1, GLU_FILL);
		gluQuadricTexture(qobj1, GLU_TRUE);
		glColor3f(1.0, 1.0, 1.0);
		gluCylinder(qobj1, 0.1, 0.1, 3.0, 5, 10);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(20.0, -20.0, height1 + 3.5);
		glRotatef(rotation, 0, 0, 1);
		gluQuadricDrawStyle(qobjtop1, GLU_FILL);
		gluQuadricTexture(qobjtop1, GLU_TRUE);
		glColor3f(1.0, 0.0, 0.0);
		gluSphere(qobjtop1, 0.5, 5, 10);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-20.0, 20.0, height2);
		glRotatef(rotation, 0, 0, 1);
		gluQuadricDrawStyle(qobj2, GLU_FILL);
		gluQuadricTexture(qobj2, GLU_TRUE);
		glColor3f(1.0, 1.0, 1.0);
		gluCylinder(qobj2, 0.1, 0.1, 4.0, 5, 10);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-20.0, 20.0, height2 + 5.0);
		glRotatef(rotation, 0, 0, 1);
		gluQuadricDrawStyle(qobjtop2, GLU_FILL);
		gluQuadricTexture(qobjtop2, GLU_TRUE);
		glColor3f(0.0, 1.0, 0.0);
		gluSphere(qobjtop2, 1.0, 10, 15);
	glPopMatrix();	

	glPushMatrix();
		glTranslatef(-20.0, -20.0, height3);
		glRotatef(rotation, 0, 0, 1);
		gluQuadricDrawStyle(qobj3, GLU_FILL);
		gluQuadricTexture(qobj3, GLU_TRUE);
		glColor3f(1.0, 1.0, 1.0);
		gluCylinder(qobj3, 0.1, 0.1, 5.0, 5, 10);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-20.0, -20.0, height3 + 6.5);
		glRotatef(rotation, 0, 0, 1);
		gluQuadricDrawStyle(qobjtop3, GLU_FILL);
		gluQuadricTexture(qobjtop3, GLU_TRUE);
		glColor3f(0.0, 0.0, 1.0);
		gluSphere(qobjtop3, 1.5, 15, 20);
	glPopMatrix();	
}

void Balloons::Update(float dt)
{
	height1 += (float)(7.0f * dt);
	if (height1 >= 150.0f)
	{
		height1 = 5.0f;
	}

	height2 += (float)(7.0f * dt);
	if (height2 >= 150.0f)
	{
		height2 = 10.0f;
	}

	height3 += (float)(7.0f * dt);
	if (height3 >= 150.0f)
	{
		height3 = 20.0f;
	}

	rotation += (float)(20.0f * dt);
	if(rotation >= 360.0f)
	{
		rotation = 0.0f;
	}
}