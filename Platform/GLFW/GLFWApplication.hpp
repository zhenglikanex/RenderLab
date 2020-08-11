#include "Framework/Common/BaseApplication.hpp"

struct GLFWwindow;

namespace Aurora
{
	class GLFWApplication : public BaseApplication
	{
	public:
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	public:
		GLFWApplication(GfxConfiguration& cfg,std::unique_ptr<Engine>&& engine);
		~GLFWApplication();

		bool Initialize() override;
		void Finalize() override;
		void Tick() override;
	private:
		GLFWwindow* window_;
	};
}