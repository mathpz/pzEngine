project "pzEngine-Core"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. OutputDir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. OutputDir .. "/%{prj.name}")

    pchheader "pzpch.hpp"
    pchsource "Source/pzpch.cpp"

    files
    {
        "Source/**.h",
        "Source/**.hpp",
        "Source/**.cpp",

        "Assets/Shaders/**.vert",
        "Assets/Shaders/**.frag",

        "Third_Party/glm/glm/**.hpp",
        "Third_Party/glm/glm/**.inl",
    }


    -- GLSLC_PATH = "..\\Vendor\\Binaries\\glslc"

    defines
    {
        "_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",
        "PZ_PLATFORM_WINDOWS",
    }

    includedirs
    {
        "Source",
        "Assets/Shaders",
        "Assets/Models",
        "Third_Party/spdlog/include",

        "%{IncludeDir.GLFW}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.VulkanSDK}",
        "%{IncludeDir.tinyobjloader}"
    }

    libdirs
    {
        "%{LibraryDir.VulkanSDK}"
    }

    links
    {
        "GLFW",
        "vulkan-1",
        "ImGui",
        "vk-bootstrap"
    }


    -- WINDOWS BUILD WITH VISUAL STUDIO
    filter {"system:windows", "action:vs*"}
    buildoptions { "/utf-8" }
    -- prebuildcommands
    -- {
    --    "%{wks.location}\\pzEngine-Core\\Shaders\\glslc.exe Shaders\\simple_shader.vert -o Shaders\\simple_shader.vert.spv",
    --    "%{wks.location}\\pzEngine-Core\\Shaders\\glslc.exe Shaders\\simple_shader.frag -o Shaders\\simple_shader.frag.spv"
    -- }


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


    --  TODO COMPLETE LINUX BUILD
    -- filter {"system:linux"}
    --    prebuildcommands { GLSLC_PATH .. "/Linux/glslc Shaders/simple_shader.vert -o Shaders/simple_shader.vert.spv", GLSLC_PATH .. "/Linux/glslc Shaders/simple_shader.frag -o Shaders/simple_shader.frag.spv" }
    --    buildoptions { "-I. -I$(VULKAN_SDK_PATH)/include"}
    --    linkoptions { "-L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan" }
    --    defines { }