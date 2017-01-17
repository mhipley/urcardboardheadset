#include <Automaton.h>

/**
  By Martha Hipley
  Pressure Sensor Detection Code
  For Sensual Interactions
*/

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

//initialize variables
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(7, PIN, NEO_GRB + NEO_KHZ800);
int sensorPin = 0;
int sensorValue = 0;
int motor = 3;
int peltier = 9;

void setup() {
  pinMode(motor, OUTPUT) ;
  pinMode(peltier, OUTPUT);
  Serial.begin(9600);
  strip.begin();
  strip.show();
}

int sensorScaled = 0;
int ledScaled = 0;
int brightnessScaled = 0;
int pulseScaled = 0;
long heartBeatArray[] = {
    80, 80, 40, 600};
int hbeatIndex = 1;   
long prevMillis;
int heat = 0;

void loop() {
  // read the value from the sensor
  sensorValue = analogRead(sensorPin);

  //scale the sensor variable for the various outputs
  sensorScaled = sensorValue / 1000.0 * 255.0;
  float brightness = sensorScaled;
  brightness = map(sensorScaled, 0, 1000, 0, 255);
  heat = sensorValue / 1000.0 * 255;
  ledScaled = sensorValue / 1000.0 * 7;
  brightnessScaled = sensorValue / 1000.0 * 255;
  float pulseScaled = ((sensorValue / 900.00) * 10);
  float pulse = 4 - ((pulseScaled * 3) /10);
  float hue = -(1200 - sensorValue) * 1.5 / 2400;
  int rgb[3];
 
  // convert color
  hslToRgb(hue, 1.0, 0.5, rgb);
  int r = rgb[0];
  r = map(r, 0, 1000, 0, brightness);
  int g = rgb[1];
  g = map(g, 0, 1000, 0, brightness);
  int b = rgb[2];
  b = map(b, 0, 1000, 0, brightness);

  //run buzzer
  heartBeat(pulse, sensorScaled);

  //run peltier
  analogWrite(peltier, heat);

  //run LED
  colorWipe(strip.Color(r, g, b), brightnessScaled, ledScaled);

}

//heartbeat
void heartBeat(float tempo, int intensity){
    if ((millis() - prevMillis) > (long)(heartBeatArray[hbeatIndex] * tempo)){
        hbeatIndex++;
        if (hbeatIndex > 3) hbeatIndex = 0;

        if ((hbeatIndex % 2) == 0){     // modulo 2 operator will be true on even counts
            digitalWrite(motor, intensity);
        }
        else{ 
            digitalWrite(motor, 0); 
        }
        //  Serial.println(hbeatIndex);
        prevMillis = millis();
    }
}

// colorwipe LEDs
void colorWipe(uint32_t c, uint8_t bright, uint8_t num) {
  for (uint16_t i = 0; i <= num ; i++) {
    strip.setPixelColor(i, c);
  }
  strip.setBrightness(bright);
  strip.show();
}

//scale color spectrum
void hslToRgb(float h, int s, float l, int* rgb) {
  float r, g, b;
  if (s == 0.0) {
    r = g = b = l; // achromatic
  } else {
    float q = l < 0.5 ? l * (1.0 + s) : l + s - l * s;
    float p = 2.0 * l - q;
    r = hue2rgb(p, q, h + 1.0 / 3.0);
    g = hue2rgb(p, q, h);
    b = hue2rgb(p, q, h - 1.0 / 3.0);
  }
  rgb[0] = (float)r * 255.0;
  rgb[1] = (float)g * 255.0;
  rgb[2] = (float)b * 255.0;
}

float hue2rgb(float p, float q, float t) {
  if (t < 0.0) t += 1.0;
  if (t > 1.0) t -= 1;
  if (t < 1.0 / 6.0) return p + (q - p) * 6.0 * t;
  if (t < 1.0 / 2.0) return q;
  if (t < 2.0 / 3.0) return p + (q - p) * (2.0 / 3.0 - t) * 6.0;
  return p;
}

