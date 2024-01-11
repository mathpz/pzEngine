#pragma once

// This isnt tested properly yet, so it may not work
#ifdef PZ_PLATFORM_WINDOWS
    #ifdef PZ_BUILD_DLL
        #define PZ_API __attribute__((dllexport))
    #else
        #define PZ_API __attribute__((dllimport))
    #endif
#else
    #define PZ_API
#endif