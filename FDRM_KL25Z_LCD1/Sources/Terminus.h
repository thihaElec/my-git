/*
 * Terminus.h
 *
 *  Created on: 10 Nov, 2015
 *      Author: kzw
 */

#ifndef SOURCES_TERMINUS_H_
#define SOURCES_TERMINUS_H_

typedef struct FontMetrics_struct{
	unsigned short Offset;
	unsigned short Width;
	unsigned short Height;
    const short* Position;
    const char* Data;
}font_t;

extern const font_t TerminusBigFont;

#endif /* SOURCES_TERMINUS_H_ */
