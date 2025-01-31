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

   externalproject "vk-bootstrap"
   location "pzEngine-Core/Third_Party/vk-bootstrap/build/"
   uuid "860c8470-e01f-11ef-a7bf-0800200c9a66"
   kind "StaticLib"
   language "C++"

group ""

group "Core"
	include "pzEngine-Core/Build-Core.lua"
group ""

group "Misc"
   include "App/Build-App.lua"
group ""