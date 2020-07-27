#pragma once

#include <memory>
#include <list>

namespace Aurora
{
	class BaseSceneNode
	{
	public:
		std::list<std::unique_ptr<BaseSceneNode>> children_;
		//std::list<std::unique_ptr<>>
	protected:

	};
}