#pragma once

#include <memory>
#include <string>

#include "Framework/Common/SceneNode.hpp"

namespace Aurora
{
	class SceneParser
	{
	public:
		virtual std::unique_ptr<BaseSceneNode> Parser(const std::string& buf) = 0;
	};
}