#pragma once

#include "Framework/Common/IRuntimeModule.hpp"

namespace Aurora
{
	enum class DefaultShaderIndex
	{
		ShadowMap = 0,
		Forward,
		Differed,
		Debug,
		Copy,
		Water,
		SkyBox,
	};

	class IShaderManager : public IRuntimeModule
	{
	public:
		virtual bool InitializeShaders() = 0;
		virtual void ClearShaders() = 0;

		virtual intptr_t GetDefaultShaderProgram(DefaultShaderIndex index) = 0;
	private:
	};
}