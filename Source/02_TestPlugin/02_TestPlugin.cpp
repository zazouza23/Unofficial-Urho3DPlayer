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