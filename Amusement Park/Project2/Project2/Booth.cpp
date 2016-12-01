/*
 * Booth.cpp: A class for drawing the ticket booth.
 *
 * (c) 2001-2002: Stephen Chenney, University of Wisconsin at Madison.
 */


#include "Booth.h"
#include "libtarga.h"
#include <stdio.h>
#include <GL/glu.h>

// Destructor
Booth::~Booth(void)
{
    if ( initialized )
    {
	glDeleteLists(front_list, 1);
	glDeleteLists(right_list, 1);
	glDeleteLists(left_list, 1);
	glDeleteLists(bottom_list, 1);
	glDeleteLists(back_list, 1);
	glDeleteLists(roof_list, 1);
	glDeleteTextures(1, &texture_obj1);
	glDeleteTextures(1, &texture_obj2);
	glDeleteTextures(1, &texture_obj3);
    }
}


// Initializer. Returns false if something went wrong, like not being able to
// load the texture.
bool
Booth::Initialize(void)
{
	//data for the sides
    ubyte   *image_data1;
    int	    image_height1, image_width1;

	//data for the front
	ubyte	*image_data2;
	int		image_height2, image_width2;

	//data for the roof
	ubyte	*image_data3;
	int		image_height3, image_width3;

    // Load the image for the texture. The texture file has to be in
    // a place where it will be found.
    if ( ! ( image_data1 = (ubyte*)tga_load("Sides.tga", &image_width1,
					   &image_height1, TGA_TRUECOLOR_24) ) )
    {
	fprintf(stderr, "Booth::Initialize: Couldn't load Sides.tga\n");
	return false;
    }

	    // Load the image for the texture. The texture file has to be in
    // a place where it will be found.
    if ( ! ( image_data2 = (ubyte*)tga_load("Front.tga", &image_width2,
					   &image_height2, TGA_TRUECOLOR_24) ) )
    {
	fprintf(stderr, "Booth::Initialize: Couldn't load Front.tga\n");
	return false;
    }

	    // Load the image for the texture. The texture file has to be in
    // a place where it will be found.
    if ( ! ( image_data3 = (ubyte*)tga_load("Roof.tga", &image_width3,
					   &image_height3, TGA_TRUECOLOR_24) ) )
    {
	fprintf(stderr, "Booth::Initialize: Couldn't load Roof.tga\n");
	return false;
    }

    // This creates a texture object and binds it, so the next few operations
    // apply to this texture.
    glGenTextures(1, &texture_obj1);
    glBindTexture(GL_TEXTURE_2D, texture_obj1);

    // This sets a parameter for how the texture is loaded and interpreted.
    // basically, it says that the data is packed tightly in the image array.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // This sets up the texture with high quality filtering. First it builds
    // mipmaps from the image data, then it sets the filtering parameters
    // and the wrapping parameters. We want the wood to be repeated over the sides
    gluBuild2DMipmaps(GL_TEXTURE_2D,3, image_width1, image_height1, 
		      GL_RGB, GL_UNSIGNED_BYTE, image_data1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		    GL_NEAREST_MIPMAP_NEAREST);

    // This says what to do with the texture. Modulate will multiply the
    // texture by the underlying color.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 

    // Now do the geometry. Create the display list.
    bottom_list = glGenLists(1);
    glNewList(bottom_list, GL_COMPILE);
	// Use white, because the texture supplies the color.
	glColor3f(1.0, 1.0, 1.0);

	// Turn on texturing and bind the wood textures.
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_obj1);

	// Draw the bottom
	glBegin(GL_QUADS);

	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(5.0f, 4.0f, 0.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(4.0f, 4.0f, 0.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(4.0f, 5.0f, 0.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(5.0f, 5.0f, 0.0f);

	glEnd();
	glDisable(GL_TEXTURE_2D);
    glEndList();

	// This creates a texture object and binds it, so the next few operations
    // apply to this texture.
    glGenTextures(1, &texture_obj1);
    glBindTexture(GL_TEXTURE_2D, texture_obj1);

    // This sets a parameter for how the texture is loaded and interpreted.
    // basically, it says that the data is packed tightly in the image array.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // This sets up the texture with high quality filtering. First it builds
    // mipmaps from the image data, then it sets the filtering parameters
    // and the wrapping parameters. We want the wood to be repeated over the sides
    gluBuild2DMipmaps(GL_TEXTURE_2D,3, image_width1, image_height1, 
		      GL_RGB, GL_UNSIGNED_BYTE, image_data1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		    GL_NEAREST_MIPMAP_NEAREST);

    // This says what to do with the texture. Modulate will multiply the
    // texture by the underlying color.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 

    // Now do the geometry. Create the display list.
    left_list = glGenLists(1);
    glNewList(left_list, GL_COMPILE);
	// Use white, because the texture supplies the color.
	glColor3f(1.0, 1.0, 1.0);

	// Turn on texturing and bind the wood textures.
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_obj1);

	// Draw the sides
	glBegin(GL_QUADS);

	glNormal3f(5.5f, 0.0f, 0.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(5.0f, 5.0f, 0.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(5.0f, 5.0f, 4.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(5.0f, 4.0f, 4.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(5.0f, 4.0f, 0.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
    glEndList();

	// This creates a texture object and binds it, so the next few operations
    // apply to this texture.
    glGenTextures(1, &texture_obj1);
    glBindTexture(GL_TEXTURE_2D, texture_obj1);

    // This sets a parameter for how the texture is loaded and interpreted.
    // basically, it says that the data is packed tightly in the image array.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // This sets up the texture with high quality filtering. First it builds
    // mipmaps from the image data, then it sets the filtering parameters
    // and the wrapping parameters. We want the wood to be repeated over the sides
    gluBuild2DMipmaps(GL_TEXTURE_2D,3, image_width1, image_height1, 
		      GL_RGB, GL_UNSIGNED_BYTE, image_data1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		    GL_NEAREST_MIPMAP_NEAREST);

    // This says what to do with the texture. Modulate will multiply the
    // texture by the underlying color.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 

    // Now do the geometry. Create the display list.
    right_list = glGenLists(1);
    glNewList(right_list, GL_COMPILE);
	// Use white, because the texture supplies the color.
	glColor3f(1.0, 1.0, 1.0);

	// Turn on texturing and bind the wood textures.
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_obj1);

	// Draw the sides
	glBegin(GL_QUADS);
	glNormal3f(3.5f, 0.0f, 0.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(4.0f, 5.0f, 4.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(4.0f, 5.0f, 0.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(4.0f, 4.0f, 0.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(4.0f, 4.0f, 4.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
    glEndList();

		// This creates a texture object and binds it, so the next few operations
    // apply to this texture.
    glGenTextures(1, &texture_obj1);
    glBindTexture(GL_TEXTURE_2D, texture_obj1);

    // This sets a parameter for how the texture is loaded and interpreted.
    // basically, it says that the data is packed tightly in the image array.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // This sets up the texture with high quality filtering. First it builds
    // mipmaps from the image data, then it sets the filtering parameters
    // and the wrapping parameters. We want the wood to be repeated over the sides
    gluBuild2DMipmaps(GL_TEXTURE_2D,3, image_width1, image_height1, 
		      GL_RGB, GL_UNSIGNED_BYTE, image_data1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		    GL_NEAREST_MIPMAP_NEAREST);

    // This says what to do with the texture. Modulate will multiply the
    // texture by the underlying color.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 

    // Now do the geometry. Create the display list.
    back_list = glGenLists(1);
    glNewList(back_list, GL_COMPILE);
	// Use white, because the texture supplies the color.
	glColor3f(1.0, 1.0, 1.0);

	// Turn on texturing and bind the wood textures.
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_obj1);

	// Draw the sides
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 3.5f, 0.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(5.0f, 4.0f, 0.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(5.0f, 4.0f, 4.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(4.0f, 4.0f, 4.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(4.0f, 4.0f, 0.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
    glEndList();

	// This creates a texture object and binds it, so the next few operations
    // apply to this texture.
    glGenTextures(1, &texture_obj3);
    glBindTexture(GL_TEXTURE_2D, texture_obj3);

    // This sets a parameter for how the texture is loaded and interpreted.
    // basically, it says that the data is packed tightly in the image array.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // This sets up the texture with high quality filtering. First it builds
    // mipmaps from the image data, then it sets the filtering parameters
    // and the wrapping parameters. We want the wood to be repeated over the sides
    gluBuild2DMipmaps(GL_TEXTURE_2D,3, image_width3, image_height3, 
		      GL_RGB, GL_UNSIGNED_BYTE, image_data3);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		    GL_NEAREST_MIPMAP_NEAREST);

    // This says what to do with the texture. Modulate will multiply the
    // texture by the underlying color.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 

    // Now do the geometry. Create the display list.
    roof_list = glGenLists(1);
    glNewList(roof_list, GL_COMPILE);
	// Use white, because the texture supplies the color.
	glColor3f(1.0, 1.0, 1.0);

	// Turn on texturing and bind the wood textures.
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_obj3);

    // Draw the roof
	glBegin(GL_QUADS);

	glNormal3f(0.0f, 0.0f, 4.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(5.0f, 6.0f, 4.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(4.0f, 6.0f, 4.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(4.0f, 4.0f, 4.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(5.0, 4.0f, 4.0f);

	glEnd();
	glDisable(GL_TEXTURE_2D);
    glEndList();

	// This creates a texture object and binds it, so the next few operations
    // apply to this texture.
    glGenTextures(1, &texture_obj2);
    glBindTexture(GL_TEXTURE_2D, texture_obj2);

    // This sets a parameter for how the texture is loaded and interpreted.
    // basically, it says that the data is packed tightly in the image array.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // This sets up the texture with high quality filtering. First it builds
    // mipmaps from the image data, then it sets the filtering parameters
    // and the wrapping parameters. We want the wood to be repeated over the sides
    gluBuild2DMipmaps(GL_TEXTURE_2D,3, image_width2, image_height2, 
		      GL_RGB, GL_UNSIGNED_BYTE, image_data2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		    GL_NEAREST_MIPMAP_LINEAR);

    // This says what to do with the texture. Modulate will multiply the
    // texture by the underlying color.
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 

    // Now do the geometry. Create the display list.
    front_list = glGenLists(1);
    glNewList(front_list, GL_COMPILE);
	// Use white, because the texture supplies the color.
	glColor3f(1.0, 1.0, 1.0);

	// Turn on texturing and bind the front textures.
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_obj2);

    // Draw the front
	glBegin(GL_QUADS);

	glNormal3f(0.0f, 5.5f, 0.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(5.0f, 5.0f, 4.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(5.0f, 5.0f, 0.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(4.0f, 5.0f, 0.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(4.0f, 5.0f, 4.0f);

	glEnd();
	glDisable(GL_TEXTURE_2D);
    glEndList();

    // We only do all this stuff once, when the GL context is first set up.
    initialized = true;

    return true;
}


// Draw just calls the display lists we set up earlier.
void
Booth::Draw(void)
{
    glPushMatrix();
    glCallList(right_list);
	glPushMatrix();
    glCallList(left_list);
	glPushMatrix();
    glCallList(bottom_list);
	glPushMatrix();
    glCallList(back_list);
	glPushMatrix();
	glCallList(front_list);
	glPushMatrix();
	glCallList(roof_list);
    glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
}