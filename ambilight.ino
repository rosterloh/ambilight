#include "FastSPI_LED2.h"

#define NUM_LEDS 60

struct CRGB { byte g; byte r; byte b; };

struct CRGB leds[NUM_LEDS];

WS2811Controller800Mhz<4> LED;

// specified under `rate` in the `[device]` section of /etc/boblight.conf
#define serialRate 115200

// boblightd sends a prefix (defined in /etc/boblight.conf) before sending the pixel data
uint8_t prefix[] = {0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA};

void setup()
{
  LED.init();
  
  // Start with all LEDs off
  memset(leds, 0, NUM_LEDS * 3);
  LED.showRGB((byte*)leds, NUM_LEDS);;
  
  Serial.begin(serialRate);
}

void loop() {
  // wait until we see the prefix
  for(int i = 0; i < sizeof prefix; ++i) {
    waitLoop: while (!Serial.available()) ;;
    // look for the next byte in the sequence if we see the one we want
    if(prefix[i] == Serial.read()) continue;
    // otherwise, start over
    i = 0;
    goto waitLoop;
  }
  memset(leds, 0, NUM_LEDS * 3);
  // read the transmitted data
  for (int i = 0; i < NUM_LEDS; i++) {
    byte r, g, b;    
    while(!Serial.available());
    r = Serial.read();
    while(!Serial.available());
    g = Serial.read();
    while(!Serial.available());
    b = Serial.read();
    // For power reasons I will currently discard half the values
    if(i % 2) {
      leds[i].r = r;
      leds[i].g = g;
      leds[i].b = b;
    }
  }
  LED.showRGB((byte*)leds, NUM_LEDS);;
}

