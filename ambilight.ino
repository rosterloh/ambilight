#include <FastSPI_LED2.h>

#define NUM_LEDS 91
#define DATA_PIN 4
// If its's too bright with all the LEDS you can skip every nth led
#define EVERY_NTH 1

CRGB leds[NUM_LEDS];

// specified under `rate` in the `[device]` section of /etc/boblight.conf
#define serialRate 115200

// boblightd sends a prefix (defined in /etc/boblight.conf) before sending the pixel data
uint8_t prefix[] = {0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA};

void setup()
{
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);
   
  LEDS.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS);
  
  // Start with all LEDs off
  memset(leds, 0, NUM_LEDS * sizeof(struct CRGB));
  LEDS.show();
  
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
  memset(leds, 0,  NUM_LEDS * sizeof(struct CRGB));
  // read the transmitted data
  for (int iLed = 0; iLed < NUM_LEDS; iLed++) {
    byte r, g, b;    
    while(!Serial.available());
    r = Serial.read();
    while(!Serial.available());
    g = Serial.read();
    while(!Serial.available());
    b = Serial.read();
    if(!(iLed % EVERY_NTH)) {
      leds[iLed].r = r;
      leds[iLed].g = g;
      leds[iLed].b = b;
    }
  }
  LEDS.show();
}

