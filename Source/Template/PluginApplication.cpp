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

#include "../Core/Thread.h"

#include "PluginApplication.h"

PluginApplication::PluginApplication(Context* context) :
	Object(context)
{
	// Create special plugin log (see PluginLog.h to know why)
	auto* pluginLog = new PluginLog(context_);

	// ToDo: (bug here)
	// Problem to Register on the subsystem because macro URHO3D_OBJECT create bug id on Type
	// and conflic with other log from other plugin
	//context_->RegisterSubsystem(pluginLog);

	// Assume this class is create on main thread
	Thread::SetMainThread();

	// Copy data from main log and init new file
	auto* log = GetSubsystem<Log>();
	pluginLog->SetLevel(log->GetLevel());
	pluginLog->SetQuiet(log->IsQuiet());
	pluginLog->Open(PLUGIN_NAME + String(".log"));

}