void rainbow() 
{

    //If function toggle is on, run the rainbow fade.
    if(currentState->funcToggle) {
        //Set starting values for each pin      
        static uint8_t _red = 1;
        static uint8_t _green = 127;
        static uint8_t _blue = 254;

        //Initiate incrementation or decrementation of each colors value
        static uint8_t redDirection = -1;
        static uint8_t greenDirection = 1;
        static uint8_t blueDirection = -1;

        now = millis();
        
        if(now - prevMillis > currentState->speed) {
          
            analogWrite(red.pin, _red);
            analogWrite(green.pin, _green);
            analogWrite(blue.pin, _blue);

            //Increase/decrease PWM value of each pin up to or down to 0-255
            _red += redDirection;
            _green += greenDirection;
            _blue += blueDirection;

            // Switch to increment or decrement of direction 
            // when value is out of PWM bounds.
            if (_red >= 255 || _red <= 0) {
                redDirection *= -1;
            }
          
            if (_green >= 255 || _green <= 0) {
                greenDirection *= -1;
            }
          
            if (_blue >= 255 || _blue <= 0) {
              b lueDirection *= -1;
            }
            prevMillis = now;
        }
    } else clearLights();
}

void clearLights() 
{
    digitalWrite(blue.pin, HIGH);
    digitalWrite(green.pin, HIGH);
    digitalWrite(red.pin, HIGH);
}

void ledToggle() 
{
  
    //Make sure current color is ON.
    digitalWrite(current->pin, LOW);
    
    //Change color if toggle button has been pressed
    //by setting current colors' pin to HIGH, and then 
    //setting the next pin in the list to LOW.
    if(currentState->funcToggle) {
        if(current) {
            digitalWrite(current->pin, HIGH);
            current = current->next;
        } else 
            current = &red;
        digitalWrite(current->pin, LOW); 
        currentState->funcToggle = false;
    }
}
// Blinks LED like a siren
void fiveO() 
  {
  //Only run function if it's toggled.
  if(currentState->funcToggle) {    

      //Set incremental value of fade variable
      static uint8_t fade = 5;
      static uint8_t brightness = 0;

      now = millis();
      //Run changes only if our waiting timer has reached it's limit
      if(now - prevMillis > currentState->speed) {
            
          prevMillis = now;
        
          analogWrite(blue.pin, brightness);
          brightness += fade;

          if (brightness <= 0 || brightness >= 255)
              fade = -fade;
        }
    } else 
          clearLights();
}
