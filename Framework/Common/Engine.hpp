#pragma once

#include <memory>

#include "Framework/Common/IRuntimeModule.hpp"

namespace Aurora
{
	class GraphicsManager;

	class Engine : public IRuntimeModule
	{
	public:
		Engine(std::unique_ptr<GraphicsManager>&& graphics_manager);

		bool Initialize() override;
		void Finalize() override;

		void Tick() override;
	private:
		std::unique_ptr<GraphicsManager> graphics_manager_;
	};
}