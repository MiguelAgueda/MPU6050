  /* 
   MPU6xx0.cpp: Implementation of MPU6xx0 classes

   Copyright (C) 2018 Simon D. Levy

   Adapted from https://github.com/kriswiner/MPU6050/blob/master/MPU6050BasicExample.ino

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

#include "MPU6xx0.h"

#include <math.h>

bool MPU6xx0::checkNewData(void)
{
    return MPUIMU::checkNewData();
}

MPU6xx0::MPU6xx0(Ascale_t ascale, Gscale_t gscale, uint8_t sampleRateDivisor) 
    : MPUIMU(ascale, gscale, sampleRateDivisor)
{
}

MPUIMU::Error_t MPU6xx0::begin(void)
{
    if (getId() != MPU_ADDRESS) {
        return ERROR_IMU_ID;
    }

    if (!selfTest()) {
        return ERROR_SELFTEST;
    }


    calibrate();

    init();

    return ERROR_NONE;
}

void MPU6xx0::readGyrometer(float & gx, float & gy, float & gz)
{
    MPUIMU::readGyrometer(gx, gy, gz);

    gx -= _gyroBias[0];
    gy -= _gyroBias[1];  
    gz -= _gyroBias[2];   
}

float MPU6xx0::readTemperature()
{
    int16_t t = MPUIMU::readRawTemperature();
    return ((float) t) / 340. + 36.53; // Temperature in degrees Centigrade
}

// Configure the motion detection control for low power accelerometer mode
void MPU6xx0::lowPowerAccelOnly()
{
    // The sensor has a high-pass filter necessary to invoke to allow the sensor motion detection algorithms work properly
    // Motion detection occurs on free-fall (acceleration below a threshold for some time for all axes), motion (acceleration
    // above a threshold for some time on at least one axis), and zero-motion toggle (acceleration on each axis less than a
    // threshold for some time sets this flag, motion above the threshold turns it off). The high-pass filter takes gravity out
    // consideration for these threshold evaluations; otherwise, the flags would be set all the time!

    uint8_t c = readMPURegister(PWR_MGMT_1);
    writeMPURegister(PWR_MGMT_1, c & ~0x30); // Clear sleep and cycle bits [5:6]
    writeMPURegister(PWR_MGMT_1, c |  0x30); // Set sleep and cycle bits [5:6] to zero to make sure accelerometer is running

    c = readMPURegister(PWR_MGMT_2);
    writeMPURegister(PWR_MGMT_2, c & ~0x38); // Clear standby XA, YA, and ZA bits [3:5]
    writeMPURegister(PWR_MGMT_2, c |  0x00); // Set XA, YA, and ZA bits [3:5] to zero to make sure accelerometer is running

    c = readMPURegister(ACCEL_CONFIG);
    writeMPURegister(ACCEL_CONFIG, c & ~0x07); // Clear high-pass filter bits [2:0]
    // Set high-pass filter to 0) reset (disable), 1) 5 Hz, 2) 2.5 Hz, 3) 1.25 Hz, 4) 0.63 Hz, or 7) Hold
    writeMPURegister(ACCEL_CONFIG,  c | 0x00);  // Set ACCEL_HPF to 0; reset mode disbaling high-pass filter

    c = readMPURegister(CONFIG);
    writeMPURegister(CONFIG, c & ~0x07); // Clear low-pass filter bits [2:0]
    writeMPURegister(CONFIG, c |  0x00);  // Set DLPD_CFG to 0; 260 Hz bandwidth, 1 kHz rate

    c = readMPURegister(INT_ENABLE);
    writeMPURegister(INT_ENABLE, c & ~0xFF);  // Clear all interrupts
    writeMPURegister(INT_ENABLE, 0x40);  // Enable motion threshold (bits 5) interrupt only

    // Motion detection interrupt requires the absolute value of any axis to lie above the detection threshold
    // for at least the counter duration
    writeMPURegister(MOT_THR, 0x80); // Set motion detection to 0.256 g; LSB = 2 mg
    writeMPURegister(MOT_DUR, 0x01); // Set motion detect duration to 1  ms; LSB is 1 ms @ 1 kHz rate

    delay (100);  // Add delay for accumulation of samples

    c = readMPURegister(ACCEL_CONFIG);
    writeMPURegister(ACCEL_CONFIG, c & ~0x07); // Clear high-pass filter bits [2:0]
    writeMPURegister(ACCEL_CONFIG, c |  0x07);  // Set ACCEL_HPF to 7; hold the initial accleration value as a referance

    c = readMPURegister(PWR_MGMT_2);
    writeMPURegister(PWR_MGMT_2, c & ~0xC7); // Clear standby XA, YA, and ZA bits [3:5] and LP_WAKE_CTRL bits [6:7]
    writeMPURegister(PWR_MGMT_2, c |  0x47); // Set wakeup frequency to 5 Hz, and disable XG, YG, and ZG gyros (bits [0:2])

    c = readMPURegister(PWR_MGMT_1);
    writeMPURegister(PWR_MGMT_1, c & ~0x20); // Clear sleep and cycle bit 5
    writeMPURegister(PWR_MGMT_1, c |  0x20); // Set cycle bit 5 to begin low power accelerometer motion interrupts

}

void MPU6xx0::init(void)
{
    // wake up device-don't need this here if using calibration function below
    //  writeMPURegister(PWR_MGMT_1, 0x00); // Clear sleep mode bit (6), enable all sensors
    //  delay(100); // Delay 100 ms for PLL to get established on x-axis gyro; should check for PLL ready interrupt

    // get stable time source
    writeMPURegister(PWR_MGMT_1, 0x01);  // Set clock source to be PLL with x-axis gyroscope reference, bits 2:0 = 001

    // Configure Gyro and Accelerometer
    // Disable FSYNC and set accelerometer and gyro bandwidth to 44 and 42 Hz, respectively;
    // DLPF_CFG = bits 2:0 = 010; this sets the sample rate at 1 kHz for both
    // Maximum delay time is 4.9 ms corresponding to just over 200 Hz sample rate
    writeMPURegister(CONFIG, 0x03);

    // Set sample rate = gyroscope output rate/(1 + SMPLRT_DIV)
    writeMPURegister(SMPLRT_DIV, 0x04);  // Use a 200 Hz rate; the same rate set in CONFIG above

    // Set gyroscope full scale range
    // Range selects FS_SEL and AFS_SEL are 0 - 3, so 2-bit values are left-shifted into positions 4:3
    uint8_t c =  readMPURegister(GYRO_CONFIG);
    writeMPURegister(GYRO_CONFIG, c & ~0xE0); // Clear self-test bits [7:5]
    writeMPURegister(GYRO_CONFIG, c & ~0x18); // Clear AFS bits [4:3]
    writeMPURegister(GYRO_CONFIG, c | _gScale << 3); // Set full scale range for the gyro

    // Set accelerometer configuration
    c =  readMPURegister(ACCEL_CONFIG);
    writeMPURegister(ACCEL_CONFIG, c & ~0xE0); // Clear self-test bits [7:5]
    writeMPURegister(ACCEL_CONFIG, c & ~0x18); // Clear AFS bits [4:3]
    writeMPURegister(ACCEL_CONFIG, c | _aScale << 3); // Set full scale range for the accelerometer

    // Configure Interrupts and Bypass Enable
    // Set interrupt pin active high, push-pull, and clear on read of INT_STATUS, enable I2C_BYPASS_EN so additional chips
    // can join the I2C bus and all can be controlled by the Arduino as master
    writeMPURegister(INT_PIN_CFG, 0x22);
    writeMPURegister(INT_ENABLE, 0x01);  // Enable data ready (bit 0) interrupt
}

// Accelerometer and gyroscope self test; check calibration wrt factory settings.
// Should return percent deviation from factory trim values, +/- 14 or less deviation is a pass.
bool MPU6xx0::selfTest(void)
{
    uint8_t rawData[4];
    uint8_t selfTest[6];
    float factoryTrim[6];

    // Configure the accelerometer for self-test
    writeMPURegister(ACCEL_CONFIG, 0xF0); // Enable self test on all three axes and set accelerometer range to +/- 8 g
    writeMPURegister(GYRO_CONFIG,  0xE0); // Enable self test on all three axes and set gyro range to +/- 250 degrees/s
    delay(250);  // Delay a while to let the device execute the self-test
    rawData[0] = readMPURegister(SELF_TEST_X_ACCEL); // X-axis self-test results
    rawData[1] = readMPURegister(SELF_TEST_Y_ACCEL); // Y-axis self-test results
    rawData[2] = readMPURegister(SELF_TEST_Z_ACCEL); // Z-axis self-test results
    rawData[3] = readMPURegister(SELF_TEST_A); // Mixed-axis self-test results
    // Extract the acceleration test results first
    selfTest[0] = (rawData[0] >> 3) | (rawData[3] & 0x30) >> 4 ; // XA_TEST result is a five-bit unsigned integer
    selfTest[1] = (rawData[1] >> 3) | (rawData[3] & 0x0C) >> 2 ; // YA_TEST result is a five-bit unsigned integer
    selfTest[2] = (rawData[2] >> 3) | (rawData[3] & 0x03) ; // ZA_TEST result is a five-bit unsigned integer
    // Extract the gyration test results first
    selfTest[3] = rawData[0]  & 0x1F ; // XG_TEST result is a five-bit unsigned integer
    selfTest[4] = rawData[1]  & 0x1F ; // YG_TEST result is a five-bit unsigned integer
    selfTest[5] = rawData[2]  & 0x1F ; // ZG_TEST result is a five-bit unsigned integer
    // Process results to allow final comparison with factory set values
    factoryTrim[0] = (4096.0 * 0.34) * (pow( (0.92 / 0.34) , (((float)selfTest[0] - 1.0) / 30.0))); // FT[Xa] factory trim calculation
    factoryTrim[1] = (4096.0 * 0.34) * (pow( (0.92 / 0.34) , (((float)selfTest[1] - 1.0) / 30.0))); // FT[Ya] factory trim calculation
    factoryTrim[2] = (4096.0 * 0.34) * (pow( (0.92 / 0.34) , (((float)selfTest[2] - 1.0) / 30.0))); // FT[Za] factory trim calculation
    factoryTrim[3] =  ( 25.0 * 131.0) * (pow( 1.046 , ((float)selfTest[3] - 1.0) ));         // FT[Xg] factory trim calculation
    factoryTrim[4] =  (-25.0 * 131.0) * (pow( 1.046 , ((float)selfTest[4] - 1.0) ));         // FT[Yg] factory trim calculation
    factoryTrim[5] =  ( 25.0 * 131.0) * (pow( 1.046 , ((float)selfTest[5] - 1.0) ));         // FT[Zg] factory trim calculation

    //  Output self-test results and factory trim calculation if desired
    //  Serial.println(selfTest[0]); Serial.println(selfTest[1]); Serial.println(selfTest[2]);
    //  Serial.println(selfTest[3]); Serial.println(selfTest[4]); Serial.println(selfTest[5]);
    //  Serial.println(factoryTrim[0]); Serial.println(factoryTrim[1]); Serial.println(factoryTrim[2]);
    //  Serial.println(factoryTrim[3]); Serial.println(factoryTrim[4]); Serial.println(factoryTrim[5]);

    // Report results as a ratio of (STR - FT)/FT; the change from Factory Trim of the Self-Test Response
    // To get to percent, must multiply by 100 and subtract result from 100
    float tolerances[6];
    for (int i = 0; i < 6; i++) {
        tolerances[i] = 100.0 + 100.0 * ((float)selfTest[i] - factoryTrim[i]) / factoryTrim[i]; // Report percent differences
    }

    // Check results
    for (uint8_t k=0; k<6; ++k) {
        if (tolerances[k] >= 14.f) {
            return false;
        }
    }

    // Success!
    return true;
}
