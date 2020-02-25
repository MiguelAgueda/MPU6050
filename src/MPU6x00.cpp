  /* 
   MPU6x00.cpp: Implementation of MPU6x00 classes

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

#include "MPU6x00.h"

#include "CrossPlatformSPI.h"

MPU6x00::MPU6x00(Ascale_t ascale, Gscale_t gscale, uint8_t sampleRateDivisor) 
    : MPU6xx0(ascale, gscale, sampleRateDivisor)
{
}

MPUIMU::Error_t MPU6x00::begin(void)
{
    //if (getId() != MPU_ADDRESS) {
    //    return ERROR_IMU_ID;
    //}

    //if (!selfTest()) {
    //    return ERROR_SELFTEST;
    //}

    cpspi_writeRegister(PWR_MGMT_1, 0x80);
    delay(100);

    cpspi_writeRegister(SIGNAL_PATH_RESET, 0x80);
    delay(100);

    cpspi_writeRegister(PWR_MGMT_1, 0x00);
    delay(100);

    cpspi_writeRegister(PWR_MGMT_1, INV_CLK_PLL);
    delay(15);

    cpspi_writeRegister(GYRO_CONFIG, _gScale << 3);
    delay(15);

    cpspi_writeRegister(ACCEL_CONFIG, _aScale << 3);
    delay(15);

    cpspi_writeRegister(CONFIG, 0); // no DLPF bits
    delay(15);

    cpspi_writeRegister(SMPLRT_DIV, _sampleRateDivisor); 
    delay(100);

    // Data ready interrupt configuration
    cpspi_writeRegister(INT_PIN_CFG, 0x10);  
    delay(15);

    cpspi_writeRegister(INT_ENABLE, 0x01); 
    delay(15);

    _accelBias[0] = 0;
    _accelBias[1] = 0;
    _accelBias[2] = 0;

    return ERROR_NONE;
}

void MPU6x00::writeMPURegister(uint8_t subAddress, uint8_t data)
{
    cpspi_writeRegister(subAddress, data);
}
