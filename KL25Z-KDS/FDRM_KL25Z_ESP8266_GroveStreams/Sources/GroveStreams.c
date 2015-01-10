/*
 * GroveStreams.c
 *
 *  Created on: 2 Jan, 2015
 *      Author: kzw
 */


#include "GroveStreams.h"
#include "ESP8266.h"
#include "UTIL1.h"
#include "WAIT1.h"

#define GROVESTREAMS_IP_STR      "173.236.12.163" /* grovestreams.com IP Address */
#define GROVESTREAMS_IP_PORT     80 /* port number */
#define GROVESTREAMS_KEY_STR     "XXXXXXXXXXXXXXXX" /* API key */

#define GROVESTREAMS_TEMP_ID     "D11" /* Template ID - must match with Grovestreams ID*/
#define GROVESTREAMS_API_KRY     "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX" /* API key */
#define GROVESTREAMS_COM_NAME     "Custom" /* component name - it could auto add to ID*/
#define MY_MAC     "194.168.0.116" /* my MAC address - must always be MAC address*/
#define GROVESTREAMS_ID_1     "s1" /* ID 1 */
#define GROVESTREAMS_ID_2     "s2" /* ID 2 */

#define GROVESTREAMS_CHANNEL     XXXXX /* channel ID */
//#define THING_SPEAK_LABEL_STR1   "1"


/*!
 * \brief Sends a value to the ThingSpeak server
 * \param value Value to be sent
 * \param io Shell I/O handler or NULL if not used
 * \return Error code, ERR_OK for no failure
 */
uint8_t GROVESTREAMS_SendValue(int32_t value1,int32_t value2, const CLS1_StdIOType *io) {
  /* Does the same as the following shell commands:
   ESP send AT+CIPMUX=1
   ESP send AT+CIPSTART=4,"TCP","184.106.153.149",80  // 4 is the channel ID
   ESP send AT+CIPSEND=4,44                           // size with \r\n!
   ESP send GET /update?key=J4P1LICZI6ZF0YWG&field1=60
   ESP send AT+CIPCLOSE
   */
  uint8_t res = ERR_OK;
  uint8_t buf[256];
//  uint8_t buf1[128];
  uint8_t ch_id = 4;

  res = ESP_SetNumberOfConnections(1, io, ESP_DEFAULT_TIMEOUT_MS);
  if (res==ERR_OK) {
    res = ESP_OpenConnection(ch_id, TRUE, GROVESTREAMS_IP_STR, GROVESTREAMS_IP_PORT, 5000, io);
    if (res==ERR_OK) {
      UTIL1_strcpy(buf, sizeof(buf), "PUT /api/feed?compTmplId=");
      UTIL1_strcat(buf, sizeof(buf), GROVESTREAMS_TEMP_ID);
      UTIL1_strcat(buf, sizeof(buf), "&compId=");
      UTIL1_strcat(buf, sizeof(buf), MY_MAC);
      UTIL1_strcat(buf, sizeof(buf), "&compName=");
      UTIL1_strcat(buf, sizeof(buf), GROVESTREAMS_COM_NAME);
      UTIL1_strcat(buf, sizeof(buf), "&api_key=");
      UTIL1_strcat(buf, sizeof(buf), GROVESTREAMS_API_KRY);

      UTIL1_strcat(buf, sizeof(buf), "&s1=");
      UTIL1_strcatNum32s(buf, sizeof(buf), value1);
      UTIL1_strcat(buf, sizeof(buf), "&s2=");
      UTIL1_strcatNum32s(buf, sizeof(buf), value2);
      UTIL1_strcat(buf, sizeof(buf), "\0");

      res = ESP_PrepareMsgSend(ch_id, UTIL1_strlen(buf), 5000, io);
      if (res==ERR_OK) {
        /* sending data */
        res = ESP_SendATCommand(buf, NULL, 0, NULL, ESP_DEFAULT_TIMEOUT_MS, io);
        if (res!=ERR_OK) {
          CLS1_SendStr("Sending page failed!\r\n", io->stdErr); /* copy on console */
        } else {
          for(;;) { /* breaks */
            res = ESP_ReadCharsUntil(buf, sizeof(buf), '\n', 1000);
            if (res==ERR_OK) { /* line read */
              if (io!=NULL) {
                CLS1_SendStr(buf, io->stdOut); /* copy on console */
              }
            }
            if (UTIL1_strncmp(buf, "SEND OK\r\n", sizeof("SEND OK\r\n")-1)==0) { /* ok from module */
              break;
            }
          }
        }
      }

      UTIL1_strcpy(buf, sizeof(buf), "Host: \r\n");
      UTIL1_strcat(buf, sizeof(buf), "Connection: close\r\n");
      UTIL1_strcat(buf, sizeof(buf), "X-Forwarded-For: ");
      UTIL1_strcat(buf, sizeof(buf), MY_MAC);
      UTIL1_strcat(buf, sizeof(buf), "\r\n");
      UTIL1_strcat(buf, sizeof(buf), "Content-Type: application/json \r\n");


      res = ESP_PrepareMsgSend(ch_id, UTIL1_strlen(buf), 3000, io);
      if (res==ERR_OK) {
        res = ESP_SendATCommand(buf, NULL, 0, NULL, ESP_DEFAULT_TIMEOUT_MS, io);
        if (res!=ERR_OK) {
          CLS1_SendStr("Sending page failed string 2!\r\n", io->stdErr);
        } else {
          for(;;) {
            res = ESP_ReadCharsUntil(buf, sizeof(buf), '\n', 1000);
            if (res==ERR_OK) {
              if (io!=NULL) {
                CLS1_SendStr(buf, io->stdOut);
              }
            }
            if (UTIL1_strncmp(buf, "SEND OK\r\n", sizeof("SEND OK\r\n")-1)==0) {
              break;
            }
          }
        }
      }


    }
    (void)ESP_CloseConnection(ch_id, io, ESP_DEFAULT_TIMEOUT_MS);
  }
  return res;
}

static uint8_t GROVESTREAMS_PrintHelp(const CLS1_StdIOType *io) {
  CLS1_SendHelpStr("GROVESTREAMS", "GROVESTREAMS commands\r\n", io->stdOut);
  CLS1_SendHelpStr("  help|status", "Print help or status information\r\n", io->stdOut);
  CLS1_SendHelpStr("  send <value>", "Send a value\r\n", io->stdOut);
  return ERR_OK;
}

static uint8_t GROVESTREAMS_PrintStatus(const CLS1_StdIOType *io) {
  uint8_t buf[24];

  CLS1_SendStatusStr("ThingSpeak", "\r\n", io->stdOut);
  CLS1_SendStatusStr("  IP", GROVESTREAMS_IP_STR "\r\n", io->stdOut);
//  UTIL1_Num32uToStr(buf, sizeof(buf), GROVESTREAMS_IP_PORT);
  UTIL1_strcat(buf, sizeof(buf), "\r\n");
  CLS1_SendStatusStr("  Port", buf, io->stdOut);
  CLS1_SendStatusStr("  API key", GROVESTREAMS_KEY_STR "\r\n", io->stdOut);
//  UTIL1_Num32uToStr(buf, sizeof(buf), GROVESTREAMS_CHANNEL);
  UTIL1_strcat(buf, sizeof(buf), "\r\n");
  CLS1_SendStatusStr("  Channel ID", buf, io->stdOut);
  return ERR_OK;
}

uint8_t GROVESTREAMS_ParseCommand(const unsigned char *cmd, bool *handled, const CLS1_StdIOType *io) {
  int32_t val1,val2;
  uint8_t res;
  const unsigned char *p;

  if (UTIL1_strcmp((char*)cmd, CLS1_CMD_HELP)==0 || UTIL1_strcmp((char*)cmd, "ThingSpeak help")==0) {
    *handled = TRUE;
    res = GROVESTREAMS_PrintHelp(io);
  } else if (UTIL1_strcmp((char*)cmd, CLS1_CMD_STATUS)==0 || UTIL1_strcmp((char*)cmd, "ThingSpeak status")==0) {
    *handled = TRUE;
    res = GROVESTREAMS_PrintStatus(io);
  } else if (UTIL1_strncmp((char*)cmd, "ThingSpeak send ", sizeof("ThingSpeak send ")-1)==0) {
    *handled = TRUE;
    p = cmd + sizeof("ThingSpeak send ")-1;
    res = UTIL1_ScanDecimal32sNumber(&p, &val1);  //potential problem
    if (res==ERR_OK) {
      res = GROVESTREAMS_SendValue(val1,val2, io);
      if (res==ERR_OK) {
        CLS1_SendStr("Sent Data: OK!\r\n", io->stdOut);
      }
    }
  }
  return res;
}

void GROVESTREAMS_Deinit(void) {
  /* nothing to do */
}

void GROVESTREAMS_Init(void) {
  /* nothing to do */
}
