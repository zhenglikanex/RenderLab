#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include "Framework/Common/GraphicsManager.hpp"
#include "Framework/Common/GeomMath.hpp"
#include "Framework/Common/SceneObject.hpp"
#include "Framework/Common/SceneNode.hpp"

#ifdef ANDROID
#else
#include "glad/glad.h"
#endif

namespace Aurora
{
	class OpenGLGraphicsManagerCommonBase : public GraphicsManager
	{
	public:
		void Finalize() override;

		void InitializeBuffers(const Scene& scene) override;
		void ClearBuffers() override;

		void Clear() override;
		void Draw() override;

		void UseShaderProgram(intptr_t shader_program) override;
		void SetPerFrameConstants(const DrawFrameContext& context) override;
		void DrawBatch(const DrawBatchContext& context) override;

		void DrawBatchDepthOnly(const DrawBatchContext& context) override;

		intptr_t GenerateShadowMap(const uint32_t width, const uint32_t height) override;
		intptr_t GenerateCubeShadowMapArray(const uint32_t width, const uint32_t height, const uint32_t count) override;
		intptr_t GenerateShadowMapArray(const uint32_t width, const uint32_t height, const uint32_t count) override;
		void BeginShadowMap(const Light& light, const intptr_t shadowmap, const uint32_t width, const uint32_t height, const uint32_t layer_index) override;
		void EndShadowMap(const intptr_t shadowmap, const uint32_t layer_index) override;
		void SetShadowMap(const intptr_t shadowmap) override;
		void SetShadowMaps(const Frame& frame) override;
		void SetGlobalShadowMap(const intptr_t shadowmap) override;
		void DestroyShadowMap(intptr_t& shadowmap) override;
#ifdef DEBUG
		void DrawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color) override;
		void DrawBox(const glm::vec3& bbMin, const glm::vec3 bbMax, const glm::vec3& color) override;
		void DrawTextureOverlay(const intptr_t shadowmap, uint32_t layer_index, float vp_left, float vp_top, float vp_width, float vp_height) override;
		void DrawCubeMapOverlay(const intptr_t cuebemap, uint32_t layer_index, float vp_left, float vp_top, float vp_width, float vp_height) override;
		void ClearDebugBuffers() override;
#endif

	protected:
		bool SetShaderParameters(const std::string& param_name, const glm::mat4& param);
		bool SetShaderParameters(const std::string& param_name, const glm::mat4* param, const int32_t count);
		bool SetShaderParameters(const std::string& param_name, const glm::vec3& param);
		bool SetShaderParameters(const std::string& param_name, const float param);
		bool SetShaderParameters(const std::string& param_name, const int32_t param);
		bool SetShaderParameters(const std::string& param_name, const uint32_t param);
		bool SetShaderParameters(const std::string& param_name, const bool param);
		bool SetPerFrameShaderParameters(const DrawFrameContext& context);

	protected:
		std::unordered_map<std::string, GLint> texture_index_;
		GLuint shadowmap_framebuffer_name_;
		GLuint current_shader_;
		GLuint ubo_buffer_ = 0;

		struct OpenGLDrawBatchContext : public DrawBatchContext {
			GLuint vao;
			GLenum mode;
			GLenum type;
			GLsizei count;
		};

		std::vector<GLuint> buffers_;
		std::vector<GLuint> textures_;

#ifdef DEBUG
		struct DebugDrawBathContext {
			GLuint vao;
			GLenum mode;
			GLsizei count;
			glm::vec3 color;
		};

		GLuint debug_vertex_shader_;
		GLuint debug_fragment_shader_;
		GLuint debug_shader_program_;
		std::vector<DebugDrawBathContext> debug_draw_batch_contenxt_;
		std::vector<GLuint> debug_buffers_;
#endif
		GLADloadproc loader_;
	};
}