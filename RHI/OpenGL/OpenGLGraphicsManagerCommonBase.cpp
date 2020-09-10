#include "OpenGLGraphicsManagerCommonBase.hpp"

#include <iostream>
#include <fstream>

#include "Framework/Common/IApplication.hpp"
#include "Framework/Common/Engine.hpp"
#include "Framework/Common/SceneManager.hpp"
#include "Framework/Common/SceneNode.hpp"
#include "Framework/Utils/FileUtils.hpp"
#include "Framework/Utils/FileHandle.hpp"

using namespace Aurora;

void OpenGLGraphicsManagerCommonBase::Finalize()
{
	ClearBuffers();
}

void OpenGLGraphicsManagerCommonBase::Clear()
{
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLGraphicsManagerCommonBase::Draw()
{
	GraphicsManager::Draw();
	glFlush();
}

#ifdef DEBUG

void Aurora::OpenGLGraphicsManagerCommonBase::DrawLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color)
{
	GLfloat vertices[6];
	vertices[0] = from.x;
	vertices[1] = from.y;
	vertices[2] = from.z;
	vertices[3] = to.x;
	vertices[4] = to.y;
	vertices[5] = to.z;

	GLuint vao;
	GLuint buffer;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &buffer);

	debug_buffers_.push_back(buffer);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6, vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	DebugDrawBathContext& dbc = *(new DebugDrawBathContext);
	dbc.vao = vao;
	dbc.mode = GL_LINES;
	dbc.count = 2;
	dbc.color = color;

	debug_draw_batch_contenxt_.push_back(std::move(dbc));
}

void Aurora::OpenGLGraphicsManagerCommonBase::DrawBox(const glm::vec3& bbMin, const glm::vec3 bbMax, const glm::vec3& color)
{
	GLfloat vertices[12 * 2 * 3];

	// top
	vertices[0] = bbMax.x;
	vertices[1] = bbMax.y;
	vertices[2] = bbMax.z;
	vertices[3] = bbMax.x;
	vertices[4] = bbMin.y;
	vertices[5] = bbMax.z;

	vertices[6] = bbMax.x;
	vertices[7] = bbMin.y;
	vertices[8] = bbMax.z;
	vertices[9] = bbMin.x;
	vertices[10] = bbMin.y;
	vertices[11] = bbMax.z;

	vertices[12] = bbMin.x;
	vertices[13] = bbMin.y;
	vertices[14] = bbMax.z;
	vertices[15] = bbMin.x;
	vertices[16] = bbMax.y;
	vertices[17] = bbMax.z;

	vertices[18] = bbMin.x;
	vertices[19] = bbMax.y;
	vertices[20] = bbMax.z;
	vertices[21] = bbMax.x;
	vertices[22] = bbMax.y;
	vertices[23] = bbMax.z;

	// bottom
	vertices[24] = bbMax.x;
	vertices[25] = bbMax.y;
	vertices[26] = bbMin.z;
	vertices[27] = bbMax.x;
	vertices[28] = bbMin.y;
	vertices[29] = bbMin.z;

	vertices[30] = bbMax.x;
	vertices[31] = bbMin.y;
	vertices[32] = bbMin.z;
	vertices[33] = bbMin.x;
	vertices[34] = bbMin.y;
	vertices[35] = bbMin.z;

	vertices[36] = bbMin.x;
	vertices[37] = bbMin.y;
	vertices[38] = bbMin.z;
	vertices[39] = bbMin.x;
	vertices[40] = bbMax.y;
	vertices[41] = bbMin.z;

	vertices[42] = bbMin.x;
	vertices[43] = bbMax.y;
	vertices[44] = bbMin.z;
	vertices[45] = bbMax.x;
	vertices[46] = bbMax.y;
	vertices[47] = bbMin.z;

	// side 1
	vertices[48] = bbMax.x;
	vertices[49] = bbMax.y;
	vertices[50] = bbMax.z;
	vertices[51] = bbMax.x;
	vertices[52] = bbMax.y;
	vertices[53] = bbMin.z;

	// side 2
	vertices[54] = bbMin.x;
	vertices[55] = bbMax.y;
	vertices[56] = bbMax.z;
	vertices[57] = bbMin.x;
	vertices[58] = bbMax.y;
	vertices[59] = bbMin.z;

	// side 3
	vertices[60] = bbMin.x;
	vertices[61] = bbMin.y;
	vertices[62] = bbMax.z;
	vertices[63] = bbMin.x;
	vertices[64] = bbMin.y;
	vertices[65] = bbMin.z;

	// side 4
	vertices[66] = bbMax.x;
	vertices[67] = bbMin.y;
	vertices[68] = bbMax.z;
	vertices[69] = bbMax.x;
	vertices[70] = bbMin.y;
	vertices[71] = bbMin.z;

	GLuint vao;
	GLuint buffer;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &buffer);

	debug_buffers_.push_back(buffer);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 72, vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	DebugDrawBathContext& dbc = *(new DebugDrawBathContext);
	dbc.vao = vao;
	dbc.mode = GL_LINES;
	dbc.count = 24;
	dbc.color = color;

	debug_draw_batch_contenxt_.push_back(std::move(dbc));
}

void OpenGLGraphicsManagerCommonBase::DrawOverlay(const intptr_t shadowmap,uint32_t layer_index, float vp_left, float vp_top, float vp_width, float vp_height)
{
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLint texture_id = (GLuint)shadowmap;

	glActiveTexture(GL_TEXTURE0 + texture_id);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id);

	SetShaderParameters("depthSampler", texture_id);
	SetShaderParameters("layer_index", float(layer_index));
	GLfloat vertices[] = {
		vp_left, vp_top, 0.0f,
		vp_left, vp_top - vp_height, 0.0f,
		vp_left + vp_width, vp_top, 0.0f,
		vp_left + vp_width, vp_top - vp_height, 0.0f
	};

	GLfloat uv[] = {
		0.0f,1.0f,
		0.0f,0.0f,
		1.0f,1.0f,
		1.0f,0.0f
	};

	GLuint buffer_id[2];
	
	glGenBuffers(2, buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);

	// Bind the vertex buffer and load the vertex (uv) data into the vertex buffer.
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);

	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, 0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0x00, 4);

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(2, buffer_id);
}

void Aurora::OpenGLGraphicsManagerCommonBase::ClearDebugBuffers()
{
	for (auto& dbc : debug_draw_batch_contenxt_)
	{
		glDeleteVertexArrays(1, &dbc.vao);
	}
	debug_draw_batch_contenxt_.clear();

	for (auto& buffer : debug_buffers_)
	{
		glDeleteBuffers(1, &buffer);
	}
	debug_buffers_.clear();
}

#endif

bool OpenGLGraphicsManagerCommonBase::SetPerFrameShaderParameters(const DrawFrameContext& context)
{
	int location;

	location = glGetUniformLocation(current_shader_, "worldMatrix");
	if (location == -1)
	{
		return false;
	}
	glUniformMatrix4fv(location, 1, false, glm::value_ptr(context.world_matrix));

	location = glGetUniformLocation(current_shader_, "viewMatrix");
	if (location == -1)
	{
		return false;
	}
	glUniformMatrix4fv(location, 1, false, glm::value_ptr(context.view_matrix));

	location = glGetUniformLocation(current_shader_, "projectionMatrix");
	if (location == -1)
	{
		return false;
	}
	glUniformMatrix4fv(location, 1, false, glm::value_ptr(context.projection_matrix));

	SetShaderParameters("viewPosition", context.view_pos);

	location = glGetUniformLocation(current_shader_, "ambientColor");
	if (location == -1)
	{
		return false;
	}
	glUniform3fv(location, 1, glm::value_ptr(context.ambient_color));

	location = glGetUniformLocation(current_shader_, "numLights");
	if (location == -1)
	{
		return false;
	}
	glUniform1i(location, context.lights.size());

	std::string name;
	for (int i = 0; i < context.lights.size(); ++i)
	{
		auto& light = context.lights[i];
		name = "allLights[" + std::to_string(i) + "].lightPosition";
		location = glGetUniformLocation(current_shader_, name.c_str());
		if (location == -1)
		{
			return false;
		}
		glUniform4fv(location, 1, glm::value_ptr(light.light_position));

		name = "allLights[" + std::to_string(i) + "].lightSize";
		location = glGetUniformLocation(current_shader_, name.c_str());
		if (location == -1)
		{
			return false;
		}
		glUniform2fv(location, 1, glm::value_ptr(light.light_size));

		name = "allLights[" + std::to_string(i) + "].lightColor";
		location = glGetUniformLocation(current_shader_, name.c_str());
		if (location == -1)
		{
			return false;
		}
		glUniform4fv(location, 1, glm::value_ptr(light.light_color));
		if (location == -1)
		{
			return false;
		}

		name = "allLights[" + std::to_string(i) + "].lightDirection";
		location = glGetUniformLocation(current_shader_, name.c_str());
		if (location == -1)
		{
			return false;
		}
		glUniform4fv(location, 1, glm::value_ptr(light.light_direction));

		name = "allLights[" + std::to_string(i) + "].lightIntensity";
		location = glGetUniformLocation(current_shader_, name.c_str());
		if (location == -1)
		{
			return false;
		}
		glUniform1f(location, light.light_intensity);

		name = "allLights[" + std::to_string(i) + "].lightDistAttenCurveType";
		location = glGetUniformLocation(current_shader_, name.c_str());
		if (location == -1)
		{
			return false;
		}
		GLint dist_atten_type = (GLint)light.light_dist_atten_curve_type;
		glUniform1i(location, dist_atten_type);

		name = "allLights[" + std::to_string(i) + "].lightDistAttenCurveParams";
		location = glGetUniformLocation(current_shader_, name.c_str());
		if (location == -1)
		{
			return false;
		}
		glUniform1fv(location, 5, light.light_dist_atten_curve_params);

		name = "allLights[" + std::to_string(i) + "].lightAngleAttenCurveType";
		location = glGetUniformLocation(current_shader_, name.c_str());
		if (location == -1)
		{
			return false;
		}
		GLint angle_atten_type = (GLint)light.light_angle_atten_curve_type;
		glUniform1i(location, angle_atten_type);

		name = "allLights[" + std::to_string(i) + "].lightAngleAttenCurveParams";
		location = glGetUniformLocation(current_shader_, name.c_str());
		if (location == -1)
		{
			return false;
		}
		glUniform1fv(location, 5, light.light_angle_atten_curve_params);
	}

	return true;
}

bool OpenGLGraphicsManagerCommonBase::SetShaderParameters(const std::string& param_name, const glm::mat4& param)
{
	unsigned int location;
	location = glGetUniformLocation(current_shader_, param_name.c_str());
	if (location == -1)
	{
		return false;
	}
	glUniformMatrix4fv(location, 1, false, glm::value_ptr(param));

	return true;
}

bool OpenGLGraphicsManagerCommonBase::SetShaderParameters(const std::string& param_name, const glm::vec3& param)
{
	unsigned int location;

	location = glGetUniformLocation(current_shader_, param_name.c_str());
	if (location == -1)
	{
		std::cout << "set param error param_name:" << param_name << std::endl;
		return false;
	}
	glUniform3fv(location, 1, glm::value_ptr(param));

	return true;
}

bool OpenGLGraphicsManagerCommonBase::SetShaderParameters(const std::string& param_name, const float param)
{
	unsigned int location;

	location = glGetUniformLocation(current_shader_, param_name.c_str());
	if (location == -1)
	{
		return false;
	}
	glUniform1f(location, param);

	return true;
}

bool OpenGLGraphicsManagerCommonBase::SetShaderParameters(const std::string& param_name, const int param)
{
	unsigned int location;

	location = glGetUniformLocation(current_shader_, param_name.c_str());
	if (location == -1)
	{
		return false;
	}
	glUniform1i(location, param);

	return true;
}

bool OpenGLGraphicsManagerCommonBase::SetShaderParameters(const std::string& param_name, const bool param)
{
	unsigned int location;

	location = glGetUniformLocation(current_shader_, param_name.c_str());
	if (location == -1)
	{
		return false;
	}
	glUniform1f(location, param);

	return true;
}


void OpenGLGraphicsManagerCommonBase::InitializeBuffers(const Scene& scene)
{
	for (auto& obj : scene.GeometryNodes)
	{
		auto& geometry_node = obj.second;
		if (geometry_node->IsVisible())
		{
			auto geometry = scene.GetGeometry(geometry_node->GetSceneObjectRef());
			auto mesh = geometry->GetMesh().lock();
			if (!mesh) continue;;

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
							else if (texture.bitcount == 4)
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

				auto dbc = std::make_shared<OpenGLDrawBatchContext>();
				dbc->vao = vao;
				dbc->mode = mode;
				dbc->type = type;
				dbc->count = index_count;
				dbc->node = geometry_node;
				dbc->material = material;

				frames_[frame_index_].batch_contexts.push_back(dbc);
			}
		}
	}

	// init sky box

	ImageParser parser;
	std::string faces[6] = { "right","left","top","bottom","front","back" };
	//std::string faces[6] = { "Epic_BlueSunset_Cam_3_Right-X.png","Epic_BlueSunset_Cam_2_Left+X.png","Epic_BlueSunset_Cam_4_Up+Y.png","Epic_BlueSunset_Cam_5_Down-Y.png","Epic_BlueSunset_Cam_0_Front+Z.png","Epic_BlueSunset_Cam_1_Back-Z.png" };

	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glActiveTexture(GL_TEXTURE0 + texture_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
	
	for (unsigned int i = 0; i < 6; i++)
	{
		auto img = parser.Parser("Textures/sky_box2/" + faces[i] + ".png");
		if (img)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, img->Width, img->Height, 0, GL_RGB, GL_UNSIGNED_BYTE, img->data
			);
			//stbi_image_free(data);
		}
		else
		{
			//std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			//stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	texture_index_["sky_box"] = texture_id;
	textures_.push_back(texture_id);


	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &sky_box_vao_);
	glBindVertexArray(sky_box_vao_);

	GLuint buffer_id;
	glGenBuffers(1, &buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	

	buffers_.push_back(buffer_id);
}

void OpenGLGraphicsManagerCommonBase::ClearBuffers()
{
	for (auto& frame : frames_)
	{
		auto& batch_contexts = frame.batch_contexts;
		for (auto& dbc : batch_contexts)
		{
			glDeleteVertexArrays(1, &std::dynamic_pointer_cast<OpenGLDrawBatchContext>(dbc)->vao);
		}
	}

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
}

void OpenGLGraphicsManagerCommonBase::UseShaderProgram(intptr_t shader_program)
{
	current_shader_ = static_cast<GLuint>(shader_program);
	glUseProgram(current_shader_);
}

void OpenGLGraphicsManagerCommonBase::SetPerFrameConstants(const DrawFrameContext& context)
{
	SetPerFrameShaderParameters(context);
}

void OpenGLGraphicsManagerCommonBase::DrawBatch(const DrawBatchContext& context)
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

	const OpenGLDrawBatchContext& dbc = dynamic_cast<const OpenGLDrawBatchContext&>(context);
	SetShaderParameters("modelMatrix",dbc.trans);

	glBindVertexArray(dbc.vao);

	SetShaderParameters("usingdiffuseMap", false);
	SetShaderParameters("usingNormalMap", false);

	SetShaderParameters("dt", g_app->GetElapse());
	SetShaderParameters("skybox", texture_index_["sky_box"]);
	//后面根据材质进行分组渲染
	if (dbc.material)
	{
		Color color = dbc.material->GetBaseColor();
		if (color.ValueMap)
		{
			SetShaderParameters("diffuseMap", texture_index_[color.ValueMap->GetName()]);

			// 告诉shader使用texture
			SetShaderParameters("usingdiffuseMap", true);
		}
		else
		{
			SetShaderParameters("diffuseColor", color.Value);
		}

		color = dbc.material->GetSpecularColor();
		SetShaderParameters("specularColor", color.Value);

		Parameter param = dbc.material->GetSpecularPower();
		SetShaderParameters("specularPower", param.Value);

		Normal noraml = dbc.material->GetNormal();
		if (noraml.ValueMap)
		{
			SetShaderParameters("normalMap", texture_index_[noraml.ValueMap->GetName()]);
			SetShaderParameters("usingNormalMap", true);
		}
	}

	glDrawElements(dbc.mode, dbc.count, dbc.type, 0);

	/*auto index_buffer_count = dbc.counts.size();
	const GLvoid** indicies = new const GLvoid*[index_buffer_count];
	memset(indicies, 0x00, sizeof(GLvoid*) * index_buffer_count);
	glMultiDrawElements(dbc.mode, dbc.counts.data(), dbc.type, indicies, index_buffer_count);
	delete[] indicies;*/

#ifdef DEBUG
	/*glUseProgram(debug_shader_program_);

	SetShaderParameters(debug_shader_program_);

	for (auto& dbc : debug_draw_batch_contenxt_)
	{
		SetPerBatchShaderParameters(debug_shader_program_, "lineColor", dbc.color);

		glBindVertexArray(dbc.vao);
		glDrawArrays(dbc.mode, 0, dbc.count);
	}*/

#endif
}

void OpenGLGraphicsManagerCommonBase::DrawBatchDepthOnly(const DrawBatchContext& context)
{
	const OpenGLDrawBatchContext& dbc = dynamic_cast<const OpenGLDrawBatchContext&>(context);
	SetShaderParameters("modelMatrix", dbc.trans);
	glBindVertexArray(dbc.vao);
	glDrawElements(dbc.mode, dbc.count, dbc.type, 0);
}

void OpenGLGraphicsManagerCommonBase::DrawSkyBox()
{
	auto shader_program = g_app->GetEngine()->GetShaderManager()->GetDefaultShaderProgram(DefaultShaderIndex::SkyBox);
	UseShaderProgram(shader_program);

	auto view = glm::mat3(frames_[0].frame_context.view_matrix);
	SetShaderParameters("worldMatrix", frames_[0].frame_context.world_matrix);
	SetShaderParameters("viewMatrix", view);
	glm::mat4 model = glm::identity<glm::mat4>();
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	SetShaderParameters("modelMatrix", model);
	SetShaderParameters("projectionMatrix", frames_[0].frame_context.projection_matrix);
	SetShaderParameters("skybox", texture_index_["sky_box"]);
	glDepthFunc(GL_LEQUAL);
	glBindVertexArray(sky_box_vao_);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS);
}

intptr_t OpenGLGraphicsManagerCommonBase::GenerateShadowMapArray(uint32_t count)
{
	const int32_t kShadowMapWidth = 1024;
	const int32_t kShadowMapHeight = 1024;

	GLuint shadowmap;
	glGenTextures(1, &shadowmap);
	glActiveTexture(GL_TEXTURE0 + shadowmap);
	glBindTexture(GL_TEXTURE_2D_ARRAY, shadowmap);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_DEPTH_COMPONENT24, kShadowMapWidth, kShadowMapHeight, count);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT24, kShadowMapWidth, kShadowMapHeight, count, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	return static_cast<intptr_t>(shadowmap);
}

void OpenGLGraphicsManagerCommonBase::BeginShadowMap(const Light& light, const intptr_t shadowmap,uint32_t layer_index)
{
	const int32_t kShadowMapWidth = 1024;
	const int32_t kShadowMapHeight = 1024;

	glGenFramebuffers(1, &shadowmap_framebuffer_name_);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowmap_framebuffer_name_);
	
#ifdef OPENGL_ES
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
#else
	glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,(GLuint)shadowmap,0,layer_index);
#endif

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		assert(0);
	}
	
	//glDrawBuffer(GL_NONE);
	glDrawBuffers(0, nullptr);
	glDepthMask(TRUE);
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, kShadowMapWidth, kShadowMapHeight);

	glm::mat4 depthVP;
	glm::mat4 view;
	glm::mat4 projection;
	glm::vec3 position = light.light_position;
	glm::vec3 lookAt = light.light_position + light.light_direction;
	glm::vec3 up = { 0.0f,0.0f,1.0f };
	view = glm::lookAtRH(position, lookAt,up);

	float fov = PI / 3.0f;
	float near_clip_distance = 1.0f;
	float far_clip_distance = 100.0f;

	projection = glm::perspectiveFovRH(fov,(float)kShadowMapWidth,(float)kShadowMapHeight,near_clip_distance,far_clip_distance);
	/*GLfloat near_plane = 1.0f, far_plane = 100.0f;*/
	//projection  = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_clip_distance, far_clip_distance);
	depthVP = projection * view;
	SetShaderParameters("depthVP", depthVP);
}

void OpenGLGraphicsManagerCommonBase::EndShadowMap(const intptr_t shadowmap,uint32_t layer_index)
{
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glDeleteFramebuffers(1, &shadowmap_framebuffer_name_);

	const GfxConfiguration& conf = g_app->GetConfiguration();
	glViewport(0, 0, conf.screen_width, conf.screen_height);
}

void OpenGLGraphicsManagerCommonBase::SetShadowMap(const intptr_t shadowmap)
{
	
}

void OpenGLGraphicsManagerCommonBase::DestroyShadowMap(intptr_t& shadowmap)
{
	GLuint id = (GLuint)shadowmap;
	glDeleteTextures(1, &id);
	shadowmap = -1;
}

void OpenGLGraphicsManagerCommonBase::SetPolygonMode(PolygonMode mode)
{
	switch (mode)
	{
	case PolygonMode::kLine:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case PolygonMode::kFill:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	default:
		break;
	}
}