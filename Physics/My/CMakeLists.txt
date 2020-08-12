add_library(MyPhysics
        MyPhysicsManager.hpp
		MyPhysicsManager.cpp
        RigidBody.hpp
        MotionState.hpp
)

SET_PROPERTY(TARGET MyPhysics PROPERTY FOLDER "Physics")