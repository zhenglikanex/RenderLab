#include <cstdint>
#include <iostream>
#include <string>

namespace Aurora
{
	struct GfxConfiguration
	{
		GfxConfiguration(uint32_t r = 8,uint32_t g=8,
			uint32_t b= 8,uint32_t a = 8,
			uint32_t d = 24,uint32_t s = 0,uint32_t msaa = 0,
			uint32_t width = 1920,uint32_t height = 1080,
			const std::string& name = "MyEngine")
			:red_bits(r),green_bits(g),blue_bits(b), alpha_bits(a),
			depth_bits(d),stencil_bits(s),msaa_samples(msaa),
			screen_width(width),screen_height(height),
			app_name(name)
		{}
		
		friend std::ostream& operator<<(std::ostream& out, const GfxConfiguration& conf)
		{
			out << "GfxConfiguration:" << "\n"
				<< "R:" << conf.red_bits << "\n"
				<< "G:" << conf.green_bits << "\n"
				<< "B:" << conf.blue_bits << "\n"
				<< "A:" << conf.alpha_bits << "\n"
				<< "D:" << conf.depth_bits << "\n"
				<< "S:" << conf.stencil_bits << "\n"
				<< "M:" << conf.msaa_samples << "\n"
				<< "W:" << conf.screen_width << "\n"
				<< "H:" << conf.screen_height << "\n"
				<< "app_name:" << conf.app_name
				<< std::endl;

			return out;
		}

		uint32_t red_bits;
		uint32_t green_bits;
		uint32_t blue_bits;
		uint32_t alpha_bits;
		uint32_t depth_bits;
		uint32_t stencil_bits;
		uint32_t msaa_samples;
		uint32_t screen_width;
		uint32_t screen_height;
		std::string app_name;
	};

	
}