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

#pragma once

#include <Urho3D/Core/Context.h>

using namespace Urho3D;

class Plugin : public Object
{
	URHO3D_OBJECT(Plugin, Object);

	friend class Urho3DPlayer;
	public:
		/// Construct.
		Plugin(Context* context);

		~Plugin();

		/// Load plugin return true if successfull. 
		/// Usefull to force start if the plugin is loaded on the runtime.
		bool Load(const String& name, bool forceToStart = false);
		/// Unload plugin.
		void Unload(const String& name, bool forceToStop = false);
		/// Unload all plugins.
		void UnloadAll();
		/// Check if the plugin is loaded
		bool IsLoaded(const String& name) const;
		/// Verify if have plugin object
		bool Empty() const;

	protected:

		class PluginObject
		{
		public:

			const char* (*GetUrhoCompatibleVersion)();
			const char* (*GetCompatibleCompilatorName)();
			const char* (*GetCompatibleCompilatorVersion)();
			const char* (*GetCompatibleOSVersion)();
			const char* (*GetCompatibleGraphicAPI)();

			void(*CreatePluginApplication)(Context*);
			void(*DestroyPluginApplication)(Context*);

			void(*Setup)(VariantMap& parameters);
			void(*Start)();
			void(*Stop)();
			void(*OnScriptBinding)(const char*, void*);

			void* handle_ = nullptr;
		};

		/// Setup all plugin application in same time of setup application (use on internal application only).
		void Setup(VariantMap& parameters);
		/// Start all plugin application in same time of start application (use on internal application only).
		void Start();
		/// Stop all plugin application in same time of stop application (use on internal application only).
		void Stop();
		/// Call on script binding and send scriptContext. Use to extend script binding in the plugin. 
		void OnScriptBinding(const String scriptTypeName, void* scriptContext);

		HashMap<String, PluginObject> pluginObjects_;
};