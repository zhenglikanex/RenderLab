#include "Framework/Common/BaseApplication.hpp"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace Aurora
{
	class GLFWApplication : public BaseApplication
	{
	public:
		GLFWApplication(GfxConfiguration& cfg);
		~GLFWApplication();

		bool Initialize() override;
		void Finalize() override;
		void Tick() override;
	private:
		GLFWwindow* window_;
	};
}