#include "Encoder.h"
#include "Joystick.h"

// Pin Definitions
#define ROTARYENCI_PIN_CLK	2
#define ROTARYENCI_PIN_D	3

#define GAZ A0
#define FREN A1
#define DEBRIYAJ A2

// Global variables and defines
long rotaryEncIOldPosition  = 0;

int ButonPinleri[] = {};
int PPR = 1000;
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
  for (int i = 0; i < sizeof(ButonPinleri) / sizeof(int); i++) {
    int durum = digitalRead(ButonPinleri[i]);
    if (durum == HIGH) {
      Joystick.pressButton(i);
    } else {
      Joystick.releaseButton(i); 
    }
  }

  int AcceleratorValue = analogRead(GAZ);
  int BreakValue = analogRead(FREN);
  int RudderValue = analogRead(DEBRIYAJ);

  if (AcceleratorValue != lastAccelerator){
    lastAccelerator = AcceleratorValue;
    Joystick.setAccelerator(AcceleratorValue);
  }

  if (BreakValue != lastBreak){
    BreakValue = lastBreak;
    Joystick.setBrake(BreakValue);
  }

  if (RudderValue != lastRudder){
    RudderValue = lastRudder;
    Joystick.setRudder(RudderValue);
  }

  int16_t rotaryEncINewPosition = constrain(rotaryEncI.read(),-32768,32768);
  if (rotaryEncINewPosition != rotaryEncIOldPosition) {
    rotaryEncIOldPosition = rotaryEncINewPosition;
    Joystick.setSteering(constrain(360*(rotaryEncINewPosition/kalb),-int(aci/2),int(aci/2)));
  }

  check_serial();

  Joystick.sendState();
}

void check_serial(){
  if (Serial.available()) {
    // Veri geldiğinde
    byte boyut = Serial.read(); // İlk byte, veri boyutunu belirtir
    byte dizi[boyut]; // Veri boyutu kadar dizi oluştur

    for (int i = 0; i < boyut; i++) {
      dizi[i] = Serial.read(); // Veriyi diziye oku
    }

    //PPR komutu
    if (dizi[0] == 0){
      PPR = int(dizi[1]);
      kalb = (aci/180)*PPR;
    }

    //Buton komutu
    if (dizi[0] == 1){
      for (int i = 0; i < sizeof(ButonPinleri) / sizeof(int); i++) {
        ButonPinleri[i] = 32;
      }
      for(int i = 1; i<boyut; i++){
        ButonPinleri[i] =  dizi[i];
      }
      setup_button_pins();
    }

    //Açı sistemi
    if (dizi[0] == 2){
      Joystick.setSteeringRange(dizi[1],dizi[2]);
    }
  }
}