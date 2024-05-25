//
// framebuffer.h
// Nya framebuffer library. Image class
//

#include <stdint.h>
#include <stdbool.h>
#include <string>

#ifndef _NYAFB_IMAGE_H
#define _NYAFB_IMAGE_H

namespace nyafb {
		
	class c_image
	{
	public:
		// Constructor
		c_image();
		
		// Is device available
		// return: true, is available
		virtual bool is_available();
		
		// Get image width
		// return: width, pixels
		virtual uint16_t get_width();
		
		// Get image height
		// return: height, pixels
		virtual uint16_t get_height();

		// Get color
		//      x: x offset
		//      y: y offset
		// return: color 0xAABBGGRR
		virtual uint32_t get_pixel(uint16_t x, uint16_t y);
	};

}

#endif
