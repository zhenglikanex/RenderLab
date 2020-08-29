#include "OpenGLShaderManagerCommonBase.hpp"

#include <iostream>
#include <fstream>

#include "Framework/Utils/FileUtils.hpp"
#include "Framework/Utils/FileHandle.hpp"

#ifdef DEBUG
const char DEBUG_VS_SHADER_SOURCE_FILE[] = "Shaders/debug_vs.glsl";
const char DEBUG_PS_SHADER_SOURCE_FILE[] = "Shaders/debug_ps.glsl";
#endif
const char VS_SHADER_SOURCE_FILE[] = "Shaders/basic_vs.glsl";
const char PS_SHADER_SOURCE_FILE[] = "Shaders/basic_ps.glsl";

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
	const char* vs_filename = VS_SHADER_SOURCE_FILE;
	const char* fs_filename = PS_SHADER_SOURCE_FILE;

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
	glBindAttribLocation(shader_program_, 2, "inputUV");

	glLinkProgram(shader_program_);

	glGetProgramiv(shader_program_, GL_LINK_STATUS, &status);
	if (status != 1)
	{
		OutputLinkerErrorMessage(shader_program_);
		return false;
	}

#ifdef DEBUG
	const char* debug_vs_filename = DEBUG_VS_SHADER_SOURCE_FILE;
	const char* debug_fs_filename = DEBUG_PS_SHADER_SOURCE_FILE;

	std::string debug_vs_vertex_shader_buffer;
	std::string debgu_fragment_shader_buffer;

	debug_vs_vertex_shader_buffer = FileUtils::GetInstance()->OpenFileAndReadString(debug_vs_filename);
	if (debug_vs_vertex_shader_buffer.empty())
	{
		return false;
	}

	debgu_fragment_shader_buffer = FileUtils::GetInstance()->OpenFileAndReadString(debug_fs_filename);
	if (debgu_fragment_shader_buffer.empty())
	{
		return false;
	}

	debug_vertex_shader_ = glCreateShader(GL_VERTEX_SHADER);
	debug_fragment_shader_ = glCreateShader(GL_FRAGMENT_SHADER);

	auto debug_vs_c_str = debug_vs_vertex_shader_buffer.c_str();
	glShaderSource(debug_vertex_shader_, 1, &debug_vs_c_str, NULL);
	auto debug_f_c_str = debgu_fragment_shader_buffer.c_str();
	glShaderSource(debug_fragment_shader_, 1, &debug_f_c_str, NULL);

	glCompileShader(debug_vertex_shader_);
	glCompileShader(debug_fragment_shader_);

	glGetShaderiv(debug_vertex_shader_, GL_COMPILE_STATUS, &status);
	if (status != 1)
	{
		OutputShaderErrorMessage(debug_vertex_shader_, debug_vs_filename);
		return false;
	}

	glGetShaderiv(debug_fragment_shader_, GL_COMPILE_STATUS, &status);
	if (status != 1)
	{
		OutputShaderErrorMessage(debug_fragment_shader_, debug_fs_filename);
		return false;
	}

	debug_shader_program_ = glCreateProgram();

	glAttachShader(debug_shader_program_, debug_vertex_shader_);
	glAttachShader(debug_shader_program_, debug_fragment_shader_);

	glBindAttribLocation(debug_shader_program_, 0, "inputPosition");

	glLinkProgram(debug_shader_program_);

	glGetProgramiv(debug_shader_program_, GL_LINK_STATUS, &status);
	if (status != 1)
	{
		OutputLinkerErrorMessage(debug_shader_program_);
		return false;
	}

#endif

	return true;
}

void OpenGLShaderManagerCommonBase::ClearShaders()
{
	glDetachShader(shader_program_, vertex_shader_);
	glDetachShader(shader_program_, fragment_shader_);

	glDeleteShader(vertex_shader_);
	glDeleteShader(fragment_shader_);

	glDeleteProgram(shader_program_);
}

void* OpenGLShaderManagerCommonBase::GetDefaultShaderProgram()
{
	return static_cast<void*>(&shader_program_);
}

#ifdef DEBUG

void* OpenGLShaderManagerCommonBase::GetDebugShaderProgram()
{
	return static_cast<void*>(&debug_shader_program_);
}

#endif