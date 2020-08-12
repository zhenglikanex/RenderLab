add_subdirectory(Common)
add_subdirectory(Utils)
add_subdirectory(Geometries)

SET_PROPERTY(TARGET Common PROPERTY FOLDER "Framework")
SET_PROPERTY(TARGET Utils PROPERTY FOLDER "Framework")
SET_PROPERTY(TARGET Geometries PROPERTY FOLDER "Framework")