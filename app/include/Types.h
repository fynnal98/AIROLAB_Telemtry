#pragma once

#include <cstdint>

namespace aerolab::Telemetry
{
    /**
     * Structure representing the accelleration read from the MPU
     */
    struct MPUAccelleration
    {
        int16_t x;
        int16_t y;
        int16_t z;

        MPUAccelleration() : x(0.0f), y(0.0f), z(0.0f) {}

        MPUAccelleration(int16_t x_, int16_t y_, int16_t z_)
            : x(x_), y(y_), z(z_) {}
    };

    /**
     * Structure representing the gyro data read from the MPU
     */
    struct MPUGyro
    {
        int16_t x;
        int16_t y;
        int16_t z;

        MPUGyro() : x(0.0f), y(0.0f), z(0.0f) {}

        MPUGyro(int16_t x_, int16_t y_, int16_t z_)
            : x(x_), y(y_), z(z_) {}
    };

    /**
     * Structure representing the data read from the BMP
     */
    struct BMPData
    {
        float pressure;
        float temp;
        float altitude;

        BMPData() : pressure(0.0f), temp(0.0f), altitude(0.0f) {}

        BMPData(float pressure_, float temp_, float altitude_)
            : pressure(pressure_), temp(temp_), altitude(altitude_) {}
    };
}
