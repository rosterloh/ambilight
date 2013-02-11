#include "Adafruit_FloraPixel.h"

// Output pin is hardcoded to Digital 4 on arduino or DIO P1 on Jeenode

// specified under `rate` in the `[device]` section of /etc/boblight.conf
#define serialRate 115200

// boblightd sends a prefix (defined in /etc/boblight.conf) before sending the pixel data
uint8_t prefix[] = {0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA};

// Set the first variable to the NUMBER of pixels.
Adafruit_FloraPixel strip = Adafruit_FloraPixel(60);

void setup() {
    
  strip.begin();

  // Update the strip, to start they are all 'off'
  strip.show();
  
  Serial.begin(serialRate);
}

void loop() {
  // wait until we see the prefix
  for(byte i = 0; i < sizeof prefix; ++i) {
    waitLoop: while (!Serial.available()) ;;
    // look for the next byte in the sequence if we see the one we want
    if(prefix[i] == Serial.read()) continue;
    // otherwise, start over
    i = 0;
    goto waitLoop;
  }
  // read the transmitted data
  for (uint8_t i = 0; i < strip.numPixels(); i++) {
    byte r, g, b;    
    while(!Serial.available());
    r = Serial.read();
    while(!Serial.available());
    g = Serial.read();
    while(!Serial.available());
    b = Serial.read();
    // Only do 10 because of power constraints
    if(i % 6 == 0)
      strip.setPixelColor(i, r, g, b);
  }
  strip.show();
  delay(1);
}
