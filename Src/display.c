/*
 * display.c
 *
 *  Created on: Nov 5, 2026
 *      Author: lveac
 *  
 *  Dynamic speedometer dashboard - consolidated implementation
 *  with proper 7-segment LCD display
 */
#include "font5x7.h"
#include <string.h>
#include "display.h"
#include "oled_ssd1306.h"

#define GLYPH_GAP 1   /* blank pixel columns between characters */

static uint8_t          fb[DISPLAY_FB_SIZE];
static Display_Driver   drv;
static uint8_t          drv_set;
static Display_Data     current_data = {0, 0, 0};

/*
 * 7-Segment display segment layout:
 *  _a_
 * |   |
 * f   b
 * |_g_|
 * |   |
 * e   c
 * |_d_|
 */

typedef struct {
    uint8_t a;  /* top */
    uint8_t b;  /* top-right */
    uint8_t c;  /* bottom-right */
    uint8_t d;  /* bottom */
    uint8_t e;  /* bottom-left */
    uint8_t f;  /* top-left */
    uint8_t g;  /* middle */
} SevenSegment;

/* 7-segment patterns for digits 0-9 */
static const SevenSegment segments[10] = {
    {1, 1, 1, 1, 1, 1, 0},  /* 0 */
    {0, 1, 1, 0, 0, 0, 0},  /* 1 */
    {1, 1, 0, 1, 1, 0, 1},  /* 2 */
    {1, 1, 1, 1, 0, 0, 1},  /* 3 */
    {0, 1, 1, 0, 0, 1, 1},  /* 4 */
    {1, 0, 1, 1, 0, 1, 1},  /* 5 */
    {1, 0, 1, 1, 1, 1, 1},  /* 6 */
    {1, 1, 1, 0, 0, 0, 0},  /* 7 */
    {1, 1, 1, 1, 1, 1, 1},  /* 8 */
    {1, 1, 1, 1, 0, 1, 1},  /* 9 */
};

/*
 * Draw a single 7-segment LCD digit in black on white background
 * x, y: top-left corner position
 * width: segment width in pixels
 * height: segment height in pixels
 * digit: 0-9
 */
static void draw_seven_segment(int16_t x, int16_t y, int16_t width, int16_t height, uint8_t digit)
{
    if (digit > 9) return;
    
    const SevenSegment *seg = &segments[digit];
    int16_t thickness = 5;  /* Extra thick lines for bold display */
    
    /* Segment 'a' (top horizontal) */
    if (seg->a) {
        OLED_FillRect(x + thickness, y, width - 2*thickness, thickness, OLED_BLACK);
    }
    
    /* Segment 'b' (top-right vertical) */
    if (seg->b) {
        OLED_FillRect(x + width - thickness, y + thickness, thickness, height/2 - thickness, OLED_BLACK);
    }
    
    /* Segment 'c' (bottom-right vertical) */
    if (seg->c) {
        OLED_FillRect(x + width - thickness, y + height/2, thickness, height/2 - thickness, OLED_BLACK);
    }
    
    /* Segment 'd' (bottom horizontal) */
    if (seg->d) {
        OLED_FillRect(x + thickness, y + height - thickness, width - 2*thickness, thickness, OLED_BLACK);
    }
    
    /* Segment 'e' (bottom-left vertical) */
    if (seg->e) {
        OLED_FillRect(x, y + height/2, thickness, height/2 - thickness, OLED_BLACK);
    }
    
    /* Segment 'f' (top-left vertical) */
    if (seg->f) {
        OLED_FillRect(x, y + thickness, thickness, height/2 - thickness, OLED_BLACK);
    }
    
    /* Segment 'g' (middle horizontal) */
    if (seg->g) {
        OLED_FillRect(x + thickness, y + height/2 - thickness/2, width - 2*thickness, thickness, OLED_BLACK);
    }
}

HAL_StatusTypeDef Display_Init(Display_Driver driver)
{
    if (driver.init == NULL || driver.flush == NULL) {
        return HAL_ERROR;
    }
    drv = driver;
    drv_set = 1;
    return drv.init(drv.ctx);
}

HAL_StatusTypeDef Display_Flush(void)
{
    if (!drv_set) {
        return HAL_ERROR;
    }
    return drv.flush(drv.ctx, fb, DISPLAY_FB_SIZE);
}

void Display_Clear(Display_Pixel value)
{
    memset(fb, (value == DISPLAY_PIXEL_ON) ? 0xFF : 0x00, sizeof(fb));
}

void Display_DrawBackground(void)
{
    /* Fill entire display with white background */
    OLED_FillRect(0, 0, 128, 64, OLED_WHITE);
}

/*
 * Draw speed value as three 7-segment LCD digits
 * Speed range: 0-999 MPH
 */
void Display_DrawSpeed(uint16_t speed)
{
    if (speed > 99) speed = 99;  /* Limit to 2 digits (0-99) */
    
    uint8_t tens = (speed / 10) % 10;
    uint8_t ones = speed % 10;
    
    /* Larger digit dimensions for better visibility */
    int16_t digit_width = 36;
    int16_t digit_height = 50;
    int16_t digit_gap = 8;
    
    /* Shift left to make room for MPH */
    int16_t start_x = 4;   /* Reduced from 10 */
    int16_t start_y = 6;   /* Top padding */
    
    /* Draw the two digits in black on white background */
    draw_seven_segment(start_x, start_y, digit_width, digit_height, tens);
    draw_seven_segment(start_x + digit_width + digit_gap, start_y, digit_width, digit_height, ones);
    
    /* Draw MPH label larger - scale 2 for better visibility */
    int16_t mph_x = start_x + 2*digit_width + digit_gap + 3;
    int16_t mph_y = start_y + 12;
    OLED_DrawText(mph_x, mph_y, "MPH", 2, OLED_BLACK);
}

void Display_Update(const Display_Data *data)
{
    if (data == NULL) return;
    
    memcpy(&current_data, data, sizeof(Display_Data));
    
    /* Clear and redraw everything */
    Display_DrawBackground();
    Display_DrawSpeed(current_data.speed);
    
    Display_Refresh();
}

void Display_Refresh(void)
{
    OLED_Update();
}
