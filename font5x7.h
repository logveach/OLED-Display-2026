/*
 * oled_ssd1306.h
 *
 *  Created on: 10/31/2026
 *      Author: lveac
 */
#ifndef FONT5X7_H
#define FONT5X7_H

#include <stdint.h>

typedef struct {
    char    ch;
    uint8_t width;
    uint8_t cols[5];
} Font5x7_Glyph;

const Font5x7_Glyph *Font5x7_Find(char ch);

#endif /* FONT5X7_H */
