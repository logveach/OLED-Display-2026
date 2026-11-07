/*
 * display2.c
 *
 *  Created on: Nov 5, 2026
 *      Author: lveac
 *  
 *  Second OLED display - Battery/Percentage display (I2C3)
 *  Shows two 7-segment digits with a "%" label
 */

#include "display2.h"
#include "oled_ssd1306.h"
#include <string.h>

static I2C_HandleTypeDef *display2_hi2c = NULL;

/* 7-Segment display segment layout */
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

/* Draw a single 7-segment LCD digit in black on white background */
static void draw_seven_segment_2(int16_t x, int16_t y, int16_t width, int16_t height, uint8_t digit)
{
    if (digit > 9) return;
    
    const SevenSegment *seg = &segments[digit];
    int16_t thickness = 5;  /* Extra thick lines */
    
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

/* Draw the % logo symbol - from provided bitmap */
static void draw_percent_sign(int16_t x, int16_t y)
{
    /* Bitmap data provided by user (12 bits wide x 12 rows) */
    const uint16_t logo_bitmap[16] = {
        0x7807,
        0xFC0F,
        0xCC1F,
        0xCC3E,
        0xFC7C,
        0x78F8,
        0x01F0,
        0x03E0,
		0x07C0,
		0x0F80,
        0x1F1E,
        0x3E3F,
        0x7C33,
        0xF833,
		0xF03F,
		0xE01E,
    };
    
    for (int16_t row = 0; row < 16; row++) {
        uint16_t pixels = logo_bitmap[row];
        for (int16_t col = 0; col < 16; col++) {
            if (pixels & (1u << (15 - col))) {
                OLED_DrawPixel(x + col, y + row, OLED_BLACK);
            }
        }
    }
}
static void draw_cat(int16_t x, int16_t y)
{
	const uint16_t cat_bitmap[16] = {
		0x0000,
		0x700E,
		0x5FFA,
		0x4002,
		0x6006,
		0x2E74,
		0x6426,
		0x4002,
		0x4002,
		0x43C2,
		0x4182,
		0x6006,
		0x33CC,
		0x1818,
		0x0C30,
		0x07E0,
	};

	for (int16_t row = 0; row < 16; row++) {
		uint16_t pixels = cat_bitmap[row];
		for (int16_t col = 0; col < 16; col++) {
			if (pixels & (1u << (15-col))) {
				OLED_DrawPixel(x + col, y + row, OLED_BLACK);
			}
		}
	}
}

void Display2_Init(I2C_HandleTypeDef *hi2c)
{
    display2_hi2c = hi2c;
    OLED_Clear(OLED_BLACK);
}

void Display2_DrawPercentage(uint8_t percentage)
{
    if (percentage > 100) percentage = 100;
    
    /* Extract digits - always show 100 as 1, 0, 0 */
    uint8_t hundreds = 1;  /* Always "1" for 100% */
    uint8_t tens = 0;      /* "0" */
    uint8_t ones = 0;      /* "0" */
    
    /* Same large digit dimensions as display.c */
    int16_t digit_width = 36;
    int16_t digit_height = 50;
    int16_t digit_gap = 8;
    
    /* Further left positioning */
    int16_t start_x = -20;  /* Shift even further left */
    int16_t start_y = 6;
    
    /* Clear display with white background */
    OLED_FillRect(0, 0, 128, 64, OLED_WHITE);
    
    /* Draw the three digits in black on white background */
    draw_seven_segment_2(start_x, start_y, digit_width, digit_height, hundreds);
    draw_seven_segment_2(start_x + digit_width + digit_gap, start_y, digit_width, digit_height, tens);
    draw_seven_segment_2(start_x + 2*(digit_width + digit_gap), start_y, digit_width, digit_height, ones);
    
    /* Draw % symbol in scale 2 (same as MPH on I2C1) */
    int16_t percent_x = start_x + 3*digit_width + 2*digit_gap + 3;
    int16_t percent_y = start_y + 12;
    OLED_DrawText(percent_x, percent_y, "%", 2, OLED_BLACK);
    
    /* Draw % logo at bottom right */
    int16_t logo_x = 107;  /* Shifted further to the right */
    int16_t logo_y = 15;   /* Lower on the display */
    draw_percent_sign(logo_x, logo_y);

    int16_t logo_x2 = 107;
    int16_t logo_y2 = 40;
    draw_cat(logo_x2, logo_y2);
}

void Display2_Update(I2C_HandleTypeDef *hi2c, const Display2_Data *data)
{
    if (data == NULL) return;
    
    Display2_DrawPercentage(data->percentage);
    OLED_Update();
}
