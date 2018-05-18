# Unofficial-Urho3DPlayer
-----------------------------------------------------------------------------------

Description
-----------------------------------------------------------------------------------
This is the unofficial Urho3DPlayer similar to the original except that it is integrated a plug-in system. 

How to install
-----------------------------------------------------------------------------------

#### 1. Go to the official page of Urho3D

1.1 Go to this page https://urho3d.github.io/ and follows the steps to install and build urho3D with source code

#### 2. Download and install Unofficial Urho3DPlayer with CMake

1.1 Download and copy those files and folders from Urho

* CMake/
* cmake_generic.bat (or .sh for non windows)

1.2. Copy one or more of the other cmake_*.bat (Windows) or cmake_*.sh (Unix/Linux) files from Urho, depending on the build system you want to use. If you are for example using Codeblocks copy cmake_codeblocks.bat / cmake_codeblocks.sh.

1.3. Copy your bin folder from Urho3D or your own project with a CoreData and a Data folder inside. 

#### 3. Prepare Build

1.1 Use CMake to prepare the build. If you don't know to use it, read info on this page: https://github.com/urho3d/Urho3D/wiki/Setting-up-a-Project-(CMake)  

#### 4. Build

How this work
-----------------------------------------------------------------------------------

Open your console on your os system and when you want open Urho3DPlayer add option :
```
  52_HelloWorldPlugin.as -plugin 01_TestPlugin -plugin 02_TestPlugin
```
It is supposed to get the same result as on the screenshot.

Other option, It is possible to download directly your plugin on Angelscript with call:
```
  plugin.Load("MyPluginName");
``` 
But keep in mind that you have to reload your script related to your plugin. 

Screenshot
-----------------------------------------------------------------------------------
![alt tag](https://github.com/zazouza23/Unofficial-Urho3DPlayer/blob/master/Screenshot/TestPlugin.png)

How to create your own plug-in
-----------------------------------------------------------------------------------
Just add .cpp and .h files from Source/Template on your project.
And look at 01_TestPlugin or 02_testPlugin to know how this work. 


---  
### License
The MIT License (MIT)

