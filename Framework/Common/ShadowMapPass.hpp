#pragma once

#include "IDrawPass.hpp"

namespace Aurora
{
	class ShadowMapPass : public IDrawPass
	{
	public:
		~ShadowMapPass() = default;
		void Draw(Frame& frame) override;
	};
}