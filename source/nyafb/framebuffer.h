//
// framebuffer.h
// Nya framebuffer library
//

#include <stdint.h>
#include <stdbool.h>
#include <string>
#include <memory>

#ifndef _NYAFB_FRAMEBUFFER_H
#define _NYAFB_FRAMEBUFFER_H

#include "image.h"

namespace nyafb {
		
	class c_framebuffer
	{
	private:
		// File descriptor
		int fd;
		
		// Framebuffer memory pointer
		uint8_t * fbp;
		
		// Framebuffer width
		uint16_t width;
		
		// Framebuffer height
		uint16_t height;
		
		// X offset to start visible zone in framebuffer
		uint16_t xoffset;
		
		// Y offset to start visible zone in framebuffer
		uint16_t yoffset;
		
		// Framebuffer bit per pixel
		uint8_t  bpp;
		
		// Memory map size
		uint32_t memorysize;
		
		// Line length in bytes
		uint16_t line_length;
		
	public:
		// Constructor
		// path: path to fb device
		c_framebuffer(std::string path);
		
		// Destructor
		~c_framebuffer();
		
		// Is device available
		// return: true
		bool is_available();
		
		// Get framebuffer width
		// return: width, pixels
		uint16_t get_width();
		
		// Get framebuffer height
		// return: height, pixels
		uint16_t get_height();

		// Get framebuffer bpp
		// return: bits per pixel
		uint8_t get_bpp();

		// Set color
		// x: x offset
		// y: y offset
		// r: red component
		// g: green component
		// b: blue component
		// a: alpha (0 - opaque, 255 - transparent)
		void set_pixel(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0);
		
		// Set color
		// x: x offset
		// y: y offset
		// color: color 0xAABBGGRR
		void set_pixel(uint16_t x, uint16_t y, uint32_t color);

		// Fill by color
		// r: red component
		// g: green component
		// b: blue component
		// a: alpha (0 - opaque, 255 - transparent)
		void fill(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

		// Fill by color
		// color: color 0xAABBGGRR
		void fill(uint32_t color);

		// Draw image
		//     x: x offset
		//     y: y offset
		// image: image to draw
		void draw_image(uint16_t x, uint16_t y, std::shared_ptr<c_image> image);
	};

}

#endif
