#pragma once

#ifdef PZ_PLATFORM_WINDOWS
	#define PZ_API
#else
	#define PZ_API
#endif

#ifdef PZ_ENABLE_ASSERTS
#define PZ_ASSERT(x, ...) { if(!(x)) { PZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define PZ_CORE_ASSERT(x, ...) { if(!(x)) { PZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define PZ_ASSERT(x, ...)
#define PZ_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)
