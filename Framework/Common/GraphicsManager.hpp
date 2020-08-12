#pragma once

#include "Framework/Common/IRuntimeModule.hpp"
#include "Framework/Common/GeomMath.hpp"
#include "Framework/Common/SceneObject.hpp"
#include "Framework/Common/Scene.hpp"

namespace Aurora
{
	class GraphicsManager : public IRuntimeModule
	{
	public:
		virtual ~GraphicsManager() = 0 {};

		virtual bool Initialize();
		virtual void Finalize();

		virtual void Tick();

		virtual void InitConstants();
		virtual bool InitializeShader(const char* vs_filename, const char* fs_filename);
		virtual void ClearShader();
		virtual void InitializeBuffers(const Scene& scene);
		virtual void ClearBuffers();

		virtual void Clear();
		virtual void Draw();

		void WorldRotateX(float radians);
		void WorldRotateY(float radians);

#ifdef DEBUG
		virtual void DrawLien(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color);
		virtual void DrawBox(const glm::vec3& bbMin, const glm::vec3 bbMax, const glm::vec3& color);
		virtual void ClearDebugBuffers();
#endif
	protected:
		virtual bool SetPerBatchShaderParameters();
		virtual bool SetPerBatchShaderParameters(const std::string& param_name, const glm::mat4& param);
		virtual bool SetPerBatchShaderParameters(const std::string& param_name, const glm::vec3& param);
		virtual bool SetPerBatchShaderParameters(const std::string& param_name, const float param);
		virtual bool SetPerBatchShaderParameters(const std::string& param_name, const int texture_index);

		virtual void CalculateCameraMatrix();
		virtual void CalculateLights();
		virtual void RenderBuffers();
	protected:
		struct DrawFrameContext {
			glm::mat4 world_matrix;
			glm::mat4 view_matrix;
			glm::mat4 projection_matrix;
			glm::vec3 light_position;
			glm::vec4 light_color;
		};
		DrawFrameContext draw_frame_context_;
	};
}