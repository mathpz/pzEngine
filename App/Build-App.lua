project "First-App"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir "Binaries/%{cfg.buildcfg}"
    staticruntime "on"

    files 
    { 
    "Source/**.hpp",
    "Source/**.cpp",
    "**.h",

    }

    includedirs
    {
        "Source",

        "../pzEngine-Core/Source",
        "../pzEngine-Core/vendor/GLFW/include",
        "../pzEngine-Core/vendor/imgui",

        "$(VULKAN_SDK)/Include"
    }

    links
    {
        "pzEngine-Core",
        "GLFW",
        "ImGui"
    }


   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"
       defines { "PZ_PLATFORM_WINDOWS" }

   filter "configurations:Debug"
       defines { "PZ_DEBUG" }
       runtime "Debug"
       symbols "on"

   filter "configurations:Release"
       defines { "PZ_RELEASE" }
       runtime "Release"
       optimize "on"
       symbols "on"

   filter "configurations:Dist"
       defines { "PZ_DIST" }
       runtime "Release"
       optimize "on"
       symbols "off"