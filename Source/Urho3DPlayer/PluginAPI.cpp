//
// Copyright (c) 2008-2018 the Urho3DPlayer project.
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

#include "../AngelScript/APITemplates.h"
#include "../Core/Context.h"

#include "Plugin.h"
#include "PluginAPI.h"

void RegisterPlugin(Context* context, asIScriptEngine* engine)
{
	RegisterObject<Plugin>(engine, "Plugin");
	engine->RegisterObjectMethod("Plugin", "bool Load(const String& name, bool forceToStart = true)", asMETHOD(Plugin, Load), asCALL_THISCALL);
	engine->RegisterObjectMethod("Plugin", "void Unload(const String& name, bool forceToStop = true)", asMETHOD(Plugin, Unload), asCALL_THISCALL);
	engine->RegisterObjectMethod("Plugin", "void UnloadAll()", asMETHOD(Plugin, UnloadAll), asCALL_THISCALL);
	engine->RegisterObjectMethod("Plugin", "bool get_isLoaded()", asMETHOD(Plugin, IsLoaded), asCALL_THISCALL);
	engine->RegisterObjectMethod("Plugin", "bool get_empty()", asMETHOD(Plugin, Empty), asCALL_THISCALL);

	static Context* staticContext = context;
	engine->RegisterGlobalFunction("Plugin@+ get_plugin()", asFUNCTIONPR([]() {
		return staticContext->GetSubsystem<Plugin>(); }, (), Plugin*), asCALL_CDECL);
}