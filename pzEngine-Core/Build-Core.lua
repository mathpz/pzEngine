project "pzEngine-Core"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    targetdir "Binaries/%{cfg.buildcfg}"
    staticruntime "on"

    pchheader "pzpch.hpp"
   
    files { "Source/**.hpp", "Source/**.cpp", "**.h", "**.vert", "**.frag"}

    targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

    GLSLC_PATH = "..\\Vendor\\Binaries\\glslc"

    includedirs
    {
        "Source",
        "Shaders",

        -- vulkan sdk
        "$(VULKAN_SDK)/Include",
        
        -- vendor
        "vendor/GLFW/include",

        "vendor/glm/glm/**.hpp",
        "vendor/glm/glm/**.inl",

        "vendor/imgui",
        "vendor/tinyobjloader/tiny_obj_loader.h"
    }

    libdirs
    {
        "$(VULKAN_SDK)/Lib"
    }
    
    links
    {
        "GLFW",
        "vulkan-1",
        "ImGui"
    }


    -- WINDOWS BUILD WITH VISUAL STUDIO
    filter {"system:windows", "action:vs*"}
        pchsource "Source/pzpch.cpp"
        
        prebuildcommands 
        { 
            GLSLC_PATH .. "\\Windows\\glslc.exe Shaders\\simple_shader.vert -o Shaders\\simple_shader.vert.spv", 
            GLSLC_PATH .. "\\Windows\\glslc.exe Shaders\\simple_shader.frag -o Shaders\\simple_shader.frag.spv" 
        
        }

        defines
        {
            "PZ_PLATFORM_WINDOWS",
            "PZ_BUILD_DLL",
        }

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
    filter {"system:linux"}
        prebuildcommands { GLSLC_PATH .. "/Linux/glslc Shaders/simple_shader.vert -o Shaders/simple_shader.vert.spv", GLSLC_PATH .. "/Linux/glslc Shaders/simple_shader.frag -o Shaders/simple_shader.frag.spv" }
        buildoptions { "-I. -I$(VULKAN_SDK_PATH)/include"}
        linkoptions { "-L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan" }
        defines { }