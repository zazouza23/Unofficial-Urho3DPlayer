set(TARGET_NAME 02_TestPlugin)

if (URHO3D_OPENGL)
	Set(GRAPHICS_APINAME GL2)
else ()
	if (URHO3D_D3D11)
		Set(GRAPHICS_APINAME D3D11)
	else()
		Set(GRAPHICS_APINAME D3D9)
	endif()
endif()

file(WRITE PluginInfo.h
     "#pragma once\n
#define PLUGIN_NAME \"${TARGET_NAME}\"\n
#define PluginLog PluginLog_${TARGET_NAME}\n
inline const char* GetGraphicAPIName() { return \"${GRAPHICS_APINAME}\"; }\n
inline const char* GetUrhoVersion() { return \"${URHO3D_VERSION}\"; }\n
inline const char* GetCompilerID() { return \"${CMAKE_CXX_COMPILER_ID}\"; }\n
inline const char* GetCompilerVersion() { return \"${CMAKE_CXX_COMPILER_VERSION}\"; }"
)

define_source_files()

setup_library(SHARED)
