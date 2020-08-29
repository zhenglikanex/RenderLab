#pragma once
#include <iostream>

namespace Aurora
{
	class IDrawPass
	{
	public:
		IDrawPass() = default;
		virtual ~IDrawPass() {};
		
		virtual void Draw() = 0;
	};
}