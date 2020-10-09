#pragma once

#include "Framework/Common/IDrawPass.hpp"

namespace Aurora
{
	class HUDPass : public IDrawPass
	{
	public:
		~HUDPass() = default;
		void Draw(Frame& frame);
	};
}