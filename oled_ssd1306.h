/*
 * oled_ssd1306.h
 *
 *  Created on: 10/31/2026
 *      Author: lveac
 */
#ifndef OLED_SSD1306_H
#define OLED_SSD1306_H

#include "stm32l4xx_hal.h"
#include <stdint.h>

#define OLED_WIDTH        128
#define OLED_HEIGHT       64
#define OLED_I2C_ADDR     (0x3C << 1)   /* change to (0x3D << 1) if blank */
#define OLED_I2C_ADDR_ALT (0x3D << 1)

typedef enum { OLED_BLACK = 0, OLED_WHITE = 1 } OLED_Color;

/* Init tries 0x3C then 0x3D. Returns HAL_ERROR if nothing answers. */
HAL_StatusTypeDef OLED_Init(I2C_HandleTypeDef *hi2c);

void OLED_Clear(OLED_Color color);
void OLED_DrawPixel(int16_t x, int16_t y, OLED_Color color);
void OLED_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, OLED_Color color);
void OLED_DrawRect(int16_t x, int16_t y, int16_t w, int16_t h, OLED_Color color);
int16_t OLED_TextWidth(const char *str, uint8_t scale);
void    OLED_DrawText(int16_t x, int16_t y, const char *str, uint8_t scale, OLED_Color color);
HAL_StatusTypeDef OLED_Update(void);

#endif /* OLED_SSD1306_H */
