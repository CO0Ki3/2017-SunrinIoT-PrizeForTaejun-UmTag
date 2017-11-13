#define CODEBENDER true

#if defined(ARDUINO_ARCH_SAMD)
  #define SerialMonitorInterface SerialUSB
#else
  #define SerialMonitorInterface Serial
#endif

#define BLE_DEBUG true

#include <SPI.h>
#include "lib_aci.h"
#include "aci_setup.h"
#include "uart_over_ble.h"
#include "services.h"

uint8_t ble_rx_buffer[21];
uint8_t ble_rx_buffer_len = 0;
uint8_t ble_connection_state = false;
char compareInIfLockOn[] = {'l'};
char compareInIfLockOff[] = {'f'};
char ble_rx_buffer_type_change[21];
#if CODEBENDER
#include "UART.h"
#endif


void setup() {
  SerialMonitorInterface.begin(9600);
  delay(100);
  BLEsetup();
}

void loop() {
  aci_loop();
  if (SerialMonitorInterface.available()) {
    delay(50);//should catch full input
    uint8_t sendBuffer[21];
    uint8_t sendLength = 0;
    lib_aci_send_data(PIPE_UART_OVER_BTLE_UART_TX_TX, sendBuffer, sendLength);
    while (SerialMonitorInterface.available()) {
      sendBuffer[sendLength] = SerialMonitorInterface.read();
      sendLength++;
    }
    sendBuffer[sendLength] = '\0';
    sendLength++;
    int i;
    char UniToText[21]={};
  for(i = 0; i < 21; i++) {
    UniToText[i] = (char)sendBuffer[i];
    Serial.print(UniToText[i]);
  }
  Serial.println();
  }
}
