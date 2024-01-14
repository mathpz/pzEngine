project "pzEngine-Core"
   kind "SharedLib"
   language "C++"
   cppdialect "C++17"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   pchheader "pzpch.hpp"
   
   files { "Source/**.hpp", "Source/**.cpp", "pzEngine.hpp" }

   includedirs
   {
        -- Include Core
        "Source",
        "Shaders",

        -- Vendor
        "C:/msys64/mingw64/include/",
        "../Vendor/glm",
        "vendor/spdlog/include"
    }
   
    targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

    GLSLC_PATH = "../Vendor/Binaries/glslc"

    -- WINDOWS BUILD WITH GMAKE2
    filter {"system:windows", "action:gmake2"}
        prebuildcommands
        {
            GLSLC_PATH .. "/Windows/glslc.exe Shaders/simple_shader.vert -o Shaders/simple_shader.vert.spv", 
            GLSLC_PATH .. "/Windows/glslc.exe Shaders/simple_shader.frag -o Shaders/simple_shader.frag.spv" 
        
        }
        systemversion "latest"
        
        defines { "PZ_PLATFORM_WINDOWS", "PZ_BUILD_DLL" }

        links 
        {
            "glfw3",
            "vulkan-1",
            "gdi32"
        }

        libdirs
        {
            "C:/msys64/mingw64/lib"
        }
        
        postbuildmessage ("Compiled for Windows")
        postbuildmessage ("Copying DLLs to App")
        
        postbuildcommands
        { 
            ("cp %{cfg.buildtarget.relpath} ../Binaries/" .. OutputDir .. "/First-App/")
        }

    filter {"system:windows", "action:vs*"}
        pchsource "Source/pzpch.cpp"

    --  TODO COMPLETE LINUX BUILD 
    filter {"system:linux"}
        prebuildcommands { GLSLC_PATH .. "/Linux/glslc Shaders/simple_shader.vert -o Shaders/simple_shader.vert.spv", GLSLC_PATH .. "/Linux/glslc Shaders/simple_shader.frag -o Shaders/simple_shader.frag.spv" }
        buildoptions { "-I. -I$(VULKAN_SDK_PATH)/include"}
        linkoptions { "-L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan" }
        defines { }

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
