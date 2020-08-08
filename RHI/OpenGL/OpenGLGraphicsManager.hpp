#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include "Framework/Common/GraphicsManager.hpp"
#include "Framework/Common/GeomMath.hpp"
#include "Framework/Common/SceneObject.hpp"

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
		bool SetPerBatchShaderParameters(const std::string& param_name, const glm::mat4& param);
		bool SetPerBatchShaderParameters(const std::string& param_name, const glm::vec3& param);
		bool SetPerBatchShaderParameters(const std::string& param_name, const float param);
		bool SetPerBatchShaderParameters(const std::string& param_name, const GLint texture_index);
		bool SetPerBatchShaderParameters();

		bool InitializeBuffers();
		void RenderBuffers();
		void CalculateCameraMatrix();
		void CalculateLights();
		bool InitializeShader(const char* vs_filename, const char* fs_filename);
	private:
		uint32_t vertex_shader_;
		uint32_t fragment_shader_;
		uint32_t shader_program_;
		std::unordered_map<std::string, GLint> texture_index_;

		struct DrawFrameContext {
			glm::mat4 world_matrix;
			glm::mat4 view_matrix;
			glm::mat4 projection_matrix;
			glm::vec3 light_position;
			glm::vec4 light_color;
		};

		struct DrawBathContext {
			GLuint vao;
			GLenum mode;
			GLenum type;
			GLsizei count;
			std::shared_ptr<glm::mat4> transform;
			std::shared_ptr<SceneObjectMaterial> material;
		};

		DrawFrameContext draw_frame_context_;
		std::vector<DrawBathContext> draw_batch_context_;
		std::vector<uint32_t> buffers_;
		std::vector<GLuint> textures_;
		GLADloadproc loader_;
	};
}