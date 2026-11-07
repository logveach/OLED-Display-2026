/*
 * display_test.h
 *
 *  Created on: Nov 5, 2026
 *      Author: lveac
 *  
 *  Test file header for display testing
 */

#ifndef INC_DISPLAY_TEST_H_
#define INC_DISPLAY_TEST_H_

/*
 * Run display test - cycles through values 0.0 to 9.9
 * Each value displays for 1 second with fixed RPM at 50% and fuel at 75%
 * Recommended for quick visual verification of the display module
 */
void Display_Test_Run(void);

/*
 * Simpler version - directly cycles through 0.0, 1.1, 2.2, ... 9.9
 * Each value displays for 1 second
 */
void Display_Test_Run_Simple(void);

#endif /* INC_DISPLAY_TEST_H_ */

