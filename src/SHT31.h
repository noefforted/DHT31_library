#ifndef SHT31_H
#define SHT31_H

#include <Arduino.h>
#include <Wire.h>

class SHT31
{
public:
    SHT31(uint8_t address);
    bool begin();
    bool readSensor();
    float readTemp();
    float readHumid();

private:
    uint8_t _address;
    bool writeCommand(uint16_t command);
    uint16_t rawTemp;
    uint16_t rawHumid;
    uint8_t checkSum(uint8_t data[], uint8_t length);
};

#endif