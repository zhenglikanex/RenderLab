add_subdirectory(HelloD3D)
add_subdirectory(HelloOpenGL)
add_subdirectory(HelloGLFW)
add_subdirectory(BulletTest)

SET_PROPERTY(TARGET HelloD3D PROPERTY FOLDER "Test")
SET_PROPERTY(TARGET HelloOpenGL PROPERTY FOLDER "Test")
SET_PROPERTY(TARGET HelloGLFW PROPERTY FOLDER "Test")
SET_PROPERTY(TARGET BulletTest PROPERTY FOLDER "Test")