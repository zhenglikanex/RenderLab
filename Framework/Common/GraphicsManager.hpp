#pragma once

#include "Framework/Common/IRuntimeModule.hpp"
#include "Framework/Common/GeomMath.hpp"
#include "Framework/Common/SceneObject.hpp"

namespace Aurora
{
	class GraphicsManager : public IRuntimeModule
	{
	public:
		virtual ~GraphicsManager() = 0 {};

		virtual bool Initialize();
		virtual void Finalize();

		virtual void Tick();

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
		bool SetPerBatchShaderParameters();
		bool SetPerBatchShaderParameters(const std::string& param_name, const glm::mat4& param);
		bool SetPerBatchShaderParameters(const std::string& param_name, const glm::vec3& param);
		bool SetPerBatchShaderParameters(const std::string& param_name, const float param);
		bool SetPerBatchShaderParameters(const std::string& param_name, const int texture_index);

		void InitConstants();
		bool InitializeShader(const char* vs_filename, const char* fs_filename);
		void InitializeBuffers();
		void CalculateCameraMatrix();
		void CalculateLights();
		void RenderBuffers();
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