#pragma once

#include <cstdint>

namespace aerolab::Telemetry
{
    /// @brief Expected length of telemetry datagrams
    const size_t EXPECTED_DATAGRAM_LENGTH = 43;
    /// @brief Expected Start byte of the telemetry datagram
    const uint8_t START_BYTE = 0x20;
    /// @brief Expected End byte of the telemetry datagram
    const uint8_t END_BYTE = 0x30;
}