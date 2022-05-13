// Interrupt handlers for buttons. If buttons aren'y already in a debouncing
// state, set keys to status 'debouncing' and set debounce_key* variables
// to current time.

void ISR_key1() 
{
    if(!key1Debouncing) {
        key1DebounceTimer = millis();
        key1Debouncing = true;
    }
}

void ISR_key2() 
{
    if(!key2Debouncing) {
        key2DebounceTimer = millis();
        key2Debouncing = true;
    }
}

void checkButtons() {

  //If either key is debouncing, read corresponding pin
    if(key1Debouncing || key2Debouncing) {
    
        key1Prev = key1Curr;
        key2Prev = key2Curr;
      
        if ((millis() - key1DebounceTimer) > debounceDelay) {
            key1Curr = digitalRead(pin2);
            key1Debouncing = false;
        }
        if ((millis() - key2DebounceTimer) > debounceDelay) {
            key2Curr = digitalRead(pin3);
            key2Debouncing = false;
        }
        // If either key is pressed, set news variable to act on in
        // subsequent main loop functions
        if (key1Curr == HIGH && key1Prev == LOW) {
            newsToActOn = TOGGLE;
            
        }
        if (key2Curr == HIGH && key2Prev == LOW) {
            newsToActOn = NEXT;
        }
    }
}


 
