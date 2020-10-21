#pragma once

#include "Framework/Common/IRuntimeModule.hpp"
#include "Framework/Common/GfxStructures.hpp"
#include "Framework/Common/IDrawPass.hpp"
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
		virtual void InitializeBuffers(const Scene& scene);
		virtual void ClearBuffers();

		virtual void Clear();
		virtual void Draw();

		virtual void UseShaderProgram(intptr_t shader_program);
		virtual void SetPerFrameConstants(const DrawFrameContext& context);
		virtual void DrawBatch(const DrawBatchContext& context);
		virtual void DrawBatchDepthOnly(const DrawBatchContext& context);
		
		virtual intptr_t GenerateShadowMapArray(uint32_t count);
		virtual void BeginShadowMap(const Light& light, const intptr_t shadowmap,uint32_t layer_index);
		virtual void EndShadowMap(const intptr_t shadowmap,uint32_t layer_index);
		virtual void SetShadowMap(const intptr_t shadowmap);
		virtual void DestroyShadowMap(intptr_t& shadowmap);

#ifdef DEBUG
		virtual void DrawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color);
		virtual void DrawBox(const glm::vec3& bbMin, const glm::vec3 bbMax, const glm::vec3& color);
		virtual void DrawOverlay(const intptr_t shadowmap,uint32_t layer_index,float vp_left, float vp_top, float vp_width, float vp_height);
		virtual void ClearDebugBuffers();
#endif
	protected:
		virtual void CalculateCameraMatrix();
		virtual void CalculateLights();
		virtual void UpdateConstants();
	protected:
		static const uint32_t kFrameCount = 2;
		static const uint32_t kMaxSceneObjectCount = 65535;
		static const uint32_t kMaxTextureCount = 2048;
		const int32_t kShadowMapWidth = 512;
		const int32_t kShadowMapHeight = 512;

		uint32_t frame_index_ = 0;
		std::vector<Frame> frames_;
		std::vector<std::shared_ptr<IDrawPass>> draw_passes_;
	};
}