#include "ImageParser.hpp"

#include "Framework/Utils/FileUtils.hpp"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image.h"
#include "stb_image_write.h"

using namespace Aurora;

std::shared_ptr<Image> ImageParser::Parser(const std::string& file_name)
{
	auto path = FileUtils::GetInstance()->GetFullPath(file_name);
	if (!path.empty())
	{
		int width, height, bitcount;
		//stbi_set_flip_vertically_on_load(true);
		uint8_t* data = (uint8_t*)stbi_load(path.string().c_str(), &width, &height, &bitcount, 0);
		if (data)
		{
			std::shared_ptr<Image> image = std::make_shared<Image>();
			image->Width = width;
			image->Height = height;
			image->bitcount = bitcount;
			image->data = data;
			image->data_size = image->Width * image->Height * image->bitcount;

			return image;
		}
		else
		{
			stbi_image_free(data);
		}

		return nullptr;
	}

	return nullptr;
}