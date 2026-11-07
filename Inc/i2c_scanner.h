/*
 * i2c_scanner.h
 *
 *  Created on: Nov 5, 2026
 *      Author: lveac
 *  
 *  I2C device scanner - detects all devices on the I2C bus
 */

#ifndef INC_I2C_SCANNER_H_
#define INC_I2C_SCANNER_H_

#include "stm32l4xx_hal.h"

/*
 * Scan the I2C bus and print all detected device addresses
 * Returns the number of devices found
 */
uint8_t I2C_ScanBus(I2C_HandleTypeDef *hi2c);

#endif /* INC_I2C_SCANNER_H_ */
