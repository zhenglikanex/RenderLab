#include "OpenGLGraphicsManager.hpp"

#include <iostream>
#include <fstream>

#include "Framework/Common/IApplication.hpp"
#include "Framework/Common/Engine.hpp"
#include "Framework/Common/SceneManager.hpp"
#include "Framework/Utils/FileUtils.hpp"
#include "Framework/Utils/FileHandle.hpp"

const char VS_SHADER_SOURCE_FILE[] = "Shaders/basic.vs";
const char PS_SHADER_SOURCE_FILE[] = "Shaders/basic.ps";

using namespace Aurora;

extern struct gladGLversionStruct GLVersion;

static void OutputShaderErrorMessage(uint32_t shader_id, const char* shader_filename)
{
    int log_size, i;
    char* info_log;
    std::ofstream fout;

    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_size);

    log_size++;

    info_log = new char[log_size];
    if (!info_log)
    {
        return;
    }

    glGetShaderInfoLog(shader_id, log_size, NULL, info_log);

    fout.open("shader_error.txt");
    for (i = 0; i < log_size; ++i)
    {
        fout << info_log[i];
    }

    fout.close();

    std::cerr << "Error compiling shader. Check shader_error.txt for message." << shader_filename << std::endl;

    return;
}

static void OutputLinkerErrorMessage(uint32_t program_id)
{
    int log_size, i;
    char* info_log;
    std::ofstream fout;
	
	glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_size);

	log_size++;

	info_log = new char[log_size];
	if (!info_log)
	{
		return;
	}

	glGetProgramInfoLog(program_id, log_size, NULL, info_log);
	fout.open("linker-error.txt");

	for (i = 0; i < log_size; ++i)
	{
		fout << info_log[i];
	}

	fout.close();

	std::cerr << "Error compiling linker.  Check linker-error.txt for message." << std::endl;
}

OpenGLGraphicsManager::OpenGLGraphicsManager(GLADloadproc loader)
    : loader_(loader)
{

}

bool OpenGLGraphicsManager::Initialize()
{
    if (loader_)
    {
        if (gladLoadGLLoader(loader_) != 1)
        {
            std::cout << "glad load failed !" << std::endl;
            return false;
        }
    }
    else {
        if (gladLoadGL() != 1)
        {
            std::cout << "glad load failed !" << std::endl;
            return false;
        }
    }

	std::cout << "OpenGL Version " << GLVersion.major << "." << GLVersion.minor << " loaded" << std::endl;

    if (GLAD_GL_VERSION_3_0) {
        // Set the depth buffer to be entirely cleared to 1.0 values.
        glClearDepth(1.0f);

        // Enable depth testing.
        glEnable(GL_DEPTH_TEST);

        // Set the polygon winding to front facing for the left handed system.
        glFrontFace(GL_CCW);

        // Enable back face culling.
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

		draw_frame_context_.world_matrix = glm::identity<glm::mat4>();
    }

	InitializeShader(VS_SHADER_SOURCE_FILE, PS_SHADER_SOURCE_FILE);
	InitializeBuffers();

	return true;
}

void OpenGLGraphicsManager::Finalize()
{
	for (auto& dbc : draw_batch_context_)
	{
		glDeleteVertexArrays(1, &dbc.vao);
	}

	draw_batch_context_.clear();

	
	//去除index buffer;
	for (auto i = 0; i < buffers_.size() - 1; i++)
	{
		glDisableVertexAttribArray(i);
	}

	for (auto& buf : buffers_)
	{
		glDeleteBuffers(1, &buf);
	}
	buffers_.clear();	

	glDetachShader(shader_program_, vertex_shader_);
	glDetachShader(shader_program_, fragment_shader_);

	glDeleteShader(vertex_shader_);
	glDeleteShader(fragment_shader_);

	glDeleteProgram(shader_program_);
}

void OpenGLGraphicsManager::Tick()
{

}

void OpenGLGraphicsManager::Clear()
{
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLGraphicsManager::Draw()
{
	RenderBuffers();

	glFlush();
}

bool OpenGLGraphicsManager::SetPerBatchShaderParameters()
{
	unsigned int location;

	location = glGetUniformLocation(shader_program_, "worldMatrix");
	if (location == -1)
	{
		return false;
	}
	glUniformMatrix4fv(location, 1, false,glm::value_ptr(draw_frame_context_.world_matrix));
	
	location = glGetUniformLocation(shader_program_, "viewMatrix");
	if (location == -1)
	{
		return false;
	}
	glUniformMatrix4fv(location, 1, false, glm::value_ptr(draw_frame_context_.view_matrix));

	location = glGetUniformLocation(shader_program_, "projectionMatrix");
	if (location == -1)
	{
		return false;
	}
	glUniformMatrix4fv(location, 1, false, glm::value_ptr(draw_frame_context_.projection_matrix));

	location = glGetUniformLocation(shader_program_, "lightPosition");
	if (location == -1) 
	{
		return false;
	}
	glUniform3fv(location, 1, glm::value_ptr(draw_frame_context_.light_position));
	
	location = glGetUniformLocation(shader_program_, "lightColor");
	if (location == -1)
	{
		return false;
	}
	glUniform4fv(location, 1, glm::value_ptr(draw_frame_context_.light_color));
	if (location == -1)
	{
		return false;
	}

    return true;
}

bool OpenGLGraphicsManager::SetPerBatchShaderParameters(const std::string& param_name, float* param)
{
	unsigned int location;
	location = glGetUniformLocation(shader_program_,param_name.c_str());
	if (location == -1)
	{
		return false;
	}
	glUniformMatrix4fv(location, 1, false, param);

	return true;
}

bool OpenGLGraphicsManager::InitializeBuffers()
{
	auto& scene = g_app->GetEngine()->GetSceneManager()->GetSceneForRendering();
	auto geometry_node = scene.GetFirstGeometryNode();
	while (geometry_node)
	{
		auto geometry = scene.GetGeometry(geometry_node->GetSceneObjectRef());
		auto mesh = geometry->GetMesh().lock();
		if (!mesh) return false;

		// 顶点属性数量
		auto vertex_properties_count = mesh->GetVertexPropertiesCount();

		// 顶点数量
		auto vertex_count = mesh->GetVertexCount();

		// 分配opengl vertex array object
		GLuint vao;
		glGenVertexArrays(1, &vao);

		glBindVertexArray(vao);

		GLuint buffer_id;

		for (int32_t i = 0; i < vertex_properties_count; ++i)
		{
			const SceneObjectVertexArray& v_property_array = mesh->GetVertexPropertyArray(i);
			auto v_property_array_data_size = v_property_array.GetDataSize();
			auto v_property_array_data = v_property_array.GetData();

			glGenBuffers(1, &buffer_id);

			glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
			glBufferData(GL_ARRAY_BUFFER, v_property_array_data_size, v_property_array_data, GL_STATIC_DRAW);

			glEnableVertexAttribArray(i);

			glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
			switch (v_property_array.GetDataType())
			{
			case VertexDataType::kVertexDataTypeFloat1:
				glVertexAttribPointer(i, 1, GL_FLOAT, false, 0, 0);
				break;
			case VertexDataType::kVertexDataTypeFloat2:
				glVertexAttribPointer(i, 2, GL_FLOAT, false, 0, 0);
				break;
			case VertexDataType::kVertexDataTypeFloat3:
				glVertexAttribPointer(i, 3, GL_FLOAT, false, 0, 0);
				break;
			case VertexDataType::kVertexDataTypeFloat4:
				glVertexAttribPointer(i, 4, GL_FLOAT, false, 0, 0);
				break;
			case VertexDataType::kVertexDataTypeDouble1:
				glVertexAttribPointer(i, 1, GL_DOUBLE, false, 0, 0);
				break;
			case VertexDataType::kVertexDataTypeDouble2:
				glVertexAttribPointer(i, 2, GL_DOUBLE, false, 0, 0);
				break;
			case VertexDataType::kVertexDataTypeDouble3:
				glVertexAttribPointer(i, 3, GL_DOUBLE, false, 0, 0);
				break;
			case VertexDataType::kVertexDataTypeDouble4:
				glVertexAttribPointer(i, 4, GL_DOUBLE, false, 0, 0);
				break;
			default:
				assert(0);
				break;
			}

			buffers_.push_back(buffer_id);
		}

		glGenBuffers(1, &buffer_id);

		const SceneObjectIndexArray& index_array = mesh->GetIndexArray(0);
		auto index_array_size = index_array.GetDataSize();
		auto index_array_data = index_array.GetData();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_array_size, index_array_data, GL_STATIC_DRAW);

		GLsizei index_count = static_cast<GLsizei>(index_array.GetIndexCount());
		GLenum mode;
		switch (mesh->GetPrimitiveType())
		{
		case PrimitiveType::kPrimitiveTypePointList:
			mode = GL_POINTS;
			break;
		case PrimitiveType::kPrimitiveTypeLineList:
			mode = GL_LINES;
			break;
		case PrimitiveType::kPrimitiveTypeLineStrip:
			mode = GL_LINE_STRIP;
			break;
		case PrimitiveType::kPrimitiveTypeTriList:
			mode = GL_TRIANGLES;
			break;
		case PrimitiveType::kPrimitiveTypeTriStrip:
			mode = GL_TRIANGLE_STRIP;
			break;
		case PrimitiveType::kPrimitiveTypeTriFan:
			mode = GL_TRIANGLE_FAN;
			break;
		default:
			continue;
		}

		GLenum type;
		switch (index_array.GetIndexType())
		{
		case IndexDataType::kIndexDataTypeInt8:
			type = GL_UNSIGNED_BYTE;
			break;
		case IndexDataType::kIndexDataTypeInt16:
			type = GL_UNSIGNED_SHORT;
			break;
		case IndexDataType::kIndexDataTypeInt32:
			type = GL_UNSIGNED_INT;
			break;
		default:
			std::cerr << "Error: Unsupported Index Type " << index_array << std::endl;
			continue;
		}

		buffers_.push_back(buffer_id);

		DrawBathContext& dbc = *(new DrawBathContext);
		dbc.vao = vao;
		dbc.mode = mode;
		dbc.type = type;
		dbc.count = index_count;
		dbc.transform = geometry_node->GetCalculatedTransform();

		draw_batch_context_.push_back(std::move(dbc));

		geometry_node = scene.GetNextGeometryNode();
	}
    return true;
}

void OpenGLGraphicsManager::RenderBuffers()
{
	static float rotateAngle = 0.0f;

	// Update world matrix to rotate the model
	rotateAngle += PI / 120;
	//glm::mat4 rotationMatrixY = glm::identity<glm::mat4>();
	glm::mat4 rotationMatrixZ = glm::identity<glm::mat4>();
	//rotationMatrixY = glm::rotate(rotationMatrixY, rotateAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	rotationMatrixZ = glm::rotate(rotationMatrixZ, rotateAngle, glm::vec3(0.0f, 0.0f, 1.0f));

	//world_matrix_ = rotationMatrixY * rotationMatrixZ;
	draw_frame_context_.world_matrix = rotationMatrixZ;

	CalculateCameraMatrix();
	CalculateLights();

	SetPerBatchShaderParameters();

	for (auto& dbc : draw_batch_context_)
	{
		glUseProgram(shader_program_);
		glm::mat4 model_matrix = *dbc.transform;
		SetPerBatchShaderParameters("objectLocalMatrix", glm::value_ptr(model_matrix));		

		glBindVertexArray(dbc.vao);

		glDrawElements(dbc.mode, dbc.count, dbc.type, 0);
	}
}

void OpenGLGraphicsManager::CalculateCameraMatrix()
{
	auto& scene = g_app->GetEngine()->GetSceneManager()->GetSceneForRendering();
	auto camera_node = scene.GetFirstCameraNode();
	if (camera_node)
	{
		draw_frame_context_.view_matrix = glm::inverse(*camera_node->GetCalculatedTransform());
	}
	else 
	{
		glm::vec3 position = { 0,0,5 }, look_at = { 0,0,0 }, up = { 0,1,0 };
		draw_frame_context_.view_matrix = glm::lookAtLH(position, look_at, up);
	}
	
	auto camera = scene.GetCamera(camera_node->GetSceneObjectRef());

	float fov = std::dynamic_pointer_cast<SceneObjectPerspectiveCamera>(camera)->GetFov();
	const GfxConfiguration& conf = g_app->GetConfiguration();
	draw_frame_context_.projection_matrix = glm::perspectiveFovRH(fov, (float)conf.screen_width, (float)conf.screen_height,camera->GetNearClipDistance(),camera->GetFarClipDistance());
}

bool OpenGLGraphicsManager::InitializeShader(const char* vs_filename, const char* fs_filename)
{
	std::string vertex_shader_buffer;
	std::string fragment_shader_buffer;
	int status;
	
	vertex_shader_buffer = FileUtils::GetInstance()->OpenFileAndReadString(vs_filename);
	if (vertex_shader_buffer.empty())
	{
		return false;
	}

	fragment_shader_buffer = FileUtils::GetInstance()->OpenFileAndReadString(fs_filename);
	if (fragment_shader_buffer.empty())
	{
		return false;
	}
	
	vertex_shader_ = glCreateShader(GL_VERTEX_SHADER);
	fragment_shader_ = glCreateShader(GL_FRAGMENT_SHADER);

	auto _vs_c_str = vertex_shader_buffer.c_str();
	glShaderSource(vertex_shader_, 1, &_vs_c_str, NULL);
	auto _f_c_str = fragment_shader_buffer.c_str();
	glShaderSource(fragment_shader_, 1, &_f_c_str, NULL);

	glCompileShader(vertex_shader_);
	glCompileShader(fragment_shader_);
	
	glGetShaderiv(vertex_shader_, GL_COMPILE_STATUS, &status);
	if (status != 1)
	{
		OutputShaderErrorMessage(vertex_shader_, vs_filename);
		return false;
	}

	glGetShaderiv(fragment_shader_, GL_COMPILE_STATUS, &status);
	if (status != 1)
	{
		OutputShaderErrorMessage(fragment_shader_, fs_filename);
		return false;
	}

	shader_program_ = glCreateProgram();

	glAttachShader(shader_program_, vertex_shader_);
	glAttachShader(shader_program_, fragment_shader_);

	glBindAttribLocation(shader_program_, 0, "inputPosition");
	glBindAttribLocation(shader_program_, 1, "inputNormal");

	glLinkProgram(shader_program_);

	glGetProgramiv(shader_program_, GL_LINK_STATUS, &status);
	if (status != 1)
	{
		OutputLinkerErrorMessage(shader_program_);
		return false;
	}

    return true;
}

void OpenGLGraphicsManager::CalculateLights()
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
		draw_frame_context_.light_position = { 10.0f,10.0f,-10.0f };
		draw_frame_context_.light_color = { 1.0f,1.0f,1.0f,1.0f };
	}
}