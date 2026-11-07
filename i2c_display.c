/*
 * i2c_display.c
 *
 *  Created on: Nov 5, 2026
 *      Author: lveac
 *  
 *  Display I2C scan results on OLED in readable format
 */

#include "i2c_display.h"
#include "oled_ssd1306.h"
#include <stdio.h>
#include <string.h>

void I2C_DisplayScan(I2C_HandleTypeDef *hi2c)
{
    uint8_t devices[10];
    uint8_t device_count = 0;
    char line_buffer[10];
    
    /* Scan I2C addresses */
    for (uint8_t addr = 0x08; addr <= 0x77; addr++) {
        if ((addr & 0xF8) == 0x00 || (addr & 0xF8) == 0xF8) {
            continue;
        }
        
        if (HAL_I2C_IsDeviceReady(hi2c, addr << 1, 3, 5) == HAL_OK) {
            if (device_count < 10) {
                devices[device_count] = addr;
                device_count++;
            }
        }
    }
    
    /* Clear display */
    OLED_Clear(OLED_BLACK);
    
    int16_t y_pos = 5;
    
    if (device_count == 0) {
        OLED_DrawText(20, 30, "No devices", 1, OLED_WHITE);
    } else {
        for (uint8_t i = 0; i < device_count && i < 7; i++) {
            snprintf(line_buffer, sizeof(line_buffer), "0x%02X", devices[i]);
            OLED_DrawText(40, y_pos, line_buffer, 1, OLED_WHITE);
            y_pos += 8;
        }
    }
    
    OLED_Update();
}
