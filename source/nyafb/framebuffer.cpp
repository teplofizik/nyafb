
//
// framebuffer.h
// Nya framebuffer library
//

#include "framebuffer.h"
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

using namespace nyafb;
// Constructor
// path: path to fb device
c_framebuffer::c_framebuffer(std::string path) : fd(-1)
{
	int fbfd = open(path.c_str(), O_RDWR);
	
	if(fbfd >= 0)
	{
		struct fb_var_screeninfo vinfo;
		struct fb_fix_screeninfo finfo;
		
		// Get fixed screen information
		if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo))
		{
			std::printf("Error reading fixed information.\n");
			close(fbfd);
			return;
		}

		// Get variable screen information
		if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo))
		{
			std::printf("Error reading variable information.\n");
			close(fbfd);
			return;
		}
		
		width       = vinfo.xres;               // Framebuffer width
		height      = vinfo.yres;               // Framebuffer height
		bpp         = vinfo.bits_per_pixel;     // Framebuffer bit per pixel
		memorysize  = width * height * bpp / 8; // ...
		line_length = finfo.line_length;
		xoffset     = vinfo.xoffset;
		yoffset     = vinfo.yoffset;
		
		fbp = (uint8_t *)mmap(0, memorysize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
		if ((int64_t)fbp == -1)
		{
			printf("Error: failed to map framebuffer device to memory.\n");
			close(fbfd);
			return;
		}
		
		fd = fbfd;
	}
}

// Destructor
c_framebuffer::~c_framebuffer()
{
	if(fd >= 0)
	{
		munmap(fbp, memorysize);
		close(fd);
		
		fbp = (uint8_t*)0;
		fd = -1;
	}
}

// Get cropped dimension size of object
//      v: x or y
//   size: width or height
// fbsize: framebuffer width or height
uint16_t c_framebuffer::get_cropped_size(uint16_t v, uint16_t size, uint16_t fbsize)
{
	uint16_t last_v = v + size - 1;
	
	if(size >= fbsize)
		size -= (size - fbsize);
	
	return size;
}
		
// Set color
// x: x offset
// y: y offset
// r: red component
// g: green component
// b: blue component
// a: alpha (0 - opaque, 255 - transparent)
void c_framebuffer::set_pixel(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	if(fd >= 0)
	{
		uint32_t offset = (x + xoffset) * (bpp / 8) + (y + yoffset) * line_length;
		uint8_t * dst = &fbp[offset];
		
		// TODO: color profiles...
		if (bpp == 32)
		{
			dst[0] = b;
			dst[1] = g;
			dst[2] = r;
			dst[3] = a;
		}
		else if (bpp == 16)
		{
			uint16_t val = ((b >> 3) & 0x1F) |
			               (((g >> 2) & 0x3F) << 5) |
						   (((r >> 3) & 0x1F) << 11);
			
			*(uint16_t *)dst = val;
		}
	}
}

// Set color
// x: x offset
// y: y offset
// color: color 0xAABBGGRR
void c_framebuffer::set_pixel(uint16_t x, uint16_t y, uint32_t color)
{
	uint8_t alpha = (color >> 24) & 0xFF;
	uint8_t red   = (color >> 0)  & 0xFF;
	uint8_t green = (color >> 8)  & 0xFF;
	uint8_t blue  = (color >> 16) & 0xFF;
	
	set_pixel(x, y, red, green, blue, alpha);
}

// Is device available
// return: true
bool c_framebuffer::is_available()
{
	return fd >= 0;
}

// Get framebuffer width
// return: width, pixels
uint16_t c_framebuffer::get_width()
{
	return width;
}

// Get framebuffer height
// return: height, pixels
uint16_t c_framebuffer::get_height()
{
	return height;
}

// Get framebuffer bpp
// return: bits per pixel
uint8_t c_framebuffer::get_bpp()
{
	return bpp;
}

// Fill by color
// r: red component
// g: green component
// b: blue component
// a: alpha (0 - opaque, 255 - transparent)
void c_framebuffer::fill(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	for (uint16_t y = 0; y < height; y++)
	{
		for (uint16_t x = 0; x < width; x++)
		{
			set_pixel(x, y, r, g, b, a);
		}
	}
}

// Fill by color
// color: color 0xAABBGGRR
void c_framebuffer::fill(uint32_t color)
{
	uint8_t alpha = (color >> 24) & 0xFF;
	uint8_t red   = (color >> 0)  & 0xFF;
	uint8_t green = (color >> 8)  & 0xFF;
	uint8_t blue  = (color >> 16) & 0xFF;
	
	fill(red, green, blue, alpha);
}

// Fill rect by color
// x: x offset
// y: y offset
// w: rect width
// y: rect height
// r: red component
// g: green component
// b: blue component
// a: alpha (0 - opaque, 255 - transparent)
void c_framebuffer::rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	uint16_t rwidth = get_cropped_size(x, w, width);
	uint16_t rheight = get_cropped_size(y, h, height);
	
	for (uint16_t ry = 0; ry < rheight; ry++)
	{
		for (uint16_t rx = 0; rx < rwidth; rx++)
		{
			set_pixel(x + rx, y + ry, r, g, b, a);
		}
	}
}

// Fill rect by color
// x: x offset
// y: y offset
// w: rect width
// y: rect height
// color: color 0xAABBGGRR
void c_framebuffer::rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color)
{
	uint8_t alpha = (color >> 24) & 0xFF;
	uint8_t red   = (color >> 0)  & 0xFF;
	uint8_t green = (color >> 8)  & 0xFF;
	uint8_t blue  = (color >> 16) & 0xFF;
	
	rect(x, y, w, h, red, green, blue, alpha);
}

// Draw image
//     x: x offset
//     y: y offset
// image: image to draw
void c_framebuffer::draw_image(uint16_t x, uint16_t y, std::shared_ptr<c_image> image)
{
	if((image == nullptr) || !image->is_available() || (x >= width) || (y >= height))
		return;
	
	uint16_t iwidth = get_cropped_size(x, image->get_width(), width);
	uint16_t iheight = get_cropped_size(y, image->get_height(), height);
	
	for(int ix = 0; ix < iwidth; ix++)
	{
		for(int iy = 0; iy < iheight; iy++)
		{
			uint32_t color = image->get_pixel(ix, iy);
			
			set_pixel(x + ix, y + iy, color);
		}
	}
}
