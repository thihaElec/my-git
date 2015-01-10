/*
 * GroveStreams.h
 *
 *  Created on: 1 Jan, 2015
 *      Author: kzw
 */

#ifndef GROVESTREAMS_H_
#define GROVESTREAMS_H_

#include "CLS1.h"

/*!
 * \brief Command line parser routine
 * \param cmd Pointer to command line string
 * \param handled Return value if command has been handled
 * \param io Standard Shell I/O handler
 * \return Error code, ERR_OK for no failure
 */
uint8_t GROVESTREAMS_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io);

/*!
 * \brief Driver initialization
 */
void GROVESTREAMS_Deinit(void);

/*!
 * \brief Driver de-initialization
 */
void GROVESTREAMS_Init(void);

uint8_t GROVESTREAMS_SendValue(int32_t value1, int32_t value2, const CLS1_StdIOType *io);


#endif /* GROVESTREAMS_H_ */
