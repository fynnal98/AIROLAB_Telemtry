#include "TelemetryDataHandler.h"

using namespace aerolab::Telemetry;

std::shared_ptr<TelemetryDataHandler> TelemetryDataHandler::GetInstance()
{
    static std::shared_ptr<TelemetryDataHandler> instance = nullptr;
    if (instance)
        instance = std::shared_ptr<TelemetryDataHandler>(new TelemetryDataHandler());
    return instance;
}

void TelemetryDataHandler::Start(Core::AsyncExecutor &exec)
{
    using namespace Core;

    LOG_INFO("Starting Telemetry Data Handler");

    auto config = JsonConfig::GetInstance();
    std::string ip = config->GetParameter<std::string>("network/localAddress");
    int port = config->GetParameter<int>("network/localPort");

    m_pInputManager = std::make_shared<InputManager>(exec.GetExecutor(), ip, port);
}

void TelemetryDataHandler::Stop()
{
}

// ============================================================================
// Getter
// ============================================================================

const MPUAccelleration TelemetryDataHandler::GetAccelleration()
{
    std::lock_guard<std::mutex> lock(m_dataMutex);
    return m_mpuAccelleration;
}

const MPUGyro TelemetryDataHandler::GetGyro()
{
    std::lock_guard<std::mutex> lock(m_dataMutex);
    return m_mpuGyro;
}

const BMPData TelemetryDataHandler::GetBmpData()
{
    std::lock_guard<std::mutex> lock(m_dataMutex);
    return m_bmpData;
}

// ============================================================================
// Message Callback
// ============================================================================

void TelemetryDataHandler::MessageReceivedCallback(const std::vector<uint8_t> &datagram)
{
    MPUAccelleration accelleration;
    MPUGyro gyro;
    BMPData bmpData;
    uint8_t msgId; // TODO: Implement packetloss calculation

    if (!m_pDatagram->ParseTelemetryDatagram(datagram, msgId, accelleration, gyro, bmpData))
    {
        LOG_WARNING("Failed to parse Datagram");
        return;
    }

    std::lock_guard<std::mutex> lock(m_dataMutex);

    m_mpuAccelleration = std::move(accelleration);
    m_mpuGyro = std::move(gyro);
    m_bmpData = std::move(bmpData);
}
