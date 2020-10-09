add_subdirectory(Common)
add_subdirectory(Utils)
add_subdirectory(Geometries)
add_subdirectory(DrawPass)

SET_PROPERTY(TARGET Common PROPERTY FOLDER "Framework")
SET_PROPERTY(TARGET Utils PROPERTY FOLDER "Framework")
SET_PROPERTY(TARGET Geometries PROPERTY FOLDER "Framework")
SET_PROPERTY(TARGET DrawPass PROPERTY FOLDER "Framework")