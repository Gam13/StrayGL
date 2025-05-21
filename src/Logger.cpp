#include "StrayGL/Logger.hpp"
#include "StrayGL/pch.h"

namespace Stray
{
    std::shared_ptr<spdlog::logger> Logger::coreLogger;
    std::shared_ptr<spdlog::logger> Logger::clientLogger;

    void Logger::init(const std::string& logFile)
    {
        try {
            // Cria sinks compartilhados
            std::vector<spdlog::sink_ptr> sinks;
            
            // Sink para console (colorido)
            sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
            
            // Sink para arquivo (apenas para o logger principal)
            sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFile, true));
            
            // Configuração dos loggers
            coreLogger = std::make_shared<spdlog::logger>("STRAY", begin(sinks), end(sinks));
            clientLogger = std::make_shared<spdlog::logger>("APP", sinks[0]); // Apenas console
            
            // Registro global dos loggers
            spdlog::register_logger(coreLogger);
            spdlog::register_logger(clientLogger);
            
            // Padrão de formatação
            spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] %v");
            
            // Nível padrão de log
            coreLogger->set_level(spdlog::level::trace);
            clientLogger->set_level(spdlog::level::trace);
            
            // Flush imediato para mensagens importantes
            coreLogger->flush_on(spdlog::level::warn);
            clientLogger->flush_on(spdlog::level::warn);
            
            STRAY_INFO("Logger initialized");
            STRAY_INFO("Log file: {}", logFile);
        }
        catch (const spdlog::spdlog_ex& ex) {
            // Fallback para logger básico se algo der errado
            coreLogger = spdlog::stdout_color_mt("STRAY");
            coreLogger->error("Failed to initialize logger: {}", ex.what());
        }
    }

    void Logger::shutdown()
    {
        STRAY_INFO("Shutting down logger");
        spdlog::drop_all();
        coreLogger.reset();
        clientLogger.reset();
    }

    void Logger::setCoreLevel(spdlog::level::level_enum level)
    {
        coreLogger->set_level(level);
        STRAY_DEBUG("Core log level set to {}", spdlog::level::to_string_view(level));
    }

    void Logger::setClientLevel(spdlog::level::level_enum level)
    {
        clientLogger->set_level(level);
        STRAY_DEBUG("Client log level set to {}", spdlog::level::to_string_view(level));
    }

    void Logger::flushAll()
    {
        if (coreLogger) coreLogger->flush();
        if (clientLogger) clientLogger->flush();
    }
} // namespace Stray