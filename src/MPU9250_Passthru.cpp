/* 
   MPU9250_Passthru.cpp: implementation of pass-through-mode MPU9250 class methods

   Copyright (C) 2018 Simon D. Levy

   Adapted from https://github.com/kriswiner/MPU9250/blob/master/MPU9250_MS5637_AHRS_t3.ino

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

#include "MPU9250_Passthru.h"

#include <CrossPlatformI2C.h>

MPU9250_Passthru::MPU9250_Passthru(Ascale_t ascale, Gscale_t gscale, Mscale_t mscale, Mmode_t mmode, uint8_t sampleRateDivisor) 
    : MPU9250(ascale, gscale, mscale, mmode, sampleRateDivisor, true)
{
}

MPUIMU::Error_t MPU9250_Passthru::begin(uint8_t i2cbus)
{
    MPU9250::begin(i2cbus);

    _i2c = cpi2c_open(MPU_ADDRESS, i2cbus);

    _mag = cpi2c_open(AK8963_ADDRESS);

    return runTests();
}
bool MPU9250_Passthru::checkNewAccelGyroData()
{
    return MPUIMU::checkNewData();
}

bool MPU9250_Passthru::checkNewMagData()
{
    return readAK8963Register(AK8963_ST1) & 0x01;
}

void MPU9250_Passthru::writeAK8963Register(uint8_t subAddress, uint8_t data)
{
    writeRegister(_mag, subAddress, data);
}

void MPU9250_Passthru::readAK8963Registers(uint8_t subAddress, uint8_t count, uint8_t* dest)
{
    readRegisters(_mag, subAddress, count, dest);
}

MPUIMU::Error_t MPU9250::begin(uint8_t bus)
{
    _mpu = cpi2c_open(MPU_ADDRESS, bus);

    return runTests();
}

void MPU9250_Passthru::readRegisters(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * data)
{
    cpi2c_readRegisters(address, subAddress, count, data);
}


void MPU9250_Passthru::writeRegister(uint8_t address, uint8_t subAddress, uint8_t data)
{
    cpi2c_writeRegister(address, subAddress, data);
}

void MPU9250_Passthru::writeMPURegister(uint8_t subAddress, uint8_t data)
{
    cpi2c_writeRegister(_i2c, subAddress, data);
}

void MPU9250_Passthru::readMPURegisters(uint8_t subAddress, uint8_t count, uint8_t * dest)
{
    cpi2c_readRegisters(_i2c, subAddress, count, dest);
}


