#include <Urho3D/IO/Log.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Container/Vector.h>
#include <Urho3D/Core/ProcessUtils.h>
#include <Urho3D/Graphics/Graphics.h>
#include <SDL/SDL.h>

#include "Plugin.h"
#include "Info.h"

#ifdef _WIN32
static const char* EXTENTION_PLUGIN_NAME = ".dll";
#elif __APPLE__
static const char* EXTENTION_PLUGIN_NAME = ".dylib";
#elif __linux__
static const char* EXTENTION_PLUGIN_NAME = ".so";
#elif __EMSCRIPTEN__
static const char* EXTENTION_PLUGIN_NAME = ".js";
#else
static const char* EXTENTION_PLUGIN_NAME = "";
#endif

// Macro helper to load function from plugin
#define LOAD_FUNCTION( Type, FunctionName) \
	pluginObject.FunctionName = Type SDL_LoadFunction(pluginObject.handle_, #FunctionName);\
	if (!pluginObject.FunctionName)\
	{\
		SDL_UnloadObject(pluginObject.handle_);\
		URHO3D_LOGERROR("Unfind \""+ String(#FunctionName) +"\" on plugin: \"" + name + "\"!");\
		return false;\
	}

// Macro helper to check plugin info and compatibility
#define CHECK_INFO( GetPluginInfo, GetLocalInfo ) \
	LOAD_FUNCTION( (const char* (*)()), GetPluginInfo ) \
	else\
	{\
		String pluginInfo(pluginObject.GetPluginInfo());\
		Vector<String> infos = pluginInfo.Split(';');\
	\
		if (!infos.Empty())\
		{\
			String version = GetLocalInfo();\
			bool isIncompatible = true;\
			for(String info : infos)\
			{\
				if (version == info)\
				{\
					isIncompatible = false;\
					break;\
				}\
			}\
			if(isIncompatible)\
			{\
				SDL_UnloadObject(pluginObject.handle_);\
				URHO3D_LOGWARNING("Incompatible \"" + String(#GetLocalInfo) + "\" with \"" + String(#GetPluginInfo) + "\"!");\
				return false;\
			}\
		}\
	}

Plugin::Plugin(Context* context) :
	Object(context)
{
}

Plugin::~Plugin()
{
	UnloadAll();
}

bool Plugin::Load(const String& name, bool forceToStart)
{
	URHO3D_LOGINFO("Load plugin function");
	PluginObject pluginObject;

	// Add or replace extention with current platform
	ReplaceExtension(name, String(EXTENTION_PLUGIN_NAME));

	// First load handle.
	pluginObject.handle_ = SDL_LoadObject(name.CString());
	if (!pluginObject.handle_)
	{
		URHO3D_LOGERROR("Unfind plugin: \"" + name + "\"!");
		return false;
	}
	// Get compatible urho3D version and compare.
	CHECK_INFO(GetUrhoCompatibleVersion, GetUrhoVersion)

	// Get compatible compilator name and compare.
	CHECK_INFO(GetCompatibleCompilatorName, GetCompilerID)

	// Get compatible compilator version and compare.
	CHECK_INFO(GetCompatibleCompilatorVersion, GetCompilerVersion)

	// Get compatible OS version and compare.
	CHECK_INFO(GetCompatibleOSVersion, GetOSVersion )

	// Get compatible graphics API and compare.
	auto graphics = GetSubsystem<Graphics>();
	CHECK_INFO(GetCompatibleGraphicAPI, graphics->GetApiName)

	// Load main plugin function.
	LOAD_FUNCTION((void(*)(Context*)), CreatePluginApplication)
	LOAD_FUNCTION((void(*)(Context*)), DestroyPluginApplication)
	LOAD_FUNCTION((void(*)(VariantMap& parameters)), Setup)
	LOAD_FUNCTION((void(*)()), Start)
	LOAD_FUNCTION((void(*)()), Stop)
	LOAD_FUNCTION((void(*)(const char*, void*)), OnScriptBinding)

	// Construct plugin application
	pluginObject.CreatePluginApplication(context_);

	// Force to start in case is loaded on the runtime
	if (forceToStart)
		pluginObject.Start();

	// Set on the memory.
	pluginObjects_[name] = pluginObject;

	return true;
}

void Plugin::Unload(const String& name, bool forceToStop)
{
	HashMap<String, PluginObject>::Iterator i = pluginObjects_.Find(name);
	if (i == pluginObjects_.End())
	{
		URHO3D_LOGWARNING("Impossible to unload plugin: \"" + name + "\" unfinding.");
		return;
	}

	if (forceToStop)
		i->second_.Stop();

	i->second_.DestroyPluginApplication(context_);
	pluginObjects_.Erase(i);
}

void Plugin::UnloadAll()
{
	for (HashMap<String, PluginObject>::Iterator i = pluginObjects_.Begin(); i != pluginObjects_.End(); ++i)
	{
		i->second_.DestroyPluginApplication(context_);
	}

	pluginObjects_.Clear();
}

bool Plugin::IsLoaded(const String& name) const
{
	HashMap<String, PluginObject>::ConstIterator i = pluginObjects_.Find(name);
	return (i != pluginObjects_.End());
}

void Plugin::Setup(VariantMap& parameters)
{
	for (HashMap<String, PluginObject>::Iterator i = pluginObjects_.Begin(); i != pluginObjects_.End(); ++i)
	{
		i->second_.Setup(parameters);
	}
}

void Plugin::Start()
{
	for (HashMap<String, PluginObject>::Iterator i = pluginObjects_.Begin(); i != pluginObjects_.End(); ++i)
	{
		i->second_.Start();
	}
}

void Plugin::Stop()
{
	for (HashMap<String, PluginObject>::Iterator i = pluginObjects_.Begin(); i != pluginObjects_.End(); ++i)
	{
		i->second_.Stop();
	}
}

bool Plugin::Empty() const
{
	return pluginObjects_.Empty();
}

void Plugin::OnScriptBinding(const String scriptTypeName, void* scriptContext)
{
	for (HashMap<String, PluginObject>::Iterator i = pluginObjects_.Begin(); i != pluginObjects_.End(); ++i)
	{
		i->second_.OnScriptBinding(scriptTypeName.CString(), scriptContext);
	}
}

