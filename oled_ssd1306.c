/*
 * oled_ssd1306.c
 *
 *  Created on: Oct 31, 2026
 *      Author: lveac
 */
#include "oled_ssd1306.h"
#include "font5x7.h"
#include <string.h>

#define OLED_PAGES       (OLED_HEIGHT / 8)
#define OLED_TIMEOUT_MS  200
#define CTRL_CMD         0x00
#define CTRL_DATA        0x40
#define GLYPH_GAP        1

static I2C_HandleTypeDef *s_hi2c;
static uint16_t           s_addr;
static uint8_t            s_fb[OLED_PAGES * OLED_WIDTH];

static HAL_StatusTypeDef send_cmd(const uint8_t *buf, uint16_t len)
{
    return HAL_I2C_Mem_Write(s_hi2c, s_addr, CTRL_CMD,
                             I2C_MEMADD_SIZE_8BIT, (uint8_t *)buf, len,
                             OLED_TIMEOUT_MS);
}

HAL_StatusTypeDef OLED_Init(I2C_HandleTypeDef *hi2c)
{
    static const uint8_t seq[] = {
        0xAE,             /* display off          */
        0xD5, 0x80,       /* clock                */
        0xA8, 0x3F,       /* mux ratio 64         */
        0xD3, 0x00,       /* display offset 0     */
        0x40,             /* start line 0         */
        0x8D, 0x14,       /* charge pump on       */
        0x20, 0x00,       /* horizontal addr mode */
        0xA1,             /* seg remap            */
        0xC8,             /* COM direction        */
        0xDA, 0x12,       /* COM pins             */
        0x81, 0xCF,       /* contrast             */
        0xD9, 0xF1,       /* pre-charge           */
        0xDB, 0x40,       /* VCOMH                */
        0xA4,             /* output follows RAM   */
        0xA6,             /* normal (not inverted)*/
        0x2E,             /* stop scroll          */
        0xAF              /* display on           */
    };

    s_hi2c = hi2c;

    /* auto-detect I2C address */
    if (HAL_I2C_IsDeviceReady(s_hi2c, OLED_I2C_ADDR, 3, 50) == HAL_OK) {
        s_addr = OLED_I2C_ADDR;
    } else if (HAL_I2C_IsDeviceReady(s_hi2c, OLED_I2C_ADDR_ALT, 3, 50) == HAL_OK) {
        s_addr = OLED_I2C_ADDR_ALT;
    } else {
        return HAL_ERROR;   /* nothing on the bus — check wiring */
    }

    OLED_Clear(OLED_BLACK);
    return send_cmd(seq, sizeof(seq));
}

void OLED_Clear(OLED_Color color)
{
    memset(s_fb, (color == OLED_WHITE) ? 0xFF : 0x00, sizeof(s_fb));
}

void OLED_DrawPixel(int16_t x, int16_t y, OLED_Color color)
{
    if (x < 0 || x >= OLED_WIDTH || y < 0 || y >= OLED_HEIGHT) return;
    uint16_t idx  = (uint16_t)(y >> 3) * OLED_WIDTH + (uint16_t)x;
    uint8_t  mask = (uint8_t)(1u << (y & 7));
    if (color == OLED_WHITE) s_fb[idx] |=  mask;
    else                     s_fb[idx] &= (uint8_t)~mask;
}

void OLED_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, OLED_Color color)
{
    for (int16_t j = 0; j < h; j++)
        for (int16_t i = 0; i < w; i++)
            OLED_DrawPixel(x + i, y + j, color);
}

void OLED_DrawRect(int16_t x, int16_t y, int16_t w, int16_t h, OLED_Color color)
{
    if (w <= 0 || h <= 0) return;
    OLED_FillRect(x,         y,         w, 1, color);
    OLED_FillRect(x,         y + h - 1, w, 1, color);
    OLED_FillRect(x,         y,         1, h, color);
    OLED_FillRect(x + w - 1, y,         1, h, color);
}

static int16_t draw_glyph(int16_t x, int16_t y, char ch, uint8_t scale, OLED_Color color)
{
    const Font5x7_Glyph *g = Font5x7_Find(ch);
    if (!g) g = Font5x7_Find(' ');
    for (uint8_t col = 0; col < g->width; col++) {
        uint8_t bits = g->cols[col];
        for (uint8_t row = 0; row < 7; row++)
            if (bits & (1u << row))
                OLED_FillRect(x + col * scale, y + row * scale, scale, scale, color);
    }
    return (int16_t)(g->width * scale + GLYPH_GAP);
}

int16_t OLED_TextWidth(const char *str, uint8_t scale)
{
    int16_t  w = 0;
    uint8_t  n = 0;
    for (const char *p = str; *p; p++) {
        const Font5x7_Glyph *g = Font5x7_Find(*p);
        w += (int16_t)((g ? g->width : Font5x7_Find(' ')->width) * scale);
        n++;
    }
    if (n > 1) w += (int16_t)(n - 1) * GLYPH_GAP;
    return w;
}

void OLED_DrawText(int16_t x, int16_t y, const char *str, uint8_t scale, OLED_Color color)
{
    for (const char *p = str; *p; p++)
        x += draw_glyph(x, y, *p, scale, color);
}

HAL_StatusTypeDef OLED_Update(void)
{
    static const uint8_t addr[] = { 0x21, 0x00, 0x7F, 0x22, 0x00, 0x07 };
    if (send_cmd(addr, sizeof(addr)) != HAL_OK) return HAL_ERROR;
    return HAL_I2C_Mem_Write(s_hi2c, s_addr, CTRL_DATA,
                             I2C_MEMADD_SIZE_8BIT, s_fb, sizeof(s_fb),
                             OLED_TIMEOUT_MS);
}


