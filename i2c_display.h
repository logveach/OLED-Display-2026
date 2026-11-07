/*
 * i2c_display.h
 *
 *  Created on: Nov 5, 2026
 *      Author: lveac
 *  
 *  Display I2C scan results on OLED in readable format
 */

#ifndef INC_I2C_DISPLAY_H_
#define INC_I2C_DISPLAY_H_

#include "stm32l4xx_hal.h"

/*
 * Scan I2C bus and display results on OLED
 * Shows all detected device addresses in a readable layout
 */
void I2C_DisplayScan(I2C_HandleTypeDef *hi2c);

#endif /* INC_I2C_DISPLAY_H_ */
