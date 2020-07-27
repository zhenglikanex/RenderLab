#pragma once

namespace Aurora
{
	class IRuntimeModule
	{
	public:
		virtual ~IRuntimeModule() = 0 {};

		virtual bool Initialize() = 0;
		virtual void Finalize() = 0;
		virtual void Tick() = 0;
	};
}
