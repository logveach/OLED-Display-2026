/*
 * display.h
 *
 *  Created on: Nov 5, 2026
 *      Author: logveach
 *  
 *  Dynamic speedometer dashboard for OLED display
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

#include "stm32l4xx_hal.h"
#include <stdint.h>

/* Display framebuffer size (128x64 pixels / 8 bits per byte) */
#define DISPLAY_FB_SIZE (128 * 64 / 8)

/* Pixel value enum */
typedef enum {
    DISPLAY_PIXEL_OFF = 0,
    DISPLAY_PIXEL_ON  = 1
} Display_Pixel;

/* Display driver callback function types */
typedef HAL_StatusTypeDef (*Display_InitFn)(void *ctx);
typedef HAL_StatusTypeDef (*Display_FlushFn)(void *ctx, const uint8_t *fb, uint16_t fb_size);

/* Display driver structure */
typedef struct {
    void               *ctx;     /* Driver context/handle */
    Display_InitFn      init;    /* Initialization callback */
    Display_FlushFn     flush;   /* Flush framebuffer callback */
} Display_Driver;

/* Dashboard data structure */
typedef struct {
    uint16_t speed;              /* Current speed in MPH (0-999) */
    uint8_t  rpm;                /* Engine RPM indicator (0-100) */
    uint8_t  fuel_level;         /* Fuel level percentage (0-100) */
} Display_Data;

/* Display API functions */
HAL_StatusTypeDef Display_Init(Display_Driver driver);
HAL_StatusTypeDef Display_Flush(void);
void Display_Clear(Display_Pixel value);

/* Dashboard functions */
void Display_DrawBackground(void);
void Display_DrawSpeed(uint16_t speed);
void Display_DrawRPMBar(uint8_t rpm_percent);
void Display_DrawFuelLevel(uint8_t fuel_percent);
void Display_Update(const Display_Data *data);
void Display_Refresh(void);

#endif /* INC_DISPLAY_H_ */

