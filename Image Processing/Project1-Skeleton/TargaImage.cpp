///////////////////////////////////////////////////////////////////////////////
//
//      TargaImage.cpp                          Author:     Stephen Chenney
//                                              Modified:   Eric McDaniel
//                                              Date:       Fall 2004
//                                              Modified:   Feng Liu
//                                              Date:       Winter 2011
//                                              Why:        Change the library file 
//      Implementation of TargaImage methods.  You must implement the image
//  modification functions.
//
///////////////////////////////////////////////////////////////////////////////

#include "Globals.h"
#include "TargaImage.h"
#include "libtarga.h"
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

// constants
const int           RED             = 0;                // red channel
const int           GREEN           = 1;                // green channel
const int           BLUE            = 2;                // blue channel
const unsigned char BACKGROUND[3]   = { 0, 0, 0 };      // background color


// Computes n choose s, efficiently
double Binomial(int n, int s)
{
    double        res;

    res = 1;
    for (int i = 1 ; i <= s ; i++)
        res = (n - i + 1) * res / i ;

    return res;
}// Binomial


///////////////////////////////////////////////////////////////////////////////
//
//      Constructor.  Initialize member variables.
//
///////////////////////////////////////////////////////////////////////////////
TargaImage::TargaImage() : width(0), height(0), data(NULL)
{}// TargaImage

///////////////////////////////////////////////////////////////////////////////
//
//      Constructor.  Initialize member variables.
//
///////////////////////////////////////////////////////////////////////////////
TargaImage::TargaImage(int w, int h) : width(w), height(h)
{
   data = new unsigned char[width * height * 4];
   ClearToBlack();
}// TargaImage



///////////////////////////////////////////////////////////////////////////////
//
//      Constructor.  Initialize member variables to values given.
//
///////////////////////////////////////////////////////////////////////////////
TargaImage::TargaImage(int w, int h, unsigned char *d)
{
    int i;

    width = w;
    height = h;
    data = new unsigned char[width * height * 4];

    for (i = 0; i < width * height * 4; i++)
	    data[i] = d[i];
}// TargaImage

///////////////////////////////////////////////////////////////////////////////
//
//      Copy Constructor.  Initialize member to that of input
//
///////////////////////////////////////////////////////////////////////////////
TargaImage::TargaImage(const TargaImage& image) 
{
   width = image.width;
   height = image.height;
   data = NULL; 
   if (image.data != NULL) {
      data = new unsigned char[width * height * 4];
      memcpy(data, image.data, sizeof(unsigned char) * width * height * 4);
   }
}


///////////////////////////////////////////////////////////////////////////////
//
//      Destructor.  Free image memory.
//
///////////////////////////////////////////////////////////////////////////////
TargaImage::~TargaImage()
{
    if (data)
        delete[] data;
}// ~TargaImage


///////////////////////////////////////////////////////////////////////////////
//
//      Converts an image to RGB form, and returns the rgb pixel data - 24 
//  bits per pixel. The returned space should be deleted when no longer 
//  required.
//
///////////////////////////////////////////////////////////////////////////////
unsigned char* TargaImage::To_RGB(void)
{
    unsigned char   *rgb = new unsigned char[width * height * 3];
    int		    i, j;

    if (! data)
	    return NULL;

    // Divide out the alpha
    for (i = 0 ; i < height ; i++)
    {
	    int in_offset = i * width * 4;
	    int out_offset = i * width * 3;

	    for (j = 0 ; j < width ; j++)
        {
	        RGBA_To_RGB(data + (in_offset + j*4), rgb + (out_offset + j*3));
	    }
    }

    return rgb;
}// TargaImage


///////////////////////////////////////////////////////////////////////////////
//
//      Save the image to a targa file. Returns 1 on success, 0 on failure.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Save_Image(const char *filename)
{
    TargaImage	*out_image = Reverse_Rows();

    if (! out_image)
	    return false;

    if (!tga_write_raw(filename, width, height, out_image->data, TGA_TRUECOLOR_32))
    {
	    cout << "TGA Save Error: %s\n", tga_error_string(tga_get_last_error());
	    return false;
    }

    delete out_image;

    return true;
}// Save_Image


///////////////////////////////////////////////////////////////////////////////
//
//      Load a targa image from a file.  Return a new TargaImage object which 
//  must be deleted by caller.  Return NULL on failure.
//
///////////////////////////////////////////////////////////////////////////////
TargaImage* TargaImage::Load_Image(char *filename)
{
    unsigned char   *temp_data;
    TargaImage	    *temp_image;
    TargaImage	    *result;
    int		        width, height;

    if (!filename)
    {
        cout << "No filename given." << endl;
        return NULL;
    }// if

    temp_data = (unsigned char*)tga_load(filename, &width, &height, TGA_TRUECOLOR_32);
    if (!temp_data)
    {
        cout << "TGA Error: %s\n", tga_error_string(tga_get_last_error());
	    width = height = 0;
	    return NULL;
    }
    temp_image = new TargaImage(width, height, temp_data);
    free(temp_data);

    result = temp_image->Reverse_Rows();

    delete temp_image;

    return result;
}// Load_Image


///////////////////////////////////////////////////////////////////////////////
//
//      Convert image to grayscale.  Red, green, and blue channels should all 
//  contain grayscale value.  Alpha channel shoould be left unchanged.  Return
//  success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::To_Grayscale()
{
	double temp = 0;

	try
	{
		for(int i = 0; i < width * height * 4; i = i + 4)
		{	
			temp = (double)data[i] * 0.299;
			temp = temp + ((double)data[i+1] * 0.587);
			temp = temp + ((double)data[i+2] * 0.114);
			data[i] = (int)temp;
			data[i + 1] = (int)temp;
			data[i + 2] = (int)temp;
			temp = 0;
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}// To_Grayscale


///////////////////////////////////////////////////////////////////////////////
//
//  Convert the image to an 8 bit image using uniform quantization.  Return 
//  success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Quant_Uniform()
{
	try
	{
		for(int i = 0; i < width * height * 4; i = i + 4)
		{	
			if(data[i] >= 0 && data[i] <= 31)
			{
				data[i] = 0;
			}
			else if(data[i] >= 32 && data[i] <= 63)
			{
				data[i] = 32;
			}
			else if(data[i] >= 64 && data[i] <= 95)
			{
				data[i] = 64;
			}
			else if(data[i] >= 96 && data[i] <=127)
			{
				data[i] = 96;
			}
			else if(data[i] >= 128 && data[i] <= 159)
			{
				data[i] = 128;
			}
			else if(data[i] >= 160 && data[i] <= 191)
			{
				data[i] = 160;
			}
			else if(data[i] >= 192 && data[i] <= 223)
			{
				data[i] = 192;
			}
			else if(data[i] >= 224 && data[i] <= 255)
			{
				data[i] = 224;
			}
			
			if(data[i + 1] >= 0 && data[i + 1] <= 31)
			{
				data[i+1] = 0;
			}
			else if(data[i+1] >= 32 && data[i+1] <= 63)
			{
				data[i+1] = 32;
			}
			else if(data[i+1] >= 64 && data[i+1] <= 95)
			{
				data[i+1] = 64;
			}
			else if(data[i+1] >= 96 && data[i+1] <=127)
			{
				data[i+1] = 96;
			}
			else if(data[i+1] >= 128 && data[i+1] <= 159)
			{
				data[i+1] = 128;
			}
			else if(data[i+1] >= 160 && data[i+1] <= 191)
			{
				data[i+1] = 160;
			}
			else if(data[i+1] >= 192 && data[i+1] <= 223)
			{
				data[i+1] = 192;
			}
			else if(data[i+1] >= 224 && data[i+1] <= 255)
			{
				data[i+1] = 224;
			}

			if(data[i+2] >= 0 && data[i+2] <= 63)
			{
				data[i+2] = 0;
			}
			else if(data[i+2] >= 64 && data[i+2] <= 127)
			{
				data[i+2] = 64;
			}
			else if(data[i+2] >= 128 && data[i+2] <= 191)
			{
				data[i+2] = 128;
			}
			else if(data[i+2] >= 192 && data[i+2] <= 255)
			{
				data[i+2] = 192;
			}
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}// Quant_Uniform


///////////////////////////////////////////////////////////////////////////////
//
//      Convert the image to an 8 bit image using populosity quantization.  
//  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Quant_Populosity()
{
	double temp = 0;
	unsigned char bin[32];
	unsigned short *** colors;
	int r = 0;
	int g = 0;
	int b = 0;
	unsigned char ra[256];
	unsigned char ga[256];
	unsigned char ba[256];
	unsigned char * nImage = NULL;

	for(int i = 0; i < 32; i = i + 1)
	{
		bin[i] = i * 8;
	}

	colors = new unsigned short**[32];
	for(int a = 0; a < 32; a = a + 1)
	{
		colors[a] = new unsigned short * [32];
		
		for(int b = 0; b < 32; b = b + 1)
		{
			colors[a][b] = new unsigned short[32];

			for(int c = 0; c < 32; c = c + 1)
			{
				colors[a][b][c] = 0;
			}
		}
	}

	nImage = new unsigned char[width * height * 4];

	try
	{
		//uniform quant down to 32 levels of r, g and b
		for(int i = 0; i < width * height * 4; i = i + 4)
		{	
			nImage[i] = bin[(int)floor(data[i] / 8.0)];
			nImage[i + 1] = bin[(int)floor(data[i + 1] / 8.0)];
			nImage[i + 2] = bin[(int)floor(data[i + 2] / 8.0)];
			nImage[i + 3] = data[i + 3];
		}

		//count up occurences of colors
		for(int i = 0; i < width * height * 4; i = i + 4)
		{
			r = nImage[i] / 8;
			g = nImage[i + 1] / 8;
			b = nImage[i + 2] / 8;

			colors[r][g][b] = colors[r][g][b] + 1;
		}

		//find most popular 256 colors
		for(int i = 0; i < 256; i = i + 1)
		{
			for(int j = 0; j < 32; j = j + 1)
			{
				for(int k = 0; k < 32; k = k + 1)
				{
					for(int l = 0; l < 32; l = l + 1)
					{
						if(temp <= colors[j][k][l])
						{
							temp = colors[j][k][l];
							r = j;
							g = k;
							b = l;
						}
					}
				}
			}
			ra[i] = r * 8;
			ga[i] = g * 8;
			ba[i] = b * 8;
			colors[r][g][b] = 0;
			temp = 0;
		}

		double comp = 0;
		temp = 999999999;

		//map colors
		for(int i = 0; i < width * height * 4; i = i + 4)
		{
			for(int j = 0; j < 256; j = j + 1)
			{
				comp = sqrt((double)pow((double)abs(data[i] - ra[j]), 2) + (double)pow((double)abs(data[i + 1] - ga[j]), 2) + (double)pow((double)abs(data[i + 2] - ba[j]), 2));
				if(temp >= comp)
				{
					temp = comp;
					r = ra[j];
					g = ga[j];
					b = ba[j];
				}
			}
			data[i] = r;
			data[i + 1] = g;
			data[i + 2] = b;
			temp = 999999999;
		}

		delete[] nImage;
		nImage = NULL;

		for(int a = 0; a < 32; a = a + 1)
		{
			for(int b = 0; b < 32; b = b + 1)
			{
				delete[] colors[a][b];
			}
			delete[] colors[a];
		}

		delete[] colors;
		colors = NULL;
	}
	catch(...)
	{
		return false;
	}

	return true;
}// Quant_Populosity


///////////////////////////////////////////////////////////////////////////////
//
//      Dither the image using a threshold of 1/2.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Dither_Threshold()
{
    double temp = 0;

	if(!To_Grayscale())
	{
		return false;
	}

	try
	{
		for(int i = 0; i < width * height * 4; i = i + 4)
		{	
			temp = data[i] / (double)255;

			if(temp < 0.5)
			{
				data[i] = 0;
				data[i+1] = 0;
				data[i + 2] = 0;
			}
			else
			{
				data[i] = 255;
				data[i+1] = 255;
				data[i+2] = 255;
			}
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}// Dither_Threshold


///////////////////////////////////////////////////////////////////////////////
//
//      Dither image using random dithering.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Dither_Random()
{
	double temp = 0;
	double temp2 = 0;

	try
	{
		for(int i = 0; i < width * height * 4; i = i + 4)
		{	
			temp = data[i] / (double)255;
			temp2 = rand() % 41;
			temp2 = temp2 / 100.0;
			temp2 = temp2 - 0.2;
			temp = temp + temp2;

			if(temp < 0.5)
			{
				data[i] = 0;
				data[i+1] = 0;
				data[i + 2] = 0;
			}
			else
			{
				data[i] = 255;
				data[i+1] = 255;
				data[i+2] = 255;
			}
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}// Dither_Random


///////////////////////////////////////////////////////////////////////////////
//
//      Perform Floyd-Steinberg dithering on the image.  Return success of 
//  operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Dither_FS()
{
	double temp = 0;
	bool even = false;
	float * ar = NULL;

	if(!To_Grayscale())
	{
		return false;
	}

	try
	{
		ar = new float[width * height * 4];

		for(int k = 0; k < width * height * 4; k = k + 4)
		{
			ar[k] = data[k] / 255.0;
			ar[k + 1] = data[k + 1] / 255.0;
			ar[k + 2] = data[k + 2] / 255.0;
			ar[k + 3] = data[k + 3];
		}
		if(height % 2 == 0)
		{
			even = true;
		}
		
		int i = 0;

		if(even)
		{
			even = false;
			while(i != ((width * height * 4) - width * 4))
			{
				temp = ar[i];
				if(temp < 0.5)
				{
					ar[i] = 0;
					ar[i + 1] = 0;
					ar[i + 2] = 0;
				}
				else
				{
					ar[i] = 1.0;
					ar[i + 1] = 1.0;
					ar[i + 2] = 1.0;
					temp = temp - 1.0;
				}

				//distribute error to 4 neighbors
				if(even)
				{
					if(i % (width * 4) - 4 >= 0)
					{
						ar[i - 4] = ar[i - 4] + ((7.0/16.0)* temp);
						ar[i - 3] = ar[i - 3] + ((7.0/16.0)* temp);
						ar[i - 2] = ar[i - 2] + ((7.0/16.0)* temp);
					}
					if(i % (width * 4) - 4 >= 0 && i + (width * 4) < width * height * 4)
					{
						ar[i - 4 + (width * 4)] = ar[i - 4 + (width * 4)] + ((1.0/16.0) * temp);
						ar[i - 3 + (width * 4)] = ar[i - 3 + (width * 4)] + ((1.0/16.0) * temp);
						ar[i - 2 + (width * 4)] = ar[i - 2 + (width * 4)] + ((1.0/16.0) * temp);
					}
					if(i + (width * 4) < width * height * 4)
					{
						ar[i + (width * 4)] = ar[i + (width * 4)] + ((5.0 / 16.0) * temp);
						ar[i + 1 +(width * 4)] = ar[i + 1 + (width * 4)] + ((5.0 / 16.0) * temp);
						ar[i + 2 + (width * 4)] = ar[i + 2 + (width * 4)] + ((5.0 / 16.0) * temp);
					}
					if(i + (width * 4) < width * height * 4 && i % (width * 4) + 4 < width * 4)
					{
						ar[i + (width * 4) + 4] = ar[i + (width * 4) + 4] + ((3.0 / 16.0) * temp);
						ar[i + (width * 4) + 5] = ar[i + (width * 4) + 5] + ((3.0 / 16.0) * temp);
						ar[i + (width * 4) + 6] = ar[i + (width * 4) + 6] + ((3.0 / 16.0) * temp);
					}
				}
				else
				{
					if(i % (width * 4) + 4 < width * 4)
					{
						ar[i + 4] = ar[i + 4] + ((7.0/16.0)* temp);
						ar[i + 5] = ar[i + 5] + ((7.0/16.0)* temp);
						ar[i + 6] = ar[i + 6] + ((7.0/16.0)* temp);
					}

					if(i % (width * 4) + 4 < width * 4 && i + (width * 4) < width * height * 4)
					{
						ar[i + 4 + (width * 4)] = ar[i + 4 + (width * 4)] + ((1.0/16.0) * temp);
						ar[i + 5 + (width * 4)] = ar[i + 5 + (width * 4)] + ((1.0/16.0) * temp);
						ar[i + 6 + (width * 4)] = ar[i + 6 + (width * 4)] + ((1.0/16.0) * temp);
					}

					if(i + (width * 4) < width * height * 4)
					{
						ar[i + (width * 4)] = ar[i + (width * 4)] + ((5.0 / 16.0) * temp);
						ar[i + 1 +(width * 4)] = ar[i + 1 + (width * 4)] + ((5.0 / 16.0) * temp);
						ar[i + 2 + (width * 4)] = ar[i + 2 + (width * 4)] + ((5.0 / 16.0) * temp);
					}
					if(i + (width * 4) < width * height * 4 && i % (width * 4) - 4 >= 0)
					{
						ar[i + (width * 4) - 4] = ar[i + (width * 4) - 4] + ((3.0 / 16.0) * temp);
						ar[i + (width * 4) - 3] = ar[i + (width * 4) - 3] + ((3.0 / 16.0) * temp);
						ar[i + (width * 4) - 2] = ar[i + (width * 4) - 2] + ((3.0 / 16.0) * temp);
					}
				}

				if(!even && (i % (width * 4)) + 4 < width * 4)
				{
					i = i + 4;
				}
				else if(!even && (i % (width * 4)) + 4 >= width * 4)
				{
					i = i + (width * 4);
					even = true;
				}
				else if(even && (i % (width * 4)) - 4 >= 0)
				{
					i = i - 4;
				}
				else if(even && (i % (width * 4)) - 4 < 0)
				{
					i = i + (width * 4);
					even = false;
				}
			}
		}
		else
		{
			even = false;
			while(i < width * height * 4)
			{
				temp = ar[i];
				if(temp < 0.5)
				{
					ar[i] = 0;
					ar[i + 1] = 0;
					ar[i + 2] = 0;
				}
				else
				{
					ar[i] = 1.0;
					ar[i + 1] = 1.0;
					ar[i + 2] = 1.0;
					temp = temp - 1.0;
				}

				//distribute error to 4 neighbors
				if(even)
				{
					if(i % (width * 4) - 4 >= 0)
					{
						ar[i - 4] = ar[i - 4] + ((7.0/16.0)* temp);
						ar[i - 3] = ar[i - 3] + ((7.0/16.0)* temp);
						ar[i - 2] = ar[i - 2] + ((7.0/16.0)* temp);
					}
					if(i % (width * 4) - 4 >= 0 && i + (width * 4) < width * height * 4)
					{
						ar[i - 4 + (width * 4)] = ar[i - 4 + (width * 4)] + ((1.0/16.0) * temp);
						ar[i - 3 + (width * 4)] = ar[i - 3 + (width * 4)] + ((1.0/16.0) * temp);
						ar[i - 2 + (width * 4)] = ar[i - 2 + (width * 4)] + ((1.0/16.0) * temp);
					}
					if(i + (width * 4) < width * height * 4)
					{
						ar[i + (width * 4)] = ar[i + (width * 4)] + ((5.0 / 16.0) * temp);
						ar[i + 1 +(width * 4)] = ar[i + 1 + (width * 4)] + ((5.0 / 16.0) * temp);
						ar[i + 2 + (width * 4)] = ar[i + 2 + (width * 4)] + ((5.0 / 16.0) * temp);
					}
					if(i + (width * 4) < width * height * 4 && i % (width * 4) + 4 < width * 4)
					{
						ar[i + (width * 4) + 4] = ar[i + (width * 4) + 4] + ((3.0 / 16.0) * temp);
						ar[i + (width * 4) + 5] = ar[i + (width * 4) + 5] + ((3.0 / 16.0) * temp);
						ar[i + (width * 4) + 6] = ar[i + (width * 4) + 6] + ((3.0 / 16.0) * temp);
					}
				}
				else
				{
					if(i % (width * 4) + 4 < width * 4)
					{
						ar[i + 4] = ar[i + 4] + ((7.0/16.0)* temp);
						ar[i + 5] = ar[i + 5] + ((7.0/16.0)* temp);
						ar[i + 6] = ar[i + 6] + ((7.0/16.0)* temp);
					}

					if(i % (width * 4) + 4 < width * 4 && i + (width * 4) < width * height * 4)
					{
						ar[i + 4 + (width * 4)] = ar[i + 4 + (width * 4)] + ((1.0/16.0) * temp);
						ar[i + 5 + (width * 4)] = ar[i + 5 + (width * 4)] + ((1.0/16.0) * temp);
						ar[i + 6 + (width * 4)] = ar[i + 6 + (width * 4)] + ((1.0/16.0) * temp);
					}

					if(i + (width * 4) < width * height * 4)
					{
						ar[i + (width * 4)] = ar[i + (width * 4)] + ((5.0 / 16.0) * temp);
						ar[i + 1 +(width * 4)] = ar[i + 1 + (width * 4)] + ((5.0 / 16.0) * temp);
						ar[i + 2 + (width * 4)] = ar[i + 2 + (width * 4)] + ((5.0 / 16.0) * temp);
					}
					if(i + (width * 4) < width * height * 4 && i % (width * 4) - 4 >= 0)
					{
						ar[i + (width * 4) - 4] = ar[i + (width * 4) - 4] + ((3.0 / 16.0) * temp);
						ar[i + (width * 4) - 3] = ar[i + (width * 4) - 3] + ((3.0 / 16.0) * temp);
						ar[i + (width * 4) - 2] = ar[i + (width * 4) - 2] + ((3.0 / 16.0) * temp);
					}
				}

				if(!even && (i % (width * 4)) + 4 < width * 4)
				{
					i = i + 4;
				}
				else if(!even && (i % (width * 4)) + 4 >= width * 4)
				{
					i = i + (width * 4);
					even = true;
				}
				else if(even && (i % (width * 4)) - 4 >= 0)
				{
					i = i - 4;
				}
				else if(even && (i % (width * 4)) - 4 < 0)
				{
					i = i + (width * 4);
					even = false;
				}
			}
		}

		for(int j = 0; j < width * height * 4; j = j + 4)
		{
			data[j] = (unsigned char)floor(ar[j] * 255);
			data[j + 1] = (unsigned char)floor(ar[j] * 255);
			data[j + 2] = (unsigned char)floor(ar[j] * 255);
		}
		delete[] ar;
		ar = NULL;
	}
	catch(...)
	{
		return false;
	}

	return true;
}// Dither_FS


///////////////////////////////////////////////////////////////////////////////
//
//      Dither the image while conserving the average brightness.  Return 
//  success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Dither_Bright()
{
	double temp = 0;
	double temp2 = 0;

	if(!To_Grayscale())
	{
		return false;
	}

	try
	{
		for(int i = 0; i < width * height * 4; i = i + 4)
		{	
			temp = temp + (data[i] / 255.0);
		}
		temp = temp / (width * height);

		for(int i = 0; i < width * height * 4; i = i + 4)
		{	
			temp2 = data[i] / 255.0;
			if(temp2 < temp)
			{
				data[i] = 0;
				data[i+1] = 0;
				data[i + 2] = 0;
			}
			else
			{
				data[i] = 255;
				data[i+1] = 255;
				data[i+2] = 255;
			}
		}
	}
	catch(...)
	{
		return false;
	}

	return true;
}// Dither_Bright


///////////////////////////////////////////////////////////////////////////////
//
//      Perform clustered differing of the image.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Dither_Cluster()
{
    ClearToBlack();
    return false;
}// Dither_Cluster


///////////////////////////////////////////////////////////////////////////////
//
//  Convert the image to an 8 bit image using Floyd-Steinberg dithering over
//  a uniform quantization - the same quantization as in Quant_Uniform.
//  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Dither_Color()
{
    ClearToBlack();
    return false;
}// Dither_Color


///////////////////////////////////////////////////////////////////////////////
//
//      Composite the current image over the given image.  Return success of 
//  operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Comp_Over(TargaImage* pImage)
{
    if (width != pImage->width || height != pImage->height)
    {
        cout <<  "Comp_Over: Images not the same size\n";
        return false;
    }

    ClearToBlack();
    return false;
}// Comp_Over


///////////////////////////////////////////////////////////////////////////////
//
//      Composite this image "in" the given image.  See lecture notes for 
//  details.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Comp_In(TargaImage* pImage)
{
    if (width != pImage->width || height != pImage->height)
    {
        cout << "Comp_In: Images not the same size\n";
        return false;
    }

    ClearToBlack();
    return false;
}// Comp_In


///////////////////////////////////////////////////////////////////////////////
//
//      Composite this image "out" the given image.  See lecture notes for 
//  details.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Comp_Out(TargaImage* pImage)
{
    if (width != pImage->width || height != pImage->height)
    {
        cout << "Comp_Out: Images not the same size\n";
        return false;
    }

    ClearToBlack();
    return false;
}// Comp_Out


///////////////////////////////////////////////////////////////////////////////
//
//      Composite current image "atop" given image.  Return success of 
//  operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Comp_Atop(TargaImage* pImage)
{
    if (width != pImage->width || height != pImage->height)
    {
        cout << "Comp_Atop: Images not the same size\n";
        return false;
    }

    ClearToBlack();
    return false;
}// Comp_Atop


///////////////////////////////////////////////////////////////////////////////
//
//      Composite this image with given image using exclusive or (XOR).  Return
//  success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Comp_Xor(TargaImage* pImage)
{
    if (width != pImage->width || height != pImage->height)
    {
        cout << "Comp_Xor: Images not the same size\n";
        return false;
    }

    ClearToBlack();
    return false;
}// Comp_Xor


///////////////////////////////////////////////////////////////////////////////
//
//      Calculate the difference bewteen this imag and the given one.  Image 
//  dimensions must be equal.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Difference(TargaImage* pImage)
{
    if (!pImage)
        return false;

    if (width != pImage->width || height != pImage->height)
    {
        cout << "Difference: Images not the same size\n";
        return false;
    }// if

    for (int i = 0 ; i < width * height * 4 ; i += 4)
    {
        unsigned char        rgb1[3];
        unsigned char        rgb2[3];

        RGBA_To_RGB(data + i, rgb1);
        RGBA_To_RGB(pImage->data + i, rgb2);

        data[i] = abs(rgb1[0] - rgb2[0]);
        data[i+1] = abs(rgb1[1] - rgb2[1]);
        data[i+2] = abs(rgb1[2] - rgb2[2]);
        data[i+3] = 255;
    }

    return true;
}// Difference


///////////////////////////////////////////////////////////////////////////////
//
//      Perform 5x5 box filter on this image.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Filter_Box()
{
	double temp = 0;
	unsigned char * nImage = new unsigned char[width * height * 4];
	int j = 0;
	int k = 0;

	try
	{
		for(int i = 0; i < width * height * 4; i = i + 4)
		{	
			for(j = i; j < i + 4; j = j + 1)
			{
				if(j == i + 3)
				{
					nImage[j] = data[j];
					break;
				}
				//trivial center case
				temp = data[j] / 25.0;
				
				//1 left of center
				if((j % (width * 4)) - 4 < 0)
				{
					k = j;
				}
				else
				{
					k = j - 4;
				}
				temp = temp + (data[k] / 25.0);

				//2 left of center
				if((j % (width * 4)) - 8 < 0)
				{
					if((j % (width * 4)) - 4 < 0)
					{
						k = j;
					}
					else
					{
						k = j - 4;
					}
				}
				else
				{
					k = j - 8;
				}
				temp = temp + (data[k] / 25.0);

				//1 right of center
				if((j % (width * 4)) + 4 >= width * 4)
				{
					k = j;
				}
				else
				{
					k = j + 4;
				}
				temp = temp + (data[k] / 25.0);

				//2 right of center
				if((j % (width * 4)) + 8 >= width * 4)
				{
					if((j % (width * 4)) + 4 >= width * 4)
					{
						k = j;
					}
					else
					{
						k = j + 4;
					}
				}
				else
				{
					k = j + 8;
				}
				temp = temp + (data[k] / 25.0);

				//1 up of center
				if(j - (width * 4) < 0)
				{
					k = j;
				}
				else
				{
					k = j - (width * 4);
				}
				temp = temp + (data[k] / 25.0);

				//2 up of center
				if(j - ((width * 4) * 2) < 0)
				{
					if(j - (width * 4) < 0)
					{
						k = j;
					}
					else
					{
						k = j - (width * 4);
					}
				}
				else
				{
					k = j - ((width * 4) * 2);
				}
				temp = temp + (data[k] / 25.0);

				//1 down of center
				if(j + (width * 4) >= (width * height * 4))
				{
					k = j;
				}
				else
				{
					k = j + (width * 4);
				}
				temp = temp + (data[k] / 25.0);

				//2 down of center
				if(j + ((width * 4) * 2) >= (width * height * 4))
				{
					if(j + (width * 4) >= (width * height * 4))
					{
						k = j;
					}
					else
					{
						k = j + (width * 4);
					}
				}
				else
				{
					k = j + ((width * 4) * 2);
				}
				temp = temp + (data[k] / 25.0);

				//1 up and left
				if(j - (width * 4) < 0 || j % (width * 4) - 4 < 0)
				{
					k = j;
				}
				else
				{
					k = j - (width * 4) - 4;
				}
				temp = temp + (data[k] / 25.0);

				//1 up and 2 left
				if(j - (width * 4) < 0 || j % (width * 4) - 8 < 0)
				{
					k = j;
				}
				else
				{
					k = j - (width * 4) - 8;
				}
				temp = temp + (data[k] / 25.0);

				//2 up and 2 left
				if(j - ((width * 4) * 2) < 0 || j % (width * 4) - 8 < 0)
				{
					k = j;
				}
				else
				{
					k = j - ((width * 4) * 2) - 8;
				}
				temp = temp + (data[k] / 25.0);

				//2 up and 1 left
				if(j - ((width * 4) * 2) < 0 || j % (width * 4) - 4 < 0)
				{
					k = j;
				}
				else
				{
					k = j - ((width * 4) * 2) - 4;
				}
				temp = temp + (data[k] / 25.0);

				//1 up and right
				if(j - (width * 4) < 0 || j % (width * 4) + 4 >= width * 4)
				{
					k = j;
				}
				else
				{
					k = j - (width * 4) + 4;
				}
				temp = temp + (data[k] / 25.0);

				//1 up and 2 right
				if(j - (width * 4) < 0 || j % (width * 4) + 8 > width * 4)
				{
					k = j;
				}
				else
				{
					k = j - (width * 4) + 8;
				}
				temp = temp + (data[k] / 25.0);

				//2 up and right
				if(j - ((width * 4) * 2) < 0 || j % (width * 4) + 8 > width * 4)
				{
					k = j;
				}
				else
				{
					k = j - ((width * 4) * 2) + 8;
				}
				temp = temp + (data[k] / 25.0);

				//2 up and 1 right
				if(j - ((width * 4) * 2) < 0 || j % (width * 4) + 4 > width * 4)
				{
					k = j;
				}
				else
				{
					k = j - ((width * 4) * 2) + 4;
				}
				temp = temp + (data[k] / 25.0);

				//1 down and 1 left
				if(j + (width * 4) >= (width * height * 4) || j % (width * 4) - 4 < 0)
				{
					k = j;
				}
				else
				{
					k = j + (width * 4) - 4;
				}
				temp = temp + (data[k] / 25.0);

				//1 down and 2 left
				if(j + (width * 4) >= (width * height * 4) || j % (width * 4) - 8 < 0)
				{
					k = j;
				}
				else
				{
					k = j + (width * 4) - 8;
				}
				temp = temp + (data[k] / 25.0);

				//2 down and 2 left
				if(j + ((width * 4) * 2) >= (width * height * 4) || j % (width * 4) - 8 < 0)
				{
					k = j;
				}
				else
				{
					k = j + ((width * 4) * 2) - 8;
				}
				temp = temp + (data[k] / 25.0);

				//2 down and 1 left
				if(j + ((width * 4) * 2) >= (width * height * 4) || j % (width * 4) - 4 < 0)
				{
					k = j;
				}
				else
				{
					k = j + ((width * 4) * 2) - 4;
				}
				temp = temp + (data[k] / 25.0);

				//1 down and 1 right
				if(j + (width * 4) >= (width * height * 4) || j % (width * 4) + 4 >= width * 4)
				{
					k = j;
				}
				else
				{
					k = j + (width * 4) + 4;
				}
				temp = temp + (data[k] / 25.0);

				//1 down and 2 right
				if(j + (width * 4) >= (width * height * 4) || j % (width * 4) + 8 >= width * 4)
				{
					k = j;
				}
				else
				{
					k = j + (width * 4) + 8;
				}
				temp = temp + (data[k] / 25.0);

				//2 down and 2 right
				if(j + ((width * 4) * 2) >= (width * height * 4) || j % (width * 4) + 8 >= width * 4)
				{
					k = j;
				}
				else
				{
					k = j + ((width * 4) * 2) + 8;
				}
				temp = temp + (data[k] / 25.0);

				//2 down and 1 right
				if(j + ((width * 4) * 2) >= (width * height * 4) || j % (width * 4) + 4 >= width * 4)
				{
					k = j;
				}
				else
				{
					k = j + ((width * 4) * 2) + 4;
				}
				temp = temp + (data[k] / 25.0);

				nImage[j] = temp;
			}
		}
		for(int i = 0; i < width * height * 4; i = i + 4)
		{
			data[i] = nImage[i];
			data[i + 1] = nImage[i + 1];
			data[i + 2] = nImage[i + 2];
		}

		delete[] nImage;
	}
	catch(...)
	{
		return false;
	}

	return true;
}// Filter_Box


///////////////////////////////////////////////////////////////////////////////
//
//      Perform 5x5 Bartlett filter on this image.  Return success of 
//  operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Filter_Bartlett()
{
	double temp = 0;
	unsigned char * nImage = new unsigned char[width * height * 4];
	int j = 0;
	int k = 0;

	try
	{
		for(int i = 0; i < width * height * 4; i = i + 4)
		{	
			for(j = i; j < i + 4; j = j + 1)
			{
				if(j == i + 3)
				{
					nImage[j] = data[j];
					break;
				}
				//trivial center case
				temp = data[j] * (9.0 / 81.0);
				
				//1 left of center
				if((j % (width * 4)) - 4 < 0)
				{
					k = j;
				}
				else
				{
					k = j - 4;
				}
				temp = temp + data[k] * (6.0 / 81.0);

				//2 left of center
				if((j % (width * 4)) - 8 < 0)
				{
					if((j % (width * 4)) - 4 < 0)
					{
						k = j;
					}
					else
					{
						k = j - 4;
					}
				}
				else
				{
					k = j - 8;
				}
				temp = temp + data[k] * (3.0 / 81.0);

				//1 right of center
				if((j % (width * 4)) + 4 >= width * 4)
				{
					k = j;
				}
				else
				{
					k = j + 4;
				}
				temp = temp + data[k] * (6.0 / 81.0);

				//2 right of center
				if((j % (width * 4)) + 8 >= width * 4)
				{
					if((j % (width * 4)) + 4 >= width * 4)
					{
						k = j;
					}
					else
					{
						k = j + 4;
					}
				}
				else
				{
					k = j + 8;
				}
				temp = temp + data[k] * (3.0 / 81.0);

				//1 up of center
				if(j - (width * 4) < 0)
				{
					k = j;
				}
				else
				{
					k = j - (width * 4);
				}
				temp = temp + data[k] * (6.0 / 81.0);

				//2 up of center
				if(j - ((width * 4) * 2) < 0)
				{
					if(j - (width * 4) < 0)
					{
						k = j;
					}
					else
					{
						k = j - (width * 4);
					}
				}
				else
				{
					k = j - ((width * 4) * 2);
				}
				temp = temp + data[k] * (3.0 / 81.0);

				//1 down of center
				if(j + (width * 4) >= (width * height * 4))
				{
					k = j;
				}
				else
				{
					k = j + (width * 4);
				}
				temp = temp + data[k] * (6.0 / 81.0);

				//2 down of center
				if(j + ((width * 4) * 2) >= (width * height * 4))
				{
					if(j + (width * 4) >= (width * height * 4))
					{
						k = j;
					}
					else
					{
						k = j + (width * 4);
					}
				}
				else
				{
					k = j + ((width * 4) * 2);
				}
				temp = temp + data[k] * (3.0 / 81.0);

				//1 up and left
				if(j - (width * 4) < 0 || j % (width * 4) - 4 < 0)
				{
					k = j;
				}
				else
				{
					k = j - (width * 4) - 4;
				}
				temp = temp + data[k] * (4.0 / 81.0);

				//1 up and 2 left
				if(j - (width * 4) < 0 || j % (width * 4) - 8 < 0)
				{
					k = j;
				}
				else
				{
					k = j - (width * 4) - 8;
				}
				temp = temp + data[k] * (2.0 / 81.0);

				//2 up and 2 left
				if(j - ((width * 4) * 2) < 0 || j % (width * 4) - 8 < 0)
				{
					k = j;
				}
				else
				{
					k = j - ((width * 4) * 2) - 8;
				}
				temp = temp + data[k] * (1.0 / 81.0);

				//2 up and 1 left
				if(j - ((width * 4) * 2) < 0 || j % (width * 4) - 4 < 0)
				{
					k = j;
				}
				else
				{
					k = j - ((width * 4) * 2) - 4;
				}
				temp = temp + data[k] * (2.0 / 81.0);

				//1 up and right
				if(j - (width * 4) < 0 || j % (width * 4) + 4 >= width * 4)
				{
					k = j;
				}
				else
				{
					k = j - (width * 4) + 4;
				}
				temp = temp + data[k] * (4.0 / 81.0);

				//1 up and 2 right
				if(j - (width * 4) < 0 || j % (width * 4) + 8 > width * 4)
				{
					k = j;
				}
				else
				{
					k = j - (width * 4) + 8;
				}
				temp = temp + data[k] * (2.0 / 81.0);

				//2 up and right
				if(j - ((width * 4) * 2) < 0 || j % (width * 4) + 8 > width * 4)
				{
					k = j;
				}
				else
				{
					k = j - ((width * 4) * 2) + 8;
				}
				temp = temp + data[k] * (1.0 / 81.0);

				//2 up and 1 right
				if(j - ((width * 4) * 2) < 0 || j % (width * 4) + 4 > width * 4)
				{
					k = j;
				}
				else
				{
					k = j - ((width * 4) * 2) + 4;
				}
				temp = temp + data[k] * (2.0 / 81.0);

				//1 down and 1 left
				if(j + (width * 4) >= (width * height * 4) || j % (width * 4) - 4 < 0)
				{
					k = j;
				}
				else
				{
					k = j + (width * 4) - 4;
				}
				temp = temp + data[k] * (4.0 / 81.0);

				//1 down and 2 left
				if(j + (width * 4) >= (width * height * 4) || j % (width * 4) - 8 < 0)
				{
					k = j;
				}
				else
				{
					k = j + (width * 4) - 8;
				}
				temp = temp + data[k] * (2.0 / 81.0);

				//2 down and 2 left
				if(j + ((width * 4) * 2) >= (width * height * 4) || j % (width * 4) - 8 < 0)
				{
					k = j;
				}
				else
				{
					k = j + ((width * 4) * 2) - 8;
				}
				temp = temp + data[k] * (1.0 / 81.0);

				//2 down and 1 left
				if(j + ((width * 4) * 2) >= (width * height * 4) || j % (width * 4) - 4 < 0)
				{
					k = j;
				}
				else
				{
					k = j + ((width * 4) * 2) - 4;
				}
				temp = temp + data[k] * (2.0 / 81.0);

				//1 down and 1 right
				if(j + (width * 4) >= (width * height * 4) || j % (width * 4) + 4 >= width * 4)
				{
					k = j;
				}
				else
				{
					k = j + (width * 4) + 4;
				}
				temp = temp + data[k] * (4.0 / 81.0);

				//1 down and 2 right
				if(j + (width * 4) >= (width * height * 4) || j % (width * 4) + 8 >= width * 4)
				{
					k = j;
				}
				else
				{
					k = j + (width * 4) + 8;
				}
				temp = temp + data[k] * (2.0 / 81.0);

				//2 down and 2 right
				if(j + ((width * 4) * 2) >= (width * height * 4) || j % (width * 4) + 8 >= width * 4)
				{
					k = j;
				}
				else
				{
					k = j + ((width * 4) * 2) + 8;
				}
				temp = temp + data[k] * (1.0 / 81.0);

				//2 down and 1 right
				if(j + ((width * 4) * 2) >= (width * height * 4) || j % (width * 4) + 4 >= width * 4)
				{
					k = j;
				}
				else
				{
					k = j + ((width * 4) * 2) + 4;
				}
				temp = temp + data[k] * (2.0 / 81.0);

				nImage[j] = temp;
			}
		}
		for(int i = 0; i < width * height * 4; i = i + 4)
		{
			data[i] = nImage[i];
			data[i + 1] = nImage[i + 1];
			data[i + 2] = nImage[i + 2];
		}

		delete[] nImage;
	}
	catch(...)
	{
		return false;
	}

	return true;
}// Filter_Bartlett


///////////////////////////////////////////////////////////////////////////////
//
//      Perform 5x5 Gaussian filter on this image.  Return success of 
//  operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Filter_Gaussian()
{
	double temp = 0;
	unsigned char * nImage = new unsigned char[width * height * 4];
	int j = 0;
	int k = 0;

	try
	{
		for(int i = 0; i < width * height * 4; i = i + 4)
		{	
			for(j = i; j < i + 4; j = j + 1)
			{
				if(j == i + 3)
				{
					nImage[j] = data[j];
					break;
				}
				//trivial center case
				temp = data[j] * (36.0 / 256.0);
				
				//1 left of center
				if((j % (width * 4)) - 4 < 0)
				{
					k = j;
				}
				else
				{
					k = j - 4;
				}
				temp = temp + data[k] * (24.0 / 256.0);

				//2 left of center
				if((j % (width * 4)) - 8 < 0)
				{
					if((j % (width * 4)) - 4 < 0)
					{
						k = j;
					}
					else
					{
						k = j - 4;
					}
				}
				else
				{
					k = j - 8;
				}
				temp = temp + data[k] * (6.0 / 256.0);

				//1 right of center
				if((j % (width * 4)) + 4 >= width * 4)
				{
					k = j;
				}
				else
				{
					k = j + 4;
				}
				temp = temp + data[k] * (24.0 / 256.0);

				//2 right of center
				if((j % (width * 4)) + 8 >= width * 4)
				{
					if((j % (width * 4)) + 4 >= width * 4)
					{
						k = j;
					}
					else
					{
						k = j + 4;
					}
				}
				else
				{
					k = j + 8;
				}
				temp = temp + data[k] * (6.0 / 256.0);

				//1 up of center
				if(j - (width * 4) < 0)
				{
					k = j;
				}
				else
				{
					k = j - (width * 4);
				}
				temp = temp + data[k] * (24.0 / 256.0);

				//2 up of center
				if(j - ((width * 4) * 2) < 0)
				{
					if(j - (width * 4) < 0)
					{
						k = j;
					}
					else
					{
						k = j - (width * 4);
					}
				}
				else
				{
					k = j - ((width * 4) * 2);
				}
				temp = temp + data[k] * (6.0 / 256.0);

				//1 down of center
				if(j + (width * 4) >= (width * height * 4))
				{
					k = j;
				}
				else
				{
					k = j + (width * 4);
				}
				temp = temp + data[k] * (24.0 / 256.0);

				//2 down of center
				if(j + ((width * 4) * 2) >= (width * height * 4))
				{
					if(j + (width * 4) >= (width * height * 4))
					{
						k = j;
					}
					else
					{
						k = j + (width * 4);
					}
				}
				else
				{
					k = j + ((width * 4) * 2);
				}
				temp = temp + data[k] * (6.0 / 256.0);

				//1 up and left
				if(j - (width * 4) < 0 || j % (width * 4) - 4 < 0)
				{
					k = j;
				}
				else
				{
					k = j - (width * 4) - 4;
				}
				temp = temp + data[k] * (16.0 / 256.0);

				//1 up and 2 left
				if(j - (width * 4) < 0 || j % (width * 4) - 8 < 0)
				{
					k = j;
				}
				else
				{
					k = j - (width * 4) - 8;
				}
				temp = temp + data[k] * (4.0 / 256.0);

				//2 up and 2 left
				if(j - ((width * 4) * 2) < 0 || j % (width * 4) - 8 < 0)
				{
					k = j;
				}
				else
				{
					k = j - ((width * 4) * 2) - 8;
				}
				temp = temp + data[k] * (1.0 / 256.0);

				//2 up and 1 left
				if(j - ((width * 4) * 2) < 0 || j % (width * 4) - 4 < 0)
				{
					k = j;
				}
				else
				{
					k = j - ((width * 4) * 2) - 4;
				}
				temp = temp + data[k] * (4.0 / 256.0);

				//1 up and right
				if(j - (width * 4) < 0 || j % (width * 4) + 4 >= width * 4)
				{
					k = j;
				}
				else
				{
					k = j - (width * 4) + 4;
				}
				temp = temp + data[k] * (16.0 / 256.0);

				//1 up and 2 right
				if(j - (width * 4) < 0 || j % (width * 4) + 8 > width * 4)
				{
					k = j;
				}
				else
				{
					k = j - (width * 4) + 8;
				}
				temp = temp + data[k] * (4.0 / 256.0);

				//2 up and right
				if(j - ((width * 4) * 2) < 0 || j % (width * 4) + 8 > width * 4)
				{
					k = j;
				}
				else
				{
					k = j - ((width * 4) * 2) + 8;
				}
				temp = temp + data[k] * (1.0 / 256.0);

				//2 up and 1 right
				if(j - ((width * 4) * 2) < 0 || j % (width * 4) + 4 > width * 4)
				{
					k = j;
				}
				else
				{
					k = j - ((width * 4) * 2) + 4;
				}
				temp = temp + data[k] * (4.0 / 256.0);

				//1 down and 1 left
				if(j + (width * 4) >= (width * height * 4) || j % (width * 4) - 4 < 0)
				{
					k = j;
				}
				else
				{
					k = j + (width * 4) - 4;
				}
				temp = temp + data[k] * (16.0 / 256.0);

				//1 down and 2 left
				if(j + (width * 4) >= (width * height * 4) || j % (width * 4) - 8 < 0)
				{
					k = j;
				}
				else
				{
					k = j + (width * 4) - 8;
				}
				temp = temp + data[k] * (4.0 / 256.0);

				//2 down and 2 left
				if(j + ((width * 4) * 2) >= (width * height * 4) || j % (width * 4) - 8 < 0)
				{
					k = j;
				}
				else
				{
					k = j + ((width * 4) * 2) - 8;
				}
				temp = temp + data[k] * (1.0 / 256.0);

				//2 down and 1 left
				if(j + ((width * 4) * 2) >= (width * height * 4) || j % (width * 4) - 4 < 0)
				{
					k = j;
				}
				else
				{
					k = j + ((width * 4) * 2) - 4;
				}
				temp = temp + data[k] * (4.0 / 256.0);

				//1 down and 1 right
				if(j + (width * 4) >= (width * height * 4) || j % (width * 4) + 4 >= width * 4)
				{
					k = j;
				}
				else
				{
					k = j + (width * 4) + 4;
				}
				temp = temp + data[k] * (16.0 / 256.0);

				//1 down and 2 right
				if(j + (width * 4) >= (width * height * 4) || j % (width * 4) + 8 >= width * 4)
				{
					k = j;
				}
				else
				{
					k = j + (width * 4) + 8;
				}
				temp = temp + data[k] * (4.0 / 256.0);

				//2 down and 2 right
				if(j + ((width * 4) * 2) >= (width * height * 4) || j % (width * 4) + 8 >= width * 4)
				{
					k = j;
				}
				else
				{
					k = j + ((width * 4) * 2) + 8;
				}
				temp = temp + data[k] * (1.0 / 256.0);

				//2 down and 1 right
				if(j + ((width * 4) * 2) >= (width * height * 4) || j % (width * 4) + 4 >= width * 4)
				{
					k = j;
				}
				else
				{
					k = j + ((width * 4) * 2) + 4;
				}
				temp = temp + data[k] * (4.0 / 256.0);

				nImage[j] = temp;
			}
		}
		for(int i = 0; i < width * height * 4; i = i + 4)
		{
			data[i] = nImage[i];
			data[i + 1] = nImage[i + 1];
			data[i + 2] = nImage[i + 2];
		}

		delete[] nImage;
	}
	catch(...)
	{
		return false;
	}

	return true;
}// Filter_Gaussian

///////////////////////////////////////////////////////////////////////////////
//
//      Perform NxN Gaussian filter on this image.  Return success of 
//  operation.
//
///////////////////////////////////////////////////////////////////////////////

bool TargaImage::Filter_Gaussian_N( unsigned int N )
{
    ClearToBlack();
   return false;
}// Filter_Gaussian_N


///////////////////////////////////////////////////////////////////////////////
//
//      Perform 5x5 edge detect (high pass) filter on this image.  Return 
//  success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Filter_Edge()
{
    ClearToBlack();
    return false;
}// Filter_Edge


///////////////////////////////////////////////////////////////////////////////
//
//      Perform a 5x5 enhancement filter to this image.  Return success of 
//  operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Filter_Enhance()
{
    ClearToBlack();
    return false;
}// Filter_Enhance


///////////////////////////////////////////////////////////////////////////////
//
//      Run simplified version of Hertzmann's painterly image filter.
//      You probably will want to use the Draw_Stroke funciton and the
//      Stroke class to help.
// Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::NPR_Paint()
{
    ClearToBlack();
    return false;
}



///////////////////////////////////////////////////////////////////////////////
//
//      Halve the dimensions of this image.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Half_Size()
{
    ClearToBlack();
    return false;
}// Half_Size


///////////////////////////////////////////////////////////////////////////////
//
//      Double the dimensions of this image.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Double_Size()
{
    ClearToBlack();
    return false;
}// Double_Size


///////////////////////////////////////////////////////////////////////////////
//
//      Scale the image dimensions by the given factor.  The given factor is 
//  assumed to be greater than one.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Resize(float scale)
{
    ClearToBlack();
    return false;
}// Resize


//////////////////////////////////////////////////////////////////////////////
//
//      Rotate the image clockwise by the given angle.  Do not resize the 
//  image.  Return success of operation.
//
///////////////////////////////////////////////////////////////////////////////
bool TargaImage::Rotate(float angleDegrees)
{
    ClearToBlack();
    return false;
}// Rotate


//////////////////////////////////////////////////////////////////////////////
//
//      Given a single RGBA pixel return, via the second argument, the RGB
//      equivalent composited with a black background.
//
///////////////////////////////////////////////////////////////////////////////
void TargaImage::RGBA_To_RGB(unsigned char *rgba, unsigned char *rgb)
{
    const unsigned char	BACKGROUND[3] = { 0, 0, 0 };

    unsigned char  alpha = rgba[3];

    if (alpha == 0)
    {
        rgb[0] = BACKGROUND[0];
        rgb[1] = BACKGROUND[1];
        rgb[2] = BACKGROUND[2];
    }
    else
    {
	    float	alpha_scale = (float)255 / (float)alpha;
	    int	val;
	    int	i;

	    for (i = 0 ; i < 3 ; i++)
	    {
	        val = (int)floor(rgba[i] * alpha_scale);
	        if (val < 0)
		    rgb[i] = 0;
	        else if (val > 255)
		    rgb[i] = 255;
	        else
		    rgb[i] = val;
	    }
    }
}// RGA_To_RGB


///////////////////////////////////////////////////////////////////////////////
//
//      Copy this into a new image, reversing the rows as it goes. A pointer
//  to the new image is returned.
//
///////////////////////////////////////////////////////////////////////////////
TargaImage* TargaImage::Reverse_Rows(void)
{
    unsigned char   *dest = new unsigned char[width * height * 4];
    TargaImage	    *result;
    int 	        i, j;

    if (! data)
    	return NULL;

    for (i = 0 ; i < height ; i++)
    {
	    int in_offset = (height - i - 1) * width * 4;
	    int out_offset = i * width * 4;

	    for (j = 0 ; j < width ; j++)
        {
	        dest[out_offset + j * 4] = data[in_offset + j * 4];
	        dest[out_offset + j * 4 + 1] = data[in_offset + j * 4 + 1];
	        dest[out_offset + j * 4 + 2] = data[in_offset + j * 4 + 2];
	        dest[out_offset + j * 4 + 3] = data[in_offset + j * 4 + 3];
        }
    }

    result = new TargaImage(width, height, dest);
    delete[] dest;
    return result;
}// Reverse_Rows


///////////////////////////////////////////////////////////////////////////////
//
//      Clear the image to all black.
//
///////////////////////////////////////////////////////////////////////////////
void TargaImage::ClearToBlack()
{
    memset(data, 0, width * height * 4);
}// ClearToBlack


///////////////////////////////////////////////////////////////////////////////
//
//      Helper function for the painterly filter; paint a stroke at
// the given location
//
///////////////////////////////////////////////////////////////////////////////
void TargaImage::Paint_Stroke(const Stroke& s) {
   int radius_squared = (int)s.radius * (int)s.radius;
   for (int x_off = -((int)s.radius); x_off <= (int)s.radius; x_off++) {
      for (int y_off = -((int)s.radius); y_off <= (int)s.radius; y_off++) {
         int x_loc = (int)s.x + x_off;
         int y_loc = (int)s.y + y_off;
         // are we inside the circle, and inside the image?
         if ((x_loc >= 0 && x_loc < width && y_loc >= 0 && y_loc < height)) {
            int dist_squared = x_off * x_off + y_off * y_off;
            if (dist_squared <= radius_squared) {
               data[(y_loc * width + x_loc) * 4 + 0] = s.r;
               data[(y_loc * width + x_loc) * 4 + 1] = s.g;
               data[(y_loc * width + x_loc) * 4 + 2] = s.b;
               data[(y_loc * width + x_loc) * 4 + 3] = s.a;
            } else if (dist_squared == radius_squared + 1) {
               data[(y_loc * width + x_loc) * 4 + 0] = 
                  (data[(y_loc * width + x_loc) * 4 + 0] + s.r) / 2;
               data[(y_loc * width + x_loc) * 4 + 1] = 
                  (data[(y_loc * width + x_loc) * 4 + 1] + s.g) / 2;
               data[(y_loc * width + x_loc) * 4 + 2] = 
                  (data[(y_loc * width + x_loc) * 4 + 2] + s.b) / 2;
               data[(y_loc * width + x_loc) * 4 + 3] = 
                  (data[(y_loc * width + x_loc) * 4 + 3] + s.a) / 2;
            }
         }
      }
   }
}


///////////////////////////////////////////////////////////////////////////////
//
//      Build a Stroke
//
///////////////////////////////////////////////////////////////////////////////
Stroke::Stroke() {}

///////////////////////////////////////////////////////////////////////////////
//
//      Build a Stroke
//
///////////////////////////////////////////////////////////////////////////////
Stroke::Stroke(unsigned int iradius, unsigned int ix, unsigned int iy,
               unsigned char ir, unsigned char ig, unsigned char ib, unsigned char ia) :
   radius(iradius),x(ix),y(iy),r(ir),g(ig),b(ib),a(ia)
{
}

