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
		virtual bool InitializeShaders();
		virtual void ClearShaders();
		virtual void InitializeBuffers(const Scene& scene);
		virtual void ClearBuffers();

		virtual void Clear();
		virtual void Draw();

		void WorldRotateX(float radians);
		void WorldRotateY(float radians);

#ifdef DEBUG
		virtual void DrawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color);
		virtual void DrawBox(const glm::vec3& bbMin, const glm::vec3 bbMax, const glm::vec3& color);
		virtual void ClearDebugBuffers();
#endif
	protected:
		virtual void CalculateCameraMatrix();
		virtual void CalculateLights();
		virtual void RenderBuffers();
	protected:
		struct Light
		{
			glm::vec4 light_position;
			glm::vec4 light_color;
			glm::vec4 light_direction;
			glm::vec2 light_size;
			float light_intensity;
			AttenCurveType light_dist_atten_curve_type;
			float light_dist_atten_curve_params[5];
			AttenCurveType light_angle_atten_curve_type;
			float light_angle_atten_curve_params[5];

			Light()
			{
				light_position = { 0.0f,0.0f,0.0f,1.0f };
				light_size = { 0.0f,0.0f };
				light_color = { 1.0f,1.0f,1.0f,1.0f };
				light_direction = { 0.0f,0.0f,-1.0f,0.0f };
				light_intensity = 0.5f;
				light_dist_atten_curve_type = AttenCurveType::kNone;
				light_angle_atten_curve_type = AttenCurveType::kNone;
			}
		};

		struct DrawFrameContext {
			glm::mat4 world_matrix;
			glm::mat4 view_matrix;
			glm::mat4 projection_matrix;
			glm::vec3 ambient_color;
			std::vector<Light> lights;
		};
		DrawFrameContext draw_frame_context_;
	};
}