/*
 * AT_commands.h
 *
 *  Created on: 25 Apr 2026
 *      Author: mikih
 */

#ifndef INC_MQTT_H_
#define INC_MQTT_H_

#include "stm32l4xx_hal.h"
#include <string.h>

extern char uart3_buf[256];  // shared buffer

void AT_Send(const char* cmd);
void AT_Receive(void);
void MQTT_Init(void);
void MQTT_Publish(float acc_x, float acc_y, float acc_z);

#endif /* INC_MQTT_H_ */
