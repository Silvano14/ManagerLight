#include <SPI.h>
#include <RTCZero.h>
#include <BlynkSimpleWiFiNINA.h>
#include <TimeLord.h>

#include "credentials.h"

#define BLYNK_PRINT Serial
#define PIR_PIN 7
#define LIGHT_PIN 6

const int PROGMEM
TIMEZONE = 1;
const double PROGMEM
LONGITUDE = 12.655040;
const double PROGMEM
LATITUDE = 45.962650;

TimeLord myLord;
RTCZero rtc;
BlynkTimer timer;

// Greenwich Mean Time
const int GMT = 1;

int lightState = LOW;
int sunset = 17;
boolean isSavedTimeState = false;
boolean isEnablePIR = true;
boolean notifying = false;
boolean hasNotified = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");
  Blynk.begin(AUTH_BLYNK, SSID, PSWD);

  unsigned long epoch;

  int numberOfTries = 0, maxTries = 5;

  Serial.println("Waiting for connection...");
  delay(10000);

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }

  rtc.begin();

  do {
    epoch = WiFi.getTime();
    numberOfTries++;
  } while ((epoch == 0) && (numberOfTries < maxTries));

  if (numberOfTries == maxTries) {
    Serial.print("NTP unreachable!!");
    while (1);
  } else {
    Serial.print("Epoch received: ");
    Serial.println(epoch);
    rtc.setEpoch(epoch);
  }

  // Pins
  pinMode(PIR_PIN, INPUT);
  pinMode(LIGHT_PIN, OUTPUT);

  // Setup a function to be called every second
  timer.setInterval(1000L, updateBlynkInterface);

  myLord.TimeZone(TIMEZONE * 60);
  myLord.Position(LATITUDE, LONGITUDE);
}

void loop() {
  Blynk.run();

  // Setup sunset 
  byte day[] = {0, 0, 0, rtc.getDay(), rtc.getMonth(), rtc.getYear()};
  myLord.SunSet(day);

  if (digitalRead(PIR_PIN) == HIGH && isSunset(sunset, day) && isEnablePIR) {
    Serial.println("I see you!");
    lightState = HIGH;
  }

  if (notifying) {
    hasNotified = false;
  }

  digitalWrite(LIGHT_PIN, lightState);
  keepOnForNMinutes(1);

  // Start BlynkTimer
  timer.run();

  // See updateBlynkInterface method
  delay(1000);
}

boolean isSunset(int timeSunset, byte day[]) {
  return (((rtc.getHours() + GMT) >= day[tl_hour]) && (rtc.getMinutes() >= day[tl_minute]));
}

int sum = 0;
int hoursState = 0;
int minutesState = 0;
int secondsState = 0;

void keepOnForNMinutes(int minutes) {
  if (lightState) {

    if (!isSavedTimeState) {
      hoursState = rtc.getHours() + GMT;
      minutesState = rtc.getMinutes();
      secondsState = rtc.getSeconds();
      Serial.println("Time get it! - " + String(hoursState) + ":" + String(minutesState) + ":" + String(secondsState));
      Serial.println("Keeping on...");
      sum = minutesState + minutes;
      isSavedTimeState = true;
    }

    if (minutes <= 0) {
      Serial.println("You put a wrong value: " + String(minutes));
    }

    if (sum >= 60) {
      sum -= 60;
    }

    if (notifying && !hasNotified) {
      Blynk.notify("The light go on until: " + String(hoursState) + ":" + String(sum) + ":" + String(secondsState));
      hasNotified = true;
      // Just one time
      notifying = false;
    }

    if (rtc.getMinutes() == sum && rtc.getSeconds() == secondsState) {
      isSavedTimeState = false;
      lightState = LOW;
    }
  }
}

void setManualLight(int value) {
  lightState = value;

  // Cleaning status
  isSavedTimeState = 0;
  hoursState = 0;
  minutesState = 0;
  secondsState = 0;
}

void updateBlynkInterface() {
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V1, isEnablePIR);
  Blynk.virtualWrite(V3, lightState);
  Blynk.virtualWrite(V4, notifying);
}

// Use virtual pin 1 to active Passive InfraRed
BLYNK_WRITE(V1){
    isEnablePIR = param.asInt();
}

// Virtual Pin 3 turns on the light for N minutes
BLYNK_WRITE(V3){
    setManualLight(param.asInt());
}

// Use virtual Pin 4 if you want receive a notify
BLYNK_WRITE(V4){
    notifying = param.asInt();
}
