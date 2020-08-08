#pragma once

#include <memory>
#include <string>

#include "Framework/Common/Image.hpp"


namespace Aurora
{
	class ImageParser
	{
	public:
		std::shared_ptr<Image> Parser(const std::string& file_name);
	private:
		
	};
}