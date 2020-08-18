add_library(MyPhysics
        MyPhysicsManager.hpp
		MyPhysicsManager.cpp
        RigidBody.hpp
        MotionState.hpp
)
target_link_libraries(MyPhysics
Geometries)

SET_PROPERTY(TARGET MyPhysics PROPERTY FOLDER "Physics")