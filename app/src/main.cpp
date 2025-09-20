#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <Logger.h>
using namespace aerolab::Core;

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

    LOG_INIT("Test.log");

    LOG_ERROR("Test Error Message.");
    LOG_WARNING("Test Warning Message.");
    LOG_INFO("Test info message.");
    LOG_DEBUG("Test debug message.");

    return app.exec();
}
