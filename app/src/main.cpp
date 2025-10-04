#include <JsonConfig.h>
#include <Logger.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
using namespace aerolab::Core;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    LOG_INIT("logs.txt");

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []()
        { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Telemetry", "Main");

    // Config test
    std::string configPath = ".\\config\\config.json";
    JsonConfig::Init(configPath);

    std::shared_ptr<JsonConfig> config = nullptr;

    try
    {
        config = JsonConfig::GetInstance();
    }
    catch (...)
    {
        LOG_ERROR("Could not fetch Instance");
    }

    return app.exec();
}
