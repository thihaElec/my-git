/*
 * ILI9328.h
 *
 *  Created on: 8 Nov, 2015
 *      Author: kzw
 */

#ifndef SOURCES_ILI9328_H_
#define SOURCES_ILI9328_H_

/** \enum Orientation_enum
 *  \brief Display orientation.
 */
enum Orientation_enum
{
    PORTRAIT = 0, /**< Top row of the screen is at 12 o'clock. */
    LANDSCAPE = 1, /**< Top row of the screen is at 9 o'clock. */
    PORTRAIT_REV = 2, /**< Top row of the screen is at 6 o'clock. */
    LANDSCAPE_REV = 3, /**< Top row of the screen is at 3 o'clock. */
};

typedef enum Orientation_enum orientation_t;
typedef enum ColorDepth_enum colordepth_t;
enum ColorDepth_enum
{
    RGB16, /**< 16-bit colors, pixels can have 65K+ distinct color values */
    RGB18, /**< 18-bit colors, pixels can have 262K+ distinct color values */
    RGB24, /**< 24-bit colors, full 8 bits per component, 16M+ distinct color values */
};

enum Alignment_enum
{
    LEFT = 0, /**< Left-oriented, naturally gravitate closer to the left edge of the screen. */
    CENTER = 9998, /**< Center-oriented, try to fit in the middle of the available space with equal free space to the left and right of the text. */
    RIGHT = 9999, /**< Right-oriented, naturally gravitate closer to the right edge of the screen, leaving any remaining free space to the left of the text. */
};

void ILI9328_Init( orientation_t orientation, colordepth_t colors );
void LCD_Print( const char *str, unsigned short x, unsigned short y);

#endif /* SOURCES_ILI9328_H_ */
