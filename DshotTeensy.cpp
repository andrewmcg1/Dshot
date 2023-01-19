#include "DshotTeensy.h"
#include <iostream>
#include "Arduino.h"
#include <bitset>

Dshot::Dshot(int version, bool direction, bool tele)
{
  const int versions[] = {150, 300, 600, 1200};
  const double highTimes[] = {5.00, 2.50, 1.25, .625};
  const double lowTimes[] = {2.50, 1.25, .625, .3125};
  const double bitTimes[] = {6.67, 3.33, 1.67, .83};
  const double frameTimes[] = {106.72, 53.28, 26.72, 13.28};

  int i = 0;
  while(version > versions[i] && i < 4)
  {
    i++;
  }

  highTime = highTimes[i];
  lowTime = lowTimes[i];
  bitTime = bitTimes[i];
  frameTime = frameTimes[i];
  baud = versions[i] * 1000;
  biDirectional = direction;

  upTimer.priority(0);
  bitTimer.priority(1);
}

void Dshot::attach(unsigned int num)
{
  pin = num;
  pinMode(pin, OUTPUT);

  if(biDirectional == false)
  {
    digitalWrite(pin, LOW);
  }
  else
  {
    digitalWrite(pin, HIGH);
  }
  return;
}


void Dshot::write(int value)
{
  bits(value);
  bitTimer.begin([&] {Dshot::getBit(); }, bitTime);
  return;
}

void Dshot::getBit()
{
  static int bitCounter = 0;
  if(data[bitCounter] == 1)
  {
    upTimer.begin([&] {Dshot::sendBit(); }, bitTime);
  }
  else
  {
    upTimer.begin([&] {Dshot::sendBit(); }, lowTime);
  }

  Serial.println(bitCounter);
  if(++bitCounter == 16)
  {
    bitCounter = 0;
    bitTimer.end();
  }
  return;
}

void Dshot::sendBit()
{
  static int sentCounter = 0;
  bool DshotSerialValue;
  DshotSerialValue = digitalRead(pin);
  Serial.printf(" %d\n", sentCounter);
  if(DshotSerialValue == true)
  {
    digitalWriteFast(pin, false);
  }
  else if(DshotSerialValue == false)
  {
    digitalWriteFast(pin, true);
  }

  if(++sentCounter == 2)
  {
    upTimer.end();
    sentCounter = 0;
  }

  return;
}


short int Dshot::throttle(unsigned int value)
{
  if(value >= 2048)
    return 2047;
  else if(value < 0)
    return 0;
  return value;
}


int Dshot::checkSum(int value)
{
  if(biDirectional == false)
    return (value ^ (value >> 4) ^ (value >> 8)) & 0x0F;
  else
    return (~(value ^ (value >> 4) ^ (value >> 8))) & 0x0F;
}

void Dshot::bits(unsigned int value)
{
  char CRC;
  short int throttle;
  throttle  = (Dshot::throttle(value) << 1 | tele);
  CRC = checkSum(throttle);
  
  std::bitset<12> bits(throttle);
  std::bitset<4> crcBits(CRC);

  for(int i = 0; i < 12; i++)
  {
    data[11 - i] = bits[i];
  }
  for(int i = 0; i < 4; i++)
  {
    data[12 + i] = crcBits[i];
  }

  return;
}