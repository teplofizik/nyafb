//
// image_png.h
// Nya framebuffer library. Image png class
//

#include "image_png.h"
#include <unistd.h>
#include <fcntl.h>

using namespace nyafb::images;

// Constructor
// path: path to image file
c_image_png::c_image_png(std::string path) : row_pointers(0)
{
	FILE * fp = fopen(path.c_str(), "rb");
	if(fp)
	{
		read_image(fp);
		fclose(fp);
	}
}

// Destructor
c_image_png::~c_image_png()
{
	if(row_pointers)
	{
		for(int y = 0; y < height; y++)
		{
			free(row_pointers[y]);
		}
		free(row_pointers);
		row_pointers = 0;
	}
}

// Read image to memory
void c_image_png::read_image(FILE * fp)
{
	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png) 
		return;
	
	png_infop info = png_create_info_struct(png);
	if(!info) 
		return;

	png_init_io(png, fp);

	png_read_info(png, info);

	width      = png_get_image_width(png, info);
	height     = png_get_image_height(png, info);
	color_type = png_get_color_type(png, info);
	bit_depth  = png_get_bit_depth(png, info);

	// Read any color_type into 8bit depth, RGBA format.
	// See http://www.libpng.org/pub/png/libpng-manual.txt

	if(bit_depth == 16)
		png_set_strip_16(png);

	if(color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png);

	// PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
	if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8(png);

	if(png_get_valid(png, info, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png);

	// These color_type don't have an alpha channel then fill it with 0xff.
	if(color_type == PNG_COLOR_TYPE_RGB ||
	   color_type == PNG_COLOR_TYPE_GRAY ||
	   color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

	if(color_type == PNG_COLOR_TYPE_GRAY ||
	   color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png);

	png_read_update_info(png, info);

	row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
	for(int y = 0; y < height; y++)
	{
		row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
	}

	png_read_image(png, row_pointers);
	png_destroy_read_struct(&png, &info, NULL);
}

// Is device available
// return: true, is available
bool c_image_png::is_available()
{
	return (row_pointers != 0);
}

// Get image width
// return: width, pixels
uint16_t c_image_png::get_width()
{
	return width;
}

// Get image height
// return: height, pixels
uint16_t c_image_png::get_height()
{
	return height;
}

// Get color
//      x: x offset
//      y: y offset
// return: color 0xAABBGGRR
uint32_t c_image_png::get_pixel(uint16_t x, uint16_t y)
{
	if((x < width) && (y < height))
	{
		png_bytep row = row_pointers[y];
		png_bytep px = &(row[x * 4]);

		return  (uint32_t)px[0] | 
			   ((uint32_t)px[1] << 8) |
			   ((uint32_t)px[2] << 16) |
			   ((uint32_t)px[3] << 24);
	}
	else
		return 0;
}
