#pragma once

#include <cstdint>

namespace Aurora
{
	struct Image
	{
		uint32_t Width;
		uint32_t Height;
		uint8_t* data;
		uint32_t bitcount;
		uint32_t pitch;
		size_t  data_size;
	};
}