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
        ERROR = 0,
        /// Log warnings and errors
        WARNING = 1,
        /// Log informational messages, warnings and errors
        INFO = 2,
        /// Log all messages
        DEBUG = 3
    };

    /**
     * @brief The Logger class
     * @details This class provides logging functionalities
     * it is implemented as a singleton to encure a single instance throughout the application.
     */
    class Logger
    {
    public:

        /**
         * @brief Initialize the Logger by setting the fileName of the Log file
         * @param fileName name of the log file
         * @throws std::logic_error if the filename is empty or the log folder does not exist
         */
        static void Init(const std::string& fileName) {
            int a;

            std::call_once(s_once, [&]() {
                if (fileName.empty())
                    throw std::logic_error("Log filename must not be empty on first init.");
                std::filesystem::path logDir{"logs"};
                std::error_code ec;
                std::filesystem::create_directories(logDir, ec); // ok if already exists
                if (ec) throw std::logic_error("Could not create log folder: " + ec.message());
                s_instance = std::shared_ptr<Logger>(new Logger((logDir / fileName).string()));
            });
        }

        /**
         * @brief Retrieves the singletom instance of the Logger class
         * @return A shared pointer to the Logger instance
         * @throws std::logic_error if the log folder can not be opened/created
         */
        static std::shared_ptr<Logger> GetInstance() {
            if (!s_instance)
                throw std::logic_error("Logger not initialized. Call LOG_INIT(\"<file>\") once before logging.");
            return s_instance;
        }

        /// Delete copy constructor to ensure singleton instance management
        Logger(const Logger &) = delete;
        /// Delete assign operator to ensure singleton instance management
        Logger &operator=(const Logger &) = delete;

        /// @brief Sets the Log Level for the Logger class
        /// @param loglevel Log level to be set
        void SetLogLevel(E_LogLevel logLevel) { m_logLevel = logLevel; }

        /**
         * @brief Log a message
         * @details Logs a message with the given type
         */
        void log(const std::string &type, const std::string &msg, const char *file, int line, const char *func, std::ostream &stream)
        {
            std::lock_guard<std::mutex> guard(m_mutex);

            // get current time
            std::time_t t = std::time(nullptr);
            char isotime[64]; // Character array containing a ISO 8601 formated timestring
            if (!std::strftime(isotime, sizeof(isotime), "%FT%T%z", std::localtime(&t)))
                std::strcpy(isotime, "no time");

            // Get file name
            std::string fileName = std::filesystem::path(file).filename().string();

            // Contruct log message
            std::string logMessage = std::string(isotime) + "|" + type + "|" + fileName + ":" + std::to_string(line) + "|" + func + "|" + msg;

            // Write log message to file stream
            m_fileStream << logMessage << std::endl;

            // Color terminal output and write message to terminal
            if (type.find("ERROR") != std::string::npos)
                stream << "\033[1;31m" << logMessage << "\033[0m" << std::endl;

            else if (type.find("WARNING") != std::string::npos)
                stream << "\033[33m" << logMessage << "\033[0m" << std::endl;

            else if (type.find("DEBUG") != std::string::npos)
                stream << "\033[34m" << logMessage << "\033[0m" << std::endl;

            else
                stream << logMessage << std::endl;
        }


        void logError(const std::string &msg, const char *file, int line, const char *func)
        {
            if (m_logLevel >= E_LogLevel::ERROR)
                log("ERROR", msg, file, line, func, std::cout);
        }

        void logWarning(const std::string &msg, const char *file, int line, const char *func)
        {
            if (m_logLevel >= E_LogLevel::WARNING)
                log("WARNING", msg, file, line, func, std::cout);
        }

        void logInfo(const std::string &msg, const char *file, int line, const char *func)
        {
            if (m_logLevel >= E_LogLevel::INFO)
                log("INFO", msg, file, line, func, std::cout);
        }

        void logDebug(const std::string &msg, const char *file, int line, const char *func)
        {
            if (m_logLevel >= E_LogLevel::DEBUG)
                log("DEBUG", msg, file, line, func, std::cout);
        }

    private:
        /**
         * @brief Constructor of the Logger class
         * @details Initialized the Logger by opening the specified log file.
         * @param filePath The path to the log file
         * @throws std::runtime_error if the file cannot be opened
         */
        Logger(const std::string &filePath) : 
            m_fileStream(filePath, std::ios::app), 
            m_logLevel(E_LogLevel::DEBUG)
        {
            if (!m_fileStream.is_open())
                throw std::runtime_error("Unable to open log file");
        }

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

    inline std::once_flag Logger::s_once;
    inline std::shared_ptr<Logger> Logger::s_instance{};

}