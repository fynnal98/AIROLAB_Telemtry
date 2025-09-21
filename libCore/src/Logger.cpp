#include "Logger.h"

using namespace aerolab::Core;

std::once_flag Logger::s_once;
std::shared_ptr<Logger> Logger::s_instance = nullptr;

// ============================================================================
// Singleton instance management
// ============================================================================

/**
 * @brief Initialize the Logger by setting the fileName of the Log file
 * @param fileName name of the log file
 * @throws std::logic_error if the filename is empty or the log folder does not exist
 */
void Logger::Init(const std::string &fileName)
{
    std::call_once(s_once, [&]()
                   {
        if (fileName.empty())
            throw std::logic_error("Log filename must not be empty on first init.");
        std::filesystem::path logDir{"logs"};
        std::error_code ec;
        std::filesystem::create_directories(logDir, ec); // ok if already exists
        if (ec) throw std::logic_error("Could not create log folder: " + ec.message());
        s_instance = std::shared_ptr<Logger>(new Logger((logDir / fileName).string())); });
}

/**
 * @brief Retrieves the singletom instance of the Logger class
 * @return A shared pointer to the Logger instance
 * @throws std::logic_error if the log folder can not be opened/created
 */
std::shared_ptr<Logger> Logger::GetInstance()
{
    if (!s_instance)
        throw std::logic_error("Logger not initialized. Call LOG_INIT(\"<file>\") once before logging.");
    return s_instance;
}

/**
 * @brief Constructor of the Logger class
 * @details Initialized the Logger by opening the specified log file.
 * @param filePath The path to the log file
 * @throws std::runtime_error if the file cannot be opened
 */
Logger::Logger(const std::string &filePath) : m_fileStream(filePath, std::ios::app),
                                              m_logLevel(E_LogLevel::Debug)
{
    if (!m_fileStream.is_open())
        throw std::runtime_error("Unable to open log file");
}

// ============================================================================
// Logging functions
// ============================================================================

/**
 * @brief Log a message
 * @details Logs a message with the given type
 */
void Logger::log(const std::string &type, const std::string &msg, const char *file, int line, const char *func, std::ostream &stream)
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

/**
 * @brief Logs a red error message
 * @param msg The message to be logged
 * @param file The file from which this method got called
 * @param line The line from which this method got called
 * @param func The function from which this method got called
 */
void Logger::logError(const std::string &msg, const char *file, int line, const char *func)
{
    if (m_logLevel >= E_LogLevel::Error)
        log("ERROR", msg, file, line, func, std::cout);
}

/**
 * @brief Logs a yellow warning message
 * @param msg The message to be logged
 * @param file The file from which this method got called
 * @param line The line from which this method got called
 * @param func The function from which this method got called
 */
void Logger::logWarning(const std::string &msg, const char *file, int line, const char *func)
{
    if (m_logLevel >= E_LogLevel::Warning)
        log("WARNING", msg, file, line, func, std::cout);
}

/**
 * @brief Logs a white info message
 * @param msg The message to be logged
 * @param file The file from which this method got called
 * @param line The line from which this method got called
 * @param func The function from which this method got called
 */
void Logger::logInfo(const std::string &msg, const char *file, int line, const char *func)
{
    if (m_logLevel >= E_LogLevel::Info)
        log("INFO", msg, file, line, func, std::cout);
}

/**
 * @brief Logs a blue debug message
 * @param msg The message to be logged
 * @param file The file from which this method got called
 * @param line The line from which this method got called
 * @param func The function from which this method got called
 */
void Logger::logDebug(const std::string &msg, const char *file, int line, const char *func)
{
    if (m_logLevel >= E_LogLevel::Debug)
        log("DEBUG", msg, file, line, func, std::cout);
}
