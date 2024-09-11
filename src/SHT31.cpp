#include <SHT31.h>

SHT31::SHT31(uint8_t address)
{
    _address = address;
}

bool SHT31::begin()
{
    Wire.begin();
    return true;
}

bool SHT31::writeCommand(uint16_t command)
{
    Wire.beginTransmission(_address);
    Wire.write(command >> 8);
    Wire.write(command & 0xFF);
    Wire.endTransmission();
    if (Wire.endTransmission() != 0)
    {
        return false;
    }
    return true;
}

bool SHT31::readSensor()
{
    if (!writeCommand(0x2400))
    {
        Serial.println("Command tidak terkirim");
        return false;
    }
    delay(15);

    Wire.requestFrom(_address, (uint8_t)6);

    if (Wire.available() != 6)
    {
        Serial.println("Wire tidak available");
        return false;
    }

    uint8_t tempData[2];
    tempData[0] = Wire.read(); // temp MSB
    tempData[1] = Wire.read(); // temp LSB
    uint8_t tempCrc = Wire.read();

    if (checkSum(tempData, 2) != tempCrc)
    {
        Serial.println("Checksum temp error");
        return false;
    }

    rawTemp = (tempData[0] << 8) | tempData[1];

    uint8_t humidData[2];
    humidData[0] = Wire.read(); // humid MSB
    humidData[1] = Wire.read(); // humid LSB
    uint8_t humidCrc = Wire.read();

    if (checkSum(humidData, 2) != humidCrc)
    {
        Serial.println("Checksum humid error");
        return false;
    }

    rawHumid = (humidData[0] << 8) | humidData[1];

    return true;
}

float SHT31::readTemp()
{
    if (!readSensor())
    {
        return NAN;
    }
    float temperatureC = -45 + (175 * (float)rawTemp / 65535);
    return temperatureC;
}

float SHT31::readHumid()
{
    if (!readSensor())
    {
        return NAN;
    }
    float humidity = 100 * (float)rawHumid / 65535;
    return humidity;
}

uint8_t SHT31::checkSum(uint8_t data[], uint8_t length)
{
    uint8_t crc = 0xFF;

    for (uint8_t i = 0; i < length; i++)
    {
        crc ^= data[i];
        for (uint8_t bit = 8; bit > 0; --bit)
        {
            if (crc & 0x80)
            {
                crc = (crc << 1) ^ 0x31;
            }
            else
            {
                crc <<= 1;
            }
        }
    }
    return crc;
}