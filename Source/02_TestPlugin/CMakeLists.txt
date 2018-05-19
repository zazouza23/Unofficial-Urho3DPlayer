#
# Copyright (c) 2008-2018 the Urho3D project.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

# Define target name
set(TARGET_NAME 02_TestPlugin)

# Define to detect graphic api
if (URHO3D_OPENGL)
	Set(GRAPHIC_APINAME GL2)
else ()
	if (URHO3D_D3D11)
		Set(GRAPHIC_APINAME D3D11)
	else()
		Set(GRAPHIC_APINAME D3D9)
	endif()
endif()

#Create a file info
file(WRITE PluginInfo.h
     "#pragma once\n
#define PLUGIN_NAME \"${TARGET_NAME}\"\n
#define PluginLog PluginLog_${TARGET_NAME}\n
inline const char* GetGraphicAPIName() { return \"${GRAPHIC_APINAME}\"; }\n
inline const char* GetUrhoVersion() { return \"${URHO3D_VERSION}\"; }\n
inline const char* GetCompilerID() { return \"${CMAKE_CXX_COMPILER_ID}\"; }\n
inline const char* GetCompilerVersion() { return \"${CMAKE_CXX_COMPILER_VERSION}\"; }"
)

# Define source files
define_source_files()

# Setup target in dynamic lyb
setup_library(SHARED)
