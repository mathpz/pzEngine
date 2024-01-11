-- premake5.lua
workspace "pzEngine"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "App"

   OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Core"
	include "pzEngine-Core/Build-Core.lua"
group ""

include "App/Build-App.lua"


-- WINDOWS COMPILE BUILDING WITH GMAKE2
-- g++ -m64 -std=c++17 -c -g -I/f/programmingProjects/pzEngine/Vendor/VulkanSDKWindows/Vulkan.1.3.236.0/include -MMD -MP -MF "vkInstance.o.d" -o vkInstance.o vkInstance.cpp
-- Windows LINK WHEN BUILDING WITH GMAKE2
-- g++ -m64 -std=c++17 -o firsttest vkInstance.o -L/c/msys64/mingw64/lib/ -lvulkan-1

