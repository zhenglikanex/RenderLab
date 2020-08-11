#include "OpenGLGraphicsManager.hpp"

#include <iostream>
#include <fstream>

#include "Framework/Common/IApplication.hpp"
#include "Framework/Common/Engine.hpp"
#include "Framework/Common/SceneManager.hpp"
#include "Framework/Common/SceneNode.hpp"
#include "Framework/Utils/FileUtils.hpp"
#include "Framework/Utils/FileHandle.hpp"

const char VS_SHADER_SOURCE_FILE[] = "Shaders/basic_vs.glsl";
const char PS_SHADER_SOURCE_FILE[] = "Shaders/basic_ps.glsl";

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
    }

	InitializeShader(VS_SHADER_SOURCE_FILE, PS_SHADER_SOURCE_FILE);
	InitializeBuffers();
	InitConstants();

	return true;
}

void OpenGLGraphicsManager::Finalize()
{
	for (auto& dbc : draw_batch_context_)
	{
		glDeleteVertexArrays(1, &dbc.vao);
	}

	draw_batch_context_.clear();

	int size = buffers_.size();
	//-1去除index buffer;
	for (int i = 0; i < size - 1; i++)
	{
		glDisableVertexAttribArray(i);
	}

	for (auto& buf : buffers_)
	{
		glDeleteBuffers(1, &buf);
	}

	for (auto& texture : textures_)
	{
		glDeleteTextures(1, &texture);
	}

	buffers_.clear();
	textures_.clear();

	glDetachShader(shader_program_, vertex_shader_);
	glDetachShader(shader_program_, fragment_shader_);

	glDeleteShader(vertex_shader_);
	glDeleteShader(fragment_shader_);

	glDeleteProgram(shader_program_);
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

bool OpenGLGraphicsManager::SetPerBatchShaderParameters(const std::string& param_name, const glm::mat4& param)
{
	unsigned int location;
	location = glGetUniformLocation(shader_program_,param_name.c_str());
	if (location == -1)
	{
		return false;
	}
	glUniformMatrix4fv(location, 1, false,glm::value_ptr(param));

	return true;
}

bool OpenGLGraphicsManager::SetPerBatchShaderParameters(const std::string& param_name, const glm::vec3& param)
{
	unsigned int location;

	location = glGetUniformLocation(shader_program_, param_name.c_str());
	if (location == -1)
	{
		std::cout << "set param error param_name:" << param_name << std::endl;
		return false;
	}
	glUniform3fv(location, 1, glm::value_ptr(param));

	return true;
}

bool OpenGLGraphicsManager::SetPerBatchShaderParameters(const std::string& param_name, const float param)
{
	unsigned int location;
	
	location = glGetUniformLocation(shader_program_, param_name.c_str());
	if (location == -1)
	{
		return false;
	}
	glUniform1f(location, param);

	return true;
}

bool OpenGLGraphicsManager::SetPerBatchShaderParameters(const std::string& param_name, const int param)
{
	unsigned int location;

	location = glGetUniformLocation(shader_program_, param_name.c_str());
	if (location == -1)
	{
		return false;
	}
	glUniform1i(location, param);

	return true;
}

bool OpenGLGraphicsManager::InitializeBuffers()
{
	auto& scene = g_app->GetEngine()->GetSceneManager()->GetSceneForRendering();
	for (auto& obj : scene.GeometryNodes)
	{
		auto& geometry_node = obj.second;
		if (geometry_node->IsVisible())
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

			// 按sub mesh添加
			auto index_group_count = mesh->GetIndexGroupCount();
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

			for (int i = 0; i < index_group_count; ++i)
			{
				const SceneObjectIndexArray& index_array = mesh->GetIndexArray(i);
				auto index_array_size = index_array.GetDataSize();
				auto index_array_data = index_array.GetData();

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_array_size, index_array_data, GL_STATIC_DRAW);

				GLsizei index_count = static_cast<GLsizei>(index_array.GetIndexCount());

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

				size_t material_index = index_array.GetMaterialIndex();
				std::string material_key = geometry_node->GetMaterialRef(material_index);
				auto material = scene.GetMaterial(material_key);
				if (material)
				{
					auto color = material->GetBaseColor();
					if (color.ValueMap)
					{
						auto texture = color.ValueMap->GetTexture();
						auto it = texture_index_.find(material_key);
						if (it == texture_index_.end())
						{
							GLuint texture_id;
							glGenTextures(1, &texture_id);
							glActiveTexture(GL_TEXTURE0 + texture_id);
							glBindTexture(GL_TEXTURE_2D, texture_id);
							if (texture.bitcount == 3)
							{
								glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.Width, texture.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture.data);
							}
							else if(texture.bitcount == 4)
							{
								glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.Width, texture.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.data);
							}
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

							texture_index_[color.ValueMap->GetName()] = texture_id;
							textures_.push_back(texture_id);
						}
					}
				}

				DrawBathContext& dbc = *(new DrawBathContext);
				dbc.vao = vao;
				dbc.mode = mode;
				dbc.type = type;
				dbc.count = index_count;
				dbc.node = geometry_node;
				dbc.material = material;

				draw_batch_context_.push_back(std::move(dbc));
			}
		}
	}
    return true;
}

void OpenGLGraphicsManager::RenderBuffers()
{
	//static float rotateAngle = 0.0f;

	//// Update world matrix to rotate the model
	//rotateAngle += PI / 360;
	////glm::mat4 rotationMatrixY = glm::identity<glm::mat4>();
	//glm::mat4 rotationMatrixZ = glm::identity<glm::mat4>();
	////rotationMatrixY = glm::rotate(rotationMatrixY, rotateAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	//rotationMatrixZ = glm::rotate(rotationMatrixZ, rotateAngle, glm::vec3(0.0f, 0.0f, 1.0f));

	////world_matrix_ = rotationMatrixY * rotationMatrixZ;
	//draw_frame_context_.world_matrix = rotationMatrixZ;

	SetPerBatchShaderParameters();

	for (auto& dbc : draw_batch_context_)
	{
		glUseProgram(shader_program_);

		glm::mat4 trans = *dbc.node->GetCalculatedTransform();
		if (void* rigidbody = dbc.node->RigidBody())
		{
			auto simulated_result = g_app->GetEngine()->GetPhysicsManager()->GetRigidBodyTransform(rigidbody);

			// replace the translation part of the matrix with simlation result directly
			trans[3] = glm::vec4(0.0f,0.0f,0.0f,trans[3].w);
			
			// apply the rotation part of the simlation result
			glm::mat4 rotation = glm::identity<glm::mat4>();
			rotation[0] = simulated_result[0];
			rotation[1] = simulated_result[1];
			rotation[2] = simulated_result[2];
			trans = trans * rotation;
			trans[3] = glm::vec4(simulated_result[3].x, simulated_result[3].y, simulated_result[3].z,trans[3].w);
		}

		SetPerBatchShaderParameters("modelMatrix", trans);

		glBindVertexArray(dbc.vao);
		//后面根据材质进行分组渲染
		if (dbc.material)
		{
			Color color = dbc.material->GetBaseColor();
			if (color.ValueMap)
			{
				SetPerBatchShaderParameters("defaultSampler", texture_index_[color.ValueMap->GetName()]);
				
				// 告诉shader使用texture
				SetPerBatchShaderParameters("diffuseColor", glm::vec3(-1.0f));
			}
			else
			{
				SetPerBatchShaderParameters("diffuseColor", color.Value);
			}
			
			color = dbc.material->GetSpecularColor();
			SetPerBatchShaderParameters("specularColor", color.Value);

			Parameter param = dbc.material->GetSpecularPower();
			SetPerBatchShaderParameters("specularPower", param.Value);
		}

		glDrawElements(dbc.mode, dbc.count, dbc.type, 0);

		/*auto index_buffer_count = dbc.counts.size();
		const GLvoid** indicies = new const GLvoid*[index_buffer_count];
		memset(indicies, 0x00, sizeof(GLvoid*) * index_buffer_count);
		glMultiDrawElements(dbc.mode, dbc.counts.data(), dbc.type, indicies, index_buffer_count);
		delete[] indicies;*/
	}
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
	glBindAttribLocation(shader_program_, 2, "inputUV");

	glLinkProgram(shader_program_);

	glGetProgramiv(shader_program_, GL_LINK_STATUS, &status);
	if (status != 1)
	{
		OutputLinkerErrorMessage(shader_program_);
		return false;
	}

    return true;
}