/*
 * display_test.c
 *
 *  Created on: Nov 5, 2026
 *      Author: lveac
 *  
 *  Test file for display - cycles through values 0.0 to 99.9
 *  This file can be compiled separately or included in main()
 */

#include "display.h"
#include "oled_ssd1306.h"
#include "stm32l4xx_hal.h"

/*
 * Run display test - cycles through speeds 00.0 to 99.9
 * Each value displays for 1 second
 */
void Display_Test_Run(void)
{
    Display_Data test_data;
    
    /* Initialize display background once */
    Display_DrawBackground();
    Display_Refresh();
    
    /* Cycle through all values from 0.0 to 99.9 */
    for (uint16_t speed = 0; speed <= 999; speed++) {
        /* Format as XX.X where X is a digit */
        /* Skip values that don't match the pattern (only use 0, 11, 22, 33... 99.9) */
        
        uint8_t tens_digit = (speed / 10) % 10;
        uint8_t ones_digit = speed % 10;
        
        /* Only display values where tens and ones match (0.0, 1.1, 2.2, etc.) */
        if (tens_digit == ones_digit && tens_digit <= 9) {
            /* Update speed display */
            test_data.speed = speed;
            test_data.rpm = 50;        /* Fixed at 50% */
            test_data.fuel_level = 75; /* Fixed at 75% */
            
            Display_Update(&test_data);
            
            /* 1 second delay */
            HAL_Delay(1000);
        }
    }
    
    /* Display complete - show 00.0 one more time */
    test_data.speed = 0;
    test_data.rpm = 50;
    test_data.fuel_level = 75;
    Display_Update(&test_data);
}

/*
 * Alternative test function - simpler version
 * Just displays the sequence more directly
 */
void Display_Test_Run_Simple(void)
{
    Display_Data test_data;
    
    /* Initialize display background once */
    Display_DrawBackground();
    Display_Refresh();
    
    /* Cycle through 0.0, 1.1, 2.2, ... 9.9 */
    for (uint8_t digit = 0; digit <= 9; digit++) {
        uint16_t speed = (digit * 10) + digit;  /* Creates 0, 11, 22, 33, ..., 99 */
        
        test_data.speed = speed;
        test_data.rpm = 50;
        test_data.fuel_level = 75;
        
        Display_Update(&test_data);
        
        /* 1 second delay */
        HAL_Delay(1000);
    }
    
    /* Reset to 0.0 */
    test_data.speed = 0;
    test_data.rpm = 50;
    test_data.fuel_level = 75;
    Display_Update(&test_data);
}
