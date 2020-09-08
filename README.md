# VescUart

Arduino library for interfacing with a VESC over UART. This library is based upon the library written by RollingGecko (https://github.com/RollingGecko/VescUartControl). The library is updated for the newest VESC firmware (FW3.51) and cleaned up a bit. The library is not backwards compatible, so you have to upload the newest firmware to your VESC.

**Important:** This is not a dropin replacement for RollingGeckos library. You will have to make some changes to your software, as all functions and values is now within a class, see below.

## Note from FinallyFunctional

This is a fork from [this repository](https://github.com/SolidGeek/VescUart). I noticed the author hasn't merged open pull requests for many months now so I'm just assuming the library isn't being maintained anymore.

In this version I'm merging the development branch into master. I've also added a method to set the hand brake current for the motor.

## Usage
  
Initialize VescUart class and select Serial port for UART communication.  
  
```cpp
#include <VescUart.h>

VescUart UART;

void setup() {
  Serial.begin(115200);

  while (!Serial) {;}

  UART.setSerialPort(&Serial);
}
```

You can now safely use the functions and change the values of the class. 
  
Getting VESC telemetry:
  
```cpp
if ( UART.getVescValues() ) {
  Serial.println(UART.data.rpm);
  Serial.println(UART.data.inpVoltage);
  Serial.println(UART.data.ampHours);
  Serial.println(UART.data.tachometerAbs);
}
```
  
You can find example usage and more information in the examples directory.  
  
