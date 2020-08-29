#pragma once

#include "IDrawPass.hpp"

namespace Aurora
{
	class ShadowMapPass : public IDrawPass
	{
	public:
		~ShadowMapPass() = default;
		void Draw(const Frame& frame) override;
	};
}