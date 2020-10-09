#pragma once

#include "Framework/Common/IDrawPass.hpp"

namespace Aurora
{
	class ShadowMapPass : public IDrawPass
	{
	public:
		~ShadowMapPass() = default;
		void Draw(Frame& frame) override;
	};
}