-- premake5.lua
workspace "pzEngine"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "First-App"

   OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Dependencies"
   include "pzEngine-Core/vendor/GLFW"
group ""

group "Core"
	include "pzEngine-Core/Build-Core.lua"
group ""

include "App/Build-App.lua"