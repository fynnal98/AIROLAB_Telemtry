#pragma once

#include "Constants.h"
#include "Types.h"

#include <cstdint>
#include <cstring>
#include <Logger.h>
#include <memory>
#include <vector>

namespace aerolab::Telemetry
{
    /**
     * @brief The datagram class
     * @details This class contains functions for constructing datagrams to transmit telemetry data.
     *          The format of the datagram is as follows:
     *          START_BYTE (1 Byte) | MSG_ID (1 Byte) |
     *          MPUAccX (4 Bytes) | MPUAccY (4 Bytes) | MPUAccZ (4 Bytes) |
     *          MPUGyroX (4 Bytes) | MPUGyroY (4 Bytes) | MPUGyroZ (4 Bytes) |
     *          BMPPressure (4 Bytes) | BMPTemp (4 Bytes) | BMPAltitude (4 Bytes) |
     *          CRC32_CHECKSUM (4 Bytes) | END_BYTE (1 Byte)
     */
    class Datagram
    {
    public:
        static std::shared_ptr<Datagram> GetInstance();

        Datagram(const Datagram &) = delete;
        Datagram &operator=(const Datagram &) = delete;

        ~Datagram() = default;

        bool ParseTelemetryDatagram(const std::vector<uint8_t> &datagram, uint8_t &parsedMsgId, MPUAccelleration &parsedAccelleration, MPUGyro &parsedGyro, BMPData &parsedBmpData);

    private:
        Datagram() = default;

        float deserializeFloat(std::vector<uint8_t>::const_iterator &it);
        int16_t deserializeInt16(std::vector<uint8_t>::const_iterator &it);

        bool validateChecksum(const std::vector<uint8_t> &datagram);

        /// Static message Id
        static uint8_t s_msgId;
    };
}