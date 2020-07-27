add_subdirectory(HelloD3D)
add_subdirectory(HelloOpenGL)
add_subdirectory(HelloGLFW)

SET_PROPERTY(TARGET HelloD3D PROPERTY FOLDER "Test")
SET_PROPERTY(TARGET HelloOpenGL PROPERTY FOLDER "Test")
SET_PROPERTY(TARGET HelloGLFW PROPERTY FOLDER "Test")