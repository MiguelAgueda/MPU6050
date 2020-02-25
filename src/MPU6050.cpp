  /* 
   MPU6050.cpp: Implementation of MPU6050 class methods

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

#include "MPU6050.h"

#include "CrossPlatformI2C_Core.h"

MPU6050::MPU6050(Ascale_t ascale, Gscale_t gscale, uint8_t sampleRateDivisor) 
    : MPU6xx0(ascale, gscale, sampleRateDivisor)
{
}

MPUIMU::Error_t MPU6050::begin(uint8_t bus)
{
    _i2c = cpi2c_open(MPU_ADDRESS, bus);

    return MPU6xx0::begin();
}

void MPU6050::writeMPURegister(uint8_t subAddress, uint8_t data)
{
    cpi2c_writeRegister(_i2c, subAddress, data);
}

void MPU6050::readMPURegisters(uint8_t subAddress, uint8_t count, uint8_t * dest)
{
    cpi2c_readRegisters(_i2c, subAddress, count, dest);
}


