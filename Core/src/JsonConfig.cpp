#include "JsonConfig.h"


std::shared_ptr<JsonConfig> JsonConfig::GetInstance(const std::string& configFilePath)
{
    static std::shared_ptr<JsonConfig> instance = nullptr;
    
    if (!instance)
    {
        if (configFilePath.empty())
        {   
           throw std::logic_error("Config file path must be provided on firtst call!"); 
        }

        
    }

    return instance;

}


QFile file("config.json");
if (!file.open(QIODevice::ReadOnly)) return;

QByteArray data = file.readAll();
QJsonDocument doc = QJsonDocument::fromJson(data);
QJsonObject obj = doc.object();

QString name = obj["name"].toString();
int id = obj["id"].toInt();

