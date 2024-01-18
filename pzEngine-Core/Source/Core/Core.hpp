#pragma once

// This isnt tested properly yet, so it may not work
#ifdef PZ_PLATFORM_WINDOWS
	#if PZ_DYNAMIC_LINK
		#ifdef PZ_BUILD_DLL
			#define PZ_API __declspec(dllexport)
		#else
			#define PZ_API __declspec(dllimport)
		#endif
	#else
		#define PZ_API
	#endif
#else
    #error PZ only supports Windows!
#endif

#define BIT(x) (1 << x)
