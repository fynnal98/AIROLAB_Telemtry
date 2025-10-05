#include "TelemetryDataHandler.h"

#include <AsyncExecutor.h>
#include <JsonConfig.h>
#include <Logger.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

using namespace aerolab::Core;
using namespace aerolab::Telemetry;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []()
        { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Telemetry", "Main");

    LOG_INIT("logs.txt");

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

    AsyncExecutor executor;
    executor.Start();

    auto pTelemetryDataHandler = TelemetryDataHandler::GetInstance();
    pTelemetryDataHandler->Start(executor);

    return app.exec();
}
