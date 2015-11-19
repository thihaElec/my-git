/*
 * ILI9325.c
 *
 *  Created on: 11 Nov, 2015
 *      Author: kzw
 */

#include "ILI9325.h"
#include "LED.h"
#include "LCD.h"
#include "WAIT1.h"
#include "glcdfont.c"

#define TFTWIDTH   240
#define TFTHEIGHT  320

// LCD controller chip identifiers
#define ID_932X    0
#define ID_7575    1
#define ID_9341    2
#define ID_HX8357D    3
#define ID_UNKNOWN 0xFF

#define ILI932X_START_OSC          0x00
#define ILI932X_DRIV_OUT_CTRL      0x01
#define ILI932X_DRIV_WAV_CTRL      0x02
#define ILI932X_ENTRY_MOD          0x03
#define ILI932X_RESIZE_CTRL        0x04
#define ILI932X_DISP_CTRL1         0x07
#define ILI932X_DISP_CTRL2         0x08
#define ILI932X_DISP_CTRL3         0x09
#define ILI932X_DISP_CTRL4         0x0A
#define ILI932X_RGB_DISP_IF_CTRL1  0x0C
#define ILI932X_FRM_MARKER_POS     0x0D
#define ILI932X_RGB_DISP_IF_CTRL2  0x0F
#define ILI932X_POW_CTRL1          0x10
#define ILI932X_POW_CTRL2          0x11
#define ILI932X_POW_CTRL3          0x12
#define ILI932X_POW_CTRL4          0x13
#define ILI932X_GRAM_HOR_AD        0x20
#define ILI932X_GRAM_VER_AD        0x21
#define ILI932X_RW_GRAM            0x22
#define ILI932X_POW_CTRL7          0x29
#define ILI932X_FRM_RATE_COL_CTRL  0x2B
#define ILI932X_GAMMA_CTRL1        0x30
#define ILI932X_GAMMA_CTRL2        0x31
#define ILI932X_GAMMA_CTRL3        0x32
#define ILI932X_GAMMA_CTRL4        0x35
#define ILI932X_GAMMA_CTRL5        0x36
#define ILI932X_GAMMA_CTRL6        0x37
#define ILI932X_GAMMA_CTRL7        0x38
#define ILI932X_GAMMA_CTRL8        0x39
#define ILI932X_GAMMA_CTRL9        0x3C
#define ILI932X_GAMMA_CTRL10       0x3D
#define ILI932X_HOR_START_AD       0x50
#define ILI932X_HOR_END_AD         0x51
#define ILI932X_VER_START_AD       0x52
#define ILI932X_VER_END_AD         0x53
#define ILI932X_GATE_SCAN_CTRL1    0x60
#define ILI932X_GATE_SCAN_CTRL2    0x61
#define ILI932X_GATE_SCAN_CTRL3    0x6A
#define ILI932X_PART_IMG1_DISP_POS 0x80
#define ILI932X_PART_IMG1_START_AD 0x81
#define ILI932X_PART_IMG1_END_AD   0x82
#define ILI932X_PART_IMG2_DISP_POS 0x83
#define ILI932X_PART_IMG2_START_AD 0x84
#define ILI932X_PART_IMG2_END_AD   0x85
#define ILI932X_PANEL_IF_CTRL1     0x90
#define ILI932X_PANEL_IF_CTRL2     0x92
#define ILI932X_PANEL_IF_CTRL3     0x93
#define ILI932X_PANEL_IF_CTRL4     0x95
#define ILI932X_PANEL_IF_CTRL5     0x97
#define ILI932X_PANEL_IF_CTRL6     0x98

uint32_t ILI9325readReg(uint8_t r);
void write8(uint8_t d);
uint8_t read8( void );
uint16_t ILI9325ReadID(void);
void setReadDir(void);
void setWriteDir(void);
void ILI9325Begin(uint16_t id);
void writeRegister24(uint8_t r, uint32_t d);
void setRotation(uint8_t x);
void writeRegister16(uint16_t a, uint16_t d);
void setAddrWindow(int x1, int y1, int x2, int y2);
void ILI9325FillScreen(uint16_t color);
void flood(uint16_t color, uint32_t len);
void setCursor(int16_t x, int16_t y);
void drawPixel(int16_t x, int16_t y, uint16_t color);
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
void write(uint8_t c);

int16_t getWidth();
int16_t getHeight();



uint8_t driver=ID_932X;
uint8_t rotation = 0;
int16_t cursor_x = 0;
int16_t cursor_y = 0;
int16_t _width = 0;
int16_t _height = 0;
uint8_t textsize = 5;
uint16_t textcolor = 0xFFFF;
uint16_t textbgcolor = 0xFFFF;
uint8_t wrap = 1;



// Ditto with the read/write port directions, as above.
void ILI9325Begin(uint16_t id) {

//	uint16_t a, d;
//	uint8_t i = 0;


	ILI9325Reset();
	WAIT1_Waitms(200);

	if (id==0x9325) //for future chip
	{
	    driver = ID_932X;
	    CS_ACTIVE;
	    /*
	    while(i < sizeof(ILI932x_regValues) / sizeof(uint16_t)) {
	      a = pgm_read_word(&ILI932x_regValues[i++]);
	      d = pgm_read_word(&ILI932x_regValues[i++]);
	      if(a == TFTLCD_DELAY)
	    	  {
	    	  	  delay(d);
	    	  }
	      else
	    	  {
	    	  writeRegister16(a, d);
	    	  }
	    }
	    */
//	    ILI932X_START_OSC        , 0x0001, // Start oscillator
	    writeRegister16(ILI932X_START_OSC, 0x0001);
//	    TFTLCD_DELAY             , 50,     // 50 millisecond delay
		WAIT1_Waitms(50);
//	    ILI932X_DRIV_OUT_CTRL    , 0x0100,
	    writeRegister16(ILI932X_DRIV_OUT_CTRL, 0x0100);
//	    ILI932X_DRIV_WAV_CTRL    , 0x0700,
		writeRegister16(ILI932X_DRIV_WAV_CTRL, 0x0700);
//	    ILI932X_ENTRY_MOD        , 0x1030,
		writeRegister16(ILI932X_ENTRY_MOD, 0x1030);
//	    ILI932X_RESIZE_CTRL      , 0x0000,
		writeRegister16(ILI932X_RESIZE_CTRL, 0x0000);
//	    ILI932X_DISP_CTRL2       , 0x0202,
		writeRegister16(ILI932X_DISP_CTRL2, 0x0202);
//	    ILI932X_DISP_CTRL3       , 0x0000,
		writeRegister16(ILI932X_DISP_CTRL3, 0x0000);
//	    ILI932X_DISP_CTRL4       , 0x0000,
		writeRegister16(ILI932X_DISP_CTRL4, 0x0000);
//	    ILI932X_RGB_DISP_IF_CTRL1, 0x0,
		writeRegister16(ILI932X_RGB_DISP_IF_CTRL1, 0x0);
//	    ILI932X_FRM_MARKER_POS   , 0x0,
		writeRegister16(ILI932X_FRM_MARKER_POS, 0x0);
//	    ILI932X_RGB_DISP_IF_CTRL2, 0x0,
		writeRegister16(ILI932X_RGB_DISP_IF_CTRL2, 0x0);
//	    ILI932X_POW_CTRL1        , 0x0000,
		writeRegister16(ILI932X_POW_CTRL1, 0x0000);
//	    ILI932X_POW_CTRL2        , 0x0007,
		writeRegister16(ILI932X_POW_CTRL2, 0x0007);
//	    ILI932X_POW_CTRL3        , 0x0000,
		writeRegister16(ILI932X_POW_CTRL3, 0x0000);
//	    ILI932X_POW_CTRL4        , 0x0000,
		writeRegister16(ILI932X_POW_CTRL4, 0x0000);
//	    TFTLCD_DELAY             , 200,
		WAIT1_Waitms(200);
//	    ILI932X_POW_CTRL1        , 0x1690,
		writeRegister16(ILI932X_POW_CTRL1, 0x1690);
//	    ILI932X_POW_CTRL2        , 0x0227,
		writeRegister16(ILI932X_POW_CTRL2, 0x0227);
//	    TFTLCD_DELAY             , 50,
		WAIT1_Waitms(50);
//	    ILI932X_POW_CTRL3        , 0x001A,
		writeRegister16(ILI932X_POW_CTRL3, 0x001A);
//	    TFTLCD_DELAY             , 50,
		WAIT1_Waitms(50);
//	    ILI932X_POW_CTRL4        , 0x1800,
		writeRegister16(ILI932X_POW_CTRL4, 0x1800);
//	    ILI932X_POW_CTRL7        , 0x002A,
		writeRegister16(ILI932X_POW_CTRL7, 0x002A);
//	    TFTLCD_DELAY             , 50,
		WAIT1_Waitms(50);
//	    ILI932X_GAMMA_CTRL1      , 0x0000,
		writeRegister16(ILI932X_GAMMA_CTRL1, 0x0000);
//	    ILI932X_GAMMA_CTRL2      , 0x0000,
		writeRegister16(ILI932X_GAMMA_CTRL2, 0x0000);
//	    ILI932X_GAMMA_CTRL3      , 0x0000,
		writeRegister16(ILI932X_GAMMA_CTRL3, 0x0000);
//	    ILI932X_GAMMA_CTRL4      , 0x0206,
		writeRegister16(ILI932X_GAMMA_CTRL4, 0x0206);
//	    ILI932X_GAMMA_CTRL5      , 0x0808,
		writeRegister16(ILI932X_GAMMA_CTRL5, 0x0808);
//	    ILI932X_GAMMA_CTRL6      , 0x0007,
		writeRegister16(ILI932X_GAMMA_CTRL6, 0x0007);
//	    ILI932X_GAMMA_CTRL7      , 0x0201,
		writeRegister16(ILI932X_GAMMA_CTRL7, 0x0201);
//	    ILI932X_GAMMA_CTRL8      , 0x0000,
		writeRegister16(ILI932X_GAMMA_CTRL8, 0x0000);
//	    ILI932X_GAMMA_CTRL9      , 0x0000,
		writeRegister16(ILI932X_GAMMA_CTRL9, 0x0000);
//	    ILI932X_GAMMA_CTRL10     , 0x0000,
		writeRegister16(ILI932X_GAMMA_CTRL10, 0x0000);
//	    ILI932X_GRAM_HOR_AD      , 0x0000,
		writeRegister16(ILI932X_GRAM_HOR_AD, 0x0000);
//	    ILI932X_GRAM_VER_AD      , 0x0000,
		writeRegister16(ILI932X_GRAM_VER_AD, 0x0000);
//	    ILI932X_HOR_START_AD     , 0x0000,
		writeRegister16(ILI932X_HOR_START_AD, 0x0000);
//	    ILI932X_HOR_END_AD       , 0x00EF,
		writeRegister16(ILI932X_HOR_END_AD, 0x00EF);
//	    ILI932X_VER_START_AD     , 0X0000,
		writeRegister16(ILI932X_VER_START_AD, 0x0000);
//	    ILI932X_VER_END_AD       , 0x013F,
		writeRegister16(ILI932X_VER_END_AD, 0x013F);
//	    ILI932X_GATE_SCAN_CTRL1  , 0xA700, // Driver Output Control (R60h)
		writeRegister16(ILI932X_GATE_SCAN_CTRL1, 0xA700);
//	    ILI932X_GATE_SCAN_CTRL2  , 0x0003, // Driver Output Control (R61h)
		writeRegister16(ILI932X_GATE_SCAN_CTRL2, 0x0003);
//	    ILI932X_GATE_SCAN_CTRL3  , 0x0000, // Driver Output Control (R62h)
		writeRegister16(ILI932X_GATE_SCAN_CTRL3, 0x0000);
//	    ILI932X_PANEL_IF_CTRL1   , 0X0010, // Panel Interface Control 1 (R90h)
		writeRegister16(ILI932X_PANEL_IF_CTRL1, 0X0010);
//	    ILI932X_PANEL_IF_CTRL2   , 0X0000,
		writeRegister16(ILI932X_PANEL_IF_CTRL2, 0x0000);
//	    ILI932X_PANEL_IF_CTRL3   , 0X0003,
		writeRegister16(ILI932X_PANEL_IF_CTRL3, 0X0003);
//	    ILI932X_PANEL_IF_CTRL4   , 0X1100,
		writeRegister16(ILI932X_PANEL_IF_CTRL4, 0X1100);
//	    ILI932X_PANEL_IF_CTRL5   , 0X0000,
		writeRegister16(ILI932X_PANEL_IF_CTRL5, 0x0000);
//	    ILI932X_PANEL_IF_CTRL6   , 0X0000,
		writeRegister16(ILI932X_PANEL_IF_CTRL6, 0x0000);
//	    ILI932X_DISP_CTRL1       , 0x0133, // Main screen turn on
		writeRegister16(ILI932X_DISP_CTRL1, 0x0133);

		setRotation(rotation); //hard code first
	    setAddrWindow(0, 0, TFTWIDTH-1, TFTHEIGHT-1);
	}
}
void setCursor(int16_t x, int16_t y) {
  cursor_x = x;
  cursor_y = y;
}
int16_t getCursorX(void) {
  return cursor_x;
}
int16_t getCursorY(void) {
  return cursor_y;
}
void setTextSize(uint8_t s) {
  textsize = (s > 0) ? s : 1;
}
int16_t getWidth(){
	return _width;
}
int16_t getHeight(){
	return _height;
}
void write(uint8_t c) {
  if (c == '\n') {
    cursor_y += textsize*8;
    cursor_x  = 0;
  } else if (c == '\r') {
    // skip em
  } else {
    drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
    cursor_x += textsize*6;
    if (wrap && (cursor_x > (_width - textsize*6))) {
      cursor_y += textsize*8;
      cursor_x = 0;
    }
  }
}

// Draw a character
void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size) {

  if((x >= _width)            || // Clip right
     (y >= _height)           || // Clip bottom
     ((x + 6 * size - 1) < 0) || // Clip left
     ((y + 8 * size - 1) < 0))   // Clip top
    return;

//  if(!_cp437 && (c >= 176)) c++; // Handle 'classic' charset behavior

  for (int8_t i=0; i<6; i++ ) {
    uint8_t line;
    if (i == 5)
      line = 0x0;
    else
      line = font[(c*5)+i];
    for (int8_t j = 0; j<8; j++) {
      if (line & 0x1) {
        if (size == 1) // default size
          drawPixel(x+i, y+j, color);
        else {  // big size
          fillRect(x+(i*size), y+(j*size), size, size, color);
        }
      } else if (bg != color) {
        if (size == 1) // default size
          drawPixel(x+i, y+j, bg);
        else {  // big size
          fillRect(x+i*size, y+j*size, size, size, bg);
        }
      }
      line >>= 1;
    }
  }
}


void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  drawPixel(x0  , y0+r, color);
  drawPixel(x0  , y0-r, color);
  drawPixel(x0+r, y0  , color);
  drawPixel(x0-r, y0  , color);

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 + x, y0 - y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 + y, y0 + x, color);
    drawPixel(x0 - y, y0 + x, color);
    drawPixel(x0 + y, y0 - x, color);
    drawPixel(x0 - y, y0 - x, color);
  }
}

void drawPixel(int16_t x, int16_t y, uint16_t color) {

	  // Clip
	  if((x < 0) || (y < 0) || (x >= _width) || (y >= _height)) return;

	  CS_ACTIVE;
	  if(driver == ID_932X) {
	    int16_t t;
	    switch(rotation) {
	     case 1:
	      t = x;
	      x = TFTWIDTH  - 1 - y;
	      y = t;
	      break;
	     case 2:
	      x = TFTWIDTH  - 1 - x;
	      y = TFTHEIGHT - 1 - y;
	      break;
	     case 3:
	      t = x;
	      x = y;
	      y = TFTHEIGHT - 1 - t;
	      break;
	    }
	    writeRegister16(0x0020, x);
	    writeRegister16(0x0021, y);
	    writeRegister16(0x0022, color);

	  }

	  CS_IDLE;
}
// Bresenham's algorithm - thx wikpedia
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  int16_t tmp;
  if (steep) {
//    swap(x0, y0);
	  tmp = x0;
	  x0 = y0;
	  y0 = tmp;
//    swap(x1, y1);
	  tmp = x1;
	  x1 = y1;
	  y1 = tmp;
  }

  if (x0 > x1) {
//    swap(x0, x1);
	  tmp = x0;
	  x0 = x1;
	  x1 = tmp;

//    swap(y0, y1);
	  tmp = y0;
	  y0 = y1;
	  y1 = tmp;

  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0<=x1; x0++) {
    if (steep) {
      drawPixel(y0, x0, color);
    } else {
      drawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

// Draw a rectangle
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  drawFastHLine(x, y, w, color);
  drawFastHLine(x, y+h-1, w, color);
  drawFastVLine(x, y, h, color);
  drawFastVLine(x+w-1, y, h, color);
}

void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
  // Update in subclasses if desired!
  drawLine(x, y, x, y+h-1, color);
}

void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
  // Update in subclasses if desired!
  drawLine(x, y, x+w-1, y, color);
}

void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  // Update in subclasses if desired!
  for (int16_t i=x; i<x+w; i++) {
    drawFastVLine(i, y, h, color);
  }
}

void setTextColor(uint16_t c) {
  // For 'transparent' background, we'll set the bg
  // to the same as fg instead of using a flag
  textcolor = c;
  textbgcolor = c;
}

void setRotation(uint8_t x) {

	  rotation = (x & 3);
	  switch(rotation) {
	   case 0:
	   case 2:
	    _width  = TFTWIDTH;
	    _height = TFTHEIGHT;
	    break;
	   case 1:
	   case 3:
	    _width  = TFTHEIGHT;
	    _height = TFTWIDTH;
	    break;
	  }

	  CS_ACTIVE;
	  if(driver == ID_932X) {

	    uint16_t t;
	    switch(rotation) {
	     default: t = 0x1030; break;
	     case 1 : t = 0x1028; break;
	     case 2 : t = 0x1000; break;
	     case 3 : t = 0x1018; break;
	    }
	    writeRegister16(0x0003, t ); // MADCTL
	    // For 932X, init default full-screen address window:
	    setAddrWindow(0, 0, TFTWIDTH - 1, TFTHEIGHT - 1); // CS_IDLE happens here

	  }
}

void ILI9325FillScreen(uint16_t color) {

  if(driver == ID_932X) {

    // For the 932X, a full-screen address window is already the default
    // state, just need to set the address pointer to the top-left corner.
    // Although we could fill in any direction, the code uses the current
    // screen rotation because some users find it disconcerting when a
    // fill does not occur top-to-bottom.
    uint16_t x, y;
    switch(rotation) {
      default: x = 0            ; y = 0            ; break;
      case 1 : x = TFTWIDTH  - 1; y = 0            ; break;
      case 2 : x = TFTWIDTH  - 1; y = TFTHEIGHT - 1; break;
      case 3 : x = 0            ; y = TFTHEIGHT - 1; break;
    }
    CS_ACTIVE;
    writeRegister16(0x0020, x);
    writeRegister16(0x0021, y);
  }
  flood(color, (long)TFTWIDTH * (long)TFTHEIGHT);
}
void flood(uint16_t color, uint32_t len) {
  uint16_t blocks;
  uint8_t  i, hi = color >> 8,
              lo = color;

  CS_ACTIVE;
  CD_COMMAND;
  if (driver == ID_9341) {
    write8(0x2C);
  } else if (driver == ID_932X) {
    write8(0x00); // High byte of GRAM register...
    write8(0x22); // Write data to GRAM
  } else if (driver == ID_HX8357D) {
//    write8(HX8357_RAMWR);
  } else {
    write8(0x22); // Write data to GRAM
  }

  // Write first pixel normally, decrement counter by 1
  CD_DATA;
  write8(hi);
  write8(lo);
  len--;

  blocks = (uint16_t)(len / 64); // 64 pixels/block
  if(hi == lo) {
    // High and low bytes are identical.  Leave prior data
    // on the port(s) and just toggle the write strobe.
    while(blocks--) {
      i = 16; // 64 pixels/block / 4 pixels/pass
      do {
        WR_STROBE; WR_STROBE; WR_STROBE; WR_STROBE; // 2 bytes/pixel
        WR_STROBE; WR_STROBE; WR_STROBE; WR_STROBE; // x 4 pixels
      } while(--i);
    }
    // Fill any remaining pixels (1 to 64)
    for(i = (uint8_t)len & 63; i--; ) {
      WR_STROBE;
      WR_STROBE;
    }
  } else {
    while(blocks--) {
      i = 16; // 64 pixels/block / 4 pixels/pass
      do {
        write8(hi); write8(lo); write8(hi); write8(lo);
        write8(hi); write8(lo); write8(hi); write8(lo);
      } while(--i);
    }
    for(i = (uint8_t)len & 63; i--; ) {
      write8(hi);
      write8(lo);
    }
  }
  CS_IDLE;
}

void setAddrWindow(int x1, int y1, int x2, int y2) {
  CS_ACTIVE;
  if(driver == ID_932X) {

    // Values passed are in current (possibly rotated) coordinate
    // system.  932X requires hardware-native coords regardless of
    // MADCTL, so rotate inputs as needed.  The address counter is
    // set to the top-left corner -- although fill operations can be
    // done in any direction, the current screen rotation is applied
    // because some users find it disconcerting when a fill does not
    // occur top-to-bottom.
    int x, y, t;
    switch(rotation) {
     default:
      x  = x1;
      y  = y1;
      break;
     case 1:
      t  = y1;
      y1 = x1;
      x1 = TFTWIDTH  - 1 - y2;
      y2 = x2;
      x2 = TFTWIDTH  - 1 - t;
      x  = x2;
      y  = y1;
      break;
     case 2:
      t  = x1;
      x1 = TFTWIDTH  - 1 - x2;
      x2 = TFTWIDTH  - 1 - t;
      t  = y1;
      y1 = TFTHEIGHT - 1 - y2;
      y2 = TFTHEIGHT - 1 - t;
      x  = x2;
      y  = y2;
      break;
     case 3:
      t  = x1;
      x1 = y1;
      y1 = TFTHEIGHT - 1 - x2;
      x2 = y2;
      y2 = TFTHEIGHT - 1 - t;
      x  = x1;
      y  = y2;
      break;
    }
    writeRegister16(0x0050, x1); // Set address window
    writeRegister16(0x0051, x2);
    writeRegister16(0x0052, y1);
    writeRegister16(0x0053, y2);
    writeRegister16(0x0020, x ); // Set address counter to top left
    writeRegister16(0x0021, y );

  }
}

uint16_t ILI9325ReadID(void){

  uint8_t hi, lo;
  uint16_t id;

  /*
  for (uint8_t i=0; i<128; i++) {
    Serial.print("$"); Serial.print(i, HEX);
    Serial.print(" = 0x"); Serial.println(readReg(i), HEX);
  }
  */
/*
  if (readReg(0x04) == 0x8000) { // eh close enough
    // setc!

      Serial.println("!");
      for (uint8_t i=0; i<254; i++) {
      Serial.print("$"); Serial.print(i, HEX);
      Serial.print(" = 0x"); Serial.println(readReg(i), HEX);
      }

    writeRegister24(HX8357D_SETC, 0xFF8357);
    WAIT1_Waitms(300);
    //Serial.println(readReg(0xD0), HEX);
    if (readReg(0xD0) == 0x990000) {
      return 0x8357;
    }
  }*/
  id = ILI9325readReg(0x04);
  if (id == 0x8000) {
    return id;
  }
  id = ILI9325readReg(0xD3);
  if (id == 0x9341) {
    return id;
  }

  CS_ACTIVE;
  CD_COMMAND;
  write8(0x00);
  WR_STROBE;     // Repeat prior byte (0x00)
  setReadDir();  // Set up LCD data port(s) for READ operations
  CD_DATA;
  hi = read8();
  lo = read8();
  setWriteDir();  // Restore LCD data port(s) to WRITE configuration
  CS_IDLE;

  id = ((hi<<8) & 0xFF00) | (lo & 0xFF);
  return id;
}

void ILI9325Reset(void) {
  CS_IDLE;
//  CD_DATA;
  WR_IDLE;
  RD_IDLE;

//  digitalWrite(_reset, LOW);
  GPIO_DRV_ClearPinOutput(LCDReset);

//  delay(2);
  WAIT1_Waitms(2);
//  digitalWrite(_reset, HIGH);
  GPIO_DRV_SetPinOutput(LCDReset);

  // Data transfer sync
  CS_ACTIVE;
  CD_COMMAND;
  write8(0x00);
  for(uint8_t i=0; i<3; i++) WR_STROBE; // Three extra 0x00s
  CS_IDLE;
}

uint32_t ILI9325readReg(uint8_t r)
{
  uint32_t id;
  uint8_t x;

  // try reading register #4
  CS_ACTIVE;
  CD_COMMAND;
  write8(r);
  setReadDir();  // Set up LCD data port(s) for READ operations
  CD_DATA;
//  delayMicroseconds(50);
  WAIT1_Waitus(50);
  x = read8();
  id = x;          // Do not merge or otherwise simplify
  id <<= 8;              // these lines.  It's an unfortunate
  x = read8();
  id  |= x;        // shenanigans that are going on.
  id <<= 8;              // these lines.  It's an unfortunate
  x = read8();
  id  |= x;        // shenanigans that are going on.
  id <<= 8;              // these lines.  It's an unfortunate
  x = read8();
  id  |= x;        // shenanigans that are going on.
  CS_IDLE;
  setWriteDir();  // Restore LCD data port(s) to WRITE configuration

  //Serial.print("Read $"); Serial.print(r, HEX);
  //Serial.print(":\t0x"); Serial.println(id, HEX);
  return id;
}
void setReadDir(void){

    GPIO_DRV_SetPinDir(LCDB0, 0);
    GPIO_DRV_SetPinDir(LCDB1, 0);
    GPIO_DRV_SetPinDir(LCDB2, 0);
    GPIO_DRV_SetPinDir(LCDB3, 0);
    GPIO_DRV_SetPinDir(LCDB4, 0);
    GPIO_DRV_SetPinDir(LCDB5, 0);
    GPIO_DRV_SetPinDir(LCDB6, 0);
    GPIO_DRV_SetPinDir(LCDB7, 0);
}
void setWriteDir(void){

    GPIO_DRV_SetPinDir(LCDB0, 1);
    GPIO_DRV_SetPinDir(LCDB1, 1);
    GPIO_DRV_SetPinDir(LCDB2, 1);
    GPIO_DRV_SetPinDir(LCDB3, 1);
    GPIO_DRV_SetPinDir(LCDB4, 1);
    GPIO_DRV_SetPinDir(LCDB5, 1);
    GPIO_DRV_SetPinDir(LCDB6, 1);
    GPIO_DRV_SetPinDir(LCDB7, 1);
}
void write8(uint8_t d)
{
    GPIO_DRV_WritePinOutput(LCDB0, (d & 0x01));
    GPIO_DRV_WritePinOutput(LCDB1, ((d>>1) & 0x01));
    GPIO_DRV_WritePinOutput(LCDB2, ((d>>2) & 0x01));
    GPIO_DRV_WritePinOutput(LCDB3, ((d>>3) & 0x01));
    GPIO_DRV_WritePinOutput(LCDB4, ((d>>4) & 0x01));
    GPIO_DRV_WritePinOutput(LCDB5, ((d>>5) & 0x01));
    GPIO_DRV_WritePinOutput(LCDB6, ((d>>6) & 0x01));
    GPIO_DRV_WritePinOutput(LCDB7, ((d>>7) & 0x01));
	WR_STROBE;
}
void writeRegister16(uint16_t a, uint16_t d)
{
	uint8_t hi, lo;
	hi = (a) >> 8;
	lo = (a);
	CD_COMMAND;
	write8(hi);
	write8(lo);
	hi = (d) >> 8;
	lo = (d);
	CD_DATA;
	write8(hi);
	write8(lo);
}
void writeRegister24(uint8_t r, uint32_t d) {
  CS_ACTIVE;
  CD_COMMAND;
  write8(r);
  CD_DATA;
  WAIT1_Waitus(10); //delayMicroseconds(10);
  write8(d >> 16);
  WAIT1_Waitus(10); //delayMicroseconds(10);
  write8(d >> 8);
  WAIT1_Waitus(10); //delayMicroseconds(10);
  write8(d);
  CS_IDLE;

}
uint8_t read8( void )
{
	uint8_t result;
    RD_ACTIVE;
//    DELAY7;
    WAIT1_Waitns(500);
    result = ((GPIO_DRV_ReadPinInput(LCDB0) & 0x01) | ((GPIO_DRV_ReadPinInput(LCDB1)<<1) & 0x02) | ((GPIO_DRV_ReadPinInput(LCDB2)<<2) & 0x04) | ((GPIO_DRV_ReadPinInput(LCDB3)<<3) & 0x08) | ((GPIO_DRV_ReadPinInput(LCDB4)<<4) & 0x10) | ((GPIO_DRV_ReadPinInput(LCDB5)<<5) & 0x20) | ((GPIO_DRV_ReadPinInput(LCDB6)<<6) & 0x40) | ((GPIO_DRV_ReadPinInput(LCDB7)<<7) & 0x80));
/*    		(GPIO_DRV_ReadPinInput(LCDB1) & 0x02) |  \
			(GPIO_DRV_ReadPinInput(LCDB2) & 0x04) |  \
			(GPIO_DRV_ReadPinInput(LCDB3) & 0x08) |  \
			(GPIO_DRV_ReadPinInput(LCDB4) & 0x10) |  \
			(GPIO_DRV_ReadPinInput(LCDB5) & 0x20) |  \
			(GPIO_DRV_ReadPinInput(LCDB6) & 0x40) |  \
			(GPIO_DRV_ReadPinInput(LCDB7) & 0x80));*/
    RD_IDLE;
    return result;
}

