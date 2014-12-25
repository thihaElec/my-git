/*
 * DHT.c
 *
 *  Created on: 21 Dec, 2014
 *      Author: kzw
 */

#include "Cpu.h"
#include "Events.h"
#include "CLS1.h"
#include "DHT.h"

#define MAX_TIME 85

uint8_t DHT_read(void) {

	uint8_t DHT_data[5];
	uint8_t lststate=1;
	uint8_t counter=0;
	uint8_t j=0,i;
	uint8_t buf[32]; //for printing
	const CLS1_StdIOType *io;
	io = CLS1_GetStdio();

	for(i=0;i<5;i++)
		DHT_data[i]=0;

	DHTpin_SetOutput();
	DHTpin_PutVal(0);   //MCU signal
	WAIT1_Waitms(18);
	DHTpin_PutVal(1);
	WAIT1_Waitus(40);

/*
	for(i=0;i<20;i++)
	{
	DHTpin_PutVal(0);   //Debug
	WAIT1_Waitms(18);
	DHTpin_PutVal(1);
	WAIT1_Waitus(40);
	}
*/

	DHTpin_SetInput();

	for(i=0;i<MAX_TIME;i++)
	{
	   counter=0;
	   while(DHTpin_GetVal()==lststate)
	   {
	      counter++;
	      WAIT1_Waitus(1);
	      if(counter==255)
	        break;
	   }
	   lststate=DHTpin_GetVal();
	   if(counter==255)
	       break;
	    // top 3 transistions are ignored
	   if((i>=4)&&(i%2==0))
	   {
		   DHT_data[j/8]<<=1;
	      if(counter>16)
	    	  DHT_data[j/8]|=1;
	      j++;
	    }
    }

	// verify cheksum and print the verified data
	if((j>=40)&&(DHT_data[4]==((DHT_data[0]+DHT_data[1]+DHT_data[2]+DHT_data[3])& 0xFF)))
	{
/*
//debug print
		CLS1_SendStr("1:", io->stdOut);
		CLS1_SendNum16u(DHT_data[0], io->stdOut);
		CLS1_SendStr(" 2: ", io->stdOut);
		CLS1_SendNum16u(DHT_data[1], io->stdOut);
		CLS1_SendStr(" 3: ", io->stdOut);
		CLS1_SendNum16u(DHT_data[2], io->stdOut);
		CLS1_SendStr(" 4: ", io->stdOut);
		CLS1_SendNum16u(DHT_data[3], io->stdOut);
		CLS1_SendStr("\r\n", io->stdOut);
*/
//	    farenheit=dht11_val[2]*9./5.+32;
	}
	else
	{
		CLS1_SendStr("Error\r\n", io->stdErr);
	}

	DHTpin_SetOutput();
	DHTpin_PutVal(1);   //MCU signal

	return DHT_data[2]; // return temperature
}
