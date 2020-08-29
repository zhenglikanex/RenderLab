#pragma once

#include "RHI/OpenGL/OpenGLGraphicsManagerCommonBase.hpp"

namespace Aurora
{
	class OpenGLESGraphicsManager : public OpenGLGraphicsManagerCommonBase
	{
	public:
		OpenGLESGraphicsManager(GLADloadproc loader);

		bool Initialize() override;
	private:
		GLADloadproc loader_;
	};
}