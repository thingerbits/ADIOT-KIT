/*
Cayenne BMP180 Example

This sketch shows how to send BMP180 Sensor data to the Cayenne Dashboard.

The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. Install the Adafruit Unified Sensor library (https://github.com/adafruit/Adafruit_Sensor) from the Arduino Library Manager.
2. Install the Adafruit BMP085 Unified library (https://github.com/adafruit/Adafruit_BMP085_Unified) from the Arduino Library Manager.
3. Attach a BMP180 to your Arduino.
   Schematic:
   BMP180    Arduino
   [VDD] --- [3V3]
   [GND] --- [GND]
   [SDA] --- [Analog Pin 4] (The SDA may be different on some devices, e.g. for Arduino Mega the SDA pin is Digital Pin 20)
   [SCL] --- [Analog Pin 5] (The SCL may be different on some devices, e.g. for Arduino Mega the SCL pin is Digital Pin 21)
4. Set the TEMPERATURE_VIRTUAL_CHANNEL value below to a free virtual channel (or the temperature channel of the BMP180 Sensor widget you have added) in the Dashboard.
5. Set the BAROMETER_VIRTUAL_CHANNEL value below to a free virtual channel (or the pressure channel of the BMP180 Sensor widget you have added) in the Dashboard.
6. Set the Cayenne authentication info to match the authentication info from the Dashboard.
7. Compile and upload this sketch.
8. Once the Arduino connects to the Dashboard it should automatically create temporary display widgets (or update the BMP180 Sensor widgets you have added) with data.
   To make a temporary widget permanent click the plus sign on the widget.
*/

#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <CayenneMQTTEthernet.h>

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "put your username here ";
char password[] = "put your password here";
char clientID[] = "put your clientID here";

#define TEMPERATURE_VIRTUAL_CHANNEL 5
#define BAROMETER_VIRTUAL_CHANNEL 6

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10180);
bool bmpSensorDetected = true;

void setup()
{
  Serial.begin(9600);
  Cayenne.begin(username, password, clientID);
  if (!bmp.begin())
  {
    CAYENNE_LOG("No BMP sensor detected");
    bmpSensorDetected = false;
  }
}

void loop()
{
  Cayenne.loop();
}

// This function is called at intervals to send temperature sensor data to Cayenne.
CAYENNE_OUT(TEMPERATURE_VIRTUAL_CHANNEL)
{
  if (bmpSensorDetected)
  {
    float temperature;
    bmp.getTemperature(&temperature);
    // Send the value to Cayenne in Celsius.
    Cayenne.celsiusWrite(TEMPERATURE_VIRTUAL_CHANNEL, temperature);
  }
  else
  {
    CAYENNE_LOG("No BMP sensor detected");
  }
}

// This function is called at intervals to send barometer sensor data to Cayenne.
CAYENNE_OUT(BAROMETER_VIRTUAL_CHANNEL)
{
  if (bmpSensorDetected)
  {
    // Send the command to get data.
    sensors_event_t event;
    bmp.getEvent(&event);

    if (event.pressure)
    {
      // Send the value to Cayenne in hectopascals.
      Cayenne.hectoPascalWrite(BAROMETER_VIRTUAL_CHANNEL, event.pressure);
    }
  }
  else
  {
    CAYENNE_LOG("No BMP sensor detected");
  }
}
