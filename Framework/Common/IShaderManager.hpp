#pragma once
#include "Framework/Common/IRuntimeModule.hpp"

namespace Aurora
{
	class IShaderManager : public IRuntimeModule
	{
	public:
		virtual ~IShaderManager() = default;

		virtual bool InitializeShaders() = 0;
		virtual void ClearShaders() = 0;

		virtual void* GetDefaultShaderProgram() = 0;

#ifdef DEBUG
		virtual void* GetDebugShaderProgram() = 0;
#endif
	};
}