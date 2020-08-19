#pragma once

#include <memory>

#include "Framework/Common/GeomMath.hpp"

namespace Aurora {
	class MotionState
	{
	public:
		MotionState(const glm::mat4& transition) : m_Transition(transition) {}
		void SetTransition(const glm::mat4& transition) { m_Transition = transition; }
		const glm::mat4& GetTransition() const { return m_Transition; }

	private:
		glm::mat4 m_Transition;
	};
}