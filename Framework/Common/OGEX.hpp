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
		std::unique_ptr<BaseSceneNode> Parser(const std::string& buf) override
		{
			std::unique_ptr<BaseSceneNode> root_node = std::make_unique<BaseSceneNode>("scene_root");
			OGEX::OpenGexDataDescription opengex_data_desc;
			
			ODDL::DataResult result = opengex_data_desc.ProcessText(buf.c_str());
			if (result == ODDL::kDataOkay)
			{
				const ODDL::Structure* structure = opengex_data_desc.GetRootStructure()->GetFirstSubnode();
				while (structure)
				{
					ConvertOddlStructureTOSceneNode(*structure, root_node);

					structure = structure->Next();
				}
			}
			
			return root_node;
		}
	private:
		void ConvertOddlStructureTOSceneNode(const ODDL::Structure& structure, const std::unique_ptr<BaseSceneNode>& base_node)
		{
			std::unique_ptr<BaseSceneNode> node;
		
			switch (structure.GetStructureType())
			{
			case OGEX::kStructureNode:
			{
				node = std::make_unique<SceneEmptyNode>(structure.GetStructureName());
			}
			break;
			case OGEX::kStructureGeometryNode:
			{
				node = std::make_unique<SceneGeometryNode>(structure.GetStructureName());
				SceneGeometryNode& _node = dynamic_cast<SceneGeometryNode&>(*node);
				const OGEX::GeometryNodeStructure& _structure = dynamic_cast<const OGEX::GeometryNodeStructure&>(structure);

				_node.SetVisibility(_structure.GetVisibleFlag());
				_node.SetIfCastShadow(_structure.GetShadowFlag());
				_node.SetIfMotionBlur(_structure.GetMotionBlurFlag());

				//¹Ò½ÓSceneObject
				std::string _key = _structure.GetObjectStructure()->GetStructureName();
				if (!scene_objects_[_key])
				{
					scene_objects_[_key] = std::make_shared<SceneObjectGeometry>();
				}
				_node.AddSceneObjectRef(std::dynamic_pointer_cast<SceneObjectGeometry>(scene_objects_[_key]));

				auto materials = _structure.GetMaterialStructureArray();
				auto materials_count = materials.GetElementCount();
				for (auto i = 0; i < materials_count; ++i)
				{
					auto material = materials[i];
					_key = material->GetStructureName();
					if (!scene_objects_[_key])
					{
						scene_objects_[_key] = std::make_shared<SceneObjectMaterial>();
					}
					_node.AddSceneObjectRef(std::dynamic_pointer_cast<SceneObjectMaterial>(scene_objects_[_key]));
				}
			}
			break;
			case OGEX::kStructureLightNode:
			{
				node = std::make_unique<SceneLightNode>(structure.GetStructureName());
			}
			break;
			case OGEX::kStructureCameraNode:
			{
				node = std::make_unique<SceneCameraNode>(structure.GetStructureName());
			}
			break;
			case OGEX::kStructureGeometryObject:
			{

			}
			default:
				break;
			}
	
		}
	private:
		std::unordered_map<std::string, std::shared_ptr<BaseSceneObject>> scene_objects_;
	};
}