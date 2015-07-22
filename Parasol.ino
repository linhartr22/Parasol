// Adafruit NeoPixel library.
#include <Adafruit_NeoPixel.h>

// NeoPixel Data Output Pin.
#define STRIP1_PIN 12
// Number of NeoPixels
#define STRIP1_PIX_MAX 15

// Init NeoPixel strip objects.
// Parameter 1 = number of pixels in strip.
// Parameter 2 = Arduino pin number (most are valid).
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs).
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers).
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products).
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2).
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(STRIP1_PIX_MAX, STRIP1_PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk:
//            Add a 1000 uF capacitor across pixel power leads.
//            Add a 300 - 500 Ohm resistor on first pixel's data input pin
//            and minimize distance between Arduino and first pixel.
//            Avoid connecting on a live circuit...if you must, connect GND first.

// Spoke Number (0-9).
byte spokeNum = 0;
// Spoke Direction (1, -1).
byte spokeDir = 1;
// Change Dir Timer.
unsigned long spokeDirTmr = millis();

// setup() runs once at reset.
void setup() {
  // Init Strips.
  strip1.begin();
  // Refresh Strips.
  strip1.show();
}

// loop() runs continuously after setup().
void loop() {
  // Animation Timer.
  unsigned long timerEnd;
  // Loop through examples.
  for(int exNum = 0 ; exNum < 5 ; exNum++) {
    // Run 5 - 10 seconds.
    timerEnd = millis() + 10000 + random(5000);
    // Clear Strip.
    strip1.clear();
    strip1.show();
    // Wait for timer to run out.
    while(millis() < timerEnd) {
      switch(exNum) {
        case 0:
          // Flash Random Pixel. Brightness 128 - 255, Speed 1 - 4 millis(), Interval 10 - 50 millis().
          strip1.setBrightness(128 + random(128));
          flashRandom(1 + random(3));
          delay(10 + random(40));
          break;
        case 1:
          // Rainbow Cycle. Brightness 75%. Speed 50 millis().
          strip1.setBrightness(0x3f);
          rainbowCycle(50);
          break;
        case 2:
          // Spokes.
          strip1.setBrightness(0x3f);
          // Change Direction?
          if(random(100)<4) {
            // Yes.
            spokeDir *= -1;
          }
          // Next Spoke.
          spokeNum+= spokeDir;
          if(spokeNum == 255) {
            spokeNum = 9;
          }
          if(spokeNum == 10) {
            spokeNum = 0;
          }
          showSpoke(spokeNum, 0x7f);
          delay(40);            
          break;
        case 3:
          // Rainbow Wipe. Brightness 75%. Speed 50 millis().
          strip1.setBrightness(0x3f);
          rainbowWipe(50);
          break;
        case 4:
          // Rainbow Rings. Brightness 75%. Speed 50 millis().
          strip1.setBrightness(0x3f);
          rainbowRings(50);
          break;
      }
    }
    // Clear Strip.
    strip1.clear();
    strip1.show();
    // Wait 2 - 4 seconds.
    delay(2000 + random(2000));
  }
}

// Flash Random
// Flash Colors [red, green, blu]
uint8_t flashColors[][3] = {
  {0xe8, 0x64, 0xff},   // purple.
  {0xc8, 0xc8, 0x14},   // yellow.
  {0x1e, 0xc8, 0xc8},   // blue.
};
#define FLASH_COLORS sizeof(flashColors) / 3
void flashRandom(int flashSpeed) {
  // Pick a random flash color!
  int c1 = random(FLASH_COLORS);
  int red1 = flashColors[c1][0];
  int grn1 = flashColors[c1][1];
  int blu1 = flashColors[c1][2]; 
  // Get a random pixel from the list.
  int p1 = random(strip1.numPixels());
  
  // Fade in 5 steps.
  for (int f = 1; f <= 5; f++) {
    int r1 = red1 * f; r1/= 5;
    int g1 = grn1 * f; g1/= 5;
    int b1 = blu1 * f; b1/= 5;
    strip1.setPixelColor(p1, strip1.Color(r1, g1, b1));
    strip1.show();
    delay(flashSpeed);
  }
  // Fade out 6 steps (Fades to zero).
  for (int f = 5; f >= 0; f--) {
    int r1 = red1 * f; r1/= 5;
    int g1 = grn1 * f; g1/= 5;
    int b1 = blu1 * f; b1/= 5;
    strip1.setPixelColor(p1, strip1.Color(r1, g1, b1));
    strip1.show();
    delay(flashSpeed);
  }
}

// Rainbow Cycle.
void rainbowCycle(unsigned long dly) {
  // Rainbow Color Offset.
  for(int rainbow = 0; rainbow < 256; rainbow+= 4) {
    // Set Pixels Color.
    for(int p1 = 0; p1 < strip1.numPixels(); p1++) {
      // Pixel Color = Rainbow + Pixel Offsets.
      strip1.setPixelColor(p1, wheelStrip1(rainbow));
    }
    // Refresh Strips.
    strip1.show();
    delay(dly);
  }
}

// Rainbow Wipe.
void rainbowWipe(unsigned long dly) {
  // Rainbow Color Offset.
  for(int rainbow = 0; rainbow < 256; rainbow+= 8) {
    // Set Pixels Color.
    for(int p1 = 0; p1 < strip1.numPixels(); p1++) {
      // Pixel Color = Rainbow + Pixel Offsets.
      strip1.setPixelColor(p1, wheelStrip1((rainbow + p1 * 8) & 0xff));
    }
    // Refresh Strips.
    strip1.show();
    delay(dly);
  }
}

// Show Spoke.
void showSpoke(byte spokeNum, byte spokeCol) {
  if(spokeNum >= 0 && spokeNum <= 9) {
    strip1.clear();
    strip1.setPixelColor(spokeNum, wheelStrip1(spokeCol));
    if(spokeNum & 0x01) {
      strip1.setPixelColor(14 - int(spokeNum / 2), wheelStrip1(spokeCol));
    }
    strip1.show();
  }
}

// Rainbow Rings.
void rainbowRings(unsigned long dly) {
  // Rainbow Color Offset.
  for(int rainbow = 0; rainbow < 256; rainbow+= 8) {
    // Set Outer Ring Pixels Color.
    for(int p1 = 0; p1 < 10; p1++) {
      // Pixel Color = Rainbow + Pixel Offsets.
      strip1.setPixelColor(p1, wheelStrip1(rainbow));
    }
    strip1.show();
    delay(dly);
    // Set Inner Ring Pixels Color.
    for(int p1 = 10; p1 < strip1.numPixels(); p1++) {
      strip1.setPixelColor(p1, wheelStrip1((rainbow - 8) & 0xff));
    }  
    // Refresh Strips.
    strip1.show();
    delay(dly);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t wheelStrip1(byte wheelPos) {
  wheelPos = 255 - wheelPos;
  if(wheelPos < 85) {
    return strip1.Color(255 - wheelPos * 3, 0, wheelPos * 3);
  }
  else if(wheelPos < 170) {
    wheelPos -= 85;
    return strip1.Color(0, wheelPos * 3, 255 - wheelPos * 3);
  }
  else {
    wheelPos -= 170;
    return strip1.Color(wheelPos * 3, 255 - wheelPos * 3, 0);
  }
}


