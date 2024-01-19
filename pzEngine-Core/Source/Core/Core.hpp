#pragma once

#ifdef PZ_PLATFORM_WINDOWS
	#define PZ_API
#else
	#define PZ_API
#endif

#define BIT(x) (1 << x)
