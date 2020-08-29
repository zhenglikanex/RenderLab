#pragma once

#include <string>

#include "Framework/Common/ShaderManager.hpp"

#include "glad/glad.h"

namespace Aurora
{
	class OpenGLShaderManagerCommonBase : public ShaderManager
	{
	public:
		OpenGLShaderManagerCommonBase() = default;
		~OpenGLShaderManagerCommonBase() = default;

		bool Initialize() override;
		void Finalize() override;

		void Tick() override;

		bool InitializeShaders() override;
		void ClearShaders() override;
	};
}