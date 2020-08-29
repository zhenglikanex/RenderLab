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

	auto shader_program = glCreateProgram();

	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);

	glBindAttribLocation(shader_program, 0, "inputPosition");
	glBindAttribLocation(shader_program, 1, "inputNormal");
	glBindAttribLocation(shader_program, 2, "inputUV");

	glLinkProgram(shader_program);

	glGetProgramiv(shader_program, GL_LINK_STATUS, &status);
	if (status != 1)
	{
		OutputLinkerErrorMessage(shader_program);
		return false;
	}

	default_shaders_[DefaultShaderIndex::Forward] = shader_program;

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

	auto debug_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	auto debug_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	auto debug_vs_c_str = debug_vs_vertex_shader_buffer.c_str();
	glShaderSource(debug_vertex_shader, 1, &debug_vs_c_str, NULL);
	auto debug_f_c_str = debgu_fragment_shader_buffer.c_str();
	glShaderSource(debug_fragment_shader, 1, &debug_f_c_str, NULL);

	glCompileShader(debug_vertex_shader);
	glCompileShader(debug_fragment_shader);

	glGetShaderiv(debug_vertex_shader, GL_COMPILE_STATUS, &status);
	if (status != 1)
	{
		OutputShaderErrorMessage(debug_vertex_shader, debug_vs_filename);
		return false;
	}

	glGetShaderiv(debug_fragment_shader, GL_COMPILE_STATUS, &status);
	if (status != 1)
	{
		OutputShaderErrorMessage(debug_fragment_shader, debug_fs_filename);
		return false;
	}

	auto debug_shader_program = glCreateProgram();

	glAttachShader(debug_shader_program, debug_vertex_shader);
	glAttachShader(debug_shader_program, debug_fragment_shader);

	glBindAttribLocation(debug_shader_program, 0, "inputPosition");

	glLinkProgram(debug_shader_program);

	glGetProgramiv(debug_shader_program, GL_LINK_STATUS, &status);
	if (status != 1)
	{
		OutputLinkerErrorMessage(debug_shader_program);
		return false;
	}
#endif

	default_shaders_[DefaultShaderIndex::Debug] = debug_shader_program;

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glDeleteShader(debug_vertex_shader);
	glDeleteShader(debug_fragment_shader);

	return true;
}

void OpenGLShaderManagerCommonBase::ClearShaders()
{
	for (auto& iter : default_shaders_)
	{
		glDeleteProgram(static_cast<GLuint>(iter.second));
	}
}