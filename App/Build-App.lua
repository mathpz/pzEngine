project "First-App"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"
    
    targetdir ("%{wks.location}/bin/" .. OutputDir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. OutputDir .. "/%{prj.name}")

    files 
    { 
        "Source/**.h",
        "Source/**.hpp",
        "Source/**.cpp",
    }

    includedirs
    {
        -- pzEngine-Core
        "%{wks.location}/pzEngine-Core/Source",
        
        -- vendor
        "%{wks.location}/pzEngine-Core/Third_Party/spdlog/include",
        "%{wks.location}/pzEngine-Core/Third_Party",

        "%{IncludeDir.glm}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.VulkanSDK}",

        -- TODO FIX THIS
        -- added path to vulkan manually on visual studio to work
        -- added Third_Party\glm\glm
    }

    links
    {
        "pzEngine-Core",
    }

    defines
    {
        "_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",
        "PZ_PLATFORM_WINDOWS",
    }

   filter "system:windows"
       systemversion "latest"
       buildoptions { "/utf-8" }

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