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

void OpenGLGraphicsManagerCommonBase::DrawTextureOverlay(const intptr_t shadowmap, uint32_t layer_index, float vp_left, float vp_top, float vp_width, float vp_height)
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

void OpenGLGraphicsManagerCommonBase::DrawCubeMapOverlay(const intptr_t cubemap, uint32_t layer_index, float vp_left, float vp_top, float vp_width, float vp_height)
{
	GLint texture_id = (GLuint)cubemap;
	
	glActiveTexture(GL_TEXTURE0 + texture_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, texture_id);
	auto result = SetShaderParameters("depthSampler", texture_id);
	assert(result);
	result = SetShaderParameters("layer_index", (float)layer_index);
	assert(result);

	const float cell_height = vp_height * 0.5f;
	const float cell_width = vp_width * (1.0f / 3.0f);
	GLfloat vertices[] = {
		// face 1
		vp_left, vp_top, 0.0f,
		vp_left, vp_top - cell_height, 0.0f,
		vp_left + cell_width, vp_top, 0.0f,

		vp_left + cell_width, vp_top, 0.0f,
		vp_left, vp_top - cell_height, 0.0f,
		vp_left + cell_width, vp_top - cell_height, 0.0f,

		// face 2
		vp_left + cell_width, vp_top, 0.0f,
		vp_left + cell_width, vp_top - cell_height, 0.0f,
		vp_left + cell_width * 2.0f, vp_top, 0.0f,

		vp_left + cell_width * 2.0f, vp_top, 0.0f,
		vp_left + cell_width, vp_top - cell_height, 0.0f,
		vp_left + cell_width * 2.0f, vp_top - cell_height, 0.0f,

		// face 3
		vp_left + cell_width * 2.0f, vp_top, 0.0f,
		vp_left + cell_width * 2.0f, vp_top - cell_height, 0.0f,
		vp_left + cell_width * 3.0f, vp_top, 0.0f,

		vp_left + cell_width * 3.0f, vp_top, 0.0f,
		vp_left + cell_width * 2.0f, vp_top - cell_height, 0.0f,
		vp_left + cell_width * 3.0f, vp_top - cell_height, 0.0f,

		// face 4
		vp_left, vp_top - cell_height, 0.0f,
		vp_left, vp_top - cell_height * 2.0f, 0.0f,
		vp_left + cell_width, vp_top - cell_height, 0.0f,

		vp_left + cell_width, vp_top - cell_height, 0.0f,
		vp_left, vp_top - cell_height * 2.0f, 0.0f,
		vp_left + cell_width, vp_top - cell_height * 2.0f, 0.0f,

		// face 5
		vp_left + cell_width, vp_top - cell_height, 0.0f,
		vp_left + cell_width, vp_top - cell_height * 2.0f, 0.0f,
		vp_left + cell_width * 2.0f, vp_top - cell_height, 0.0f,

		vp_left + cell_width * 2.0f, vp_top - cell_height, 0.0f,
		vp_left + cell_width, vp_top - cell_height * 2.0f, 0.0f,
		vp_left + cell_width * 2.0f, vp_top - cell_height * 2.0f, 0.0f,

		// face 6
		vp_left + cell_width * 2.0f, vp_top - cell_height, 0.0f,
		vp_left + cell_width * 2.0f, vp_top - cell_height * 2.0f, 0.0f,
		vp_left + cell_width * 3.0f, vp_top - cell_height, 0.0f,

		vp_left + cell_width * 3.0f, vp_top - cell_height, 0.0f,
		vp_left + cell_width * 2.0f, vp_top - cell_height * 2.0f, 0.0f,
		vp_left + cell_width * 3.0f, vp_top - cell_height * 2.0f, 0.0f,
	};

	GLfloat uvw[] = {
		// back
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
	   -1.0f,  1.0f,  1.0f,
	   -1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
	   -1.0f,  1.0f, -1.0f,

	   // left
	   -1.0f,  1.0f,  1.0f,
	   -1.0f,  1.0f, -1.0f,
	   -1.0f, -1.0f,  1.0f,
	   -1.0f,  1.0f, -1.0f,
	   -1.0f, -1.0f,  1.0f,
	   -1.0f, -1.0f, -1.0f,

	   // front
	   -1.0f, -1.0f,  1.0f,
	   -1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
	   -1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		// right
		 1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,

		 // top
		 -1.0f,  1.0f,  1.0f,
		 -1.0f, -1.0f,  1.0f,
		  1.0f,  1.0f,  1.0f,
		  1.0f,  1.0f,  1.0f,
		 -1.0f, -1.0f,  1.0f,
		  1.0f, -1.0f,  1.0f,

		  // bottom
		  -1.0f, -1.0f, -1.0f,
		  -1.0f,  1.0f, -1.0f,
		   1.0f, -1.0f, -1.0f,
		   1.0f, -1.0f, -1.0f,
		  -1.0f,  1.0f, -1.0f,
		   1.0f,  1.0f, -1.0f
	};

	GLuint vao;
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
	
	GLuint buffer_id[2];

	glGenBuffers(2, buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false,0,0);

	glBindBuffer(GL_ARRAY_BUFFER, buffer_id[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvw), uvw, GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, 0, 0);
	glDrawArrays(GL_TRIANGLES,0,36);
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
	GLuint block_index = glGetUniformBlockIndex(current_shader_, "DrawFrameConstants");
	if (block_index == GL_INVALID_INDEX)
	{
		return true;
	}

	GLint block_size;
	if (!ubo_buffer_)
	{
		glGenBuffers(1, &ubo_buffer_);
		glBindBuffer(GL_UNIFORM_BUFFER, ubo_buffer_);
		
		glGetActiveUniformBlockiv(current_shader_, block_index, GL_UNIFORM_BLOCK_DATA_SIZE, &block_size);

		glBufferData(GL_UNIFORM_BUFFER, block_size, nullptr, GL_DYNAMIC_DRAW);
	}
	else
	{
		glBindBuffer(GL_UNIFORM_BUFFER, ubo_buffer_);
		glGetActiveUniformBlockiv(current_shader_, block_index, GL_UNIFORM_BLOCK_DATA_SIZE, &block_size);
	}

	GLubyte* block_buffer = static_cast<GLubyte*>(glMapBufferRange(GL_UNIFORM_BUFFER,0,block_size,GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT));
	
	{
		// 查询每个变量的偏移位置
		const GLchar *names[] = { "worldMatrix","viewMatrix","projectionMatrix","ambientColor","numLights" };
		GLuint indices[5];
		glGetUniformIndices(current_shader_, 5, names, indices);

		GLint offset[5];
		glGetActiveUniformsiv(current_shader_, 5, indices, GL_UNIFORM_OFFSET, offset);

		memcpy(block_buffer + offset[0], &context.world_matrix, sizeof(glm::mat4));
		memcpy(block_buffer + offset[1], &context.view_matrix, sizeof(glm::mat4));
		memcpy(block_buffer + offset[2], &context.projection_matrix, sizeof(glm::mat4));
		memcpy(block_buffer + offset[3], &context.ambient_color, sizeof(glm::vec3));

		GLint num_lights = (GLint)context.lights.size();
		memcpy(block_buffer + offset[4], &num_lights, sizeof(GLint));
	}

	for (int i = 0; i < context.lights.size(); ++i)
	{
		char uniformNames[12][256];

		sprintf(uniformNames[0x0], "allLights[%zd].lightPosition", i);
		sprintf(uniformNames[0x1], "allLights[%zd].lightColor", i);
		sprintf(uniformNames[0x2], "allLights[%zd].lightIntensity", i);
		sprintf(uniformNames[0x3], "allLights[%zd].lightDirection", i);
		sprintf(uniformNames[0x4], "allLights[%zd].lightSize", i);
		sprintf(uniformNames[0x5], "allLights[%zd].lightDistAttenCurveType", i);
		sprintf(uniformNames[0x6], "allLights[%zd].lightDistAttenCurveParams_0", i);
		sprintf(uniformNames[0x7], "allLights[%zd].lightAngleAttenCurveType", i);
		sprintf(uniformNames[0x8], "allLights[%zd].lightAngleAttenCurveParams_0", i);
		sprintf(uniformNames[0x9], "allLights[%zd].lightShadowMapIndex", i);
		sprintf(uniformNames[0xA], "allLights[%zd].lightVP", i);
		sprintf(uniformNames[0xB], "allLights[%zd].lightType", i);

		const char* names[12] = {
			uniformNames[0x0], uniformNames[0x1], uniformNames[0x2], uniformNames[0x3],
			uniformNames[0x4], uniformNames[0x5], uniformNames[0x6], uniformNames[0x7],
			uniformNames[0x8], uniformNames[0x9], uniformNames[0xA], uniformNames[0xB]
		};

		GLuint indices[12];
		glGetUniformIndices(current_shader_, 12, names, indices);
		GLint offset[12];
		glGetActiveUniformsiv(current_shader_, 12, indices, GL_UNIFORM_OFFSET, offset);


		memcpy(block_buffer + offset[0x0], &context.lights[i].light_position, sizeof(glm::vec4));
		memcpy(block_buffer + offset[0x1], &context.lights[i].light_color, sizeof(glm::vec4));
		memcpy(block_buffer + offset[0x2], &context.lights[i].light_intensity, sizeof(float));
		memcpy(block_buffer + offset[0x3], &context.lights[i].light_direction, sizeof(glm::vec4));
		memcpy(block_buffer + offset[0x4], &context.lights[i].light_size, sizeof(glm::vec2));
		memcpy(block_buffer + offset[0x5], &context.lights[i].light_dist_atten_curve_type, sizeof(int32_t));
		memcpy(block_buffer + offset[0x6], &context.lights[i].light_dist_atten_curve_params[0], sizeof(float) * 5);
		memcpy(block_buffer + offset[0x7], &context.lights[i].light_angle_atten_curve_type, sizeof(int32_t));
		memcpy(block_buffer + offset[0x8], &context.lights[i].light_angle_atten_curve_params[0], sizeof(float) * 5);
		memcpy(block_buffer + offset[0x9], &context.lights[i].light_shadowmap_index, sizeof(int32_t));
		memcpy(block_buffer + offset[0xA], &context.lights[i].light_vp, sizeof(glm::mat4));
		memcpy(block_buffer + offset[0xB], &context.lights[i].light_type, sizeof(LightType));
	}

	glUnmapBuffer(GL_UNIFORM_BUFFER);

	glUniformBlockBinding(current_shader_, block_index, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, block_index, ubo_buffer_);

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

	if (ubo_buffer_)
	{
		glDeleteBuffers(1, &ubo_buffer_);
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

	SetShaderParameters("usingDiffuseMap", false);
	SetShaderParameters("usingNormalMap", false);

	//后面根据材质进行分组渲染
	if (dbc.material)
	{
		Color color = dbc.material->GetBaseColor();
		if (color.ValueMap)
		{
			SetShaderParameters("diffuseMap", texture_index_[color.ValueMap->GetName()]);

			// 告诉shader使用texture
			SetShaderParameters("usingDiffuseMap", true);
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

intptr_t OpenGLGraphicsManagerCommonBase::GenerateShadowMap(const uint32_t width, const uint32_t height)
{
	GLuint shadowmap;

	glGenTextures(1, &shadowmap);
	glActiveTexture(GL_TEXTURE0 + shadowmap);
	glBindTexture(GL_TEXTURE_2D, shadowmap);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	return static_cast<intptr_t>(shadowmap);
}

intptr_t OpenGLGraphicsManagerCommonBase::GenerateCubeShadowMapArray(const uint32_t width, const uint32_t height, const uint32_t count)
{
	GLuint shadowmap;
	
	glGenTextures(1, &shadowmap);
	glActiveTexture(GL_TEXTURE0 + shadowmap);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, shadowmap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexStorage3D(GL_TEXTURE_CUBE_MAP_ARRAY, 1, GL_DEPTH_COMPONENT24, width, height, count * 6);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT24, width, height, count * 6, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	
	return static_cast<intptr_t>(shadowmap);
}

intptr_t OpenGLGraphicsManagerCommonBase::GenerateShadowMapArray(const uint32_t width,const uint32_t height,uint32_t count)
{
	GLuint shadowmap;
	glGenTextures(1, &shadowmap);
	glActiveTexture(GL_TEXTURE0 + shadowmap);
	glBindTexture(GL_TEXTURE_2D_ARRAY, shadowmap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_DEPTH_COMPONENT24, width, height, count);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT24, width, height,count, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	return static_cast<intptr_t>(shadowmap);
}

void OpenGLGraphicsManagerCommonBase::BeginShadowMap(const Light& light, const intptr_t shadowmap, const uint32_t width, const uint32_t height, const uint32_t layer_index)
{
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
	glViewport(0, 0, width, height);

	SetShaderParameters("depthVP", light.light_vp);
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
	GLint texture_id = (GLint)shadowmap;
	glActiveTexture(GL_TEXTURE0 + texture_id);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	float color[] = { 1.0f,1.0f,1.0f,1.0f };
	glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, color);
	bool result = SetShaderParameters("shadowMap", texture_id);
	assert(result);

}

void OpenGLGraphicsManagerCommonBase::SetShadowMaps(const Frame& frame)
{
	GLint texture_id = (GLint)frame.shadowmap;
	glActiveTexture(GL_TEXTURE0 + texture_id);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	const float color[] = { 1.0f,1.0f,1.0f,1.0f };
	glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, color);

	bool result = SetShaderParameters("shadowMap", texture_id);
	assert(result);

	texture_id = (GLint)frame.global_shadowmap;
	glActiveTexture(GL_TEXTURE0 + texture_id);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, color);

	result = SetShaderParameters("globalShadowMap", texture_id);
	assert(result);

	texture_id = (GLint)frame.cube_shadowmap;
	glActiveTexture(GL_TEXTURE0 + texture_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP_ARRAY, texture_id);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	result = SetShaderParameters("cubeShadowMap", texture_id);
	assert(result);
}

void OpenGLGraphicsManagerCommonBase::SetGlobalShadowMap(const intptr_t shadowmap)
{
	GLint texture_id = (GLint)shadowmap;
	glActiveTexture(GL_TEXTURE0 + texture_id);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture_id);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	float color[] = { 1.0f,1.0f,1.0f,1.0f };
	glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, color);
	bool result = SetShaderParameters("globalShadowMap", texture_id);
	assert(result);
}

void OpenGLGraphicsManagerCommonBase::DestroyShadowMap(intptr_t& shadowmap)
{
	GLuint id = (GLuint)shadowmap;
	glDeleteTextures(1, &id);
	shadowmap = -1;
}