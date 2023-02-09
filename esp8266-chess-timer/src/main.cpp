#include <Arduino.h>

enum STATE { IDLE, RESET, T_BLUE, T_RED };

#include <Arduino.h>
#include <TM1637Display.h> //https://github.com/avishorp/TM1637

#define DEFAULT_TIMER 900

const int CLK = 3; //Set the CLK pin connection to the display
const int DIO = 2; //Set the DIO pin connection to the display

const int CLK2 = 10;
const int DIO2 = 16;

int numCounter = 0;

enum CLOCK_STATE{
  SETUP=0,
  STANDBY,
  SKINS_ACTIVE,
  BLUES_ACTIVE,
  SKINS_WON,
  BLUES_WON,
  PAUSE
};

CLOCK_STATE state;

TM1637Display display(CLK, DIO); //set up the 4-Digit Display.
TM1637Display display2(CLK2, DIO2); //set up the 4-Digit Display.

uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };

const uint8_t lol[] = {
  SEG_F | SEG_E | SEG_D,            // L
  SEG_C | SEG_D | SEG_E | SEG_G,    // o
  SEG_F | SEG_E | SEG_D,            // L
  0            // none
};

bool oddTick;
bool clockTick;
uint16_t skinsTimer;
uint16_t bluesTimer;

void setup(void) {
  Serial.begin(115200);
  
  
  display.setBrightness(0x0f);
  display2.setBrightness(0x0f);

  // All segments off
  display.setSegments(blank);
  display2.setSegments(blank);

  Serial.println("Display connected");
  oddTick = false;

  // put your setup code here, to run once:
  state = SETUP;
  // setup timers
  skinsTimer = DEFAULT_TIMER;
  bluesTimer = DEFAULT_TIMER;
  // set up buttons



}

void loop() {
  // put your main code here, to run repeatedly:
  int32_t mins=0, secs=0;
  switch (state)
  {
    case SETUP:
    {
        /* code */
        display.setSegments(lol);
        display2.showNumberDec(15,false,2,1);
        break;
    }
    case STANDBY:
    {
        mins = (int)DEFAULT_TIMER/60;
        secs = DEFAULT_TIMER % 60;
        display.showNumberDecEx((mins*100+secs), 0b11100000, true, 4, 0);
        display2.showNumberDecEx((mins*100+secs), 0b11100000, true, 4, 0);
        break;
    }
    case SKINS_ACTIVE:
    {
        if(clockTick){
          if ( skinsTimer > 0 ) {
            skinsTimer--;
          } else {
            state = SKINS_WON;
          }
          mins = (int)skinsTimer/60;
          secs = skinsTimer % 60;
          display.showNumberDecEx((mins*100+secs), (oddTick ? 0b11100000 : 0), true, 4, 0);   
          clockTick=false;
        }
        
        break;
    }
    case BLUES_ACTIVE:
    {
        if(clockTick){
          if ( bluesTimer > 0 ) {
            bluesTimer--;
          } else {
            state = BLUES_WON;
          }
          mins = (int)bluesTimer/60;
          secs = bluesTimer % 60;
          display2.showNumberDecEx((mins*100+secs), (oddTick ? 0b11100000 : 0), true, 4, 0);   
          clockTick=false;
        }
        
        break;
    }    
    case BLUES_WON:
    {
        break;
    }
    case SKINS_WON:
    {
        break;
    }
    case PAUSE:
    default:
      break;
  }
  
  // for(numCounter = 0; numCounter < 1000; numCounter++) //Iterate numCounter
  // {
  //   display.showNumberDecEx(numCounter, (oddTick ? 0b11100000 : 0), true, 4, 0);
    
  //   display2.showNumberDecEx(1000-numCounter, (oddTick ? 0b11100000 : 0), true, 4, 0);
  //   Serial.println("Im alive!");
    
  //   oddTick = !oddTick;
  //   delay(1000);
  // }
  if(numCounter > 5){
    numCounter -=5;
  } else {
    numCounter = 1000;
    clockTick=true;
    oddTick = !oddTick;
  }
  delay(5);
}


