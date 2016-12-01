/*
 * Bench.cpp class for drawing a bench.
 *
 * (c) 2001-2002: Stephen Chenney, University of Wisconsin at Madison.
 */


#include "Bench.h"
#include "libtarga.h"
#include <stdio.h>
#include <GL/glu.h>

// Destructor
Bench::~Bench(void)
{
    if ( initialized )
    {
	glDeleteLists(front_front, 1);
	glDeleteLists(front_left, 1);
	glDeleteLists(front_right, 1);
	glDeleteLists(front_back, 1);
	glDeleteLists(seat_front, 1);
	glDeleteLists(seat_left, 1);
	glDeleteLists(seat_right, 1);
	glDeleteLists(seat_top, 1);
	glDeleteLists(back_front, 1);
	glDeleteLists(back_top, 1);
	glDeleteLists(back_left, 1);
	glDeleteLists(back_right, 1);
	glDeleteLists(back_back, 1);
	glDeleteTextures(1, &texture_obj1);
    }
}


// Initializer. Returns false if something went wrong, like not being able to
// load the texture.
bool
Bench::Initialize(void)
{	
	//data for the sides
    ubyte   *image_data1;
    int	    image_height1, image_width1;

    // Load the image for the texture. The texture file has to be in
    // a place where it will be found.
    if ( ! ( image_data1 = (ubyte*)tga_load("Wood.tga", &image_width1,
					   &image_height1, TGA_TRUECOLOR_24) ) )
    {
	fprintf(stderr, "Bench::Initialize: Couldn't load Wood.tga\n");
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
    front_front = glGenLists(1);
    glNewList(front_front, GL_COMPILE);
	// Use white, because the texture supplies the color.
	glColor3f(1.0, 1.0, 1.0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_obj1);
	glBegin(GL_QUADS);

	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0f, 2.0f, 1.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.0f, 2.0f, 0.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0f, 0.0f, 0.0f);

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
    front_left = glGenLists(1);
    glNewList(front_left, GL_COMPILE);
	// Use white, because the texture supplies the color.
	glColor3f(1.0, 1.0, 1.0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_obj1);
	// Draw the sides
	glBegin(GL_QUADS);

	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.5f, 0.0f, 1.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.5f, 0.0f, 0.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
    glEndList();
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
    front_right = glGenLists(1);
    glNewList(front_right, GL_COMPILE);
	// Use white, because the texture supplies the color.
	glColor3f(1.0, 1.0, 1.0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_obj1);
	// Draw the sides
	glBegin(GL_QUADS);

	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.5f, 2.0f, 1.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.0f, 2.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0f, 2.0f, 0.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.5f, 2.0f, 0.0f);
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
    front_back = glGenLists(1);
    glNewList(front_back, GL_COMPILE);
	// Use white, because the texture supplies the color.
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_obj1);
	// Draw the sides
	glBegin(GL_QUADS);

	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.5f, 0.0f, 0.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.5f, 2.0f, 0.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.5f, 2.0f, 1.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.5f, 0.0f, 1.0f);
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
    seat_front = glGenLists(1);
    glNewList(seat_front, GL_COMPILE);
	// Use white, because the texture supplies the color.
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_obj1);
	// Draw the sides
	glBegin(GL_QUADS);

	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.0f, 2.0f, 1.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0f, 2.0f, 1.2f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.0f, 0.0f, 1.2f);
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
    seat_left = glGenLists(1);
    glNewList(seat_left, GL_COMPILE);
	// Use white, because the texture supplies the color.
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_obj1);
	// Draw the sides
	glBegin(GL_QUADS);

	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.2f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.2f, 0.0f, 1.2f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.0f, 0.0f, 1.2f);
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
    seat_right = glGenLists(1);
    glNewList(seat_right, GL_COMPILE);
	// Use white, because the texture supplies the color.
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_obj1);
	// Draw the sides
	glBegin(GL_QUADS);

	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.2f, 2.0f, 1.2f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.2f, 2.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0f, 2.0f, 1.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0f, 2.0f, 1.2f);
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
    seat_top = glGenLists(1);
    glNewList(seat_top, GL_COMPILE);
	// Use white, because the texture supplies the color.
	glColor3f(1.0, 1.0, 1.0);	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_obj1);

	// Draw the sides
	glBegin(GL_QUADS);

	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(1.2f, 2.0f, 1.2f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.0f, 2.0f, 1.2f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(0.0f, 0.0f, 1.2f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.2, 0.0f, 1.2f);
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
    back_front = glGenLists(1);
    glNewList(back_front, GL_COMPILE);
	// Use white, because the texture supplies the color.
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_obj1);
	// Draw the sides
	glBegin(GL_QUADS);

	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.2f, 2.0f, 0.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.2f, 2.0f, 2.5f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.2f, 0.0f, 2.5f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.2f, 0.0f, 0.0f);

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
    back_left = glGenLists(1);
    glNewList(back_left, GL_COMPILE);
	// Use white, because the texture supplies the color.
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_obj1);
	// Draw the sides
	glBegin(GL_QUADS);

	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.2f, 0.0f, 0.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.7, 0.0f, 0.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.7f, 0.0f, 2.5f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.2f, 0.0f, 2.5f);
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
    back_right = glGenLists(1);
    glNewList(back_right, GL_COMPILE);
	// Use white, because the texture supplies the color.
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_obj1);
	// Draw the sides
	glBegin(GL_QUADS);

	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.2f, 2.0f, 0.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.7f, 2.0f, 0.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.7f, 2.0f, 2.5f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.2f, 2.0f, 2.5f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
    glEndList();

		// This creates a texture object and binds it, so the next few operations
    // apply to this texture.
	glEnable(GL_TEXTURE_2D);
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
    back_back = glGenLists(1);
    glNewList(back_back, GL_COMPILE);
	// Use white, because the texture supplies the color.
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_obj1);
	// Draw the sides
	glBegin(GL_QUADS);

	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.7f, 0.0f, 0.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.7f, 2.0, 0.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.7f, 2.0f, 2.5f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.7f, 0.0f, 2.5f);
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
    back_top = glGenLists(1);
    glNewList(back_top, GL_COMPILE);
	// Use white, because the texture supplies the color.
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_obj1);
	// Draw the sides
	glBegin(GL_QUADS);

	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.2f, 0.0f, 2.5f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.2f, 2.0f, 2.5f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.7f, 2.0f, 2.5f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.7f, 0.0f, 2.5f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
    glEndList();

    // We only do all this stuff once, when the GL context is first set up.
    initialized = true;

    return true;
}


// Draw just calls the display lists we set up earlier.
void
Bench::Draw(void)
{
    glPushMatrix();
	glTranslatef(15.0, 0.0, 0.0);
	glCallList(front_front);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15.0, 0.0, 0.0);
    glCallList(front_left);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15.0, 0.0, 0.0);
    glCallList(front_right);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15.0, 0.0, 0.0);
    glCallList(front_back);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15.0, 0.0, 0.0);
	glCallList(seat_front);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15.0, 0.0, 0.0);
	glCallList(seat_left);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15.0, 0.0, 0.0);
	glCallList(seat_right);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15.0, 0.0, 0.0);
	glCallList(seat_top);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15.0, 0.0, 0.0);
	glCallList(back_front);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15.0, 0.0, 0.0);
	glCallList(back_back);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15.0, 0.0, 0.0);
	glCallList(back_left);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15.0, 0.0, 0.0);
	glCallList(back_right);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(15.0, 0.0, 0.0);
	glCallList(back_top);
	glPopMatrix();
}