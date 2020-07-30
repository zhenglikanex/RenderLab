#pragma once

#include <vector>
#include <unordered_map>

#include "Framework/Common/GraphicsManager.hpp"
#include "Framework/Common/GeomMath.hpp"

#include "glad/glad.h"

namespace Aurora
{
	class OpenGLGraphicsManager : public GraphicsManager
	{
	public:
		OpenGLGraphicsManager(GLADloadproc loader);

		bool Initialize() override;
		void Finalize() override;

		void Tick() override;

		void Clear() override;
		void Draw() override;
	private:
		bool SetShaderParameters(float* world_matrix, float* view_matrix, float projection_matrix);

		bool InitializeBuffers();
		void RenderBuffers();
		void CalculateCameraPosition();
		bool InitializeShader(const char* vs_filename, const char* fs_filename);
	private:
		uint32_t vertex_shader_;
		uint32_t fragment_shader_;
		uint32_t shader_program_;

		const bool VSYNC_ENABLED = true;
		const float screen_depth = 1000.0f;
		const float screen_near = 0.1f;

		struct DrawBathContext {
			GLuint vao;
			GLenum mode;
			GLenum type;
			GLsizei count;
		};

		std::vector<DrawBathContext> VAO_;
		std::unordered_map<std::string, uint32_t> buffers_;
		
		float position_x_ = 0, position_y_ = 0, position_z_ = -10;
		float rotation_x_ = 0, rotaion_y_ = 0, rotation_z = 0;

		glm::mat4 world_matrix_;
		glm::mat4 view_matrix_;
		glm::mat4 projection_matrix_;

		GLADloadproc loader_;
	};
}