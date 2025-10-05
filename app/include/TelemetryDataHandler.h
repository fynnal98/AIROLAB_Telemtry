#pragma once

#include "Datagram.h"
#include "Types.h"

#include <AsyncExecutor.h>
#include <InputManager.h>
#include <JsonConfig.h>
#include <memory>
#include <mutex>
#include <QObject>
#include <vector>

namespace aerolab::Telemetry
{
    class TelemetryDataHandler : public QObject
    {
        Q_OBJECT
        
    public:
        static std::shared_ptr<TelemetryDataHandler> GetInstance();

        void Start(Core::AsyncExecutor &exec);
        void Stop();

        void MessageReceivedCallback(const std::vector<uint8_t> &datagram);

        const MPUAccelleration GetAccelleration();
        const MPUGyro GetGyro();
        const BMPData GetBmpData();

    private:
        MPUAccelleration m_mpuAccelleration;
        MPUGyro m_mpuGyro;
        BMPData m_bmpData;

        std::shared_ptr<Core::InputManager> m_pInputManager;
        std::shared_ptr<Datagram> m_pDatagram;

        std::mutex m_dataMutex;
    };
}
