  /* 
   MPU6500.cpp: Implementation of MPU6500 class methods

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

#include "MPU6500.h"

#include <CrossPlatformSPI.h>

MPU6500::MPU6500(Ascale_t ascale, Gscale_t gscale, uint8_t sampleRateDivisor) 
    : MPU6x00(ascale, gscale, sampleRateDivisor)
{
}

MPUIMU::Error_t MPU6500::begin(void)
{
    return MPU6x00::begin();
}

bool MPU6500::checkNewData(void)
{
    return MPUIMU::checkNewData();
}

void MPU6500::readGyrometer(float & gx, float & gy, float & gz)
{
    MPUIMU::readGyrometer(gx, gy, gz);
}

void MPU6500::readMPURegisters(uint8_t subAddress, uint8_t count, uint8_t * dest) 
{
    cpspi_readRegisters(subAddress | 0x80, count, dest);
}
