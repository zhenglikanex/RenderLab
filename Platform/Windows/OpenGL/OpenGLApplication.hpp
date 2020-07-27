#pragma once

#include "Platform/Windows/WindowsApplication.hpp"

namespace Aurora
{
	class OpenGLApplication : public WindowsApplication
	{
	public:
		OpenGLApplication(GfxConfiguration& cfg);

		bool Initialize() override;
		void Finalize() override;
		void Tick() override;
	private:
		HGLRC render_context_;
	};
}