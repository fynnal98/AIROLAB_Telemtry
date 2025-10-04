#pragma once

#include <fstream>
#include <Logger.h>
#include <memory>
#include <mutex>
#include <nlohmann/json.hpp>
#include <string>
#include <sstream>

namespace aerolab::Core
{

/**
 * @brief The JsonConfig Class
 * @details This class provides functions to handle config parameters from a json config file.
 *          Parameters can be read and written as well as saved to the config file
 * @note This class uses the nlohmann json lib for handling json files.
 */
class JsonConfig
{
public:
    static std::shared_ptr<JsonConfig> GetInstance();
    static void Init(const std::string &configFilePath = "");

    /**
     * @brief Get the parameter at the given path
     * @param parameterPath The path to the parameter
     * @return The config parameter with the specified type T
     * @throws std::runtime_error if the parameter is not found.
     */
    template <typename T>
    T GetParameter(const std::string &parameterPath) const
    {
        using json = nlohmann::json;

        LOG_DEBUG("Retrieving Parameter: " + parameterPath);

        // Split path to parameter
        std::vector<std::string> parts = splitPath(parameterPath);

        // Get json from member
        const json *pNode = &m_json;

        // Iterate through split parameter path
        for (const auto &key : parts)
        {
            if (!pNode->contains(key))
                throw std::runtime_error("Could not find config parameter with path: " + parameterPath);

            pNode = &((*pNode)[key]);
        }

        return pNode->get<T>();
    }

    /**
     * @brief Set the given parameter at the given path
     * @details This function sets the given parameter in the member variable. To write the config to the file use SaveConfig.
     * @param parameterPath The path to the parameter to set
     * @param value The value of the parameter to set
     */
    template <typename T>
    void WriteParameter(const std::string &parameterPath, const T &value)
    {
        using json = nlohmann::json;

        // Split path to parameter
        const auto parts = splitPath(parameterPath);

        // Get json from member
        json *pNode = &m_json;

        // Navigate to the given path
        for (size_t i = 0; i < parts.size(); i++)
        {
            const auto &key = parts[i];

            // If last element -> Set Value
            if (i == parts.size() - 1)
                (*pNode)[key] = value;
            else // Node -> add object if it does not exist yet
            {
                if (!pNode->contains(key) || !(*pNode)[key].is_object())
                    (*pNode)[key] = json::object();

                // Next node
                pNode = &((*pNode)[key]);
            }
        }
    }

    bool SaveJson();

    /// Default destructor
    ~JsonConfig() = default;
    /// Delete copy constructor to ensure singleton pattern
    JsonConfig(const JsonConfig &) = delete;
    /// Delete assign operator to ensure singleton pattern
    JsonConfig &operator=(const JsonConfig &) = delete;

private:
    JsonConfig(const std::string &configFilePath);

    static std::vector<std::string> splitPath(const std::string &parameterPath);

    /// Path to the config file
    std::string m_configFilePath;
    /// Internal representation of the config file
    nlohmann::json m_json;
    /// once flag for init function
    static std::once_flag s_once;
    /// singleton instance
    static std::shared_ptr<JsonConfig> s_instance;
};

}