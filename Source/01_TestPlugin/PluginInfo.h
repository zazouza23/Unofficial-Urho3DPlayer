#pragma once

#define PLUGIN_NAME "01_TestPlugin"

#define PluginLog PluginLog_01_TestPlugin

inline const char* GetGraphicAPIName() { return "D3D11"; }

inline const char* GetUrhoVersion() { return "Unversioned"; }

inline const char* GetCompilerID() { return "MSVC"; }

inline const char* GetCompilerVersion() { return "19.13.26129.0"; }