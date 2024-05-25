//
// image.h
// Nya framebuffer library
//

#include "image.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

using namespace nyafb;

// Constructor
c_image::c_image() 
{
	
}

// Is device available
// return: true, is available
bool c_image::is_available()
{
	return 0;
}

// Get image width
// return: width, pixels
uint16_t c_image::get_width()
{
	return 0;
}

// Get image height
// return: height, pixels
uint16_t c_image::get_height()
{
	return 0;
}

// Get color
//      x: x offset
//      y: y offset
// return: color 0xAABBGGRR
uint32_t c_image::get_pixel(uint16_t x, uint16_t y)
{
	return 0;
}
