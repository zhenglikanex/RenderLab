#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include "Framework/Common/GraphicsManager.hpp"
#include "Framework/Common/GeomMath.hpp"
#include "Framework/Common/SceneObject.hpp"
#include "Framework/Common/SceneNode.hpp"

#include "glad/glad.h"

namespace Aurora
{
	class OpenGLGraphicsManager : public GraphicsManager
	{
	public:
		OpenGLGraphicsManager(GLADloadproc loader);

		bool Initialize() override;
		void Finalize() override;

		void InitializeBuffers(const Scene& scene) override;
		void ClearBuffers() override;
		bool InitializeShader(const char* vs_filename, const char* fs_filename) override;

		void Clear() override;
		void Draw() override;

#ifdef DEBUG
		void DrawLien(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color) override;
		void DrawBox(const glm::vec3& bbMin, const glm::vec3 bbMax, const glm::vec3& color) override;
		void ClearDebugBuffers() override;
#endif

	protected:
		bool SetPerBatchShaderParameters(GLuint shader,const std::string& param_name, const glm::mat4& param);
		bool SetPerBatchShaderParameters(GLuint shader, const std::string& param_name, const glm::vec3& param);
		bool SetPerBatchShaderParameters(GLuint shader, const std::string& param_name, const float param);
		bool SetPerBatchShaderParameters(GLuint shader, const std::string& param_name, const int param);
		bool SetPerBatchShaderParameters(GLuint shader);

		void RenderBuffers() override;
	private:
		GLuint vertex_shader_;
		GLuint fragment_shader_;
		GLuint shader_program_;
		std::unordered_map<std::string, GLint> texture_index_;

		struct DrawBathContext {
			GLuint vao;
			GLenum mode;
			GLenum type;
			GLsizei count;
			std::shared_ptr<SceneGeometryNode> node;
			std::shared_ptr<SceneObjectMaterial> material;
		};
		std::vector<DrawBathContext> draw_batch_context_;
		std::vector<GLuint> buffers_;
		std::vector<GLuint> textures_;

		struct DebugDrawBathContext {
			GLuint vao;
			GLenum mode;
			GLsizei count;
			glm::vec3 color;
		};

#ifdef DEBUG
		GLuint debug_vertex_shader_;
		GLuint debug_fragment_shader_;
		GLuint debug_shader_progam_;
		std::vector<DebugDrawBathContext> debug_draw_batch_contenxt_;
		std::vector<GLuint> debug_buffers_;
#endif
		GLADloadproc loader_;
	};
}