#pragma once

#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>
#include <string>

class JsonConfig{
public:
    static std::shared_ptr<JsonConfig> GetInstance();
    void Init(const std::string &configFilePath = "");

    ~JsonConfig();
    JsonConfig(const JsonConfig&) = delete;
    JsonConfig& operator=(const JsonConfig&) = delete;

private:
    JsonConfig(const std::string &configFilePath);

    std::string m_configFilePath;

    nlohmann::json m_json;

    static std::once_flag s_once;
    static std::shared_ptr<JsonConfig> s_instance;
};