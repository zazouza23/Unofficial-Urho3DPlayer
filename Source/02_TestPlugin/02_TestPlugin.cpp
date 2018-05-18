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
#include "../Resource/ResourceCache.h"
#include "../UI/UI.h"
#include "../UI/Text.h"
#include "../UI/Font.h"
#include "02_TestPlugin.h"

URHO3D_DEFINE_PLUGIN_APPLICATION(TestPlugin, GetUrhoVersion(), GetCompilerID(), GetCompilerVersion(), "", GetGraphicAPIName())

TestPlugin::TestPlugin(Context* context) :
	PluginApplication(context)
{
}

void TestPlugin::Setup(VariantMap& parameters)
{
}

void TestPlugin::Start()
{
	// Construct new Text object
	SharedPtr<Text> helloText(new Text(context_));

	// Set Name
	helloText->SetName("HelloWorld");

	// Set String to display
	helloText->SetText("Hello World from 02_TestPlugin!");

	// Set font and text color
	helloText->SetFont(GetSubsystem<ResourceCache>()->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 30);
	helloText->SetColor(Color(0.0f, 1.0f, 0.0f));

	// Align Text center-screen
	helloText->SetHorizontalAlignment(HA_CENTER);
	helloText->SetVerticalAlignment(VA_CENTER);

	// Add Text instance to the UI root element
	GetSubsystem<UI>()->GetRoot()->AddChild(helloText);
}

void TestPlugin::Stop()
{
}

void TestPlugin::OnScriptBinding(const char* scriptTypeName, void* scriptContext)
{
}