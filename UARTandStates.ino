void checkSerial() {
  // Code from https://forum.arduino.cc/t/serial-input-basics-updated/382007/2
  // If data available in serial, read until new line, store data in receivedChars,
  // and set flag newData.
  
  static uint8_t ndx = 0;
  char endMarker = '\n';
  char rc;
  
  if (Serial.available() > 0) {
    rc = Serial.read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) 
        ndx = numChars - 1;
    }
    else {
      receivedChars[ndx] = '\0'; 
      ndx = 0;
      newData = true;
    }
  }
}

void checkCommands() {
  // If data has been received through serial, parse data for commands and 
  // set variable newsToActOn to corresponding value, to be acted upon by 
  // stateSetter(); and set newData to false since command has been acted upon.

  if(newData) {
    if(strcmp(receivedChars, "prev") == 0)          newsToActOn = PREV;
    else if(strcmp(receivedChars, "next") == 0)     newsToActOn = NEXT;
    else if(strcmp(receivedChars, "toggle") == 0)   newsToActOn = TOGGLE;
    else if(strcmp(receivedChars, "+") == 0)        newsToActOn = SPEEDUP;
    else if(strcmp(receivedChars, "-") == 0)        newsToActOn = SPEEDDOWN;
    else if(strcmp(receivedChars, "current") == 0)  newsToActOn = CURRENT;
    else if(strcmp(receivedChars, "menu") == 0)     newsToActOn = MENU;
    else Serial.println("No such option.");
    newData = false;
  }
}

void printToSerial() {
  //Send serial messages about taken actions. 
  switch(newsToActOn) {
  case PREV:
  case NEXT:
    Serial.println("Switched to \"" + currentState->funcDesc + "\"."); 
    break;
  case TOGGLE:
    if(currentState == &singleColor) {
      Serial.print("Toggled LED color ");
      switch(current->pin) {
        case 9:  Serial.println("off.");      break;
        case 10: Serial.println("to blue.");  break;
        case 11: Serial.println("to green."); break;
        default: Serial.println("to red.");   break;
      }
    } else Serial.println("Toggled \"" + currentState->funcDesc + 
                         (currentState->funcToggle ? "\" on." : "\" off."));
                         break;
  case CURRENT:
    Serial.println("Currently in \"" +currentState->funcDesc +"\".");
    break;
  case SPEEDUP:
   if(currentState->speed > 0)
      Serial.println("Increasing speed.");
    else
      Serial.println("Can't Increase further.");
    break;
  case SPEEDDOWN:
   if(currentState->speed < 10)
      Serial.println("Decreasing speed.");
    else 
      Serial.println("Can't decrease further.");
    break;
    
  }
}

void stateSetter() {
  // Query if there's any news (either serial communication commands or
  // button presses) to act upon, and act accordingly if there is, then
  // set news variable to 0 so we don't act again until there's new news.
  if(newsToActOn) {
    switch(newsToActOn) {
    case PREV:
      clearLights();
      currentState = currentState->prevState;
      break;
    case NEXT:
      clearLights();
      currentState = currentState->nextState;
      break;
    case SPEEDUP:
      if(trimpotSerialCurrent >= 1)
        trimpotSerialCurrent--;
        break;
    case SPEEDDOWN:
      if(trimpotSerialCurrent <= 10)
        trimpotSerialCurrent++; 
        break;   
    case TOGGLE:
      currentState->funcToggle = !currentState->funcToggle;
      break;
    case MENU:
      menu();
      break;
    }
  printToSerial();
  newsToActOn = 0;
  }   
}

void stateTimer() {
  //Sets the speed of current states' fade or blink. 
  //Only updates if there's been a change in either the
  //value read from the trim pot or from serial command
  if(currentState != &singleColor) {
    trimpotCurrent = map(analogRead(potPin), 0, 1024, 0, 10);
    if(trimpotCurrent != trimpotPrevious) {
       trimpotPrevious = trimpotCurrent;
       currentState->speed = trimpotCurrent;
       return;
    }
    else if(trimpotSerialPrev != trimpotSerialCurrent) {
      trimpotSerialPrev = trimpotSerialCurrent;
      currentState->speed = trimpotSerialCurrent;
    }
  }
}
