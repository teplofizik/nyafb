//
// ПО для программирования stm32 на дроне Кит
//

#include <iostream>
#include <memory>
#include <nyafb/framebuffer.h>
#include <nyafb/images/image_png.h>

int main(int argc, const char * argv[])
{
	if(argc == 2)
	{
		auto framebuffer = std::make_shared<nyafb::c_framebuffer>(argv[1]);
		if(framebuffer->is_available())
		{
			std::printf("%dx%d %dbpp\n", framebuffer->get_width(), 
			                              framebuffer->get_height(), 
										  framebuffer->get_bpp());
			
			framebuffer->fill(0, 0, 100, 0);
			framebuffer->rect(500, 200, 30, 30, 255, 255, 0, 0);
			
			auto image = std::make_shared<nyafb::images::c_image_png>("test.png");
			framebuffer->draw_image(10, 10, image);
		}
	}
	else
	{
		std::cout << "Usage: " << std::endl;
		std::cout << "fbtest_1 <dev>" << std::endl;
	}
	return 0;
}