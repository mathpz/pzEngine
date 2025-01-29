include "Dependencies.lua"

workspace "pzEngine"
   architecture "x86_64"
   startproject "First-App"
   
   configurations { "Debug", "Release", "Dist" }

   flags
   {
      "MultiProcessorCompile"
   }

   OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Dependencies"
   include "pzEngine-Core/Third_Party/glfw"
   include "pzEngine-Core/Third_Party/imgui"
group ""

group "Core"
	include "pzEngine-Core/Build-Core.lua"
group ""

group "Misc"
   include "App/Build-App.lua"
group ""