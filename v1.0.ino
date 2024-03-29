#include "Encoder.h"
#include "Joystick.h"

// Pin Definitions
#define ROTARYENCI_PIN_CLK	2
#define ROTARYENCI_PIN_D	3

// Global variables and defines
long rotaryEncIOldPosition  = 0;

int button_pins[] = {};
int last_button_state[] = {};

Encoder rotaryEncI(ROTARYENCI_PIN_D,ROTARYENCI_PIN_CLK);
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_MULTI_AXIS, sizeof(button_pins), 0, false, false, false, false, false, false, false, false, false, false, true);

// Setup the essentials for your circuit to work. It runs first every time your circuit is powered with electricity.
void setup() 
{
  Joystick.setSteeringRange(-450,450);
  rotaryEncI.write(0);

  for(int i = 0; i < sizeof(button_pins); i++){
    pinMode(button_pins[i],INPUT);
    last_button_state[i] = LOW;
  }

  Joystick.begin();
}

// Main logic of your circuit. It defines the interaction between the components you selected. After setup, it runs over and over again, in an eternal loop.
void loop() 
{
  for(int i = 0; i < sizeof(button_pins); i++){
    int n = digitalRead(button_pins[i]);
    if(n != last_button_state[i]){
      last_button_state[i] = !last_button_state[i];
      Joystick.pressButton(i);
    }else {
      Joystick.releaseButton(i);
    }
  }

  int32_t rotaryEncINewPosition = rotaryEncI.read();
  if (rotaryEncINewPosition != rotaryEncIOldPosition) {
    rotaryEncIOldPosition = rotaryEncINewPosition;
    Joystick.setSteering((int16_t)rotaryEncINewPosition/5);
  }

  Joystick.sendState();
}