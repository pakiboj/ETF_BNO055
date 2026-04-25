#include "MQTT.h"

#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart3;  // tells compiler huart3 exists in main.c

char uart3_buf[256];  // define the buffer here


void AT_Send(const char* cmd) {
    memset(uart3_buf, 0, sizeof(uart3_buf));

    HAL_UART_Transmit(&huart3, (uint8_t*)cmd, strlen(cmd), 1000);
    HAL_Delay(2000);  // give ESP time to respond
    HAL_UART_Receive(&huart3, (uint8_t*)uart3_buf, sizeof(uart3_buf)-1, 2000);

    // Print both what we sent and what ESP replied
    printf(">>> %s", cmd);
    printf("<<< %s\r\n", uart3_buf);
}
// Receive response into buffer
void AT_Receive(void) {
    memset(uart3_buf, 0, sizeof(uart3_buf));
    HAL_UART_Receive(&huart3, (uint8_t*)uart3_buf, sizeof(uart3_buf)-1, 1000);
}

void MQTT_Init(void) {
    // 1. Test AT
    AT_Send("AT\r\n");
    HAL_Delay(500);

    // 2. Set station mode
    AT_Send("AT+CWMODE=1\r\n");
    HAL_Delay(500);

    // 3. Connect to WiFi
    AT_Send("AT+CWJAP=\"Zoja WiFi 2.4G\",\"avavavav\"\r\n");
    HAL_Delay(5000); // WiFi needs time to connect

    // 4. Configure MQTT client
    // params: LinkID, scheme(1=TCP), clientID, username, password, cert, CA, path
    AT_Send("AT+MQTTUSERCFG=0,1,\"stm32client\",\"\",\"\",0,0,\"\"\r\n");
    HAL_Delay(500);

    // 5. Connect to local Mosquitto broker
    // params: LinkID, broker_ip, port, reconnect
    AT_Send("AT+MQTTCONN=0,\"192.168.1.107\",1883,1\r\n");
    HAL_Delay(2000);
}

void MQTT_Publish(float acc_x, float acc_y, float acc_z) {
    char payload[128];
    char cmd[200];

    // Build JSON payload
    snprintf(payload, sizeof(payload),
             "{\"acc_x\":%.2f,\"acc_y\":%.2f,\"acc_z\":%.2f}",
             acc_x, acc_y, acc_z);

    memset(uart3_buf, 0, sizeof(uart3_buf));
        HAL_UART_Transmit(&huart3, (uint8_t*)cmd, strlen(cmd), 1000);
        HAL_Delay(500);
        HAL_UART_Receive(&huart3, (uint8_t*)uart3_buf, sizeof(uart3_buf)-1, 1000);

        // ESP responds with ">" when ready for payload
        if (strstr(uart3_buf, ">") != NULL) {
            // Step 2 - send raw payload
            HAL_UART_Transmit(&huart3, (uint8_t*)payload, strlen(payload), 1000);
            HAL_Delay(500);

            memset(uart3_buf, 0, sizeof(uart3_buf));
            HAL_UART_Receive(&huart3, (uint8_t*)uart3_buf, sizeof(uart3_buf)-1, 1000);
            printf("PUB result: %s\r\n", uart3_buf);
        } else {
            printf("ESP not ready, got: %s\r\n", uart3_buf);
        }

    // AT+MQTTPUB=LinkID,topic,payload,qos,retain
    snprintf(cmd, sizeof(cmd),
             "AT+MQTTPUB=0,\"sensor/imu\",\"%s\",0,0\r\n",
             payload);

    AT_Send(cmd);
}
