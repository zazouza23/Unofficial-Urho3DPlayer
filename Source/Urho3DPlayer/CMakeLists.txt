
set (TARGET_NAME Urho3DPlayer)

file(WRITE Info.h
     "#pragma once\n
inline const char* GetUrhoVersion() { return \"${URHO3D_VERSION}\"; }\n
inline const char* GetCompilerID() { return \"${CMAKE_CXX_COMPILER_ID}\"; }\n
inline const char* GetCompilerVersion() { return \"${CMAKE_CXX_COMPILER_VERSION}\"; }"
)

define_source_files ()

setup_main_executable ()