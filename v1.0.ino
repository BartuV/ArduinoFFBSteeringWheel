#include "Encoder.h"
#include "Joystick.h"
#include "Arduino.h"
#include "ArduinoJson.h"

// Pin Definitions
#define ROTARYENCI_PIN_CLK	2
#define ROTARYENCI_PIN_D	3

#define GAZ A0
#define FREN A1
#define DEBRIYAJ A2

// Global variables and defines
long rotaryEncIOldPosition  = 0;

int ButonPinleri[] = {};
int16_t PPR = 1000;
int aci = 900;
double kalb = (aci/180)*PPR;

Encoder rotaryEncI(ROTARYENCI_PIN_D,ROTARYENCI_PIN_CLK);

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
 JOYSTICK_TYPE_MULTI_AXIS, 
 JOYSTICK_DEFAULT_BUTTON_COUNT, 
 0, false, false, false, false, 
 false, false, false, false, false, false, true);

void setup_button_pins(){
  for (int i = 0; i < sizeof(ButonPinleri) / sizeof(int); i++) {
    pinMode(ButonPinleri[i], INPUT_PULLUP);
  }
}

double calculate_kalb(){
  return (aci/180)*PPR;
}

void change_PPR(int16_t target){
  PPR = target;
}

void setup() 
{
  Joystick.setSteeringRange(-int(aci/2),int(aci/2));
  Joystick.setAcceleratorRange(0,512);  
  Joystick.setBrakeRange(0,512);
  Joystick.setRudderRange(0,512);
  rotaryEncI.write(0);

  pinMode(GAZ, INPUT);
  pinMode(FREN, INPUT);
  pinMode(DEBRIYAJ, INPUT);

  setup_button_pins();

  Serial.begin(9600);

  Joystick.begin();
}

int lastAccelerator = 0;
int lastBreak = 0;
int lastRudder = 0;

void loop() 
{
  bool changed = false;

  for (int i = 0; i < sizeof(ButonPinleri) / sizeof(int); i++) {
    int durum = digitalRead(ButonPinleri[i]);
    if (durum == HIGH) {
      Joystick.pressButton(i);
      changed = true;
    } else {
      Joystick.releaseButton(i); 
      changed = true;
    }
  }

  int AcceleratorValue = analogRead(GAZ);
  int BreakValue = analogRead(FREN);
  int RudderValue = analogRead(DEBRIYAJ);

  if (AcceleratorValue != lastAccelerator){
    lastAccelerator = AcceleratorValue;
    Joystick.setAccelerator(AcceleratorValue);
    changed = true;
  }

  if (BreakValue != lastBreak){
    BreakValue = lastBreak;
    Joystick.setBrake(BreakValue);
    changed = true;
  }

  if (RudderValue != lastRudder){
    RudderValue = lastRudder;
    Joystick.setRudder(RudderValue);
    changed = true;
  }

  int16_t rotaryEncINewPosition = constrain(rotaryEncI.read(),-32768,32768);
  if (rotaryEncINewPosition != rotaryEncIOldPosition) {
    rotaryEncIOldPosition = rotaryEncINewPosition;
    Joystick.setSteering(constrain(360*(rotaryEncINewPosition/kalb),-int(aci/2),int(aci/2)));
    changed = true;
  }

  check_serial();
  if (changed){
    Joystick.sendState();
  }
}

JsonDocument doc;

void check_serial(){
  if (Serial.available() > 0) {
    String str = Serial.readStringUntil(char("}"));
    // Tam JSON verisi alındığında işleme
    if (str.startsWith("{")&&(str.endsWith("}"))) {
      DeserializationError error = deserializeJson(doc, str);
      if (error){
        Serial.print(error.f_str());
        Serial.print(";");
        Serial.print(str);
        return;
      }

      Joystick.setSteeringRange(doc["aci"][0],doc["aci"][1]);
      aci = doc["aci"][2];
      change_PPR(doc["ppr"]);
      kalb = calculate_kalb();
      
      memset(ButonPinleri, 0, sizeof(ButonPinleri));

      for (uint8_t i=0; i<doc["butons"].size(); i++) {
        ButonPinleri[i] = doc["butons"][i];
      }
    }
  }
}
