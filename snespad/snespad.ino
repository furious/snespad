/*
 * SNES Input Viewer & Gamepad
 * Developer: FURiOUS <furious.pro>
 * Version: 2.0
 * License: http://creativecommons.org/licenses/by-sa/3.0/legalcode
 * 
 * This firmware makes an arduino board intercept controller signals from a SNES console.
 */
 
#include <Gamepad.h>
#include <Keyboard.h>

/* Global Variables */
#define STATE_PIN 1 // PB1 - 15
#define CLOCK_PIN 3 // PB3 - 14
#define LATCH_PIN 2 // PB2 - 16

#define UP     4
#define DOWN   5
#define LEFT   6
#define RIGHT  7

/* Short functions for Gamepad */
#define KEYBOARD_HOTKEYS // Comment this to disable Keyboard hotkeys using controller
#define SNES_B      Gamepad.getButton(0)
#define SNES_Y      Gamepad.getButton(1)
#define SNES_SELECT Gamepad.getButton(2)
#define SNES_START  Gamepad.getButton(3)
#define SNES_A      Gamepad.getButton(4)
#define SNES_X      Gamepad.getButton(5)
#define SNES_L      Gamepad.getButton(6)
#define SNES_R      Gamepad.getButton(7)
#define SNES_UP     Gamepad.getYAxis() < 0
#define SNES_DOWN   Gamepad.getYAxis() > 0
#define SNES_LEFT   Gamepad.getXAxis() < 0
#define SNES_RIGHT  Gamepad.getXAxis() > 0

unsigned char controller[128];

/* Fast Read/Interrupt Functions */
#define PIN_READ(pin) (PINB & (1<<(pin)))
#define WAIT_FALLING_EDGE(pin) while(!PIN_READ(pin)); while(PIN_READ(pin));


/* Setting up everything */
void setup(){
  /* Enabling HID devices */
  Gamepad.begin(false);
  Keyboard.begin();
  
  /* Setting Pin Modes */
  pinMode(LATCH_PIN, INPUT);
  pinMode(CLOCK_PIN, INPUT);
  pinMode(STATE_PIN, INPUT);

  delay(500);
}

/* Loop and send button states to USB Gamepad */
void loop(){
  delay(10);
  
  /* Request controller button states */
  getControllerData();
  
  /* Process each button state */
  uint8_t pressed = 0, button = 0, btn = 0;
  int8_t Ypos = 0, Xpos = 0;
  do {
    pressed = controller[btn];
    switch(btn){
      case UP:    if(pressed) Ypos = -127; break;
      case DOWN:  if(pressed) Ypos =  127; break;
      case LEFT:  if(pressed) Xpos = -127; break;
      case RIGHT: if(pressed) Xpos =  127; break;
      default:    if(button < 8){ Gamepad.setButton(button, pressed); button++; } break;
    }
  } while(++btn < 16);

  /* Update USB Gamepad */
  Gamepad.setYAxis(Ypos);
  Gamepad.setXAxis(Xpos);
  Gamepad.sendState();

  /* Controller2Keyboard Hotkeys */
  #ifdef KEYBOARD_HOTKEYS
  if(SNES_L && SNES_R && SNES_START){ // L+R+Start -> CTRL+SHIFT+F11 (Used to start/split timer)
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(KEY_F11);
  } else if(SNES_L && SNES_R && SNES_SELECT){ // L+R+Select -> CTRL+SHIFT+F10 (Used to reset timer)
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(KEY_F10);
  } else { // Release everything
    Keyboard.releaseAll();
  }
  #endif
}

void getControllerData(){
  noInterrupts();
  unsigned char *data = controller;
  uint8_t bits = 16;

  WAIT_FALLING_EDGE(LATCH_PIN); // Wait console poll the controllers
  do {
    WAIT_FALLING_EDGE(CLOCK_PIN); // Wait console poll each button state
    *data = !PIN_READ(STATE_PIN);
    ++data;
  }
  while(--bits > 0);
  interrupts();
}
