#include <Adafruit_GFX.h>
#include <gfxfont.h>
#include <Adafruit_NeoMatrix.h>
#include <gamma.h>
#include <Adafruit_DotStar.h>
#include <Adafruit_NeoPixel.h>

#define BUTTON_PIN   6    // Digital IO pin connected to the button.  This will be
                          // driven with a pull-up resistor so the switch should
                          // pull the pin to ground momentarily.  On a high -> low
                          // transition the button press logic will execute.
#define PIXEL_PIN    12    // Digital IO pin connected to the NeoPixels.
#define PIXEL_COUNT 90
// Parameter 1 = number of pixels in strip,  neopixel stick has 8
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

bool oldState = HIGH;
int showType = 0;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
//  strip.setbrightness(50);
}

void loop() {
  // Get current button state.
  bool newState = digitalRead(BUTTON_PIN);

  // Check if state changed from high to low (button press).
  if (newState == LOW && oldState == HIGH) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    newState = digitalRead(BUTTON_PIN);
    if (newState == LOW) {
      showType++;
      if (showType > 10)
        showType=0;
      startShow(showType);
    }
  }
  // Set the last button state to the old state.
  oldState = newState;
}

void startShow(int i) {
  switch(i){
    case 0: colorWipe(strip.Color(0, 0, 0), 50);    // Black/off
            break;
    case 1: colorWipe(strip.Color(0, 0, 100), 50);  // Blue
            break;
    case 2: colorWipe(strip.Color(0, 100, 0), 50);  // Green
            break;
    case 3: colorWipe(strip.Color(0, 100, 100), 50);  // Blue
            break;
    case 4: colorWipe(strip.Color(100, 0, 0), 50);  // Red
            break;
    case 5: colorWipe(strip.Color(100, 0, 100), 50);  // Purple
            break;
    case 6: colorWipe(strip.Color(100, 100, 0), 50);  // Yellow
            break;
    case 7: colorWipe(strip.Color(100, 100, 100), 50);  // White
            break;
    case 8: rainbow1(0);
            break;
    case 9: rainbow2(0);
            break;
  }
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
  }
}

//// Slightly different, this makes the rainbow equally distributed throughout
void rainbow1(uint8_t wait) {
  uint16_t i, j;
  uint16_t stop = 0;
  for(j=0; (j<256*100) && (stop==0); j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      strip.show();
      delay(wait);
    }
    // Get current button state.
    bool newState = digitalRead(BUTTON_PIN);
      
    // Check if state changed from high to low (button press).
    if (newState == LOW && oldState == HIGH) {
      stop=1;
      // Short delay to debounce button.
      delay(20);
      // Check if button is still low after debounce.
      newState = digitalRead(BUTTON_PIN);
  
      if (newState == LOW){
        stop=1;
      }
        
      if (newState == LOW && showType == 8) {
        showType=9;
        startShow(showType);
      }          
    }
      // Set the last button state to the old state.
    oldState = newState;
  }
}
    
void rainbow2(uint8_t wait) {
  uint16_t i, j;
  uint16_t stop = 0;
  for(j=0; (j<256*100) && (stop==0); j++) {
    for(i=0; i<strip.numPixels(); i++) {
      bool newState = digitalRead(BUTTON_PIN);
      strip.setPixelColor(i, Wheel((i+j) & 255));
      strip.show();
      delay(wait);
    }
    // Get current button state.
    bool newState = digitalRead(BUTTON_PIN);
    // Check if state changed from high to low (button press).
    if (newState == LOW && oldState == HIGH) {
      stop=1;
      // Short delay to debounce button.
      delay(20);
      // Check if button is still low after debounce.
      newState = digitalRead(BUTTON_PIN);

      if (newState == LOW){
        stop=1;
      }
      
      if (newState == LOW && showType == 9) {
        showType=0; 
        startShow(showType);
      }          
    }            
    // Set the last button state to the old state.
    oldState = newState;
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
