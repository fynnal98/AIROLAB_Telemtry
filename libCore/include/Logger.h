#pragma once

#include <cstring>
#include <direct.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <limits.h>
#include <memory>
#include <mutex>
#include <string>

namespace aerolab::Core
{

#define LOG_INIT(filename) ::aerolab::Core::Logger::Init(filename)
#define LOG_ERROR(message) ::aerolab::Core::Logger::GetInstance()->logError(message, __FILE__, __LINE__, __func__)
#define LOG_WARNING(message) ::aerolab::Core::Logger::GetInstance()->logWarning(message, __FILE__, __LINE__, __func__)
#define LOG_INFO(message) ::aerolab::Core::Logger::GetInstance()->logInfo(message, __FILE__, __LINE__, __func__)
#define LOG_DEBUG(message) ::aerolab::Core::Logger::GetInstance()->logDebug(message, __FILE__, __LINE__, __func__)

    /// Enum for Log Le
    enum class E_LogLevel
    {
        /// Only log errors
        Error = 0,
        /// Log warnings and errors
        Warning = 1,
        /// Log informational messages, warnings and errors
        Info = 2,
        /// Log all messages
        Debug = 3
    };

    /**
     * @brief The Logger class
     * @details This class provides logging functionalities
     * it is implemented as a singleton to encure a single instance throughout the application.
     */
    class Logger
    {
    public:
        static void Init(const std::string& fileName);
        static std::shared_ptr<Logger> GetInstance();

        /// Delete copy constructor to ensure singleton instance management
        Logger(const Logger &) = delete;
        /// Delete assign operator to ensure singleton instance management
        Logger &operator=(const Logger &) = delete;

        /**
         * @brief Sets the Log Level for the Logger class
         * @param loglevel Log level to be set
         */
        void SetLogLevel(E_LogLevel logLevel) { m_logLevel = logLevel; }

        void log(const std::string &type, const std::string &msg, const char *file, int line, const char *func, std::ostream &stream);

        void logError(const std::string &msg, const char *file, int line, const char *func);
        void logWarning(const std::string &msg, const char *file, int line, const char *func);
        void logInfo(const std::string &msg, const char *file, int line, const char *func);
        void logDebug(const std::string &msg, const char *file, int line, const char *func);

    private:
        Logger(const std::string &filePath);

        /// Filestream of the Log file
        std::ofstream m_fileStream;
        /// LogLevel of the Logger
        E_LogLevel m_logLevel;
        /// Mutex for threadsafe logging
        std::mutex m_mutex;
        /// singleton instance of the Logger
        static std::shared_ptr<Logger> s_instance;
        /// once flag for singleton instance
        static std::once_flag s_once;
    };

}
