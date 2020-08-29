#pragma once

#include <unordered_map>

#include "Framework/Common/IShaderManager.hpp"

namespace std
{
	template<>
	struct hash<const Aurora::DefaultShaderIndex>
	{
		size_t operator()(const Aurora::DefaultShaderIndex& index) const
		{
			std::hash<std::int32_t> hasher;
			return static_cast<size_t>(hasher((int32_t)index));
		}
	};
}

namespace Aurora
{	
	class ShaderManager : public IShaderManager
	{
	public:
		virtual ~ShaderManager() = default;

		intptr_t GetDefaultShaderProgram(DefaultShaderIndex index) override
		{
			return default_shaders_[index];
		}

	protected:
		std::unordered_map<const DefaultShaderIndex, intptr_t> default_shaders_;
	};
}