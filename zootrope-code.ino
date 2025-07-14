#include <FastLED.h>
const int pinCLK    = 3;   // Encoder CLK
const int pinDT     = 5;   // Encoder DT
const int pinSW     = 6;   // Encoder push button
// const int pinSwitch = A0;   // Mode toggle button

// Motor settings
const int motorMinValue = 0;
const int motorMaxValue = 255;
const int motorStepValue = 1;

// Blink settings
const int blinkMinValue = 0;
const int blinkMaxValue = 5000;
const int blinkStepValue = 100;

int motorValue = 20;
int blinkValue = 2700;
int currentValue = 0;
bool settingMotor = true;  // true = motor, false = blink

int lastCLK = HIGH;

// Debounce for buttons and rotary
unsigned long lastDebounceSW = 0;
unsigned long lastDebounceSwitch = 0;
unsigned long lastEncoderMove = 0;
const unsigned long debounceDelay = 50;
const unsigned long encoderDebounce = 3; // ms between encoder changes
bool lastSWState = HIGH;
bool lastSwitchState = HIGH;

#define NUM_LEDS 40
#define LED_PIN 10
int motorPin = 9; // The pin connected to the transistor base
int speed = 200; // A value in the range [0..255] which represents the
int counter = 0;
int lastStateCLK = HIGH;
int currentStateCLK;
int strobeHz = 0;

const int rpm = 255;
const uint32_t rpm_us = 60000000 / rpm;

int tourminute = 0;

void setup() {
  delay(2000); // Wait 1 second for everything to stabilize
  pinMode(pinCLK, INPUT);
  pinMode(pinDT, INPUT);
  pinMode(pinSW, INPUT_PULLUP);
  pinMode(A0, INPUT_PULLUP);
  Serial.begin(9600);
  currentValue = motorValue;
  Serial.println("Mode: Motor value adjustment");
  Serial.print("Current value: ");
  Serial.println(currentValue);
  
  //motor
  pinMode(motorPin, OUTPUT);

  //led
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(0, NUM_LEDS);
  FastLED.setMaxRefreshRate(0, false);
}

void loop() {
  // Rotary encoder logic with debounce - FIXED DIRECTION
  int newCLK = digitalRead(pinCLK);
  if (lastCLK == HIGH && newCLK == LOW) { // Falling edge
    if (millis() - lastEncoderMove > encoderDebounce) {
      int dtValue = digitalRead(pinDT);
      
      // Get current min/max/step values based on mode
      int minVal, maxVal, stepVal;
      if (settingMotor) {
        minVal = motorMinValue;
        maxVal = motorMaxValue;
        stepVal = motorStepValue;
      } else {
        minVal = blinkMinValue;
        maxVal = blinkMaxValue;
        stepVal = blinkStepValue;
      }
      
      // Fixed direction logic - clockwise always increases, counterclockwise always decreases
      if (dtValue == HIGH) {
        // Clockwise - increase value (don't wrap around at max)
        if (currentValue < maxVal) {
          currentValue += stepVal;
          if (currentValue > maxVal) currentValue = maxVal;
        }
      } else {
        // Counterclockwise - decrease value (don't wrap around at min)
        if (currentValue > minVal) {
          currentValue -= stepVal;
          if (currentValue < minVal) currentValue = minVal;
        }
      }
      
      Serial.print("Current value: ");
      Serial.println(currentValue);
      lastEncoderMove = millis();
    }
  }
  lastCLK = newCLK;

  // Rotary encoder button (SW) to validate value
  bool swState = digitalRead(pinSW);
  if (swState == LOW && lastSWState == HIGH && (millis() - lastDebounceSW) > debounceDelay) {
    lastDebounceSW = millis();
    if (settingMotor) {
      motorValue = currentValue;
      Serial.print("Motor value saved: ");
      Serial.println(motorValue);
    } else {
      blinkValue = currentValue;
      Serial.print("Blink value saved: ");
      Serial.println(blinkValue);
    }
  }
  lastSWState = swState;

  // Tactile switch (pin A0) to change mode
  bool switchState = digitalRead(A0);
  if (switchState == LOW && lastSwitchState == HIGH && (millis() - lastDebounceSwitch) > debounceDelay) {
    lastDebounceSwitch = millis();
    settingMotor = !settingMotor;
    if (settingMotor) {
      currentValue = motorValue;
      Serial.println("Mode: Motor value adjustment (0-255, step 5)");
    } else {
      currentValue = blinkValue;
      Serial.println("Mode: Blink value adjustment (0-5000, step 100)");
    }
    Serial.print("Current value: ");
    Serial.println(currentValue);
  }
  lastSwitchState = switchState;
  
  FastLED.showColor(CRGB::White);
  FastLED.showColor(CRGB::Black);
  static uint64_t next_flash_us = micros();
  if (blinkValue != 0)
    tourminute = 60000000 / (blinkValue);
  else
    tourminute = 0; // or another safe value

  if ((next_flash_us += tourminute) < micros())
    next_flash_us = micros();
  while (const uint64_t delay_us = next_flash_us - micros() < tourminute)
    delayMicroseconds(delay_us);

  //rotor
  analogWrite(motorPin, motorValue);
}