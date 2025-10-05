#include "Datagram.h"

using namespace aerolab::Telemetry;
using namespace aerolab::Core;

uint8_t Datagram::s_msgId = 0;

/**
 * @brief Gets the singleton instance.
 * @details This function creates an instance of the Datagram class, if it was not created yet
 * @return Shared pointer to the singleton instance
 */
std::shared_ptr<Datagram> Datagram::GetInstance()
{
    static std::shared_ptr<Datagram> instance = nullptr;
    if (!instance)
    {
        instance = std::shared_ptr<Datagram>(new Datagram());
    }
    return instance;
}

// ============================================================================
// Datagram parsing
// ============================================================================

/**
 * @brief Parses a telemetry datagram
 * @details This method validates the start and endbytes, checks if the length is correct, validates the checksum and deserializes the data.
 * @param datagram The datagram to parse
 * @param parsedMsgId Output parameter for the parsed message ID
 * @param parsedAccelleration Output paranmeter for the parsed accelleration
 * @param parsedGyro Output parameter for the parsed Gyro data
 * @param parsedBmpData Output parameter for the parsed BMP data
 * @return True if the datagram was parsed successfully, false otherwise
 */
bool Datagram::ParseTelemetryDatagram(const std::vector<uint8_t> &datagram, uint8_t &parsedMsgId, MPUAccelleration &parsedAccelleration, MPUGyro &parsedGyro, BMPData &parsedBmpData)
{
    if (datagram.front() != START_BYTE && datagram.back() != END_BYTE)
    {
        LOG_WARNING("Start or End byte does not match");
        return false;
    }

    if (datagram.size() != EXPECTED_DATAGRAM_LENGTH)
    {
        LOG_WARNING("Message length mismatch");
        return false;
    }

    if (!validateChecksum(datagram))
    {
        LOG_WARNING("Checksum mismatch");
        return false;
    }

    auto it = datagram.begin()++;

    parsedMsgId = *it++;

    // Parse accelleration
    parsedAccelleration.x = deserializeInt16(it);
    parsedAccelleration.y = deserializeInt16(it);
    parsedAccelleration.z = deserializeInt16(it);

    // Parse gyro
    parsedGyro.x = deserializeInt16(it);
    parsedGyro.y = deserializeInt16(it);
    parsedGyro.z = deserializeInt16(it);

    // Parse BMP Data
    parsedBmpData.pressure = deserializeFloat(it);
    parsedBmpData.temp = deserializeFloat(it);
    parsedBmpData.altitude = deserializeFloat(it);

    return true;
}

// ============================================================================
// Utility functions
// ============================================================================

/**
 * @brief Interpret the next 4 bytes as float
 * @details This method reads the next 4 bytes of the vector and casts them into a float value
 * @param it Iterator of the position in the vector
 * @return The float value of the bytes
 * @attention The function moves the iterator by 4
 */
float Datagram::deserializeFloat(std::vector<uint8_t>::const_iterator &it)
{
    uint32_t asInt = static_cast<uint32_t>(*it) | (static_cast<uint32_t>(*(it + 1)) << 8) | (static_cast<uint32_t>(*(it + 2)) << 16) | (static_cast<uint32_t>(*(it + 3)) << 24);
    it += 4;

    float value;
    std::memcpy(&value, &asInt, sizeof(float));
    return value;
}

/**
 * @brief Interpret the next 2 bytes as int16_t
 * @details This method reads the next 2 bytes of the vector and casts them into a int16_t value
 * @param it Iterator of the position in the vector
 * @return The int16_t value of the bytes
 * @attention The function moves the iterator by 2
 */
int16_t Datagram::deserializeInt16(std::vector<uint8_t>::const_iterator &it)
{
    int16_t value = static_cast<int16_t>(static_cast<uint16_t>(*it) | (static_cast<uint16_t>(*(it + 1)) << 8));
    it += 2;
    return value;
}

/**
 * @brief Validates the checksum of the given datagram
 * @details This method calculates the checksum of the received datagram and validates it
 * @param datagram The datagram to validate
 * @return True if the checksum is correct, false otherwise
 */
bool Datagram::validateChecksum(const std::vector<uint8_t> &datagram)
{
    std::vector<uint8_t> parsedChecksum(datagram.end() - 5, datagram.end() - 1);
    uint32_t parsedChecksumInt = static_cast<uint32_t>(parsedChecksum[0]) | (static_cast<uint32_t>(parsedChecksum[1]) << 8) | (static_cast<uint32_t>(parsedChecksum[2]) << 16) | (static_cast<uint32_t>(parsedChecksum[3]) << 24);

    std::vector<uint8_t> checksumData(datagram.begin(), datagram.end() - 5);
    uint32_t crc = 0xFFFFFFFF;
    for (uint8_t byte : checksumData)
    {
        crc ^= byte;
        for (int i = 0; i < 8; ++i)
        {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xEDB88320;
            else
                crc >>= 1;
        }
    }
    crc ^= 0xFFFFFFFF;

    return crc == parsedChecksumInt;
}
