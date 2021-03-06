#pragma once

#include "RHI/OpenGL/OpenGLGraphicsManagerCommonBase.hpp"

namespace Aurora
{
	class OpenGLGraphicsManager : public OpenGLGraphicsManagerCommonBase
	{
	public:
		OpenGLGraphicsManager(GLADloadproc loader);

		bool Initialize() override;
	private:
		GLADloadproc loader_;
	};
}