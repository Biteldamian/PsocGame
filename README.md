---
# Dot Dodge Game with Cypress Psoc Creator 4.4

This is a simple game that uses a b10k potentiometer as a control input to move the player (a dot character) around on an SSD1306 Oled display. The result of the game is shown on a LED 7-Segment display.

## Hardware Required
- CY8CKIT-049-42xx PSoC 4 Prototyping Kit
- b10k Potentiometer
- SBc-Oled01 Display JoyIt (128x64 pixel SSD1306 monochrome OLED display)
- LED 7-Segment Display 3461AS

## Software Required
- Cypress PSoC Creator 4.4
- u8g2 library for monochrome displays

## Getting Started
1. Download the PSoC Creator Software from the [Cypress website](https://www.cypress.com/products/psoc-creator-integrated-design-environment-ide)

2. Install the u8g2 library, following instructions from [here](https://github.com/olikraus/u8g2)

3. Open this project in PSoC Creator

## How it Works
The b10k potentiometer controls the position of the dot on screen to avoid incoming obstacles that move from right to left of screen. The objective of the game is to avoid as many obstacles as possible. Your score increases for each successfully dodged obstacle, and the corresponding score is shown on the 7-segment display.

## ADC Component Settings
Name: ADC \
Converter Type: SAR \
Resolution: 12 \
Sample Rate: 100 k \
Reference: Vdda (Vdda/2) \
Input Mode: Single Ended \
Samples Averaged: 1 \
Acquisition Time: 17 \
Clock Source: External \
Clock Frequency: 1 MHz \
ANALOG PIN NAME: POT

## I2C Component Settings
Name: I2C \
Mode: I2C, Master \
Data rate: 100 kbps \

## SegmentDisplay Component Settings
Name: SegmentDisplay \
Number of segments: 8 \
Segem drive: Active High \
Number of comms :1 \
Com drive active: Low \
Timing: Internal clock \
Display Refresh rate Hz: 250

## Troubleshooting
Make sure all the hardware connections are correctly set up. The SDA and SCL lines for I2C communication should be checked for loose connections.

## Authors
\[Bitel Damian\]
 
## License
This project is licensed under the terms of the MIT license.
