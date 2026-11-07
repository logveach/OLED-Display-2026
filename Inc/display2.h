/*
 * display2.h
 *
 *  Created on: Nov 5, 2026
 *      Author: lveac
 *  
 *  Second OLED display - Battery/Percentage display (I2C3)
 */

#ifndef INC_DISPLAY2_H_
#define INC_DISPLAY2_H_

#include "stm32l4xx_hal.h"

/* Battery data structure */
typedef struct {
    uint8_t percentage;  /* 0-100% */
} Display2_Data;

/* Initialize and display battery percentage */
void Display2_Init(I2C_HandleTypeDef *hi2c);

/* Update display with new percentage */
void Display2_Update(I2C_HandleTypeDef *hi2c, const Display2_Data *data);

/* Draw the percentage display (0-100) */
void Display2_DrawPercentage(uint8_t percentage);

#endif /* INC_DISPLAY2_H_ */
