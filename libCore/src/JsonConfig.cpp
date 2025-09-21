#include "JsonConfig.h"

std::shared_ptr<JsonConfig> JsonConfig::GetInstance()
{
    if (!s_instance)
        throw std::runtime_error("Config not yet initialized.");
    return s_instance;
}

void JsonConfig::Init(const std::string &configFilePath)
{
    std::call_once(s_once, [&]()
        {
            if (configFilePath.empty())
                throw std::logic_error("Config file path is empty");
            
            s_instance = std::make_shared<JsonConfig>(new JsonConfig(configFilePath));
        });
}

JsonConfig::JsonConfig(const std::string &configFilePath)
