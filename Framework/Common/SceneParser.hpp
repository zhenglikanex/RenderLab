#pragma once

#include <memory>
#include <string>

#include "Framework/Common/Scene.hpp"

namespace Aurora
{
	class SceneParser
	{
	public:
		virtual std::unique_ptr<Scene> Parser(const std::string& buf) = 0;
	};
}