project "First-App"
   kind "WindowedApp"
   language "C++"
   cppdialect "C++17"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "on"

   files { "Source/**.h", "Source/**.cpp" }

   includedirs
   {
      "Source",

	  -- Include Core
	  "../pzEngine-Core",

      -- vendor
      "../Vendor/glm"
   }

   links
   {
      "pzEngine-Core"
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"