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
ResLoader.cpp
Guid.hpp
Guid.cpp
Engine.hpp
Engine.cpp
SceneObject.hpp
SceneObject.cpp
SceneNode.hpp
SceneNode.cpp
Image.hpp
main.cpp
)

target_include_directories(Common PUBLIC "${PROJECT_SOURCE_DIR}/3rdparty/glm/include")
