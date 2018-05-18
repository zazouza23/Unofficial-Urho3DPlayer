//
// Copyright (c) 2008-2018 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#include "../Core/Context.h"

#include "PluginLog.h"

using namespace Urho3D;

class PluginApplication : public Object
{
	URHO3D_OBJECT(PluginApplication, Object);

public:

	PluginApplication(Context* context);

	virtual void Setup(VariantMap& parameters) { }

	virtual void Start() { }

	virtual void Stop() { }

	virtual void OnScriptBinding(const char* scriptTypeName, void* scriptContext) { }
};

#ifdef __cplusplus  
#define START_EXPORT extern "C" { 
#define END_IMPORT } 
#else
#define START_EXPORT
#define END_IMPORT 
#endif

#ifdef WIN32
#define PLUGIN_EXPORT __declspec(dllexport)
#else
#define PLUGIN_EXPORT 
#endif

#define URHO3D_DEFINE_PLUGIN_APPLICATION(className, urhoVersion, compilatorName, compilatorVersion, OSVersion, graphicAPI) \
START_EXPORT \
\
PluginApplication* pluginApp; \
\
	PLUGIN_EXPORT const char* GetUrhoCompatibleVersion(void) \
	{ \
		return urhoVersion; \
	} \
\
	PLUGIN_EXPORT const char* GetCompatibleCompilatorName(void) \
	{ \
		return compilatorName; \
	} \
\
	PLUGIN_EXPORT const char* GetCompatibleCompilatorVersion(void) \
	{ \
		return compilatorVersion; \
	} \
\
	PLUGIN_EXPORT const char* GetCompatibleOSVersion(void) \
	{ \
		return OSVersion; \
	} \
\
PLUGIN_EXPORT const char* GetCompatibleGraphicAPI(void) \
	{ \
		return graphicAPI; \
	} \
\
PLUGIN_EXPORT void CreatePluginApplication(Context* context) \
	{ \
		pluginApp = new className(context); \
	} \
\
PLUGIN_EXPORT void DestroyPluginApplication(Context* context) \
	{ \
		delete pluginApp; \
		pluginApp = nullptr; \
	} \
\
PLUGIN_EXPORT void Setup(VariantMap& parameters) \
	{ \
		pluginApp->Setup(parameters); \
	} \
\
PLUGIN_EXPORT void Start(void) \
	{ \
		pluginApp->Start(); \
	} \
\
PLUGIN_EXPORT void Stop(void) \
	{ \
		pluginApp->Stop(); \
	} \
\
PLUGIN_EXPORT void OnScriptBinding(const char* scriptTypeName, void* scriptContext) \
	{ \
		pluginApp->OnScriptBinding(scriptTypeName, scriptContext); \
	} \
\
END_IMPORT 