#include "MQTT.h"

#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart3;  // tells compiler huart3 exists in main.c

char uart3_buf[256];  // define the buffer here
void ESP_SendCommand(const char *cmd, uint32_t wait_ms)
{
    uint8_t rx[256];
    memset(rx, 0, sizeof(rx));

    // Send command
    HAL_UART_Transmit(&huart3, (uint8_t*)cmd, strlen(cmd), 1000);

    // Wait for ESP to process
    HAL_Delay(wait_ms);

    // Read response
    HAL_UART_Receive(&huart3, rx, sizeof(rx) - 1, 500);

    // Print to debug UART
    printf(">> %s", cmd);
    printf("<< %s\r\n", rx);
}

void ESP_MQTTTest(void)
{
    printf("=== MQTT Test Sequence ===\r\n");

    ESP_SendCommand("AT\r\n",                                           500);
    ESP_SendCommand("AT+CWMODE=1\r\n",                                  500);

    printf("=== Test done ===\r\n");
}
