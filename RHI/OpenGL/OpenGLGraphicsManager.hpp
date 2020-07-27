#include "Framework/Common/GraphicsManager.hpp"

#include "glad/glad.h"

namespace Aurora
{
	class OpenGLGraphicsManager : public GraphicsManager
	{
	public:
		OpenGLGraphicsManager(GLADloadproc loader);

		bool Initialize() override;
		void Finalize() override;

		void Tick() override;
	private:
		GLADloadproc loader_;
	};
}