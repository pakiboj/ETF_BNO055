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

void ESP_SendCommand(const char *cmd, uint32_t wait_ms);
void ESP_MQTTTest(void);

#endif /* INC_MQTT_H_ */
