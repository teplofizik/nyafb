//
// framebuffer.h
// Nya framebuffer library. Image png class
//

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string>
#include <png.h>

#ifndef _NYAFB_IMAGE_PNG_H
#define _NYAFB_IMAGE_PNG_H

#include "../image.h"

namespace nyafb {
	
	namespace images {
		
		class c_image_png : public nyafb::c_image
		{
		private:
			// Image width
			uint16_t width;
			// Image height
			uint16_t height;
			// Image color type
			png_byte color_type;
			// Image bit depth
			png_byte bit_depth;
			
			// Image raw data
			png_bytep * row_pointers;
					
			// Read image to memory
			void read_image(FILE * fp);
			
		public:
			// Constructor
			// path: path to image file
			c_image_png(std::string path);
			
			// Destructor
			~c_image_png();
			
			// Is device available
			// return: true, is available
			virtual bool is_available() override;
			
			// Get image width
			// return: width, pixels
			virtual uint16_t get_width() override;
			
			// Get image height
			// return: height, pixels
			virtual uint16_t get_height() override;

			// Get color
			//      x: x offset
			//      y: y offset
			// return: color 0xAABBGGRR
			virtual uint32_t get_pixel(uint16_t x, uint16_t y) override;
		};
	
	}
}

#endif
