/*
 * Trees.cpp: A class for drawing the trees.
 *
 * (c) 2001-2002: Stephen Chenney, University of Wisconsin at Madison.
 */


#include "Trees.h"
#include "libtarga.h"
#include <stdio.h>

// Destructor
Trees::~Trees(void)
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
Trees::Initialize(void)
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
Trees::Draw(void)
{
	glPushMatrix();
		glTranslatef(-30.0, -30.0, 0.0);
		gluQuadricDrawStyle(qobj1, GLU_FILL);
		gluQuadricTexture(qobj1, GLU_TRUE);
		glColor3f(0.5, 0.2, 0.2);
		gluCylinder(qobj1, 1.0, 1.0, 6.0, 10, 20);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-30.0, -30.0, 6.0);
		gluQuadricDrawStyle(qobjtop1, GLU_FILL);
		gluQuadricTexture(qobjtop1, GLU_TRUE);
		glColor3f(0.0, 1.0, 0.0);
		gluCylinder(qobjtop1, 4.0, 0.0, 6.0, 5, 20);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(30.0, 30.0, 0.0);
		gluQuadricDrawStyle(qobj2, GLU_FILL);
		gluQuadricTexture(qobj2, GLU_TRUE);
		glColor3f(0.5, 0.2, 0.2);
		gluCylinder(qobj2, 0.5, 0.5, 3.0, 5, 15);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(30.0, 30.0, 3.0);
		gluQuadricDrawStyle(qobjtop2, GLU_FILL);
		gluQuadricTexture(qobjtop2, GLU_TRUE);
		glColor3f(0.0, 1.0, 0.0);
		gluCylinder(qobjtop2, 5.0, 0.0, 7.0, 5, 15);
	glPopMatrix();	

	glPushMatrix();
		glTranslatef(-5.0, -5.0, 0.0);
		gluQuadricDrawStyle(qobj3, GLU_FILL);
		gluQuadricTexture(qobj3, GLU_TRUE);
		glColor3f(0.5, 0.2, 0.2);
		gluCylinder(qobj3, 0.3, 0.3, 2.5, 3, 12);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-5.0, -5.0, 2.5);
		gluQuadricDrawStyle(qobjtop3, GLU_FILL);
		gluQuadricTexture(qobjtop3, GLU_TRUE);
		glColor3f(0.0, 1.0, 0.0);
		gluCylinder(qobjtop3, 4.0, 0.0, 4.0, 5, 10);
	glPopMatrix();	
}