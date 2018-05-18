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

#include "../Core/CoreEvents.h"
#include "01_TestPlugin.h"

URHO3D_DEFINE_PLUGIN_APPLICATION(TestPlugin, GetUrhoVersion(), GetCompilerID(), GetCompilerVersion(), "", "")

TestPlugin::TestPlugin(Context* context) :
	PluginApplication(context)
{
	URHO3D_LOGINFO("Create Test Plugin");

	SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(TestPlugin, HandleUpdate));
}

void TestPlugin::HandleUpdate(StringHash eventType, VariantMap& eventData)
{
	URHO3D_LOGINFO("Yes! 01_TestPlugin is loaded!!! :D");

	UnsubscribeFromEvent(E_UPDATE);
}

void TestPlugin::Setup(VariantMap& parameters)
{
	URHO3D_LOGINFO("Setup Test Plugin");
}

void TestPlugin::Start()
{
	URHO3D_LOGINFO("Start Test Plugin");
}

void TestPlugin::Stop()
{
	URHO3D_LOGINFO("Stop Test Plugin");
}

void TestPlugin::OnScriptBinding(const char* scriptTypeName, void* scriptContext)
{
	URHO3D_LOGINFO("The script type name is: \"" + String(scriptTypeName) + "\"");
}