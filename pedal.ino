//  _ ___ _______     ___ ___ ___  ___ _   _ ___ _____ ___ 
// / |_  )__ /   \   / __|_ _| _ \/ __| | | |_ _|_   _/ __| 
// | |/ / |_ \ |) | | (__ | ||   / (__| |_| || |  | | \__ \ 
// |_/___|___/___/   \___|___|_|_\\___|\___/|___| |_| |___/ 
// 
// Welding pedal for Jasic TIG-200P (Mini, E201), Razorweld 200 AC/DC Digital
// 
// Made by Goga Yep
// License: CC-BY-SA 3.0
// Downloaded from: https://circuits.io/circuits/4035801-welding-pedal-for-jasic-tig-200p-mini-e201-razorweld-200-ac-dc-digital

//TODO
// 1. shutdown immidiately
// 2. take low pedal pot value on start

//i#include <Adafruit_SSD1306.h>

#define PEDAL_POT 0
#define LIMITER_POT 1

#define REVERSE_BUTTON 6

#define ARC 12
#define AMP_DOWN 11
#define AMP_UP 10

#define PULSE_PERIOD 75			// ms
#define PULSE_PERIOD_DELAY 75
#define SLEEP_SEC 10			// s
#define LIMITER_MIN 10
#define LIMITER_MAX 50


#define TEXT_SIZE 6
#define OLED_RESET 4
//Adafruit_SSD1306 display(OLED_RESET);

#define CLK 5
#define LATCH 4
#define DATA 3

int minPedalPotValue = 30;
int currentAmp = 0;
int limiterRange = 0;
bool reverseDirection = false;

void setup() {
  pinMode(REVERSE_BUTTON, INPUT_PULLUP);
  
  // set singal pins to HIGH by default as welding machine listens for the LOW to inc/dec amps
  digitalWrite(ARC, HIGH);
  digitalWrite(AMP_UP, HIGH);
  digitalWrite(AMP_DOWN, HIGH);
  
  pinMode(ARC, OUTPUT);
  pinMode(AMP_UP, OUTPUT);
  pinMode(AMP_DOWN, OUTPUT);
  
  pinMode(LATCH, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(DATA, OUTPUT);

//  minPedalPotValue = analogRead(PEDAL_POT);
  minPedalPotValue = 100;
  
//  display.begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);  // initialize with the I2C addr 0x3D (for the 128x64)
//
//  display.setTextSize(TEXT_SIZE);
//  display.setTextColor(WHITE);
  
  Serial.begin(9600);
  Serial.println("Hello from pedal...");
}

void loop() {
  int currentAmpRaw = analogRead(PEDAL_POT);
  
  Serial.print("currentAmpRaw: ");
  Serial.println(currentAmpRaw);

  if (currentAmpRaw <= minPedalPotValue && currentAmp <= 0 || !limiterRange) {
    limiterRange = map(analogRead(LIMITER_POT), 0, 1023, LIMITER_MIN, LIMITER_MAX);      // range from 10 to 50
    reverseDirection = digitalRead(REVERSE_BUTTON) == HIGH;
    displayNumber(limiterRange, '=');
    delay(500);
    //sleepNow();
    return;
  }
  
//  wakeUpNow();
  
  digitalWrite(ARC, LOW);
  
  int loopAmp = map(currentAmpRaw, minPedalPotValue, 1023, 0, limiterRange);

  while (loopAmp != currentAmp) {
    bool isUp;

    if (loopAmp < currentAmp && currentAmp > 0) {
      isUp = !reverseDirection ? false : true;
      currentAmp--;
    } else if (loopAmp > currentAmp) {
      isUp = !reverseDirection ? true : false;
      currentAmp++;
    } else {
      break;
    }

    if (currentAmpRaw <= minPedalPotValue) {
      digitalWrite(ARC, HIGH);
    }

    displayNumber(currentAmp, isUp ? '+' : '-');
    
    //if (isUp) {
    //  encoderWrite(AMP_UP, AMP_DOWN);
    //} else {
    //  encoderWrite(AMP_DOWN, AMP_UP);
    //}

    pulseWrite(isUp ? AMP_UP : AMP_DOWN);

    displayNumber(currentAmp, ' ');
  }
}

void pulseWrite(int pedalPin) {
  digitalWrite(pedalPin, LOW);
  delay(PULSE_PERIOD);
  digitalWrite(pedalPin, HIGH);
  delay(PULSE_PERIOD_DELAY);
}

//void encoderWrite(int firstPin, int secondPin) {
//  digitalWrite(firstPin, HIGH);
//  delay(PULSE_PERIOD / 2);
//  digitalWrite(secondPin, HIGH);
//  delay(PULSE_PERIOD / 2);
//  digitalWrite(firstPin, LOW);
//  delay(PULSE_PERIOD / 2);
//  digitalWrite(secondPin, LOW);
//  delay(PULSE_PERIOD / 2);
//}

void displayNumber(int val, char sign) {
//  // 7-segment display
//  digitalWrite(LATCH, LOW);
//  shiftOut(DATA, CLK, LSBFIRST, 0);
//  shiftOut(DATA, CLK, LSBFIRST, 0);
//  digitalWrite(LATCH, HIGH);
//
//  if (val < 10)
//  {
//    digitalWrite(LATCH, LOW);
//    shiftOut(DATA, CLK, LSBFIRST, digit(val));
//    shiftOut(DATA, CLK, LSBFIRST, 0);
//    digitalWrite(LATCH, HIGH);
//  }
//  else if (val >= 10)
//  {
//    digitalWrite(LATCH, LOW);
//    shiftOut(DATA, CLK, LSBFIRST, digit(int(val%10))); 
//    shiftOut(DATA, CLK, LSBFIRST, digit(int(val/10))); 
//    digitalWrite(LATCH, HIGH);
//  }

  // Mini OLED Display
//  int posX = 0;
//  if (val < 10) {
//    posX = ( SSD1306_LCDWIDTH - 2*5*TEXT_SIZE - TEXT_SIZE ) / 2;
//  }
//  else {
//    posX = ( SSD1306_LCDWIDTH - 3*5*TEXT_SIZE - 2*TEXT_SIZE) / 2;
//  }
//  display.setCursor(posX,17);
//  display.clearDisplay();
//  display.print(sign);
//  display.print(val);
//  display.display();

  // Serial write
  Serial.print(millis() / 1000);
  Serial.print(": "); 
  Serial.println(val);
}

//		a | b | c | d | e | f | g | DP
//		0 | 1 | 2 | 3 | 4 | 5 | 6 | 7
// 		-------------------------------
//0		1	1	1	1	1	1	0	0
//1		0	1	1	0	0	0	0	0
//2	 	1	1	0	1	1	0	1	0
//3		1	1	1	1	0	0	1	0
//4	 	0	1	1	0	0	1	1	0
//5		1	0	1	1	0	1	1	0
//6		1	0	1	1	1	1	1	0
//7		1	1	1	0	0	0	0	0
//8		1	1	1	1	1	1	1	0
//9		1	1	1	0	0	1	1	0

//int segdisp[10] = {
//  0b11111100,
//  0b01100000,
//  0b11011010,
//  0b11110010,
//  0b01100110,
//  0b10110110,
//  0b10111110,
//  0b11100000,
//  0b11111110,
//  0b11100110
//};
//
//int digit(int theVal) {
//  return segdisp[theVal];
//}

//int awakeTime = millis() / 1000;
//void sleepNow()
//{
//  int currentTime = millis() / 1000;
//  
//  if (currentTime - awakeTime < SLEEP_SEC) {
//    return;
//  }
//  
//  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
//  sleep_enable();
//  attachInterrupt(0, wakeUpNow, CHANGE);
//  sleep_mode();
//  sleep_disable();
//  detachInterrupt(0);
//}
//
//void wakeUpNow() {
//	awakeTime = millis() / 1000;
//}

