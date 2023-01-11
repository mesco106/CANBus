#include "Arduino.h"
#include <mcp_can.h>
#include <SPI.h>

#define Pot 14

MCP_CAN CAN0(10);     // Set CS to pin 10
uint16_t Speed = 0;
uint16_t Controller = 0;
void setup()
{
  Serial.begin(115200);
  pinMode(Pot, INPUT);

  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_1000KBPS, MCP_8MHZ) == CAN_OK) Serial.println("MCP2515 Initialized Successfully!");
  else Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);   // Change to normal mode to allow messages to be transmitted
}
// byte data[8] = {0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
 byte data[8] = {0xA2, 0x00, 0x00, 0x00, 0x10, 0x50, 0x00, 0x00};

void loop()
{
  Controller = analogRead(Pot); 
  Speed = map(Controller,0,1023,0,255);
  data[5] = Speed;
  //Serial.println(Speed);
  // send data:  ID = 0x100, Standard CAN Frame, Data length = 8 bytes, 'data' = array of data bytes to send
  byte sndStat = CAN0.sendMsgBuf(0x141, 0, 8, data);
  if(sndStat == CAN_OK){
    Serial.println("Message Sent Successfully!");
  } else {
    Serial.println("Error Sending Message...");
  }
  delay(100);   // send data per 100ms
}