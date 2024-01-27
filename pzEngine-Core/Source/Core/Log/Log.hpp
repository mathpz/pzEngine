#pragma once

#include "pzpch.hpp"

#include "Core/Core.hpp"
#include "spdlog/spdlog.h"

namespace pz
{
	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};


// Core log macros
#define PZ_CORE_TRACE(...)			::pz::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define PZ_CORE_INFO(...)			::pz::Log::GetCoreLogger()->info(__VA_ARGS__)
#define PZ_CORE_WARN(...)			::pz::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define PZ_CORE_ERROR(...)			::pz::Log::GetCoreLogger()->error(__VA_ARGS__)
#define PZ_CORE_CRITICAL(...)		::pz::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define PZ_TRACE(...)				::pz::Log::GetClientLogger()->trace(__VA_ARGS__)
#define PZ_INFO(...)				::pz::Log::GetClientLogger()->info(__VA_ARGS__)
#define PZ_WARN(...)				::pz::Log::GetClientLogger()->warn(__VA_ARGS__)
#define PZ_ERROR(...)				::pz::Log::GetClientLogger()->error(__VA_ARGS__)
#define PZ_CRITICAL(...)			::pz::Log::GetClientLogger()->critical(__VA_ARGS__)

} // namespace pz