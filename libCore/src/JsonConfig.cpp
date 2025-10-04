#include "JsonConfig.h"

using namespace aerolab::Core;

std::shared_ptr<JsonConfig> JsonConfig::s_instance = nullptr;
std::once_flag JsonConfig::s_once{};

/**
 * @brief Get the singleton instance
 * @return Shared pointer to the singleton instance
 */
std::shared_ptr<JsonConfig> JsonConfig::GetInstance()
{
    if (!s_instance)
        throw std::runtime_error("Config not yet initialized.");
    LOG_DEBUG("Fetching Instance");
    return s_instance;
}

/**
 * @brief Initializes the JsonConfig
 * @details Checks if the given path is not empty and creates an instance
 * @param configFilePath The path to the config file
 */
void JsonConfig::Init(const std::string &configFilePath)
{
    std::call_once(s_once, [&]()
                   {
            if (configFilePath.empty())
                throw std::logic_error("Config file path is empty");
            
            LOG_DEBUG("Creating JsonConfig instance");
            s_instance = std::shared_ptr<JsonConfig>(new JsonConfig(configFilePath)); 
            LOG_DEBUG("Instance created"); });
}

/**
 * @brief Constructor for the JsonConfig class
 * @details Sets the configFilePath member and reads the config file
 */
JsonConfig::JsonConfig(const std::string &configFilePath) : m_configFilePath(configFilePath)
{
    std::ifstream fileStream(configFilePath);

    LOG_DEBUG("Created Filestream with path: " + configFilePath);

    if (!fileStream.is_open())
        throw std::runtime_error("Could not open config file");

    fileStream >> m_json;
}

// ============================================================================
// Json methods
// ============================================================================

/**
 * @brief Saves the member json to the config file
 * @return True if the config has been written successfully, False otherwise
 */
bool JsonConfig::SaveJson()
{
    std::ofstream fileStream(m_configFilePath);

    // Open filestream for writing config
    if (!fileStream.is_open())
        return false;

    // Write config to file
    try
    {
        fileStream << m_json.dump(4); // Indent 4 spaces
        fileStream.close();
    }
    catch (...)
    {
        return false;
    }

    return true;
}

// ============================================================================
// Utility functions
// ============================================================================

/**
 * @brief Helper method for splitting a given config path
 * @param parameterPath The path to a config parameter formatted as follows: "node/node/.../parameter"
 * @return vector of strings containing the parts of the path -> {"node", "node", ..., "parameter"}
 */
std::vector<std::string> JsonConfig::splitPath(const std::string &parameterPath)
{
    std::vector<std::string> parts;
    std::stringstream ss(parameterPath);
    std::string item;
    while (std::getline(ss, item, '/'))
    {
        if (!item.empty())
            parts.push_back(item);
    }
    return parts;
}