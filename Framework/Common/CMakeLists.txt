add_library(Common
IApplication.hpp
Interface.hpp
IruntimeModule.hpp
GfxConfiguration.hpp
BaseApplication.hpp
BaseApplication.cpp
GraphicsManager.hpp
GraphicsManager.cpp
GeomMath.hpp
ResLoader.hpp
Engine.hpp
Engine.cpp
main.cpp
)

target_include_directories(Common PUBLIC "${PROJECT_SOURCE_DIR}/3rdparty/glm/include")
