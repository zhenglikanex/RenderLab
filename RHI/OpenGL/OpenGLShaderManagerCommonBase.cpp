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
	result = LoadShaderFromFile(VS_SHADER_SOURCE_FILE, PS_SHADER_SOURCE_FILE,shader_program);
	if (result == false)
	{
		return result;
	}
	default_shaders_[DefaultShaderIndex::Forward] = shader_program;

	result = LoadShaderFromFile(VS_SHADOWMAP_SOURCE_FILE,PS_SHADOWMAP_SOURCE_FILE,shader_program);
	if (result == false)
	{
		return result;
	}
	default_shaders_[DefaultShaderIndex::ShadowMap] = shader_program;

	result = LoadShaderFromFile(VS_PASSTHROUGH_SOURCE_FILE, PS_PASSTHROUGH_SOURCE_FILE,shader_program);
	if (result == false)
	{
		return result;
	}
	default_shaders_[DefaultShaderIndex::Copy] = shader_program;

	result = LoadShaderFromFile(VS_PASSTHROUGH_CUEBEMAP_SOURCE_FILE, PS_SIMPLER_CUBEMAP_SOURCE_FILE,shader_program);
	if (result == false)
	{
		return result;
	}
	default_shaders_[DefaultShaderIndex::CopyCube] = shader_program;

#ifdef DEBUG
	result = LoadShaderFromFile(DEBUG_VS_SHADER_SOURCE_FILE, DEBUG_PS_SHADER_SOURCE_FILE,shader_program);
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