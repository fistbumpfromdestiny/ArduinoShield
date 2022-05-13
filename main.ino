#include "main.h"

void setup () {

  Serial.begin(9600);

  // Setup of LED structs
  red =   { 11, &green, "red" };
  green = { 10, &blue, "green" };
  blue =  { 9, NULL, "blue" };
  current = NULL;

  pinMode(blue.pin, OUTPUT);
  pinMode(green.pin, OUTPUT);
  pinMode(red.pin, OUTPUT);
  pinMode(pin2, INPUT);
  pinMode(pin3, INPUT);

  attachInterrupt(digitalPinToInterrupt(pin2), ISR_key1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pin3), ISR_key2, CHANGE);
  
  key1Curr = digitalRead(pin2);
  key2Curr = digitalRead(pin3);

  // State double linked list setup
  singleColor = {ledToggle, "Single colour LED", &rainbowFade, &fiveOh, false, 0};
  rainbowFade = {rainbow, "Rainbow with fade", &fiveOh, &singleColor, true, 5};
  fiveOh =      {fiveO, "Blue sirens", &singleColor, &rainbowFade, true, 5};

  // Starting state
  currentState = &singleColor;

  // Set pin 9 to 11 to HIGH.
  clearLights();
  // Display menu in terminal
  menu();

}
 
void loop () {
  //Check if and for serial communication
  checkSerial();
  //If there is serial communication, act on it
  checkCommands();
  //Handle button presses, if there's been any
  checkButtons();
  //Check for updates on states' waiting timer
  stateTimer();
  //Act on new serial commands or button presses 
  //and set a new state, if need be
  stateSetter();
  // Execute current state
  currentState->func();

}
