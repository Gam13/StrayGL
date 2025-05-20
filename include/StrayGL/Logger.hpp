#pragma once
#ifndef STRAY_LOGGER_HPP
#define STRAY_LOGGER_HPP

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "StrayGL/pch.h"

namespace Stray
{
    class Logger
    {
    public:
        static void init(const std::string& logFile = "StrayGL.log");
        static void shutdown();
        
        static std::shared_ptr<spdlog::logger>& getCoreLogger() { return coreLogger; }
        static std::shared_ptr<spdlog::logger>& getClientLogger() { return clientLogger; }
        
        static void setCoreLevel(spdlog::level::level_enum level);
        static void setClientLevel(spdlog::level::level_enum level);
        
        static void flushAll();
        
    private:
        static std::shared_ptr<spdlog::logger> coreLogger;
        static std::shared_ptr<spdlog::logger> clientLogger;
    };

} // namespace Stray

// Macros de logging para o core da engine
#define STRAY_TRACE(...)    ::Stray::Logger::getCoreLogger()->trace(__VA_ARGS__)
#define STRAY_DEBUG(...)    ::Stray::Logger::getCoreLogger()->debug(__VA_ARGS__)
#define STRAY_INFO(...)     ::Stray::Logger::getCoreLogger()->info(__VA_ARGS__)
#define STRAY_WARN(...)     ::Stray::Logger::getCoreLogger()->warn(__VA_ARGS__)
#define STRAY_ERROR(...)    ::Stray::Logger::getCoreLogger()->error(__VA_ARGS__)
#define STRAY_CRITICAL(...) ::Stray::Logger::getCoreLogger()->critical(__VA_ARGS__)

// Macros de logging para a aplicação cliente
#define APP_TRACE(...)      ::Stray::Logger::getClientLogger()->trace(__VA_ARGS__)
#define APP_DEBUG(...)      ::Stray::Logger::getClientLogger()->debug(__VA_ARGS__)
#define APP_INFO(...)       ::Stray::Logger::getClientLogger()->info(__VA_ARGS__)
#define APP_WARN(...)       ::Stray::Logger::getClientLogger()->warn(__VA_ARGS__)
#define APP_ERROR(...)      ::Stray::Logger::getClientLogger()->error(__VA_ARGS__)
#define APP_CRITICAL(...)   ::Stray::Logger::getClientLogger()->critical(__VA_ARGS__)

#endif // STRAY_LOGGER_HPP