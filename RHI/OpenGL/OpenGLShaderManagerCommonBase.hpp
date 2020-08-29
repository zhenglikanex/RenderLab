#pragma once

#include <string>

#include "Framework/Common/IShaderManager.hpp"

#include "glad/glad.h"

namespace Aurora
{
	class OpenGLShaderManagerCommonBase : public IShaderManager
	{
	public:
		OpenGLShaderManagerCommonBase() = default;
		~OpenGLShaderManagerCommonBase() = default;

		bool Initialize() override;
		void Finalize() override;

		void Tick() override;

		bool InitializeShaders() override;
		void ClearShaders() override;

		void* GetDefaultShaderProgram() override;
#ifdef DEBUG
		void* GetDebugShaderProgram() override;
#endif
	private:
		GLuint vertex_shader_;
		GLuint fragment_shader_;
		GLuint shader_program_;
#ifdef DEBUG
		GLuint debug_vertex_shader_;
		GLuint debug_fragment_shader_;
		GLuint debug_shader_program_;
#endif
	};
}