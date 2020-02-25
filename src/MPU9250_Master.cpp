/* 
   MPU9250_Master.cpp: implementation of master-mode MPU9250 class methods

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

#include "MPU9250_Master.h"

MPU9250_Master::MPU9250_Master(Ascale_t ascale, Gscale_t gscale, Mscale_t mscale, Mmode_t mmode, uint8_t sampleRateDivisor) 
    : MPU9250(ascale, gscale, mscale, mmode, sampleRateDivisor, false)
{
}

void MPU9250_Master::initMPU6500(Ascale_t ascale, Gscale_t gscale, uint8_t sampleRateDivisor) 
{ 
    MPU9250::initMPU6500(ascale, gscale, sampleRateDivisor, false); 
}

void MPU9250_Master::writeAK8963Register(uint8_t subAddress, uint8_t data)
{
    uint8_t count = 1;

    writeMPURegister(I2C_SLV0_ADDR, AK8963_ADDRESS); // set slave 0 to the AK8963 and set for write
    writeMPURegister(I2C_SLV0_REG, subAddress); // set the register to the desired AK8963 sub address
    writeMPURegister(I2C_SLV0_DO, data); // store the data for write
    writeMPURegister(I2C_SLV0_CTRL, I2C_SLV0_EN | count); // enable I2C and send 1 byte
}

void MPU9250_Master::readAK8963Registers(uint8_t subAddress, uint8_t count, uint8_t* dest)
{
    writeMPURegister(I2C_SLV0_ADDR, AK8963_ADDRESS | I2C_READ_FLAG); // set slave 0 to the AK8963 and set for read
    writeMPURegister(I2C_SLV0_REG, subAddress); // set the register to the desired AK8963 sub address
    writeMPURegister(I2C_SLV0_CTRL, I2C_SLV0_EN | count); // enable I2C and request the bytes
    delay(1); // takes some time for these registers to fill
    readMPURegisters(EXT_SENS_DATA_00, count, dest); // read the bytes off the MPU9250 EXT_SENS_DATA registers
}

bool MPU9250_Master::checkNewData(void)
{
    return (readMPURegister(INT_STATUS) & 0x01);
}
