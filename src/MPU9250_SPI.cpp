/*  Implementation code for MPU9250 class library
 *
 *  Copyright 2017 Tlera Corporation
 *  
 *  Created by Kris Winer
 *
 *  Adapted by Simon D. Levy 19 April 2018
 *  
 *  Library may be used freely and without limit with attribution.
 */

#include "MPU9250_SPI.h"

MPU_Error_t MPU9250_SPI::begin(void)
{
    return runTests();
}

void MPU9250_SPI::initMPU6500(Ascale_t ascale, Gscale_t gscale, uint8_t sampleRateDivisor) 
{ 
    MPU9250::initMPU6500(ascale, gscale, sampleRateDivisor, false); 
}

void MPU9250_SPI::writeAK8963Register(uint8_t subAddress, uint8_t data)
{
    uint8_t count = 1;

    writeMPURegister(I2C_SLV0_ADDR, AK8963_ADDRESS); // set slave 0 to the AK8963 and set for write
    writeMPURegister(I2C_SLV0_REG, subAddress); // set the register to the desired AK8963 sub address
    writeMPURegister(I2C_SLV0_DO, data); // store the data for write
    writeMPURegister(I2C_SLV0_CTRL, I2C_SLV0_EN | count); // enable I2C and send 1 byte
}

void MPU9250_SPI::readAK8963Registers(uint8_t subAddress, uint8_t count, uint8_t* dest)
{
    writeMPURegister(I2C_SLV0_ADDR, AK8963_ADDRESS | I2C_READ_FLAG); // set slave 0 to the AK8963 and set for read
    writeMPURegister(I2C_SLV0_REG, subAddress); // set the register to the desired AK8963 sub address
    writeMPURegister(I2C_SLV0_CTRL, I2C_SLV0_EN | count); // enable I2C and request the bytes
    delay(1); // takes some time for these registers to fill
    readMPURegisters(EXT_SENS_DATA_00, count, dest); // read the bytes off the MPU9250 EXT_SENS_DATA registers
}

bool MPU9250_SPI::checkNewData(void)
{
    return (readMPURegister(INT_STATUS) & 0x01);
}


