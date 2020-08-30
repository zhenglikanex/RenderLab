#pragma once
#include <iostream>

#include "Framework/Common/GfxStructures.hpp"

namespace Aurora
{
	class IDrawPass
	{
	public:
		IDrawPass() = default;
		virtual ~IDrawPass() {};
		
		virtual void Draw(Frame& frame) = 0;
	};
}