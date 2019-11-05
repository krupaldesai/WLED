/*
 * This file allows you to add own functionality to WLED more easily
 * See: https://github.com/Aircoookie/WLED/wiki/Add-own-functionality
 * EEPROM bytes 2750+ are reserved for your custom use case. (if you extend #define EEPSIZE in wled01_eeprom.h)
 * bytes 2400+ are currently ununsed, but might be used for future wled features
 */

const int motionSensor = 14; // D5 on wemos d1 mini

int sensorState = LOW;
int lastState = -1;

long lastDebounceTimeHigh = 0;
long lastDebounceTimeLow = 0;
long beginTime = 0;
long debounceDelayHigh = 100;
long debounceDelayLow = 1000;


void userBeginPreConnection()
{
  
}

void userBegin()
{
  // PIR Motion Sensor mode INPUT_PULLUP
  pinMode(motionSensor, INPUT_PULLUP);
  beginTime = millis();
  DEBUG_PRINT("Motion PIR configured");
}


void userLoop()
{
  if (lastState == -1) {
    if ((millis() - beginTime) > 2000) {
      publishMqttPir(false);
      lastState = LOW;
    }
  }
  else {
    // Get PIR state
    sensorState = digitalRead(motionSensor);

    if ( (sensorState == HIGH) ){
      if(((millis() - lastDebounceTimeHigh) > debounceDelayHigh)) {
        if (sensorState != lastState) {
          DEBUG_PRINT("Motion started");
          lastState = sensorState;
          publishMqttPir(sensorState);
        }
      }
    }
    else {
      lastDebounceTimeHigh = millis();
    }


    if ( (sensorState == LOW) ){
      if (((millis() - lastDebounceTimeLow) > debounceDelayLow)) {
        if (sensorState != lastState) {
          DEBUG_PRINT("Motion stopped");
          lastState = sensorState;
          publishMqttPir(sensorState);
        }
      }
    }
    else {
      lastDebounceTimeLow = millis();
    }
  }
}
