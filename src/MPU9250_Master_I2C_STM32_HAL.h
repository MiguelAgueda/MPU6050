/* 
   MPU9250_Master_I2C_STM32_HAL.h I^2C support for MPU9250 using ST's HAL

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

#pragma once
#ifdef STM32H7xx
    #include "stm32h7xx_hal.h"
#endif

#include "MPU9250_Master.h"

class MPU9250_Master_I2C_STM32_HAL : public MPU9250_Master {

    public:

        MPU9250_Master_I2C_STM32_HAL(Ascale_t ascale, Gscale_t gscale, Mscale_t mscale, Mmode_t mmode, I2C_HandleTypeDef *handleI2C, uint8_t sampleRateDivisor=0);

        Error_t begin(uint8_t bus=1);

        virtual void writeMPURegister(uint8_t subAddress, uint8_t data) override;

        virtual void readMPURegisters(uint8_t subAddress, uint8_t count, uint8_t * dest) override;

        virtual void readRegisters(uint8_t address, uint8_t subAddress, uint8_t count, uint8_t * data) override;

        virtual void writeRegister(uint8_t address, uint8_t subAddress, uint8_t data) override;


    private:
        // I2C using STMicroelectronics HAL layers
        I2C_HandleTypeDef *_hi2c;

};
