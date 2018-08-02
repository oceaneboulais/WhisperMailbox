#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#define NUMLEDS 70
#define PIN 4
int sensorValue = 0;         
int sensorMin = 1023;        
int sensorMax = 0; 

const int sensorPin = A0;
const int buttonPin = 2;
const int ledPin =  4;
const int thresh = 700;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMLEDS, PIN, NEO_GRB + NEO_KHZ800);
void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);
  strip.begin();
  strip.show();
}

void loop() {

  if (!digitalRead(buttonPin)) {
    digitalWrite(ledPin, HIGH);
    Serial.println(analogRead(0));
    if (analogRead(0) > thresh) {
      Serial.println("shh");
      meteorRain(50, 255, 0, 10, 64, true, 20);
    }
  } else {
    // turn LED off:
    digitalWrite(ledPin, LOW);

  }
}
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
 
void TwinkleRandom(int Count, int SpeedDelay, boolean OnlyOne) {
  // Vertical Patern
  setAll(0, 0, 0);
  for (int i = 0; i < Count; i++) {
    setPixel(random(NUMLEDS), random(0, 255), random(0, 255), random(0, 255));
    strip.show();
    delay(SpeedDelay);
    if (OnlyOne) {
      setAll(0, 0, 0);
    }
  }
  delay(SpeedDelay);
}
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {
  setAll(0, 0, 0);

  for (int i = 0; i < NUMLEDS + NUMLEDS; i++) {
    // fade brightness all LEDs one step
    for (int j = 0; j < NUMLEDS; j++) {
      if ( (!meteorRandomDecay) || (random(10) > 5) ) {
        fadeToBlack(j, meteorTrailDecay );
      }
    }
    // draw meteor
    for (int j = 0; j < meteorSize; j++) {
      if ( ( i - j < NUMLEDS) && (i - j >= 0) ) {
        setPixel(i - j, red, green, blue);
      }
    }
    showStrip();
    delay(SpeedDelay);
  }
}

void fadeToBlack(int ledNo, byte fadeValue) {
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  uint32_t oldColor;
  uint8_t r, g, b;
  int value;
  oldColor = strip.getPixelColor(ledNo);
  r = (oldColor & 0x00ff0000UL) >> 16;
  g = (oldColor & 0x0000ff00UL) >> 8;
  b = (oldColor & 0x000000ffUL);
  r = (r <= 10) ? 0 : (int) r - (r * fadeValue / 256);
  g = (g <= 10) ? 0 : (int) g - (g * fadeValue / 256);
  b = (b <= 10) ? 0 : (int) b - (b * fadeValue / 256);
  strip.setPixelColor(ledNo, r, g, b);
#endif

}

void showStrip() {
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip.show();
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  FastLED.show();
#endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip.setPixelColor(Pixel, strip.Color(red, green, blue));
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  leds[Pixel].r = red;
  leds[Pixel].g = green;
  leds[Pixel].b = blue;
#endif
}

void setAll(byte red, byte green, byte blue) {
  for (int i = 0; i < NUMLEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

//void callibrate () {
//  while (millis() < 5000) {
//    sensorValue = analogRead(sensorPin);
//
//    // record the maximum sensor value
//    if (sensorValue > sensorMax) {
//      sensorMax = sensorValue;
//    }
//
//    // record the minimum sensor value
//    if (sensorValue < sensorMin) {
//      sensorMin = sensorValue;
//    }
//  }
//}

