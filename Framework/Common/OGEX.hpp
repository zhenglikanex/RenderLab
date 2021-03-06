#pragma once

#include <unordered_map>
#include <memory>

#include "Framework/Common/SceneParser.hpp"

#include "OpenGEX.h"

namespace Aurora
{
	class OgexParser : public SceneParser
	{
	public:
		std::unique_ptr<Scene> Parser(const std::string& buf) override
		{
			std::unique_ptr<Scene> scene = std::make_unique<Scene>("ogex_scene");
			OGEX::OpenGexDataDescription opengex_data_desc;

			ODDL::DataResult result = opengex_data_desc.ProcessText(buf.c_str());
			if (result == ODDL::kDataOkay)
			{
				const ODDL::Structure* structure = opengex_data_desc.GetRootStructure()->GetFirstSubnode();
				while (structure)
				{
					ConvertOddlStructureToSceneNode(*structure, scene->SceneGraph,*scene);

					structure = structure->Next();
				}
			}

			return scene;
		}
	private:
		void ConvertOddlStructureToSceneNode(const ODDL::Structure& structure, const std::shared_ptr<BaseSceneNode>& base_node,Scene& scene)
		{
			std::shared_ptr<BaseSceneNode> node;

			switch (structure.GetStructureType())
			{
			case OGEX::kStructureNode:
			{
				node = std::make_unique<SceneEmptyNode>(structure.GetStructureName());
			}
			break;
			case OGEX::kStructureGeometryNode:
			{
				auto _node = std::make_shared<SceneGeometryNode>(structure.GetStructureName());
				const OGEX::GeometryNodeStructure& _structure = dynamic_cast<const OGEX::GeometryNodeStructure&>(structure);

				_node->SetVisibility(_structure.GetVisibleFlag());
				_node->SetIfCastShadow(_structure.GetShadowFlag());
				_node->SetIfMotionBlur(_structure.GetMotionBlurFlag());

				//�ҽ�SceneObject
				std::string _key = _structure.GetObjectStructure()->GetStructureName();
				_node->AddSceneObjectRef(_key);

				auto materials = _structure.GetMaterialStructureArray();
				auto materials_count = materials.GetElementCount();
				std::string material_key;
				for (auto i = 0; i < materials_count; ++i)
				{
					auto material = materials[i];
					material_key = material->GetStructureName();
					_node->AddMaterialRef(material_key);
				}

				std::string name = _structure.GetNodeName();
				scene.LUT_name_GeometryNodes.emplace(name, _node);
				scene.GeometryNodes.emplace(_key, _node);
				node = _node;
			}
			break;
			case OGEX::kStructureLightNode:
			{
				auto _node = std::make_shared<SceneLightNode>(structure.GetStructureName());
				const OGEX::LightNodeStructure& _structure = dynamic_cast<const OGEX::LightNodeStructure&>(structure);
				_node->SetIfCastShadow(_structure.GetShadowFlag());

				std::string _key = _structure.GetObjectStructure()->GetStructureName();
				_node->AddSceneObjectRef(_key);

				std::string name = _structure.GetNodeName();
				scene.LUT_name_LightNodes.emplace(name, _node);
				scene.LightNodes.emplace(_key, _node);
				node = _node;
			}
			break;
			case OGEX::kStructureCameraNode:
			{
				auto _node = std::make_shared<SceneCameraNode>(structure.GetStructureName());
				const OGEX::CameraNodeStructure& _structure = dynamic_cast<const OGEX::CameraNodeStructure&>(structure);
			
				std::string _key = _structure.GetObjectStructure()->GetStructureName();
				_node->AddSceneObjectRef(_key);

				scene.CameraNodes.emplace(_key, _node);
				node = _node;
			}
			break;
			case OGEX::kStructureGeometryObject:
			{
				const OGEX::GeometryObjectStructure& _structeure = dynamic_cast<const OGEX::GeometryObjectStructure&>(structure);
				std::string _key = _structeure.GetStructureName();
				auto _object = std::make_shared<SceneObjectGeometry>();
				_object->SetVisibility(_structeure.GetVisibleFlag());
				_object->SetIfCastShadow(_structeure.GetShadowFlag());
				_object->SetIfMotionBlur(_structeure.GetMotionBlurFlag());

				// extensions
				ODDL::Structure* extension = _structeure.GetFirstExtensionSubnode();
				while (extension)
				{
					const OGEX::ExtensionStructure* _extension = dynamic_cast<const OGEX::ExtensionStructure*>(extension);
					auto _appid = _extension->GetApplicationString();
					if (_appid == "MyGameEngine")
					{
						auto _type = _extension->GetTypeString();
						if (_type == "collision")
						{
							const ODDL::Structure* sub_structure = _extension->GetFirstCoreSubnode();
							const ODDL::DataStructure<ODDL::StringDataType>* dataStructure = static_cast<const ODDL::DataStructure<ODDL::StringDataType>*>(sub_structure);
							auto collision_type = dataStructure->GetDataElement(0);

							const ODDL::Structure* sub_structure2 = _extension->GetLastCoreSubnode();
							const ODDL::DataStructure<ODDL::FloatDataType>* dataStructure2 = static_cast<const ODDL::DataStructure<ODDL::FloatDataType>*>(sub_structure2);
							auto elementCount = dataStructure2->GetDataElementCount();
							float* _data = (float*)&dataStructure2->GetDataElement(0);
							if (collision_type == "plane")
							{
								_object->SetCollisionType(SceneObjectCollisionType::kSceneObjectCollisionTypePlane);
								_object->SetCollisionParameters(_data, elementCount);
							}
							else if (collision_type == "sphere")
							{
								_object->SetCollisionType(SceneObjectCollisionType::kSceneObjectCollisionTypeSphere);
								_object->SetCollisionParameters(_data, elementCount);
							}
							else if (collision_type == "box")
							{
								_object->SetCollisionType(SceneObjectCollisionType::kSceneObjectCollisionTypeBox);
								_object->SetCollisionParameters(_data, elementCount);
							}
							break;
						}
					}
					extension = extension->Next();
				}

				const ODDL::Map<OGEX::MeshStructure>* _meshs = _structeure.GetMeshMap();
				int32_t _count = _meshs->GetElementCount();
				for (int32_t i = 0; i < _count; ++i)
				{
					const OGEX::MeshStructure* _mesh = (*_meshs)[i];
					std::shared_ptr<SceneObjectMesh> mesh = std::make_shared<SceneObjectMesh>();
					const std::string _primitive_type = static_cast<const char*>(_mesh->GetMeshPrimitive());
					if (_primitive_type == "points")
					{
						mesh->SetPrimitiveType(PrimitiveType::kPrimitiveTypePointList);
					}
					else if (_primitive_type == "lines")
					{
						mesh->SetPrimitiveType(PrimitiveType::kPrimitiveTypeLineList);
					}
					else if (_primitive_type == "line_strip")
					{
						mesh->SetPrimitiveType(PrimitiveType::kPrimitiveTypeLineStrip);
					}
					else if (_primitive_type == "triangles")
					{
						mesh->SetPrimitiveType(PrimitiveType::kPrimitiveTypeTriList);
					}
					else if (_primitive_type == "triangles_strip")
					{
						mesh->SetPrimitiveType(PrimitiveType::kPrimitiveTypeTriStrip);
					}
					else if (_primitive_type == "quads")
					{
						mesh->SetPrimitiveType(PrimitiveType::kPrimitiveTypeQuadList);
					}
					else
					{
						mesh.reset();
					}

					if (mesh)
					{
						const ODDL::Structure* sub_structure = _mesh->GetFirstSubnode();
						while (sub_structure)
						{
							switch (sub_structure->GetStructureType())
							{
							case OGEX::kStructureVertexArray:
							{
								const OGEX::VertexArrayStructure* _v = dynamic_cast<const OGEX::VertexArrayStructure*>(sub_structure);
								const char* attr = _v->GetArrayAttrib();
								auto morph_index = _v->GetMorphIndex();

								const ODDL::Structure* _data_structrue = _v->GetFirstCoreSubnode();
								const ODDL::DataStructure<FloatDataType>* dataStructrue = dynamic_cast<const ODDL::DataStructure<FloatDataType>*>(_data_structrue);
								
								auto arraySize = dataStructrue->GetArraySize();
								auto elementCount = dataStructrue->GetDataElementCount();
								const void* _data = &dataStructrue->GetDataElement(0);
								void* data = new float[elementCount];
								size_t buf_size = sizeof(float) * elementCount;
								memcpy(data, _data, buf_size);
								VertexDataType vertexDataType;
								switch (arraySize)
								{
								case 1:
									vertexDataType = VertexDataType::kVertexDataTypeFloat1;
									break;
								case 2:
									vertexDataType = VertexDataType::kVertexDataTypeFloat2;
									break;
								case 3:
									vertexDataType = VertexDataType::kVertexDataTypeFloat3;
									break;
								case 4:
									vertexDataType = VertexDataType::kVertexDataTypeFloat4;
									break;
								default:
									continue;
								}
								SceneObjectVertexArray _v_array(attr, morph_index, vertexDataType, data, elementCount);
								mesh->AddVertexArray(std::move(_v_array));
							}
							break;
							case OGEX::kStructureIndexArray:
							{
								const OGEX::IndexArrayStructure* _i = dynamic_cast<const OGEX::IndexArrayStructure*>(sub_structure);
								auto material_index = _i->GetMaterialIndex();
								auto restart_index = _i->GetRestartIndex();
								const ODDL::Structure* _data_structure = _i->GetFirstCoreSubnode();
								ODDL::StructureType type = _data_structure->GetStructureType();
								int32_t elementCount = 0;
								const void* _data = nullptr;
								IndexDataType index_type = IndexDataType::kIndexDataTypeInt16;
								switch (type)
								{
								case ODDL::kDataUnsignedInt8:
								{
									index_type = IndexDataType::kIndexDataTypeInt8;
									const ODDL::DataStructure<UnsignedInt8DataType>* dataStructure = dynamic_cast<const ODDL::DataStructure<UnsignedInt8DataType>*>(_data_structure);
									elementCount = dataStructure->GetDataElementCount();
									_data = &dataStructure->GetDataElement(0);
								}break;
								case ODDL::kDataUnsignedInt16:
								{
									index_type = IndexDataType::kIndexDataTypeInt16;
									const ODDL::DataStructure<UnsignedInt16DataType>* dataStructure = dynamic_cast<const ODDL::DataStructure<UnsignedInt16DataType>*>(_data_structure);
									elementCount = dataStructure->GetDataElementCount();
									_data = &dataStructure->GetDataElement(0);
								}break;
								case ODDL::kDataUnsignedInt32:
								{
									index_type = IndexDataType::kIndexDataTypeInt32;
									const ODDL::DataStructure<UnsignedInt32DataType>* dataStructure = dynamic_cast<const ODDL::DataStructure<UnsignedInt32DataType>*>(_data_structure);
									elementCount = dataStructure->GetDataElementCount();
									_data = &dataStructure->GetDataElement(0);
								}break;
								case ODDL::kDataUnsignedInt64:
								{
									index_type = IndexDataType::kIndexDataTypeInt64;
									const ODDL::DataStructure<UnsignedInt64DataType>* dataStructure = dynamic_cast<const ODDL::DataStructure<UnsignedInt64DataType>*>(_data_structure);
									elementCount = dataStructure->GetDataElementCount();
									_data = &dataStructure->GetDataElement(0);
								}break;
								default:
									break;
								}
								int32_t data_size = 0;
								switch (index_type)
								{
								case IndexDataType::kIndexDataTypeInt8:
									data_size = 1;
									break;
								case IndexDataType::kIndexDataTypeInt16:
									data_size = 2;
									break;
								case IndexDataType::kIndexDataTypeInt32:
									data_size = 4;
									break;
								case IndexDataType::kIndexDataTypeInt64:
									data_size = 8;
									break;
								default:
									break;
								}

								size_t buf_size = elementCount * data_size;
								void* data = new uint8_t[buf_size];
								memcpy(data, _data, buf_size);
								SceneObjectIndexArray _i_array(material_index, restart_index, index_type, data, elementCount);
								mesh->AddIndexArray(std::move(_i_array));

							}break;
							default:
								break;

							}
							
							sub_structure = sub_structure->Next();
						}

						_object->AddMesh(mesh);
					}
				}
				scene.Geometries[_key] = _object;
				
			}return;
			case OGEX::kStructureTransform:
			{
				int32_t index, count;
				const OGEX::TransformStructure& _structure = dynamic_cast<const OGEX::TransformStructure&>(structure);
				bool object_flag = _structure.GetObjectFlag();
				glm::mat4 matrix;
				std::shared_ptr<SceneObjectTransform> transform;
				
				count = _structure.GetTransformCount();
				for (index = 0; index < count; ++index)
				{
					const float* data = _structure.GetTransform(index);
					memcpy(&matrix, data, sizeof(matrix));
					transform = std::make_shared<SceneObjectTransform>(matrix, object_flag);
					base_node->PrependTransform(std::move(transform));
				}
			}return;
			case OGEX::kStructureTranslation:
			{
				const OGEX::TranslationStructure& _structure = dynamic_cast<const OGEX::TranslationStructure&>(structure);
				bool object_flag = _structure.GetObjectFlag();
				std::shared_ptr<SceneObjectTranslation> translation;

				auto kind = _structure.GetTranslationKind();
				auto data = _structure.GetTranslation();
				if (kind == "xyz")
				{
					translation = std::make_shared<SceneObjectTranslation>(data[0], data[1], data[2]);
				}
				else
				{
					translation = std::make_shared<SceneObjectTranslation>(kind[0], data[0]);
				}
				base_node->PrependTransform(std::move(translation));
			}return;
			case OGEX::kStructureMaterial:
			{
				const OGEX::MaterialStructure& _structure = dynamic_cast<const OGEX::MaterialStructure&>(structure);
				std::string material_name;
				const char* _name = _structure.GetMaterialName();
				std::string _key = _structure.GetStructureName();
				auto material = std::make_shared<SceneObjectMaterial>();
				material->SetName(_name);

				const ODDL::Structure* _sub_structure = _structure.GetFirstCoreSubnode();
				while (_sub_structure) {
					std::string attrib, textureName;
					glm::vec4 color;
					float param;
					switch (_sub_structure->GetStructureType())
					{
					case OGEX::kStructureColor:
					{
						attrib = dynamic_cast<const OGEX::ColorStructure*>(_sub_structure)->GetAttribString();
						memcpy(&color, dynamic_cast<const OGEX::ColorStructure*>(_sub_structure)->GetColor(), sizeof(color));
						material->SetColor(attrib, color);
					}break;
					case OGEX::kStructureParam:
					{
						attrib = dynamic_cast<const OGEX::ParamStructure*>(_sub_structure)->GetAttribString();
						param = dynamic_cast<const OGEX::ParamStructure*>(_sub_structure)->GetParam();
						material->SetParam(attrib, param);
					}break;
					case OGEX::kStructureTexture:
					{
						attrib = dynamic_cast<const OGEX::TextureStructure*>(_sub_structure)->GetAttribString();
						textureName = dynamic_cast<const OGEX::TextureStructure*>(_sub_structure)->GetTextureName();
						material->SetTexture(attrib, textureName);
					}break;
					default:
						break;
					}
					_sub_structure = _sub_structure->Next();
				}
				scene.Materials.emplace(_key, material);

			}return;
			case OGEX::kStructureLightObject:
			{
				const OGEX::LightObjectStructure& _structure = dynamic_cast<const OGEX::LightObjectStructure&>(structure);
				std::string _type_str = _structure.GetTypeString();
				const bool _bshadow = _structure.GetShadowFlag();
				std::string _key = _structure.GetStructureName();
				std::shared_ptr<SceneObjectLight> light;
				
				if (_type_str == "infinite")
				{
					light = std::make_shared<SceneObjectInfiniteLight>();
				}
				else if (_type_str == "point")
				{
					light = std::make_shared<SceneObjectOmniLight>();
				}
				else if (_type_str == "spot")
				{
					light = std::make_shared<SceneObjectSpotLight>();
				}
				else if (_type_str == "area")
				{
					light = std::make_shared<SceneObjectAreaLight>();
				}

				light->SetIfCastShadow(_structure.GetShadowFlag());

				const ODDL::Structure* _sub_structure = _structure.GetFirstCoreSubnode();
				while (_sub_structure) {
					std::string attrib, textureName;
					glm::vec4 color;
					float param;
					switch (_sub_structure->GetStructureType())
					{
					case OGEX::kStructureColor:
					{
						attrib = dynamic_cast<const OGEX::ColorStructure*>(_sub_structure)->GetAttribString();
						const float* _color = dynamic_cast<const OGEX::ColorStructure*>(_sub_structure)->GetColor();
						memcpy(&color,_color, sizeof(glm::vec4));
						light->SetColor(attrib, color);
					}
					break;
					case OGEX::kStructureParam:
					{
						attrib = dynamic_cast<const OGEX::ParamStructure*>(_sub_structure)->GetAttribString();
						param = dynamic_cast<const OGEX::ParamStructure*>(_sub_structure)->GetParam();
						light->SetParam(attrib, param);
					}
					break;
					case OGEX::kStructureTexture:
					{
						attrib = dynamic_cast<const OGEX::TextureStructure*>(_sub_structure)->GetAttribString();
						textureName = dynamic_cast<const OGEX::TextureStructure*>(_sub_structure)->GetTextureName();
						light->SetTexture(attrib, textureName);
					}
					break;
					case OGEX::kStructureAtten:
					{
						// TODO: truly implement it
						auto atten = dynamic_cast<const OGEX::AttenStructure*>(_sub_structure);
						AttenCurve curve;
						if (atten->GetCurveType() == "linear")
						{
							curve.type = AttenCurveType::kLinear;
							curve.u.linear_params.begin_atten = atten->GetBeginParam();
							curve.u.linear_params.end_atten = atten->GetEndParam();
						}
						else if (atten->GetCurveType() == "smooth")
						{
							curve.type = AttenCurveType::kSmooth;
							curve.u.smooth_params.begin_atten = atten->GetBeginParam();
							curve.u.smooth_params.end_atten = atten->GetEndParam();
						}
						else if (atten->GetCurveType() == "inverse")
						{
							curve.type = AttenCurveType::kInverse;
							curve.u.inverse_params.scale = atten->GetScaleParam();
							curve.u.inverse_params.offset = atten->GetOffsetParam();
							curve.u.inverse_params.k1 = atten->GetLinearParam();
							curve.u.inverse_params.kc = atten->GetConstantParam();
						}
						else if (atten->GetCurveType() == "inverse_square")
						{
							curve.type = AttenCurveType::kInverseSquare;
							curve.u.inverse_squre_params.scale = atten->GetScaleParam();
							curve.u.inverse_squre_params.offset = atten->GetOffsetParam();
							curve.u.inverse_squre_params.kq = atten->GetQuadraticParam();
							curve.u.inverse_squre_params.k1 = atten->GetLinearParam();
							curve.u.inverse_squre_params.kc = atten->GetConstantParam();
						}
						
						if (atten->GetAttenKind() == "angle")
						{
							auto _light = std::dynamic_pointer_cast<SceneObjectSpotLight>(light);
							_light->SetAngleAttenuation(curve);
						}
						else if(atten->GetAttenKind() == "cos_angle")
						{
							auto _light = std::dynamic_pointer_cast<SceneObjectSpotLight>(light);
							_light->SetAngleAttenuation(curve);
						}
						else
						{
							light->SetDistanceAttenuation(curve);
						}
					}
					break;
					default:
						;
					};

					_sub_structure = _sub_structure->Next();
				}

				ODDL::Structure* extension = _structure.GetFirstExtensionSubnode();
				while (extension)
				{
					const OGEX::ExtensionStructure* _extension = dynamic_cast<const OGEX::ExtensionStructure*>(extension);
					auto _appid = _extension->GetApplicationString();
					if (_appid == "MyGameEngine")
					{
						auto _type = _extension->GetTypeString();
						if (_type == "area_light")
						{
							const ODDL::Structure* sub_structure = _extension->GetFirstCoreSubnode();
							const ODDL::DataStructure<ODDL::FloatDataType>* dataStructure1 = static_cast<const ODDL::DataStructure<ODDL::FloatDataType>*>(sub_structure);
							auto elementCount = dataStructure1->GetDataElementCount();
							assert(elementCount == 2);
							auto width = dataStructure1->GetDataElement(0);
							auto height = dataStructure1->GetDataElement(1);

							auto _light = dynamic_pointer_cast<SceneObjectAreaLight>(light);
							_light->SetDimension({ width, height });
						}
					}

					extension = extension->Next();
				}

				scene.Lights[_key] = light;

			}return;
			case OGEX::kStructureCameraObject:
			{
				const OGEX::CameraObjectStructure& _structure = dynamic_cast<const OGEX::CameraObjectStructure&>(structure);
				std::string _key = _structure.GetStructureName();
				auto camera = std::make_shared<SceneObjectPerspectiveCamera>();

				const ODDL::Structure* _sub_structure = _structure.GetFirstCoreSubnode();
				while (_sub_structure)
				{
					std::string attrib, textureName;
					glm::vec4 color;
					float param;
					switch (_sub_structure->GetStructureType())
					{
					case OGEX::kStructureColor:
					{
						attrib = dynamic_cast<const OGEX::ColorStructure*>(_sub_structure)->GetAttribString();
						const float* _color = dynamic_cast<const OGEX::ColorStructure*>(_sub_structure)->GetColor();
						memcpy(&color, _color, sizeof(glm::vec4));
						camera->SetColor(attrib, color);
						
					}break;
					case OGEX::kStructureParam:
					{
						attrib = dynamic_cast<const OGEX::ParamStructure*>(_sub_structure)->GetAttribString();
						param = dynamic_cast<const OGEX::ParamStructure*>(_sub_structure)->GetParam();
						camera->SetParam(attrib, param);
					}break;
					case OGEX::kStructureTexture:
					{
						attrib = dynamic_cast<const OGEX::TextureStructure*>(_sub_structure)->GetAttribString();
						textureName = dynamic_cast<const OGEX::TextureStructure*>(_sub_structure)->GetTextureName();
						camera->SetTexture(attrib, textureName);
					}break;
					default:
						break;
					}
					_sub_structure = _sub_structure->Next();
				}
				scene.Cameras.emplace(_key, camera);
			}return;
			default:
				return;
			}

			const ODDL::Structure* sub_structure = structure.GetFirstSubnode();
			while (sub_structure)
			{
				ConvertOddlStructureToSceneNode(*sub_structure, node,scene);
				sub_structure = sub_structure->Next();
			}

			base_node->AppendChild(std::move(node));
		}
	private:
	};
}