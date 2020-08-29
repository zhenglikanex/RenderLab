add_library(OpenGLRHI
        OpenGLGraphicsManagerCommonBase.hpp
        OpenGLGraphicsManagerCommonBase.cpp
        OpenGLGraphicsManager.hpp
		OpenGLGraphicsManager.cpp
        OpenGLShaderManager.hpp
        OpenGLShaderManager.cpp
        ${PROJECT_SOURCE_DIR}/3rdparty/glad/src/glad.c
)
target_include_directories(OpenGLRHI PUBLIC "${PROJECT_SOURCE_DIR}/3rdparty/glad/include")

SET_PROPERTY(TARGET OpenGLRHI PROPERTY FOLDER "RHI")