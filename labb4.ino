typedef struct ledPin {
  uint8_t pin;
  struct ledPin *next;
} Pin;

Pin red, green, blue, *current;

const uint8_t potPin = A0;
const uint8_t pin8 = 8;
const uint8_t pin12 = 12;
const uint8_t debounceDelay = 100;

uint16_t debounce_key1 = 0;
uint16_t debounce_key2 = 0;
uint8_t contact_12;
uint8_t contact_8;

uint8_t state, key1_prev, key2_prev,
        key1_curr, key2_curr;

uint8_t r = 255;
uint8_t g = 0;
uint8_t b = 0;

uint16_t prevMillis = 0; 

bool rainbow_toggle, five_O_toggle;

void clear_lights() {
  digitalWrite(blue.pin, HIGH);
  digitalWrite(green.pin, HIGH);
  digitalWrite(red.pin, HIGH);
 }

void led_toggle() {

  digitalWrite(current->pin, HIGH);
  if(!current) 
      current = &red;
  else
      current = current->next;
   digitalWrite(current->pin, LOW); 
}

void five_O() {

    
    static uint8_t fade = 5;
    static uint8_t rDir = -1;
    static uint8_t brightness = 0;
    if(millis() - prevMillis > (map(analogRead(potPin), 0, 1024, 0, 10))) {
        
        prevMillis = millis();
      
        analogWrite(blue.pin, brightness);
        brightness += fade;

        if (brightness <= 0 || brightness >= 255)
          fade = -fade;
    }
}

void rainbow() {

    static uint8_t rVal = 254;
    static uint8_t gVal = 1;
    static uint8_t bVal = 127;
    
    static uint8_t rDir = -1;
    static uint8_t gDir = 1;
    static uint8_t bDir = -1;

   if(millis() - prevMillis > (map(analogRead(potPin), 0, 1024, 0, 10))) {
        
      prevMillis = millis();
      analogWrite(red.pin, rVal);
      analogWrite(green.pin, gVal);
      analogWrite(blue.pin, bVal);
    
      rVal = rVal + rDir;
      gVal = gVal + gDir;
      bVal = bVal + bDir;
    
      if (rVal >= 255 || rVal <= 0) {
        rDir = rDir * -1;
      }
    
      if (gVal >= 255 || gVal <= 0) {
        gDir = gDir * -1;
      }
    
      if (bVal >= 255 || bVal <= 0) {
        bDir = bDir * -1;
      }
   }
}
 
void setup () {
    Serial.begin(9600);
    
    red.pin = 11;
    green.pin = 10;
    blue.pin = 9;
    red.next = &green;
    green.next = &blue;
    blue.next = NULL;
    current = NULL;
  
    rainbow_toggle = false;
    five_O_toggle = false;
    
    pinMode(blue.pin, OUTPUT);
    pinMode(green.pin, OUTPUT);
    pinMode(red.pin, OUTPUT);
    pinMode(pin8, INPUT);
    pinMode(pin12, INPUT);
    clear_lights();
  
    key1_curr = digitalRead(pin8);
    key2_curr = digitalRead(pin12);

}
void rgb(int r, int g, int b) {
  analogWrite(red.pin, r);
  analogWrite(green.pin, g);
  analogWrite(blue.pin, b);
}

void switchStatus() {

    state = 0;
    
    key1_prev = key1_curr;
    key2_prev = key2_curr;
    if((millis() - debounce_key1) > debounceDelay) {
      key1_curr = digitalRead(pin8);
      debounce_key1 = millis();
    }
    if((millis() - debounce_key2) > debounceDelay) {
      key2_curr = digitalRead(pin12);
      debounce_key2 = millis();
      }
    

    if(key1_curr == HIGH && key1_prev == LOW) state += 1;
    if(key2_curr == HIGH && key2_prev == LOW) state += 2;

    switch(state) {
    case 1:
      if(five_O_toggle || rainbow_toggle) {
        clear_lights();
        five_O_toggle = false;
        rainbow_toggle = false;
      }
      led_toggle();  
      break;    
    case 2:
      if(rainbow_toggle) {
        rainbow_toggle = false;
        clear_lights();
        break;
      }
      if(five_O_toggle) {
        five_O_toggle = false;
        }
        current = NULL;
        rainbow_toggle = true;
        rainbow();
        break;
    case 3:
      clear_lights();
      if(five_O_toggle) {
        five_O_toggle = false;
        break;
      }
      if(rainbow_toggle) rainbow_toggle = false;
      current = NULL;
      five_O_toggle = true;
     
    default:
        if(rainbow_toggle) rainbow();
        if(five_O_toggle) five_O();
        break;
  
  }
}

void loop () {
   
    switchStatus();
   
}
