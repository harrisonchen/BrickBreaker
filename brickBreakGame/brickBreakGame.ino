#include <I2Cdev.h>
#include <MPU6050.h>

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include <Wire.h>
#endif

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;

int converted_ax = 0;
int converted_ay = 0;

#define OUTPUT_READABLE_ACCELGYRO

int dataPin = 2;
int latchPin = 3;
int clockPin = 4;
int clearPin = 5;
int redByte = 0x01; int greenByte = 0x01; int blueByte = 0x01; int colByte = 0x01;

void setup()
{
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
    
    Serial.begin(38400);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    accelgyro.setXGyroOffset(220);
    accelgyro.setYGyroOffset(76);
    accelgyro.setZGyroOffset(-85);
  
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(clearPin, OUTPUT);
}

void shiftVal(int redByte, int greenByte, int blueByte, int colByte)
{
  digitalWrite(latchPin, LOW);
  digitalWrite(clearPin, LOW);
  digitalWrite(clearPin, HIGH);
  //shiftOut(dataPin, clockPin, bitOrder, value);
  shiftOut(dataPin, clockPin, MSBFIRST, colByte);
  shiftOut(dataPin, clockPin, MSBFIRST, (~blueByte)&0xFF);
  shiftOut(dataPin, clockPin, MSBFIRST, (~greenByte)&0xFF);
  shiftOut(dataPin, clockPin, MSBFIRST, (~redByte)&0xFF);
  digitalWrite(latchPin, HIGH);
}

void loop()
{
  
}
