#include <FastLED.h>

#define LED_PIN2     7
#define COLOR_ORDER GRB
#define CHIPSET     WS2811
#define NUM_LEDS    48
#define SCALE 5
//#define NUM_LEDS_REAL    144
#define NUM_LEDS_REAL    NUM_LEDS*SCALE
//#define NUM_LEDS    10

#define BRIGHTNESS  255
#define FRAMES_PER_SECOND 12
CRGB leds[NUM_LEDS];
CRGB ledsReal[NUM_LEDS_REAL];
CRGBPalette16 currentPalette;
void setup() {
  
  Serial.begin(9600);
  delay(3000); // sanity delay
  randomSeed(analogRead(A1));
  //FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  //FastLED.addLeds<CHIPSET, LED_PIN2, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.addLeds<CHIPSET, LED_PIN2, COLOR_ORDER>(ledsReal, NUM_LEDS_REAL).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );

currentPalette = CRGBPalette16(
                                   CRGB::Black, CRGB::Black, CRGB::Black, CHSV(0, 255,4),
                                   CHSV(0, 255, 8), CRGB::Red, CRGB::Red, CRGB::Red,                                   
                                   CRGB::DarkOrange,CRGB::Orange, CRGB::Orange, CRGB::Orange,
                                   CRGB::Yellow, CRGB::Yellow, CRGB::Gray, CRGB::Gray);

}


void SHOW(){
  int k = SCALE;
  for (int i = 0 ; i < NUM_LEDS; i++){
    for (int o = 0 ; o < k; o++){
      ledsReal[i*k+o] = leds[i];
      
    }
  }
  FastLED.show();
}

void reset(){
 for( int j = 0; j < NUM_LEDS; j++) {
   leds[j].r = 0 ;
   leds[j].b = 0 ;
   leds[j].g = 0 ;
   
 }
  //FastLED.show();
  SHOW();
}

uint8_t thisdelay = 5;
void loop() {
  int mode = random(0,5);
  //mode = 2;
  Serial.print("Mode:");
  Serial.println(mode);
  if (mode == 1 || mode == 0  ){
  while(timeTotalSec()<20){
    doTime();
    rainbow_beat();
    //FastLED.show();    
    SHOW();
  }
  timeReset();
  return ;
}

if (mode == 2 || mode == 3){
  while(timeTotalSec()<20){
    doTime();
    EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking routine to update/display the sequence.
      rainbow_march();
    }
    SHOW();
    //FastLED.show();    
  }
  timeReset();
  return ;
}

if (mode == 4 ){
  reset();
  FastLED.setBrightness( BRIGHTNESS);
  while(timeTotalSec()<70){
    doTime();
    EVERY_N_MILLISECONDS(50) {
      coals();
    }
    if (BRIGHTNESS  - (timeTotalSec()*2)<10)
      break ;
    FastLED.setBrightness( BRIGHTNESS  - (timeTotalSec()*2));
    //FastLED.show();    
    SHOW();
  }
  FastLED.setBrightness( BRIGHTNESS);
  timeReset();
  return ;
} 

  /*if (mode == 3 ){
  while(timeTotalSec()<20){
    doTime();
    dot_beat();
    FastLED.show();    
  }
  timeReset();
  return ;
  }*/

}

int ledShift [NUM_LEDS];

void coals (){
    for( int j = 0; j < NUM_LEDS; j++) {
      if (leds[j].r >150){
        ledShift[j] = random(-3,-1);
      }
      if (leds[j].r <4){
        ledShift[j] = random(1,3);
        //ledShift[j] = random(-3,3);
      }

       if (ledShift[j] == 0 ){
        ledShift[j] = random(-3,3);
        leds[j].r = 128;
        leds[j].b = 25;
        if (ledShift[j] == 0 )
          ledShift[j] = 1 ;
       }

       leds[j].r += ledShift[j];
      
      
    }
}

void rainbow_beat() {
  
  uint8_t beatA = beatsin8(17, 0, 255);                        // Starting hue
  uint8_t beatB = beatsin8(13, 0, 255);
  fill_rainbow(leds, NUM_LEDS, (beatA+beatB)/2, 8);            // Use FastLED's fill_rainbow routine.

} // rainbow_beat()

uint8_t thishue = 0;                                          // Starting hue value.
uint8_t deltahue = 10;  // Hue change between pixels.

void rainbow_march() {                                        // The fill_rainbow call doesn't support brightness levels
  thishue++;                                                  // Increment the starting hue.
  fill_rainbow(leds, NUM_LEDS, thishue, deltahue);            // Use FastLED's fill_rainbow routine.
} // rainbow_march()

unsigned long time;
unsigned long timeLast=0;
unsigned long timeTotal=0;

void doTime (){
  time = millis();
  if (timeLast==0) timeLast = time;
  timeTotal+= time- timeLast;
  timeLast = time;
}
void timeReset (){
  timeTotal=0;
}
int timeTotalSec (){
  return timeTotal/1000;
}



uint8_t bpm = 30;
uint8_t fadeval = 224; 
void dot_beat() {

  uint8_t inner = beatsin8(bpm, NUM_LEDS/4, NUM_LEDS/4*3);    // Move 1/4 to 3/4
  uint8_t outer = beatsin8(bpm, 0, NUM_LEDS-1);               // Move entire length
  uint8_t middle = beatsin8(bpm, NUM_LEDS/3, NUM_LEDS/3*2);   // Move 1/3 to 2/3

  leds[middle] = CRGB::Purple;
  leds[inner] = CRGB::Blue;
  leds[outer] = CRGB::Aqua;

  nscale8(leds,NUM_LEDS,fadeval);                             // Fade the entire array. Or for just a few LED's, use  nscale8(&leds[2], 5, fadeval);

} // dot_beat()
