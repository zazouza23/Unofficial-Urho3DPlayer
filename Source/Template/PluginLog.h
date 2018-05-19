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

#include "../IO/Log.h"
#include "PluginInfo.h"

using namespace Urho3D;

// Special hack log system
// The reason for this class it is local static logging work only on the main application.
class PluginLog : public Log
{
	URHO3D_OBJECT(PluginLog, Log);

public:

	explicit PluginLog(Context* context) :
		Log(context)
	{
	}
};

// Redefine macro logging on this special context 
#ifdef URHO3D_LOGGING
#undef URHO3D_LOGTRACE
#undef URHO3D_LOGDEBUG
#undef URHO3D_LOGINFO
#undef URHO3D_LOGWARNING
#undef URHO3D_LOGERROR
#undef URHO3D_LOGRAW
#undef URHO3D_LOGTRACEF
#undef URHO3D_LOGDEBUGF
#undef URHO3D_LOGINFOF
#undef URHO3D_LOGWARNINGF
#undef URHO3D_LOGERRORF
#undef URHO3D_LOGRAWF
#define URHO3D_LOGTRACE(message) PluginLog::Write(Urho3D::LOG_TRACE, message)
#define URHO3D_LOGDEBUG(message) PluginLog::Write(Urho3D::LOG_DEBUG, message)
#define URHO3D_LOGINFO(message) PluginLog::Write(Urho3D::LOG_INFO, message)
#define URHO3D_LOGWARNING(message) PluginLog::Write(Urho3D::LOG_WARNING, message)
#define URHO3D_LOGERROR(message) PluginLog::Write(Urho3D::LOG_ERROR, message)
#define URHO3D_LOGRAW(message) PluginLog::WriteRaw(message)
#define URHO3D_LOGTRACEF(format, ...) PluginLog::Write(Urho3D::LOG_TRACE, Urho3D::ToString(format, ##__VA_ARGS__))
#define URHO3D_LOGDEBUGF(format, ...) PluginLog::Write(Urho3D::LOG_DEBUG, Urho3D::ToString(format, ##__VA_ARGS__))
#define URHO3D_LOGINFOF(format, ...) PluginLog::Write(Urho3D::LOG_INFO, Urho3D::ToString(format, ##__VA_ARGS__))
#define URHO3D_LOGWARNINGF(format, ...) PluginLog::Write(Urho3D::LOG_WARNING, Urho3D::ToString(format, ##__VA_ARGS__))
#define URHO3D_LOGERRORF(format, ...) PluginLog::Write(Urho3D::LOG_ERROR, Urho3D::ToString(format, ##__VA_ARGS__))
#define URHO3D_LOGRAWF(format, ...) PluginLog::WriteRaw(Urho3D::ToString(format, ##__VA_ARGS__))
#endif
