#pragma once

// This isnt tested properly yet, so it may not work
#ifdef PZ_WINDOWS_PLATFORM
    #ifdef PZ_BUILD_DLL
        #define PZ_API __attribute__((dllexport))
    #else
        #define PZ_API __attribute__((dllimport))
    #endif
#endif
    # define PZ_API
#define BIT(x) (1 << x)