/*
 * ILI9328.c
 *
 *  Created on: 8 Nov, 2015
 *      Author: kzw
 */
#include "ILI9328.h"
#include "LED.h"
#include "LCD.h"
#include "WAIT1.h"
#include "Terminus.h"
#include "String.h"

orientation_t   _orientation;
colordepth_t    _colorDepth;
font_t * _font;

void LCO_portwrite( unsigned short data );
void PulseLow( uint32_t pinName );
void PulseHigh( uint32_t pinName );
void WriteCmdData( unsigned short cmd, unsigned short data );
void WriteCmd( unsigned short cmd );
void Sleep( void );
void WakeUp( void );
void Activate( void );
void Deactivate( void );
void WriteData( unsigned short data );
void LCD_PrintChar( char c, unsigned short x, unsigned short y );
void SetPixelColor( unsigned int color);

void ILI9328_Init( orientation_t orientation, colordepth_t colors )
{
    _orientation = orientation;
    _colorDepth = colors;
    _font = &TerminusBigFont;

//    _lcd_pin_reset = HIGH;
//    wait_ms( 50 );
    GPIO_DRV_SetPinOutput(LCDReset);
    WAIT1_Waitms( 50 );

//    _lcd_pin_reset = LOW;
//    wait_ms( 100 );
    GPIO_DRV_ClearPinOutput(LCDReset);
    WAIT1_Waitms( 100 );

//    _lcd_pin_reset = HIGH;
//    wait_ms( 1000 );
    GPIO_DRV_SetPinOutput(LCDReset);
    WAIT1_Waitms( 1000 );

//    _lcd_pin_cs = HIGH;
    GPIO_DRV_SetPinOutput(LCDCS);

//    if ( _lcd_pin_rd != 0 )
//        *_lcd_pin_rd = HIGH;
//    _lcd_pin_wr = HIGH;
//    wait_ms( 15 );
//    GPIO_DRV_SetPinOutput(LCDRD);
    GPIO_DRV_SetPinOutput(LCDWR);
    WAIT1_Waitms( 15 );

    Activate();

    short drivOut = 0;
    short entryMod = 0;
    short gateScan = 0x2700;
    switch ( _orientation )
    {
        case LANDSCAPE:
            drivOut = 0x0100;
            entryMod |= 0x0038;
            gateScan |= 0x0000;
            break;

        case LANDSCAPE_REV:
            drivOut = 0x0000;
            entryMod |= 0x0038;
            gateScan |= 0x8000;
            break;

        case PORTRAIT_REV:
            drivOut = 0x0000;
            entryMod |= 0x0030;
            gateScan |= 0x0000;
            break;

        case PORTRAIT:
        default:
            drivOut = 0x0100;
            entryMod |= 0x0030;
            gateScan |= 0x8000;
            break;
    }

    switch ( _colorDepth )
    {
        case RGB18:
            entryMod |= 0x9000;
            break;

        case RGB16:
        default:
            entryMod |= 0x1000;
            break;
    }

    WriteCmdData( 0xE5, 0x78F0 ); // set SRAM internal timing
    WriteCmdData( 0x01, drivOut ); // set Driver Output Control
    WriteCmdData( 0x02, 0x0200 ); // set 1 line inversion
    WriteCmdData( 0x03, entryMod ); // set GRAM write direction and BGR=1.
    WriteCmdData( 0x04, 0x0000 ); // Resize register
    WriteCmdData( 0x08, 0x0207 ); // set the back porch and front porch
    WriteCmdData( 0x09, 0x0000 ); // set non-display area refresh cycle ISC[3:0]
    WriteCmdData( 0x0A, 0x0000 ); // FMARK function
    WriteCmdData( 0x0C, 0x0000 ); // RGB interface setting
    WriteCmdData( 0x0D, 0x0000 ); // Frame marker Position
    WriteCmdData( 0x0F, 0x0000 ); // RGB interface polarity
    // ----------- Power On sequence ----------- //
    WriteCmdData( 0x10, 0x0000 ); // SAP, BT[3:0], AP, DSTB, SLP, STB
    WriteCmdData( 0x11, 0x0007 ); // DC1[2:0], DC0[2:0], VC[2:0]
    WriteCmdData( 0x12, 0x0000 ); // VREG1OUT voltage
    WriteCmdData( 0x13, 0x0000 ); // VDV[4:0] for VCOM amplitude
    WriteCmdData( 0x07, 0x0001 );
//    wait_ms( 200 ); // Dis-charge capacitor power voltage
    WAIT1_Waitms( 200 );
    WriteCmdData( 0x10, 0x1690 ); // SAP, BT[3:0], AP, DSTB, SLP, STB
    WriteCmdData( 0x11, 0x0227 ); // Set DC1[2:0], DC0[2:0], VC[2:0]
//    wait_ms( 50 ); // Delay 50ms
    WAIT1_Waitms( 50 );
    WriteCmdData( 0x12, 0x000D ); // 0012
//    wait_ms( 50 ); // Delay 50ms
    WAIT1_Waitms( 50 );
    WriteCmdData( 0x13, 0x1200 ); // VDV[4:0] for VCOM amplitude
    WriteCmdData( 0x29, 0x000A ); // 04  VCM[5:0] for VCOMH
    WriteCmdData( 0x2B, 0x000D ); // Set Frame Rate
//    wait_ms( 50 ); // Delay 50ms
    WAIT1_Waitms( 50 );
    WriteCmdData( 0x20, 0x0000 ); // GRAM horizontal Address
    WriteCmdData( 0x21, 0x0000 ); // GRAM Vertical Address
    // ----------- Adjust the Gamma Curve ----------//
    WriteCmdData( 0x30, 0x0000 );
    WriteCmdData( 0x31, 0x0404 );
    WriteCmdData( 0x32, 0x0003 );
    WriteCmdData( 0x35, 0x0405 );
    WriteCmdData( 0x36, 0x0808 );
    WriteCmdData( 0x37, 0x0407 );
    WriteCmdData( 0x38, 0x0303 );
    WriteCmdData( 0x39, 0x0707 );
    WriteCmdData( 0x3C, 0x0504 );
    WriteCmdData( 0x3D, 0x0808 );
    //------------------ Set GRAM area ---------------//
    WriteCmdData( 0x50, 0x0000 ); // Horizontal GRAM Start Address
    WriteCmdData( 0x51, 0x00EF ); // Horizontal GRAM End Address
    WriteCmdData( 0x52, 0x0000 ); // Vertical GRAM Start Address
    WriteCmdData( 0x53, 0x013F ); // Vertical GRAM Start Address
    WriteCmdData( 0x60, gateScan ); // Gate Scan Line (0xA700)
    WriteCmdData( 0x61, 0x0000 ); // NDL,VLE, REV
    WriteCmdData( 0x6A, 0x0000 ); // set scrolling line
    //-------------- Partial Display Control ---------//
    WriteCmdData( 0x80, 0x0000 );
    WriteCmdData( 0x81, 0x0000 );
    WriteCmdData( 0x82, 0x0000 );
    WriteCmdData( 0x83, 0x0000 );
    WriteCmdData( 0x84, 0x0000 );
    WriteCmdData( 0x85, 0x0000 );
    //-------------- Panel Control -------------------//
    WriteCmdData( 0x90, 0x0010 );
    WriteCmdData( 0x92, 0x0000 );
    WriteCmdData( 0x07, 0x0133 ); // 262K color and display ON

    Deactivate();
}
void SetXY( unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2 )
{
    switch ( _orientation )
    {
        case LANDSCAPE:
        case LANDSCAPE_REV:
            WriteCmdData( 0x20, y1 );
            WriteCmdData( 0x21, x1 );
            WriteCmdData( 0x50, y1 );
            WriteCmdData( 0x52, x1 );
            WriteCmdData( 0x51, y2 );
            WriteCmdData( 0x53, x2 );
            break;

        case PORTRAIT_REV:
        case PORTRAIT:
        default:
            WriteCmdData( 0x20, x1 );
            WriteCmdData( 0x21, y1 );
            WriteCmdData( 0x50, x1 );
            WriteCmdData( 0x52, y1 );
            WriteCmdData( 0x51, x2 );
            WriteCmdData( 0x53, y2 );
            break;
    }
    WriteCmd( 0x22 );
}
void Sleep( void )
{
    Activate();
    WriteCmdData( 0x10, 0x1692 ); // enter sleep mode
    WAIT1_Waitms( 200 );
//    Sleep();
    Deactivate();
}

void WakeUp( void )
{
    Activate();
    WriteCmdData( 0x10, 0x1690 ); // exit sleep mode
    WAIT1_Waitms( 200 );
//    WakeUp();
    Deactivate();
}

void Activate( void )
{
//    _lcd_pin_cs = LOW;
    GPIO_DRV_ClearPinOutput(LCDCS);
}

void Deactivate( void )
{
//    _lcd_pin_cs = HIGH;
    GPIO_DRV_SetPinOutput(LCDCS);
}

void WriteCmdData( unsigned short cmd, unsigned short data )
{
    WriteCmd( cmd );
    WriteData( data );
}
void WriteCmd( unsigned short cmd )
{
    unsigned short u,l;
    u = (cmd>>8) & 0xFF;
    l = cmd & 0xFF;

//New
//    _lcd_pin_cs = LOW;
    GPIO_DRV_ClearPinOutput(LCDCS);
//
//    _lcd_pin_rs = LOW;
    GPIO_DRV_ClearPinOutput(LCDRS);
//New
//    *_lcd_pin_rd = HIGH;
//    _lcd_pin_wr = HIGH;
    GPIO_DRV_SetPinOutput(LCDRD);
    GPIO_DRV_SetPinOutput(LCDWR);
//
//    _lcd_port->write( u );
//    pulseLow( _lcd_pin_wr );
//    _lcd_port->write( l );
//    pulseLow( _lcd_pin_wr );
    LCO_portwrite( u );
    PulseLow(LCDWR);
    LCO_portwrite( l );
    PulseLow(LCDWR);
//New
//    _lcd_pin_cs = HIGH;
    GPIO_DRV_SetPinOutput(LCDCS);
}
void WriteData( unsigned short data )
{
	unsigned short u,l;
    u = (data>>8) & 0xFF;
    l = data & 0xFF;
//New
//    _lcd_pin_cs = LOW;
    GPIO_DRV_ClearPinOutput(LCDCS);
//
//    _lcd_pin_rs = HIGH;
    GPIO_DRV_SetPinOutput(LCDRS);
//New
//    *_lcd_pin_rd = HIGH;
//    _lcd_pin_wr = HIGH;
//    GPIO_DRV_SetPinOutput(LCDRD);
    GPIO_DRV_SetPinOutput(LCDWR);
//
//    _lcd_port->write( u );
//    pulseLow( _lcd_pin_wr );
//    _lcd_port->write( l );
//    pulseLow( _lcd_pin_wr );
    LCO_portwrite( u );
    PulseLow(LCDWR);
    LCO_portwrite( l );
    PulseLow(LCDWR);

//New
//    _lcd_pin_cs = HIGH;
    GPIO_DRV_SetPinOutput(LCDCS);
}
void PulseLow( uint32_t pinName )
{
    GPIO_DRV_ClearPinOutput(pinName);
    GPIO_DRV_SetPinOutput(pinName);
}
void PulseHigh( uint32_t pinName )
{
    GPIO_DRV_SetPinOutput(pinName);
    GPIO_DRV_ClearPinOutput(pinName);
}
void LCO_portwrite( unsigned short data )
{
    GPIO_DRV_WritePinOutput(LCDB0, (data & 0x01));
    GPIO_DRV_WritePinOutput(LCDB1, (data & 0x02));
    GPIO_DRV_WritePinOutput(LCDB2, (data & 0x04));
    GPIO_DRV_WritePinOutput(LCDB3, (data & 0x08));
    GPIO_DRV_WritePinOutput(LCDB4, (data & 0x10));
    GPIO_DRV_WritePinOutput(LCDB5, (data & 0x20));
    GPIO_DRV_WritePinOutput(LCDB6, (data & 0x40));
    GPIO_DRV_WritePinOutput(LCDB7, (data & 0x80));
}

void LCD_Print( const char *str, unsigned short x, unsigned short y)
{
    int stl, i;

    stl = strlen( str );
/*
    if ( x == RIGHT )
        x = GetWidth() - ( stl * _font->Width );
*/    if ( x == CENTER )
//        x = ( GetWidth() - ( stl * _font->Width ) ) / 2;
         x = ( 240 - ( stl * _font->Width ) ) / 2;

    for ( i = 0; i < stl; i++ )
            LCD_PrintChar( *str++, x + ( i * ( _font->Width ) ), y );
}

void LCD_PrintChar( char c, unsigned short x, unsigned short y )
{
    uint8_t i, ch;
    uint16_t j;

    int usedColorFG=256;
    int usedColorBG=256;

    uint16_t totalCharBytes = ( _font->Width * _font->Height ) / 8;
    int16_t position = _font->Position[ c - _font->Offset ];
    if ( position == -1 ) position = 0; // will print space character

    Activate();

    SetXY( x, y, x + _font->Width - 1, y + _font->Height - 1 );

    for ( j = 0; j < totalCharBytes; j++ )
    {
        ch = _font->Data[ position ];
        for ( i = 0; i < 8; i++ )
        {
            if ( ( ch & ( 1 << ( 7 - i ) ) ) != 0 ) SetPixelColor( usedColorFG );
            else SetPixelColor( usedColorBG );
        }
        position++;
    }
    Deactivate();
}
void SetPixelColor( unsigned int color)
{
    unsigned char r, g, b;
    unsigned short clr;
    r = g = b = 0;

    colordepth_t mode=RGB16;

    if ( _colorDepth == RGB16 )
    {
        switch ( mode )
        {
            case RGB16:
                WriteData( color & 0xFFFF );
                break;
            case RGB18:
                r = ( color >> 10 ) & 0xF8;
                g = ( color >> 4 ) & 0xFC;
                b = ( color >> 1 ) & 0x1F;
                clr = ( ( r | ( g >> 5 ) ) << 8 ) | ( ( g << 3 ) | b );
                WriteData( clr );
                break;
            case RGB24:
                r = ( color >> 16 ) & 0xF8;
                g = ( color >> 8 ) & 0xFC;
                b = color & 0xF8;
                clr = ( ( r | ( g >> 5 ) ) << 8 ) | ( ( g << 3 ) | ( b >> 3 ) );
                WriteData( clr );
                break;
        }
    }
    else if ( _colorDepth == RGB18 )
    {
        switch ( mode )
        {
            case RGB16:
                r = ( ( color >> 8 ) & 0xF8 ) | ( ( color & 0x8000 ) >> 13 );
                g = ( color >> 3 ) & 0xFC;
                b = ( ( color << 3 ) & 0xFC ) | ( ( color >> 3 ) & 0x01 );
                break;
            case RGB18:
                b = ( color << 2 ) & 0xFC;
                g = ( color >> 4 ) & 0xFC;
                r = ( color >> 10 ) & 0xFC;
                break;
            case RGB24:
                r = ( color >> 16 ) & 0xFC;
                g = ( color >> 8 ) & 0xFC;
                b = color & 0xFC;
                break;
        }
        clr = ( r << 8 ) | ( g << 2 ) | ( b >> 4 );
        WriteData( clr );
        WriteData( b << 4 );
    }
}
