/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/


#include <project.h>
#include <stdio.h>
#include <u8x8.h>
#include <u8g2.h>
#include <stdlib.h>
// Define the number of columns you want for the game (here 16)
#define GAME_COLUMNS 16

// Define the game line (row) where you want the dot to appear
#define GAME_LINE 3
int score = 0;  // Initialize score
int obstacle_pos = -1;  // Initialize obstacle position
uint8_t psoc_gpio_and_delay_callback(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

int main(void) {
    u8x8_t EM2Display;
    int32 ADC_Result;
    float voltage;

    CyGlobalIntEnable; 

    /* Start the ADC and start conversion */
    ADC_Start();
    ADC_StartConvert();
    SegmentDisplay_Start();
    
    /* I2C Display setup */
    I2C_Start();
    u8x8_Setup(&EM2Display, u8x8_d_ssd1306_128x64_noname, u8x8_cad_ssd13xx_i2c, u8x8_byte_hw_i2c, psoc_gpio_and_delay_callback);
    u8x8_InitDisplay(&EM2Display);
    u8x8_SetPowerSave(&EM2Display, 0);
    u8x8_ClearDisplay(&EM2Display);
    u8x8_SetFont(&EM2Display, u8x8_font_amstrad_cpc_extended_f);

const int maxRows = 4;
const int maxLvl = 5;
int currentLvl = 1;
int playerScore = 0;
int obstaclePositions[4];
int obstacleSpeeds[4];
int i;

// Initialize all obstacle positions
for(i = 0; i < maxRows; ++i) obstaclePositions[i] = -1;

while(1) {
    if(ADC_IsEndConversion(ADC_WAIT_FOR_RESULT)) {
        /* Get the ADC result */
        ADC_Result = ADC_GetResult16();
        /* Convert the ADC result to voltage */
        voltage = ADC_CountsTo_Volts(ADC_Result);
        /* Map the voltage to a position within screen rows */
        int ballPosition = voltage / (3.3 / maxRows);

        // Clear screen
        u8x8_ClearDisplay(&EM2Display);

        // Draw current level
        char lvl[6];
        sprintf(lvl, "LVL %d", currentLvl);
        u8x8_DrawString(&EM2Display, 0, maxRows, lvl);

        for(i = 0; i < maxRows; ++i) {
            char row[17] = "                ";
            if(obstaclePositions[i] >= 0) {
                row[obstaclePositions[i]] = 'O';
                obstaclePositions[i] -= obstacleSpeeds[i];
                if(obstaclePositions[i] < 0 && i == ballPosition) playerScore++; // Player successfully dodged an obstacle
                if(obstaclePositions[i] < 0 && i == ballPosition) { // Player hit an obstacle
                    // return or break and handle game over
                }
            } else if(rand() % ((maxLvl + 1) - currentLvl) == 0) {
                obstaclePositions[i] = 15;
                row[obstaclePositions[i]] = 'O';
                obstacleSpeeds[i] = currentLvl;
            }
            if(i == ballPosition) row[0] = '*';

            // Write row to screen
            u8x8_DrawString(&EM2Display, 0, i, row);
        }

        // Update score on 7-segment display
        SegmentDisplay_Write7SegNumberDec(playerScore, 0, 4, SegmentDisplay_ZERO_PAD);

        // Level Up
        if(playerScore > 0 && playerScore % 20 == 0) currentLvl = playerScore / 20;
        if(currentLvl > maxLvl) currentLvl = maxLvl;

        CyDelay(100);
    }
}
}
/*
PSoC I2C Hardware Interface for U8X8
*/

uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    uint8_t *data;
    switch(msg) {
    case U8X8_MSG_BYTE_SEND:
        data = (uint8_t *)arg_ptr;
        while(arg_int > 0) {
            (void)I2C_MasterWriteByte(*data);
            data++;
            arg_int--;
        }
        break;
    case U8X8_MSG_BYTE_INIT:
        break;
    case U8X8_MSG_BYTE_SET_DC:
        break;
    case U8X8_MSG_BYTE_START_TRANSFER:
        (void)I2C_MasterSendStart(u8x8_GetI2CAddress(u8x8) >> 1, I2C_WRITE_XFER_MODE);
        break;
    case U8X8_MSG_BYTE_END_TRANSFER:
        (void)I2C_MasterSendStop();
        break;
    default:
        return 0;
    }
    return 1;
}

/*
PSoC GPIO and Delay Interface for U8X8
*/

uint8_t psoc_gpio_and_delay_callback(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    switch(msg){
    case U8X8_MSG_GPIO_AND_DELAY_INIT:  // called once during init phase of u8g2/u8x8
        break;
    case U8X8_MSG_DELAY_MILLI:      // delay arg_int * 1 milli second
        CyDelay(arg_int);
        break;
    // ..case U8X8_MSG_DELAY_10MICRO:  // delay arg_int * 10 micro seconds
      //  CyDelayUs(10 * arg_int);
      //  break;
    case U8X8_MSG_DELAY_NANO:       // delay arg_int * 1 nano second
       // your delay function
        break;    
    case U8X8_MSG_GPIO_I2C_CLOCK:
    case U8X8_MSG_GPIO_I2C_DATA:
    default:
        u8x8_SetGPIOResult(u8x8, 1);
        break;
    }
    return 1;
}

/* [] END OF FILE */
