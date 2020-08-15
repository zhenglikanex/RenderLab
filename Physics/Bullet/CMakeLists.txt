add_library(BulletPhysics
        BulletPhysicsManager.hpp
		BulletPhysicsManager.cpp
)

SET_PROPERTY(TARGET BulletPhysics PROPERTY FOLDER "Physics")