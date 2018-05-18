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

#ifdef URHO3D_ANGELSCRIPT
#include <Urho3D/AngelScript/ScriptFile.h>
#include <Urho3D/AngelScript/Script.h>
#endif
#include <Urho3D/Core/Main.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Engine/EngineDefs.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Audio/Audio.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Core/EventProfiler.h>
#ifdef URHO3D_NETWORK
#include <Urho3D/Network/Network.h>
#endif
#ifdef URHO3D_LUA
#include <Urho3D/LuaScript/LuaScript.h>
#endif
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/ResourceEvents.h>

#include "PluginAPI.h"
#include "Urho3DPlayer.h"
#include "SDL/SDL.h"

#include <Urho3D/DebugNew.h>

URHO3D_DEFINE_APPLICATION_MAIN(Urho3DPlayer);

Urho3DPlayer::Urho3DPlayer(Context* context) :
    Application(context),
    commandLineRead_(false)
{
	plugin_ = new Plugin(context_);
	context_->RegisterSubsystem(plugin_);
}

void Urho3DPlayer::Setup()
{

    // Web platform depends on the resource system to read any data files. Skip parsing the command line file now
    // and try later when the resource system is live
#ifndef __EMSCRIPTEN__
    // Read command line from a file if no arguments given. This is primarily intended for mobile platforms.
    // Note that the command file name uses a hardcoded path that does not utilize the resource system
    // properly (including resource path prefix), as the resource system is not yet initialized at this point
    auto* filesystem = GetSubsystem<FileSystem>();
    const String commandFileName = filesystem->GetProgramDir() + "Data/CommandLine.txt";
    if (GetArguments().Empty() && filesystem->FileExists(commandFileName))
    {
        SharedPtr<File> commandFile(new File(context_, commandFileName));
        if (commandFile->IsOpen())
        {
            commandLineRead_ = true;
            String commandLine = commandFile->ReadLine();
            commandFile->Close();
            ParseArguments(commandLine, false);
            // Reparse engine startup parameters now
            engineParameters_ = Engine::ParseParameters(GetArguments());
        }
    }
#endif

    // Check for script file name from the arguments
    GetScriptFileName();

	// Get plugin's name and path to load dynamic library soon
	GetPluginsName();

#ifndef __EMSCRIPTEN__
    // Show usage if not found
    if ((GetArguments().Size() || commandLineRead_) && scriptFileName_.Empty())
    {
        ErrorExit("Usage: Urho3DPlayer <scriptfile> [options]\n\n"
            "The script file should implement the function void Start() for initializing the "
            "application and subscribing to all necessary events, such as the frame update.\n"
            #ifndef _WIN32
            "\nCommand line options:\n"
            "-x <res>     Horizontal resolution\n"
            "-y <res>     Vertical resolution\n"
            "-m <level>   Enable hardware multisampling\n"
            "-v           Enable vertical sync\n"
            "-t           Enable triple buffering\n"
            "-w           Start in windowed mode\n"
            "-s           Enable resizing when in windowed mode\n"
            "-q           Enable quiet mode which does not log to standard output stream\n"
            "-b <length>  Sound buffer length in milliseconds\n"
            "-r <freq>    Sound mixing frequency in Hz\n"
            "-pp <paths>  Resource prefix path(s), separated by semicolons, default to executable path\n"
            "The resource prefix paths can also be defined using URHO3D_PREFIX_PATH env - var\n"
            "When both are defined, the paths set by -pp takes higher precedence\n"
            "-p <paths>   Resource path(s) to use, separated by semicolons, default to 'Data;CoreData'\n"
            "-pf <files>  Resource package file to use, separated by semicolons, default to none\n"
            "-ap <paths>  Resource autoload path(s), separated by semicolons, default to 'AutoLoad'\n"
            "-log <level> Change the log level, valid 'level' values: 'debug', 'info', 'warning', 'error'\n"
            "-ds <file>   Dump used shader variations to a file for precaching\n"
            "-mq <level>  Material quality level, default 2 (high)\n"
            "-tq <level>  Texture quality level, default 2 (high)\n"
            "-tf <level>  Texture filter mode, default 2 (trilinear)\n"
            "-af <level>  Texture anisotropy level, default 4. Also sets anisotropic filter mode\n"
            "-gl2         Force OpenGL 2 use even if OpenGL 3 is available\n"
            "-flushgpu    Flush GPU command queue each frame. Effective only on Direct3D\n"
            "-borderless  Borderless window mode\n"
            "-lowdpi      Force low DPI mode on Retina display\n"
            "-headless    Headless mode. No application window will be created\n"
            "-landscape   Use landscape orientations (iOS only, default)\n"
            "-portrait    Use portrait orientations (iOS only)\n"
            "-monitor <num> Monitor number to use\n"
            "-hz <freq>   Monitor refresh rate to use\n"
            "-prepass     Use light pre-pass rendering\n"
            "-deferred    Use deferred rendering\n"
            "-renderpath <name> Use the named renderpath (must enter full resource name)\n"
            "-lqshadows   Use low-quality (1-sample) shadow filtering\n"
            "-noshadows   Disable shadow rendering\n"
            "-nolimit     Disable frame limiter\n"
            "-nothreads   Disable worker threads\n"
            "-nosound     Disable sound output\n"
            "-noip        Disable sound mixing interpolation\n"
            "-touch       Touch emulation on desktop platform\n"
			"-plugin <name> Named plugin to load (must enter relative path but not necessary to enter extension)\n"
            #endif
        );
    }
    else
    {
        // Use the script file name as the base name for the log file
        engineParameters_[EP_LOG_NAME] = filesystem->GetAppPreferencesDir("urho3d", "logs") + GetFileNameAndExtension(scriptFileName_) + ".log";
    }
#else
    // On Web platform setup a default windowed resolution similar to the executable samples
    engineParameters_[EP_FULL_SCREEN]  = false;
#endif

    // Construct a search path to find the resource prefix with two entries:
    // The first entry is an empty path which will be substituted with program/bin directory -- this entry is for binary when it is still in build tree
    // The second and third entries are possible relative paths from the installed program/bin directory to the asset directory -- these entries are for binary when it is in the Urho3D SDK installation location
    if (!engineParameters_.Contains(EP_RESOURCE_PREFIX_PATHS))
        engineParameters_[EP_RESOURCE_PREFIX_PATHS] = ";../share/Resources;../share/Urho3D/Resources";
	
}

void Urho3DPlayer::Start()
{
	// First load plugin on start ( on setup we have obcure crash because the engine not initialized yet )
	// Call setup plugin and force to reinitialize engine in case if some parameters update.
	for (String pluginName : pluginsName_)
		plugin_->Load(pluginName);

	VariantMap newParameters;
	plugin_->Setup(newParameters);

	if(!plugin_->Empty() && newParameters.Size() > 0)
		Reinitialize(newParameters);

	plugin_->Start();

    // Reattempt reading the command line from the resource system now if not read before
    // Note that the engine can not be reconfigured at this point; only the script name can be specified
    if (GetArguments().Empty() && !commandLineRead_)
    {
        SharedPtr<File> commandFile = GetSubsystem<ResourceCache>()->GetFile("CommandLine.txt", false);
        if (commandFile)
        {
            String commandLine = commandFile->ReadLine();
            commandFile->Close();
            ParseArguments(commandLine, false);
        }

        GetScriptFileName();
    }

    if (scriptFileName_.Empty())
    {
        ErrorExit("Script file name not specified; cannot proceed");
        return;
    }

    String extension = GetExtension(scriptFileName_);
    if (extension != ".lua" && extension != ".luc")
    {
#ifdef URHO3D_ANGELSCRIPT
        // Instantiate and register the AngelScript subsystem
		Script* script = new Script(context_);
        context_->RegisterSubsystem(script);
		RegisterPlugin(context_, script->GetScriptEngine());

		plugin_->OnScriptBinding("Angelscript", script->GetImmediateContext());

        // Hold a shared pointer to the script file to make sure it is not unloaded during runtime
        scriptFile_ = GetSubsystem<ResourceCache>()->GetResource<ScriptFile>(scriptFileName_);

        /// \hack If we are running the editor, also instantiate Lua subsystem to enable editing Lua ScriptInstances
#ifdef URHO3D_LUA
        if (scriptFileName_.Contains("Editor.as", false))
		{
			auto* luaScript = new LuaScript(context_);
            context_->RegisterSubsystem(luaScript);

			plugin_->OnScriptBinding("Lua", luaScript->GetState());
		}
#endif
        // If script loading is successful, proceed to main loop
        if (scriptFile_ && scriptFile_->Execute("void Start()"))
        {	
            // Subscribe to script's reload event to allow live-reload of the application
            SubscribeToEvent(scriptFile_, E_RELOADSTARTED, URHO3D_HANDLER(Urho3DPlayer, HandleScriptReloadStarted));
            SubscribeToEvent(scriptFile_, E_RELOADFINISHED, URHO3D_HANDLER(Urho3DPlayer, HandleScriptReloadFinished));
            SubscribeToEvent(scriptFile_, E_RELOADFAILED, URHO3D_HANDLER(Urho3DPlayer, HandleScriptReloadFailed));
            return;
        }
#else
        ErrorExit("AngelScript is not enabled!");
        return;
#endif
    }
    else
    {
#ifdef URHO3D_LUA
        // Instantiate and register the Lua script subsystem
        auto* luaScript = new LuaScript(context_);
        context_->RegisterSubsystem(luaScript);

		plugin_->OnScriptBinding("Lua", luaScript->GetState());

        // If script loading is successful, proceed to main loop
        if (luaScript->ExecuteFile(scriptFileName_))
        {
            luaScript->ExecuteFunction("Start");
            return;
        }
#else
        ErrorExit("Lua is not enabled!");
        return;
#endif
    }

    // The script was not successfully loaded. Show the last error message and do not run the main loop
    ErrorExit();
}

void Urho3DPlayer::Stop()
{
#ifdef URHO3D_ANGELSCRIPT
    if (scriptFile_)
    {
        // Execute the optional stop function
        if (scriptFile_->GetFunction("void Stop()"))
            scriptFile_->Execute("void Stop()");
    }
#else
    if (false)
    {
    }
#endif

#ifdef URHO3D_LUA
    else
    {
        auto* luaScript = GetSubsystem<LuaScript>();
        if (luaScript && luaScript->GetFunction("Stop", true))
            luaScript->ExecuteFunction("Stop");
    }
#endif

	plugin_->Stop();
}

void Urho3DPlayer::HandleScriptReloadStarted(StringHash eventType, VariantMap& eventData)
{
#ifdef URHO3D_ANGELSCRIPT
    if (scriptFile_->GetFunction("void Stop()"))
        scriptFile_->Execute("void Stop()");
#endif
}

void Urho3DPlayer::HandleScriptReloadFinished(StringHash eventType, VariantMap& eventData)
{
#ifdef URHO3D_ANGELSCRIPT
    // Restart the script application after reload
    if (!scriptFile_->Execute("void Start()"))
    {
        scriptFile_.Reset();
        ErrorExit();
    }
#endif
}

void Urho3DPlayer::HandleScriptReloadFailed(StringHash eventType, VariantMap& eventData)
{
#ifdef URHO3D_ANGELSCRIPT
    scriptFile_.Reset();
    ErrorExit();
#endif
}

void Urho3DPlayer::GetScriptFileName()
{
    const Vector<String>& arguments = GetArguments();
    if (arguments.Size() && arguments[0][0] != '-')
        scriptFileName_ = GetInternalPath(arguments[0]);
}

void Urho3DPlayer::GetPluginsName()
{
	const Vector<String>& arguments = GetArguments();
	for (unsigned i = 0; i < arguments.Size(); ++i)
	{
		if (arguments[i].Length() > 1 && arguments[i][0] == '-')
		{
			String argument = arguments[i].Substring(1).ToLower();
			String value = i + 1 < arguments.Size() ? arguments[i + 1] : String::EMPTY;

			if (argument == "plugin")
				pluginsName_.Push(value);
		}
	}
}

void Urho3DPlayer::Reinitialize(VariantMap& parameters)
{

	auto* log = GetSubsystem<Log>();
	if (log)
	{
		if (engine_->HasParameter(parameters, EP_LOG_LEVEL))
			log->SetLevel(engine_->GetParameter(parameters, EP_LOG_LEVEL).GetInt());
		if (engine_->HasParameter(parameters, EP_LOG_QUIET))
			log->SetQuiet(engine_->GetParameter(parameters, EP_LOG_QUIET, false).GetBool());
		if (engine_->HasParameter(parameters, EP_LOG_NAME))
			log->Open(engine_->GetParameter(parameters, EP_LOG_NAME).GetString());
	}

	// Configure max FPS
	if(engine_->HasParameter(parameters, EP_FRAME_LIMITER) && 
		engine_->GetParameter(parameters, EP_FRAME_LIMITER, true) == false)
	engine_->SetMaxFps(0);

	// Add resource paths
	bool reinitializeResourceCache = false;

	if (engine_->HasParameter(parameters, EP_RESOURCE_PREFIX_PATHS))
		reinitializeResourceCache = true;
	else
		parameters[EP_RESOURCE_PREFIX_PATHS] = engine_->GetParameter(engineParameters_, EP_RESOURCE_PREFIX_PATHS, String::EMPTY);
	if (engine_->HasParameter(parameters, EP_RESOURCE_PATHS))
		reinitializeResourceCache = true;
	else
		parameters[EP_RESOURCE_PATHS] = engine_->GetParameter(engineParameters_, EP_RESOURCE_PATHS, "Data;CoreData");
	if (engine_->HasParameter(parameters, EP_RESOURCE_PACKAGES))
		reinitializeResourceCache = true;
	else
		parameters[EP_RESOURCE_PACKAGES] = engineParameters_[EP_RESOURCE_PACKAGES];
	if (engine_->HasParameter(parameters, EP_AUTOLOAD_PATHS))
		reinitializeResourceCache = true;
	else
		parameters[EP_AUTOLOAD_PATHS] = parameters[EP_RESOURCE_PATHS] = engine_->GetParameter(engineParameters_, EP_AUTOLOAD_PATHS, "Autoload");

	if(reinitializeResourceCache)
		engine_->InitializeResourceCache(parameters);

	auto* graphics = GetSubsystem<Graphics>();
	auto* renderer = GetSubsystem<Renderer>();
	auto* cache = GetSubsystem<ResourceCache>();

	if (engine_->HasParameter(parameters, EP_EXTERNAL_WINDOW))
		graphics->SetExternalWindow(engine_->GetParameter(parameters, EP_EXTERNAL_WINDOW).GetVoidPtr());
	if (engine_->HasParameter(parameters, EP_WINDOW_TITLE))
		graphics->SetWindowTitle(engine_->GetParameter(parameters, EP_WINDOW_TITLE).GetString());
	if (engine_->HasParameter(parameters, EP_WINDOW_ICON))
		graphics->SetWindowIcon(cache->GetResource<Image>(engine_->GetParameter(parameters, EP_WINDOW_ICON).GetString()));
	if (engine_->HasParameter(parameters, EP_FLUSH_GPU))
		graphics->SetFlushGPU(engine_->GetParameter(parameters, EP_FLUSH_GPU).GetBool());
	if (engine_->HasParameter(parameters, EP_ORIENTATIONS))
		graphics->SetOrientations(engine_->GetParameter(parameters, EP_ORIENTATIONS).GetString());
	if (engine_->HasParameter(parameters, EP_WINDOW_POSITION_X) && engine_->HasParameter(parameters, EP_WINDOW_POSITION_Y))
		graphics->SetWindowPosition(engine_->GetParameter(parameters, EP_WINDOW_POSITION_X).GetInt(),
			engine_->GetParameter(parameters, EP_WINDOW_POSITION_Y).GetInt());

#ifdef URHO3D_OPENGL
	if (engine_->HasParameter(parameters, EP_FORCE_GL2))
		graphics->SetForceGL2(engine_->GetParameter(parameters, EP_FORCE_GL2).GetBool());
#endif

	if (!engine_->HasParameter(parameters, EP_WINDOW_WIDTH))
		parameters[EP_WINDOW_WIDTH] = engine_->GetParameter(engineParameters_, EP_WINDOW_WIDTH, 0);
	if (!engine_->HasParameter(parameters, EP_WINDOW_HEIGHT))
		parameters[EP_WINDOW_HEIGHT] = engine_->GetParameter(engineParameters_, EP_WINDOW_HEIGHT, 0);
	if(!engine_->HasParameter(parameters, EP_FULL_SCREEN))
		parameters[EP_FULL_SCREEN] = engine_->GetParameter(engineParameters_, EP_FULL_SCREEN, true);
	if(!engine_->HasParameter(parameters, EP_BORDERLESS))
		parameters[EP_BORDERLESS] = engine_->GetParameter(engineParameters_, EP_BORDERLESS, false);
	if(!engine_->HasParameter(parameters, EP_WINDOW_RESIZABLE))
		parameters[EP_WINDOW_RESIZABLE] = engine_->GetParameter(engineParameters_, EP_WINDOW_RESIZABLE, false);
	if(!engine_->HasParameter(parameters, EP_HIGH_DPI))
		parameters[EP_HIGH_DPI] = engine_->GetParameter(engineParameters_, EP_HIGH_DPI, true);
	if(!engine_->HasParameter(parameters, EP_VSYNC))
		parameters[EP_VSYNC] = engine_->GetParameter(engineParameters_, EP_VSYNC, false);
	if(!engine_->HasParameter(parameters, EP_TRIPLE_BUFFER))
		parameters[EP_TRIPLE_BUFFER] = engine_->GetParameter(engineParameters_, EP_TRIPLE_BUFFER, false);
	if(!engine_->HasParameter(parameters, EP_MULTI_SAMPLE))
		parameters[EP_MULTI_SAMPLE] = engine_->GetParameter(engineParameters_, EP_MULTI_SAMPLE, 1);
	if(!engine_->HasParameter(parameters, EP_MONITOR))
		parameters[EP_MONITOR] = engine_->GetParameter(engineParameters_, EP_MONITOR, 0);
	if(!engine_->HasParameter(parameters, EP_REFRESH_RATE))
		parameters[EP_REFRESH_RATE] = engine_->GetParameter(engineParameters_, EP_REFRESH_RATE, 0);

	graphics->SetMode(
		engine_->GetParameter(parameters, EP_WINDOW_WIDTH).GetInt(),
		engine_->GetParameter(parameters, EP_WINDOW_HEIGHT).GetInt(),
		engine_->GetParameter(parameters, EP_FULL_SCREEN).GetBool(),
		engine_->GetParameter(parameters, EP_BORDERLESS).GetBool(),
		engine_->GetParameter(parameters, EP_WINDOW_RESIZABLE).GetBool(),
		engine_->GetParameter(parameters, EP_HIGH_DPI).GetBool(),
		engine_->GetParameter(parameters, EP_VSYNC).GetBool(),
		engine_->GetParameter(parameters, EP_TRIPLE_BUFFER).GetBool(),
		engine_->GetParameter(parameters, EP_MULTI_SAMPLE).GetInt(),
		engine_->GetParameter(parameters, EP_MONITOR).GetInt(),
		engine_->GetParameter(parameters, EP_REFRESH_RATE).GetInt());

	if (engine_->HasParameter(parameters, EP_SHADER_CACHE_DIR))
		graphics->SetShaderCacheDir(engine_->GetParameter(parameters, EP_SHADER_CACHE_DIR).GetString());
	if (engine_->HasParameter(parameters, EP_DUMP_SHADERS))
		graphics->BeginDumpShaders(engine_->GetParameter(parameters, EP_DUMP_SHADERS).GetString());
	if (engine_->HasParameter(parameters, EP_RENDER_PATH))
		renderer->SetDefaultRenderPath(cache->GetResource<XMLFile>(engine_->GetParameter(parameters, EP_RENDER_PATH).GetString()));

	if (engine_->HasParameter(parameters, EP_SHADOWS))
		renderer->SetDrawShadows(engine_->GetParameter(parameters, EP_SHADOWS, true).GetBool());
	if (renderer->GetDrawShadows() && 
		engine_->HasParameter(parameters, EP_LOW_QUALITY_SHADOWS) && 
		engine_->GetParameter(parameters, EP_LOW_QUALITY_SHADOWS, false).GetBool())
		renderer->SetShadowQuality(SHADOWQUALITY_SIMPLE_16BIT);

	if(engine_->HasParameter(parameters, EP_MATERIAL_QUALITY))
		renderer->SetMaterialQuality(engine_->GetParameter(parameters, EP_MATERIAL_QUALITY).GetInt());
	if(engine_->HasParameter(parameters, EP_TEXTURE_QUALITY))
		renderer->SetTextureQuality(engine_->GetParameter(parameters, EP_TEXTURE_QUALITY).GetInt());
	if (engine_->HasParameter(parameters, EP_TEXTURE_FILTER_MODE))
		renderer->SetTextureFilterMode((TextureFilterMode)engine_->GetParameter(parameters, EP_TEXTURE_FILTER_MODE).GetInt());
	if (engine_->HasParameter(parameters, EP_TEXTURE_ANISOTROPY))
		renderer->SetTextureAnisotropy(engine_->GetParameter(parameters, EP_TEXTURE_ANISOTROPY).GetInt());

	if (engine_->GetParameter(parameters, EP_SOUND, true).GetBool())
	{
		GetSubsystem<Audio>()->SetMode(
			engine_->GetParameter(parameters, EP_SOUND_BUFFER, 100).GetInt(),
			engine_->GetParameter(parameters, EP_SOUND_MIX_RATE, 44100).GetInt(),
			engine_->GetParameter(parameters, EP_SOUND_STEREO, true).GetBool(),
			engine_->GetParameter(parameters, EP_SOUND_INTERPOLATION, true).GetBool()
		);

	}

	// Initialize input
	if (engine_->HasParameter(parameters, EP_TOUCH_EMULATION))
		GetSubsystem<Input>()->SetTouchEmulation(engine_->GetParameter(parameters, EP_TOUCH_EMULATION).GetBool());

	// Initialize network
#ifdef URHO3D_NETWORK
	if (engine_->HasParameter(parameters, EP_PACKAGE_CACHE_DIR))
		GetSubsystem<Network>()->SetPackageCacheDir(engine_->GetParameter(parameters, EP_PACKAGE_CACHE_DIR).GetString());
#endif

#ifdef URHO3D_PROFILING
	if (engine_->HasParameter(parameters, EP_EVENT_PROFILER) &&
		engine_->GetParameter(parameters, EP_EVENT_PROFILER).GetBool())
	{
		EventProfiler::SetActive(true);
	}

#endif
}
