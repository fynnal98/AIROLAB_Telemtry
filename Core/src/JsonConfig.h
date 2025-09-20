#pragma once

#include <exceptions>
#include <memory>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <string>

class JsonConfig{
public:
    std::shared_ptr<JsonConfig> GetInstance(const std::string& configFilePath = "");

    ~JsonConfig();
    JsonConfig(const JsonConfig&) = delete;
    JsonConfig& operator=(const JsonConfig&) = delete;

private:
    JsonConfig();

    QFile m_configFile;
}