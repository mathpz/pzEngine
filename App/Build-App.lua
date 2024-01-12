project "First-App"
   kind "ConsoleApp"
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
      "C:/msys64/mingw64/include/",
      "../Vendor/glm",
      "../pzEngine-Core/vendor/spdlog/include"
   }

    links
    {
        "pzEngine-Core",
        "glfw3"
    }

    libdirs
    {
       "../Vendor/glfw-3.3.8.bin.WIN64/lib-mingw-w64"
    }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"
       defines { "PZ_PLATFORM_WINDOWS" }

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