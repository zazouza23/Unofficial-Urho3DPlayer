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