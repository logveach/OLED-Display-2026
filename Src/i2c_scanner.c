/*
 * i2c_scanner.c
 *
 *  Created on: Nov 5, 2026
 *      Author: lveac
 *  
 *  I2C device scanner - detects all devices on the I2C bus
 */

#include "i2c_scanner.h"
#include "oled_ssd1306.h"
#include <stdio.h>

/*
 * Scan the I2C bus for all connected devices
 * Tries addresses from 0x08 to 0x77 (excludes reserved addresses)
 * Displays results on OLED
 * Returns the number of devices found
 */
uint8_t I2C_ScanBus(I2C_HandleTypeDef *hi2c)
{
    uint8_t device_count = 0;
    uint8_t devices[10];  /* Store up to 10 device addresses */
    char buffer[20];
    
    /* Scan from 0x08 to 0x77 (exclude reserved addresses) */
    for (uint8_t addr = 0x08; addr <= 0x77; addr++) {
        /* Skip reserved addresses */
        if ((addr & 0xF8) == 0x00 || (addr & 0xF8) == 0xF8) {
            continue;
        }
        
        /* Try to communicate with device at this address */
        if (HAL_I2C_IsDeviceReady(hi2c, addr << 1, 3, 5) == HAL_OK) {
            if (device_count < 10) {
                devices[device_count] = addr;
                device_count++;
            }
        }
    }
    
    /* Display results on OLED */
    OLED_Clear(OLED_BLACK);
    OLED_DrawText(0, 0, "I2C SCAN", 2, OLED_WHITE);
    OLED_DrawText(0, 16, "Devices:", 1, OLED_WHITE);
    
    if (device_count == 0) {
        OLED_DrawText(0, 24, "None found", 1, OLED_WHITE);
    } else {
        for (uint8_t i = 0; i < device_count && i < 4; i++) {
            snprintf(buffer, sizeof(buffer), "0x%02X (0x%02X)", 
                     devices[i], devices[i] << 1);
            OLED_DrawText(0, 24 + (i * 8), buffer, 1, OLED_WHITE);
        }
        
        if (device_count > 4) {
            snprintf(buffer, sizeof(buffer), "+%d more", device_count - 4);
            OLED_DrawText(0, 56, buffer, 1, OLED_WHITE);
        }
    }
    
    OLED_Update();
    
    return device_count;
}

