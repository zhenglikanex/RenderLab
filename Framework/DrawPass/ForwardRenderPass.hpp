#pragma once

#include "Framework/Common/IDrawPass.hpp"

namespace Aurora
{
	class ForwardRenderPass : public IDrawPass
	{
	public:
		~ForwardRenderPass() = default;
		void Draw(Frame& frame) override;
	};
}