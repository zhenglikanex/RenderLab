#pragma once

#include <unordered_map>

#include "Framework/Common/SceneParser.hpp"

namespace Aurora
{
	class OgexParser : public SceneParser
	{
	private:
		std::unordered_map<std::string, std::unique_ptr<BaseSceneNode>> scene_nodes_;
	};
}