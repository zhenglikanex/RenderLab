#pragma once
#include <list>
#include <memory>

namespace Aurora
{
	class TreeNode
	{
	public:
		virtual ~TreeNode() {}

		virtual void AppendChild(std::shared_ptr<TreeNode>&& sub_node)
		{
			sub_node->parent_ = this;
			children_.push_back(std::move(sub_node));
		}
	private:
		TreeNode* parent_;
		std::list<std::shared_ptr<TreeNode>> children_;
	};
}