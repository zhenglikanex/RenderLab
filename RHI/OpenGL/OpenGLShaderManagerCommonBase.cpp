#include "OpenGLShaderManagerCommonBase.hpp"

#include <iostream>
#include <fstream>
#include <map>

#include "Framework/Utils/FileUtils.hpp"
#include "Framework/Utils/FileHandle.hpp"

const char COMMON_SHADER_SOURCE_FILE[] = "Shaders/cbuffer.glsl";

#ifdef DEBUG
const char DEBUG_VS_SHADER_SOURCE_FILE[] = "Shaders/debug_vs.glsl";
const char DEBUG_PS_SHADER_SOURCE_FILE[] = "Shaders/debug_ps.glsl";
#endif
const char VS_SHADOWMAP_SOURCE_FILE[] = "Shaders/shadowmap_vs.glsl";
const char PS_SHADOWMAP_SOURCE_FILE[] = "Shaders/shadowmap_ps.glsl";

const char VS_OMNI_SHADOWMAP_SOURCE_FILE[] = "Shaders/shadowmap_omni_vs.glsl";
const char GS_OMNI_SHADOWMAP_SOURCE_FILE[] = "Shaders/shadowmap_omni_gs.glsl";
const char PS_OMNI_SHADOWMAP_SOURCE_FILE[] = "Shaders/shadowmap_omni_ps.glsl";

const char VS_SHADER_SOURCE_FILE[] = "Shaders/basic_vs.glsl";
const char PS_SHADER_SOURCE_FILE[] = "Shaders/basic_ps.glsl";

const char VS_PASSTHROUGH_SOURCE_FILE[] = "Shaders/passthrough_vs.glsl";
const char PS_PASSTHROUGH_SOURCE_FILE[] = "Shaders/depthtexture_ps.glsl";

const char VS_PASSTHROUGH_CUEBEMAP_SOURCE_FILE[] = "Shaders/passthrough_cuebe_vs.glsl";
const char PS_SIMPLER_CUBEMAP_SOURCE_FILE[] = "Shaders/depthcubemap_ps.glsl";

using namespace Aurora;

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

static bool LoadShaderFromFile(const char* filename, const GLenum shader_type, GLuint& shader)
{
	std::string common_shader_buffer;
	std::string shader_buffer;
	int status;
	common_shader_buffer = FileUtils::GetInstance()->OpenFileAndReadString(COMMON_SHADER_SOURCE_FILE);
	if (common_shader_buffer.empty())
	{
		return false;
	}

	shader_buffer = FileUtils::GetInstance()->OpenFileAndReadString(filename);
	if (shader_buffer.empty())
	{
		return false;
	}

	shader_buffer = common_shader_buffer + shader_buffer;

	shader = glCreateShader(shader_type);
	const char* str = shader_buffer.c_str();
	glShaderSource(shader, 1, &str, NULL);

	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status != 1)
	{
		OutputShaderErrorMessage(shader, filename);
		return false;
	}

	return true;
}

typedef std::vector<std::pair<GLenum, std::string>> ShaderSourceList;

static bool LoadShaderProgram(const ShaderSourceList& source, GLuint& shader_program)
{
	int status;

	shader_program = glCreateProgram();
	for (auto it = source.cbegin(); it != source.end(); ++it)
	{
		GLuint shader;
		status = LoadShaderFromFile(it->second.c_str(), it->first, shader);
		if (!status)
		{
			return false;
		}

		glAttachShader(shader_program, shader);
		glDeleteShader(shader);
	}

	glLinkProgram(shader_program);
	
	glGetProgramiv(shader_program, GL_LINK_STATUS, &status);
	if (status != 1)
	{
		OutputLinkerErrorMessage(shader_program);
		return false;
	}

	return true;
}

static bool LoadShaderFromFile(const char* vs_filename, const char* fs_filename,GLuint& shader_program)
{
	std::string common_shader_buffer;
	std::string vertex_shader_buffer;
	std::string fragment_shader_buffer;
	int status;

	common_shader_buffer = FileUtils::GetInstance()->OpenFileAndReadString(COMMON_SHADER_SOURCE_FILE);
	if (common_shader_buffer.empty())
	{
		return false;
	}

	vertex_shader_buffer = FileUtils::GetInstance()->OpenFileAndReadString(vs_filename);
	if (vertex_shader_buffer.empty())
	{
		return false;
	}

	vertex_shader_buffer = common_shader_buffer + vertex_shader_buffer;

	fragment_shader_buffer = FileUtils::GetInstance()->OpenFileAndReadString(fs_filename);
	if (fragment_shader_buffer.empty())
	{
		return false;
	}

	fragment_shader_buffer = common_shader_buffer + fragment_shader_buffer;

	auto vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	auto fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	auto _vs_c_str = vertex_shader_buffer.c_str();
	glShaderSource(vertex_shader, 1, &_vs_c_str, NULL);
	auto _f_c_str = fragment_shader_buffer.c_str();
	glShaderSource(fragment_shader, 1, &_f_c_str, NULL);

	glCompileShader(vertex_shader);
	glCompileShader(fragment_shader);

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
	if (status != 1)
	{
		OutputShaderErrorMessage(vertex_shader, vs_filename);
		return false;
	}

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
	if (status != 1)
	{
		OutputShaderErrorMessage(fragment_shader, fs_filename);
		return false;
	}

	shader_program = glCreateProgram();

	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);

	glLinkProgram(shader_program);

	glGetProgramiv(shader_program, GL_LINK_STATUS, &status);
	if (status != 1)
	{
		OutputLinkerErrorMessage(shader_program);
		return false;
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return true;
}

bool OpenGLShaderManagerCommonBase::Initialize()
{
	return InitializeShaders();
}

void OpenGLShaderManagerCommonBase::Finalize()
{
	ClearShaders();
}

void OpenGLShaderManagerCommonBase::Tick()
{

}

bool OpenGLShaderManagerCommonBase::InitializeShaders()
{
	bool result = false;

	GLuint shader_program;
	ShaderSourceList list = {
		{GL_VERTEX_SHADER,VS_SHADER_SOURCE_FILE},
		{GL_FRAGMENT_SHADER,PS_SHADER_SOURCE_FILE}
	};
	result = LoadShaderProgram(list, shader_program);
	if (result == false)
	{
		return result;
	}
	default_shaders_[DefaultShaderIndex::Forward] = shader_program;

	list = {
		{GL_VERTEX_SHADER,VS_SHADOWMAP_SOURCE_FILE},
		{GL_FRAGMENT_SHADER,PS_SHADOWMAP_SOURCE_FILE}
	};
	result = LoadShaderProgram(list,shader_program);
	if (result == false)
	{
		return result;
	}
	default_shaders_[DefaultShaderIndex::ShadowMap] = shader_program;

	list = {
		{GL_VERTEX_SHADER,VS_OMNI_SHADOWMAP_SOURCE_FILE},
		{GL_GEOMETRY_SHADER,GS_OMNI_SHADOWMAP_SOURCE_FILE},
		{GL_FRAGMENT_SHADER,PS_OMNI_SHADOWMAP_SOURCE_FILE}
	};
	result = LoadShaderProgram(list, shader_program);
	if (result == false)
	{
		return result;
	}
	default_shaders_[DefaultShaderIndex::OmniShadowMap] = shader_program;

	list = {
		{GL_VERTEX_SHADER,VS_PASSTHROUGH_SOURCE_FILE},
		{GL_FRAGMENT_SHADER,PS_PASSTHROUGH_SOURCE_FILE}
	};
	result = LoadShaderProgram(list,shader_program);
	if (result == false)
	{
		return result;
	}
	default_shaders_[DefaultShaderIndex::Copy] = shader_program;

	list = {
		{GL_VERTEX_SHADER,VS_PASSTHROUGH_CUEBEMAP_SOURCE_FILE},
		{GL_FRAGMENT_SHADER,PS_SIMPLER_CUBEMAP_SOURCE_FILE}
	};
	result = LoadShaderProgram(list,shader_program);
	if (result == false)
	{
		return result;
	}
	default_shaders_[DefaultShaderIndex::CopyCube] = shader_program;

#ifdef DEBUG
	list = {
		{GL_VERTEX_SHADER,DEBUG_VS_SHADER_SOURCE_FILE},
		{GL_FRAGMENT_SHADER,DEBUG_PS_SHADER_SOURCE_FILE}
	};
	result = LoadShaderProgram(list,shader_program);
	if (result == false)
	{
		return result;
	}
	default_shaders_[DefaultShaderIndex::Debug] = shader_program;
#endif

	return result;
}

void OpenGLShaderManagerCommonBase::ClearShaders()
{
	for (auto& iter : default_shaders_)
	{
		glDeleteProgram(static_cast<GLuint>(iter.second));
	}
}