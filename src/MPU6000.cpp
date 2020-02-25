  /* 
   MPU6000.cpp: Implementation of MPU6000 classes

   Copyright (C) 2018 Simon D. Levy

   This file is part of MPU.

   MPU is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   MPU is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with MPU.  If not, see <http://www.gnu.org/licenses/>.
   */

#include "MPU6000.h"

#include "CrossPlatformSPI.h"

MPU6000::MPU6000(Ascale_t ascale, Gscale_t gscale, uint8_t sampleRateDivisor) 
    : MPU6x00(ascale, gscale, sampleRateDivisor)
{
}

MPUIMU::Error_t MPU6000::begin(void)
{
    return MPU6x00::begin();
}

void MPU6000::readGyrometer(float & gx, float & gy, float & gz)
{
    MPUIMU::readGyrometer(gx, gy, gz);

    // don't subtract bias!
}

void MPU6000::readMPURegisters(uint8_t subAddress, uint8_t count, uint8_t * dest)
{
    cpspi_readRegisters(subAddress, count, dest);
}

bool MPU6000::readAccelRaw(int16_t & x, int16_t & y, int16_t & z)
{
    uint8_t data[6];

    if (!cpspi_readRegisters(ACCEL_XOUT_H, 6, data)) return false;

    x = (int16_t)((data[0] << 8) | data[1]);
    y = (int16_t)((data[2] << 8) | data[3]);
    z = (int16_t)((data[4] << 8) | data[5]);

    return true;
}

bool MPU6000::readGyroRaw(int16_t & x, int16_t & y, int16_t & z)
{
    static const uint8_t send[7] = {GYRO_XOUT_H | 0x80, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t recv[7];

    if (!cpspi_transfer(send, recv, 7)) return false;

    x = (int16_t)((recv[1] << 8) | recv[2]);
    y = (int16_t)((recv[3] << 8) | recv[4]);
    z = (int16_t)((recv[5] << 8) | recv[6]);

    return true;
}

void MPU6000::scaleRawAccel(int16_t xraw, int16_t yraw, int16_t zraw, float & x, float & y, float & z)
{
    // Convert the accleration value into g's
    x = (float)xraw*_aRes - _accelBias[0];  
    y = (float)yraw*_aRes - _accelBias[1];   
    z = (float)zraw*_aRes - _accelBias[2];  
}

void MPU6000::scaleRawGyro(int16_t xraw, int16_t yraw, int16_t zraw, float & x, float & y, float & z)
{
    // Convert the gyro value into degrees per second
    x = (float)xraw*_gRes;  
    y = (float)yraw*_gRes;  
    z = (float)zraw*_gRes; 
}


