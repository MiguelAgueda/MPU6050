/* 
   MPU9250_Master_I2C_STM32_HAL.cpp I^2C support for MPU9250 using ST's HAL

   Copyright (C) 2020 Hector PHARAM

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

#include "MPU9250_Master_I2C_STM32_HAL.h"

MPU9250_Master_I2C_STM32_HAL::MPU9250_Master_I2C_STM32_HAL(Ascale_t ascale, Gscale_t gscale, Mscale_t mscale, Mmode_t mmode, I2C_HandleTypeDef *handleI2C, uint8_t sampleRateDivisor) :
    MPU9250_Master(ascale, gscale, mscale, mmode, sampleRateDivisor)
{
    _hi2c = handleI2C;
}

MPUIMU::Error_t MPU9250_Master_I2C_STM32_HAL::begin(uint8_t bus)
{
    return runTests();
}

void MPU9250_Master_I2C_STM32_HAL::readRegisters(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * data)
{
    HAL_I2C_Mem_Read(_hi2c, address << 1, subAddress, 1, data, count, 100);
}


void MPU9250_Master_I2C_STM32_HAL::writeRegister(uint8_t address, uint8_t subAddress, uint8_t data)
{
    HAL_I2C_Mem_Write(_hi2c, address << 1, subAddress, 1, &data, 1, 100);
}

void MPU9250_Master_I2C_STM32_HAL::writeMPURegister(uint8_t subAddress, uint8_t data)
{
    HAL_I2C_Mem_Write(_hi2c, MPU_ADDRESS << 1, subAddress, 1, &data, 1, 100);
}

void MPU9250_Master_I2C_STM32_HAL::readMPURegisters(uint8_t subAddress, uint8_t count, uint8_t * dest)
{
    HAL_I2C_Mem_Read(_hi2c, MPU_ADDRESS << 1, subAddress, 1, dest, count, 100);
}
