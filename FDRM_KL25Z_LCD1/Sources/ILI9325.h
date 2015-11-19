/*
 * ILI9325.h
 *
 *  Created on: 11 Nov, 2015
 *      Author: kzw
 */

#ifndef SOURCES_ILI9325_H_
#define SOURCES_ILI9325_H_


   // Control signals are ACTIVE LOW (idle is HIGH)
   // Command/Data: LOW = command, HIGH = data
   // These are single-instruction operations and always inline
   #define RD_ACTIVE  GPIO_DRV_ClearPinOutput(LCDRD) //RD_PORT->PIO_CODR |= RD_MASK
   #define RD_IDLE    GPIO_DRV_SetPinOutput(LCDRD)   //RD_PORT->PIO_SODR |= RD_MASK
   #define WR_ACTIVE  GPIO_DRV_ClearPinOutput(LCDWR) //WR_PORT->PIO_CODR |= WR_MASK
   #define WR_IDLE    GPIO_DRV_SetPinOutput(LCDWR)   //WR_PORT->PIO_SODR |= WR_MASK
   #define CD_COMMAND GPIO_DRV_ClearPinOutput(LCDRS) //CD_PORT->PIO_CODR |= CD_MASK
   #define CD_DATA    GPIO_DRV_SetPinOutput(LCDRS)   //CD_PORT->PIO_SODR |= CD_MASK
   #define CS_ACTIVE  GPIO_DRV_ClearPinOutput(LCDCS) //CS_PORT->PIO_CODR |= CS_MASK
   #define CS_IDLE    GPIO_DRV_SetPinOutput(LCDCS)   //CS_PORT->PIO_SODR |= CS_MASK

// Data write strobe, ~2 instructions and always inline
  #define WR_STROBE { WR_ACTIVE; WR_IDLE; }

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

//uint16_t ILI9325ReadID(void);
void ILI9325Reset(void);
//uint16_t ILI9325ReadID(void);
//void ILI9325Begin(uint16_t id);
//void ILI9325FillScreen(uint16_t color);
//int16_t getWidth();
//int16_t getHeight();

#endif /* SOURCES_ILI9325_H_ */
