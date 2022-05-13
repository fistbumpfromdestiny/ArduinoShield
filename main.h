  // Variables for pins
typedef struct ledPin {
  uint8_t pin;
  struct ledPin *next;
  String name;
} LED;

LED red, green, blue, *current;

const uint8_t potPin = A0;
const uint8_t pin2 = 2;
const uint8_t pin3 = 3;

// State and function variables 
enum buttonValues{PREV = 1, NEXT, TOGGLE, CURRENT, MENU, SPEEDUP, SPEEDDOWN};

typedef void (*stateFunc)(void);

uint8_t trimpotCurrent = 1, trimpotPrevious = 0,
        trimpotSerialCurrent = 5, trimpotSerialPrev = 0;

typedef struct States {
  stateFunc func;
  String funcDesc;
  struct States *nextState, *prevState;
  bool funcToggle;
  uint8_t speed;
} State;

State singleColor, rainbowFade, fiveOh, *currentState;

uint8_t newsToActOn = 0;

// Timer variables
uint16_t prevMillis = 0;
uint16_t now;

// Buttons and button pressing variables
uint8_t key1Prev, key2Prev,
        key1Curr, key2Curr;

const uint8_t debounceDelay = 10;        

bool volatile key1Debouncing = false;
bool volatile key2Debouncing = false;

uint16_t volatile key1DebounceTimer = 0;
uint16_t volatile key2DebounceTimer = 0;

// Serial communication variables
const byte numChars = 32;
char receivedChars[numChars]; 
boolean newData = false;
