#include "GraphicsManager.hpp"

#include <iostream>

#include "Framework/Common/IApplication.hpp"
#include "Framework/Common/Engine.hpp"
#include "Framework/Common/SceneManager.hpp"

using namespace Aurora;

bool GraphicsManager::Initialize()
{
	return true;
}
void GraphicsManager::Finalize()
{

}

void GraphicsManager::Tick()
{
	CalculateCameraMatrix();
	CalculateLights();
}

void GraphicsManager::Clear()
{

}

void GraphicsManager::Draw()
{

}

void GraphicsManager::WorldRotateX(float radians)
{
	
}

void GraphicsManager::WorldRotateY(float radians)
{

}

bool GraphicsManager::SetPerBatchShaderParameters()
{
	std::cout << "[RHI] bool GraphicsManager::SetPerBatchShaderParameters()" << std::endl;
	return true;
}

bool GraphicsManager::SetPerBatchShaderParameters(const std::string& param_name, const glm::mat4& param)
{
	std::cout << "[RHI] GraphicsManager::SetPerBatchShaderParameters(const std::string& param_name, const glm::mat4& param)" << std::endl;
	return true;
}

bool GraphicsManager::SetPerBatchShaderParameters(const std::string& param_name, const glm::vec3& param)
{
	std::cout << "[RHI] bool GraphicsManager::SetPerBatchShaderParameters(const std::string& param_name, const glm::vec3& param)" << std::endl;
	return true;
}

bool GraphicsManager::SetPerBatchShaderParameters(const std::string& param_name, const float param)
{
	std::cout << "[RHI] bool GraphicsManager::SetPerBatchShaderParameters(const std::string& param_name, const float param)" << std::endl;
	return true;
}

bool GraphicsManager::SetPerBatchShaderParameters(const std::string& param_name, const int texture_index)
{
	std::cout << "[RHI] bool GraphicsManager::SetPerBatchShaderParameters(const std::string& param_name, const int texture_index)" << std::endl;
	return true;
}

void GraphicsManager::InitConstants()
{
	draw_frame_context_.world_matrix = glm::identity<glm::mat4>();
}

bool GraphicsManager::InitializeShader(const char* vs_filename, const char* fs_filename)
{
	std::cout << "[RHI] bool GraphicsManager::InitializeShader(const char* vs_filename, const char* fs_filename)" << std::endl;
	return true;
}

void GraphicsManager::InitializeBuffers()
{
	std::cout << "[RHI] void GraphicsManager::InitializeBuffers()" << std::endl;
}

void GraphicsManager::CalculateCameraMatrix()
{
	auto& scene = g_app->GetEngine()->GetSceneManager()->GetSceneForRendering();
	auto camera_node = scene.GetFirstCameraNode();
	if (camera_node)
	{
		draw_frame_context_.view_matrix = glm::inverse(*camera_node->GetCalculatedTransform());
	}
	else
	{
		glm::vec3 position = { 0,-5,0 }, look_at = { 0,0,0 }, up = { 0,0,1 };
		draw_frame_context_.view_matrix = glm::lookAtLH(position, look_at, up);
	}

	float fov = PI / 2.0f;
	float near_clip_distance = 1.0f;
	float far_clip_distance = 100.0f;

	if (camera_node)
	{
		auto camera = scene.GetCamera(camera_node->GetSceneObjectRef());
		fov = std::dynamic_pointer_cast<SceneObjectPerspectiveCamera>(camera)->GetFov();
		near_clip_distance = 1.0f;
		far_clip_distance = 100.0f;
	}

	const GfxConfiguration& conf = g_app->GetConfiguration();
	draw_frame_context_.projection_matrix = glm::perspectiveFovRH(fov, (float)conf.screen_width, (float)conf.screen_height, near_clip_distance, far_clip_distance);
}

void GraphicsManager::CalculateLights()
{
	auto& scene = g_app->GetEngine()->GetSceneManager()->GetSceneForRendering();
	auto light_node = scene.GetFirstLightNode();
	if (light_node)
	{
		draw_frame_context_.light_position = { 0.0f,0.0f,0.0f };
		draw_frame_context_.light_position = (*light_node->GetCalculatedTransform()) * glm::vec4(0.0, 0.0, 0.0, 1.0f);

		auto light = scene.GetLight(light_node->GetSceneObjectRef());
		if (light)
		{
			draw_frame_context_.light_color = light->GetColor().Value;
		}
	}
	else
	{
		draw_frame_context_.light_position = { -1.0f, -5.0f, 0.0f };
		draw_frame_context_.light_color = { 1.0f,1.0f,1.0f,1.0f };
	}
}

void GraphicsManager::RenderBuffers()
{
	std::cout << "[RHI] void GraphicsManager::RenderBuffers()" << std::endl;
}