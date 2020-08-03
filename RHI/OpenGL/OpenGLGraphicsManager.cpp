#include "OpenGLGraphicsManager.hpp"

#include <iostream>
#include <fstream>

#include "Framework/Common/IApplication.hpp"
#include "Framework/Common/Engine.hpp"
#include "Framework/Common/SceneManager.hpp"
#include "Framework/Utils/FileUtils.hpp"
#include "Framework/Utils/FileHandle.hpp"

const char VS_SHADER_SOURCE_FILE[] = "Shaders/color.vs";
const char PS_SHADER_SOURCE_FILE[] = "Shaders/color.ps";

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
        glFrontFace(GL_CW);

        // Enable back face culling.
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

		world_matrix_ = glm::identity<glm::mat4>();

		float fov = PI / 4.0f;
		const GfxConfiguration& conf = g_app->GetConfiguration();
		
		projection_matrix_ = glm::perspectiveFovLH(fov, (float)conf.screen_width, (float)conf.screen_height, screen_near, screen_depth);
    }

	InitializeShader(VS_SHADER_SOURCE_FILE, PS_SHADER_SOURCE_FILE);
	InitializeBuffers();

	return true;
}

void OpenGLGraphicsManager::Finalize()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	

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
	CalculateCameraPosition();

	glUseProgram(shader_program_);
	SetShaderParameters(glm::value_ptr(world_matrix_), glm::value_ptr(view_matrix_), glm::value_ptr(projection_matrix_));

	RenderBuffers();

	glFlush();
}

bool OpenGLGraphicsManager::SetShaderParameters(float* world_matrix, float* view_matrix, float* projection_matrix)
{
	unsigned int location;

	location = glGetUniformLocation(shader_program_, "worldMatrix");
	if (location == -1)
	{
		return false;
	}
	glUniformMatrix4fv(location, 1, false, world_matrix);
	
	location = glGetUniformLocation(shader_program_, "viewMatrix");
	if (location == -1)
	{
		return false;
	}
	glUniformMatrix4fv(location, 1, false, view_matrix);

	location = glGetUniformLocation(shader_program_, "projectionMatrix");
	if (location == -1)
	{
		return false;
	}
	glUniformMatrix4fv(location, 1, false, projection_matrix);

    return true;
}

bool OpenGLGraphicsManager::InitializeBuffers()
{
	auto& scene = g_app->GetEngine()->GetSceneManager()->GetSceneForRendering();
	auto geometry = scene.GetFirstGeometry();
	while (geometry)
	{
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

			buffers_[v_property_array.GetAttributeName()] = buffer_id;
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

		buffers_["index"] = buffer_id;

		DrawBathContext& dbc = *(new DrawBathContext);
		dbc.vao = vao;
		dbc.mode = mode;
		dbc.type = type;
		dbc.count = index_count;

		VAO_.push_back(std::move(dbc));

		geometry = scene.GetNextGeometry();
	}
    return true;
}

void OpenGLGraphicsManager::RenderBuffers()
{
	for (auto& dbc : VAO_)
	{
		glBindVertexArray(dbc.vao);

		glDrawElements(dbc.mode, dbc.count, dbc.type, 0);
	}
}

void OpenGLGraphicsManager::CalculateCameraPosition()
{
	glm::vec3 up, position, look_at;
	float yaw, pitch, roll;
	glm::mat4 rotation_matrix = glm::identity<glm::mat4>();

	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	position.x = position_x_;
	position.y = position_y_;
	position.z = position_z_;

	look_at.x = 0.0f;
	look_at.y = 0.0f;
	look_at.z = 1.0f;

	pitch = rotation_x_ * RAD;
	yaw = rotation_x_ * RAD;
	roll = rotation_x_ * RAD;

	rotation_matrix = glm::rotate(rotation_matrix, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
	rotation_matrix = glm::rotate(rotation_matrix, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
	rotation_matrix = glm::rotate(rotation_matrix, roll, glm::vec3(0.0f, 0.0f, 1.0f));

	look_at = rotation_matrix * glm::vec4(look_at,0.0f);
	up = rotation_matrix * glm::vec4(up, 0.0f);

	look_at.x = position.x + look_at.x;
	look_at.y = position.y + look_at.y;
	look_at.z = position.z + look_at.z;

	view_matrix_ = glm::lookAtLH(position, look_at, up);
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
	glBindAttribLocation(shader_program_, 1, "inputColor");

	glLinkProgram(shader_program_);

	glGetProgramiv(shader_program_, GL_LINK_STATUS, &status);
	if (status != 1)
	{
		OutputLinkerErrorMessage(shader_program_);
		return false;
	}

    return true;
}