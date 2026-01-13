#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_PWMServoDriver.h>

// Pin definitions
#define buttonStart 23
#define buttonDecrement 22
#define buttonIncrement 24
#define buttonSetMode 25

// IR sensor pins
const int irSensorPin1 = 36;  // 28
const int irSensorPin2 = 34;  // 30
const int irSensorPin3 = 32;  // 32
const int irSensorPin4 = 30;  // 34
const int irSensorPin5 = 28;  // 36
const int irSensorPin6 = 40;  // Collating sensor

// Motors pins
#define motorPin1 49        // 31
#define motorPin2 47        // 33
#define motorPin3 45        // 35
#define motorPin4 43        // 37
#define motorPin5 41        // 39
#define StaplerMotorPinF 5  // for stapling mechanism forward
#define StaplerMotorPinB 4  // for stapling mechanism backward
#define motorPin7 39        // 41
#define motorPin8 37        // 43
#define motorPin9 35        // 45
#define motorPin10 33       // 47
#define motorPin11 31       // 49
#define motorPin12 52       // Repeater motor
// #define motorPin13 53

// Buzzer pin
#define buzzerPin 1

// Servo setup
// Servo servo1, servo2, servo3, servo4;

// LCD initialization
LiquidCrystal_I2C lcd(0x27, 20, 4);
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();  // Initialize PCA9685

// Variables
int paperCount = 1;             // Paper count (1 to 5)
int feederMotorSpeed = 225;     // Set motor speed (0 to 255)
int feederMotorSpeed2 = 120;    // Set motor speed (0 to 255)
int staplerMotorF = 180;        // Set motor speed (0 to 255)
int staplerMotorB = 180;        // Set motor speed (0 to 255)
int irCount = 0;                // For collated mode stapling mechanism
bool mode = true;               // True = Collated, False = Uncollated
bool motorOff = 0;              // Default false | motor off
bool invalidStateFlag = false;  // Global flag to track invalid state

// Servo channels for drawers (channels 0 to 4)
const int drawerServos[5] = { 0, 1, 2, 3, 4 };

// Servo channels for hatch door (channels 13 to 16)
const int hatchServos[4] = { 12, 13, 14, 15 };

const int servoMin = 150;  // Minimum pulse length (adjust based on calibration)
const int servoMax = 600;  // Maximum pulse length (adjust based on calibration)

// Function to convert angle to pulse width
int angleToPulse(int angle) {
  return map(angle, 0, 180, servoMin, servoMax);
}

// Function to initialize servos to 0 degrees
void initializeServos() {
  pwm.setPWM(drawerServos[1], 0, angleToPulse(60));  // servo 4
  pwm.setPWM(drawerServos[2], 0, angleToPulse(65));  // servo 3
  pwm.setPWM(drawerServos[3], 0, angleToPulse(60));  // servo 2
  pwm.setPWM(drawerServos[4], 0, angleToPulse(60));  // servo 1
  pwm.setPWM(drawerServos[5], 0, angleToPulse(70));  // servo 5
  // }
}

// Set hatch to close
void hatchDoorClose() {
  pwm.setPWM(hatchServos[0], 0, angleToPulse(90));
  pwm.setPWM(hatchServos[1], 0, angleToPulse(66));
  pwm.setPWM(hatchServos[2], 0, angleToPulse(50));
  pwm.setPWM(hatchServos[3], 0, angleToPulse(50));
}

// Function to activate drawer servos to 0 degrees
void activateFeederServos() {
  // for (int i = 0; i < 5; i++) {
  //   pwm.setPWM(drawerServos[i], 0, angleToPulse(3));  // Set servos to 0 degrees
  // }
  pwm.setPWM(drawerServos[1], 0, angleToPulse(2));  // servo 4
  pwm.setPWM(drawerServos[2], 0, angleToPulse(2));  // servo 3
  pwm.setPWM(drawerServos[3], 0, angleToPulse(2));  // servo 2
  pwm.setPWM(drawerServos[4], 0, angleToPulse(2));  // servo 1
  pwm.setPWM(drawerServos[5], 0, angleToPulse(2));  // servo 5
}

void staplerFunction() {
  analogWrite(StaplerMotorPinF, staplerMotorF);  // ON Motor stapler forward
  delay(400);
  analogWrite(StaplerMotorPinF, motorOff);  // OFF Motor stapler forward
  delay(900);
  analogWrite(StaplerMotorPinB, staplerMotorF);  // ON Motor stapler backward
  delay(400);
  analogWrite(StaplerMotorPinB, motorOff);  // OFF Motor stapler backward
}

// Function to activate hatch servos by opening them to 90 degrees
void activateHatchServos() {
  delay(2000);
  pwm.setPWM(hatchServos[3], 0, angleToPulse(0));  // Open hatch servo 4
  delay(900);
  pwm.setPWM(hatchServos[2], 0, angleToPulse(10));  // Open hatch servo 3
  delay(800);
  staplerFunction();
  pwm.setPWM(hatchServos[3], 0, angleToPulse(50));
  pwm.setPWM(hatchServos[2], 0, angleToPulse(50));
  delay(900);
  pwm.setPWM(hatchServos[0], 0, angleToPulse(0));    // Open hatch servo 1
  pwm.setPWM(hatchServos[1], 0, angleToPulse(140));  // Open hatch servo 2
  delay(1500);

  // Set hatch to close
  hatchDoorClose();

  // Activate feeder mechanism
  activateFeederServos();

  delay(5000);

  // Retry to feed
  FeederState();
}

void activateHatchServos2() {
  delay(2000);
  pwm.setPWM(hatchServos[3], 0, angleToPulse(0));  // Open hatch servo 4
  delay(900);
  pwm.setPWM(hatchServos[2], 0, angleToPulse(10));  // Open hatch servo 3
  delay(800);
  staplerFunction();
  pwm.setPWM(hatchServos[3], 0, angleToPulse(50));
  pwm.setPWM(hatchServos[2], 0, angleToPulse(50));
  delay(900);
  pwm.setPWM(hatchServos[0], 0, angleToPulse(0));    // Open hatch servo 1
  pwm.setPWM(hatchServos[1], 0, angleToPulse(140));  // Open hatch servo 2
  delay(1500);

  // Set hatch to close
  hatchDoorClose();

  // Activate feeder mechanism
  activateFeederServos();
}

// Function to update LCD
void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Paper/s: ");
  lcd.print(paperCount);
  lcd.setCursor(0, 1);
  lcd.print("Mode: ");
  lcd.print(mode ? "Collated" : "Uncollated");
  lcd.setCursor(0, 2);
  lcd.print("IR Count: " + String(irCount));
}

void beepBuzzerRepeatedly(int times, int delayMs, int totalDurationMs) {
  unsigned long startTime = millis();
  while (millis() - startTime < totalDurationMs) {
    for (int i = 0; i < times; i++) {
      digitalWrite(buzzerPin, HIGH);
      delay(delayMs);
      digitalWrite(buzzerPin, LOW);
      delay(delayMs);
    }
  }
}

// No paper detected message
void InvalidState(int m1) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Slot " + String(m1) + " empty!");
  lcd.setCursor(0, 1);
  lcd.print("Check slot " + String(m1) + " or");
  lcd.setCursor(0, 2);
  lcd.print("adjust paper count");
  beepBuzzerRepeatedly(3, 200, 1500);
  updateLCD();

  // Set flag to indicate an error state
  invalidStateFlag = true;

  delay(500);  // Debounce delay
}

void MotorStop() {
  analogWrite(motorPin1, motorOff);   // Stop motor 1
  analogWrite(motorPin2, motorOff);   // Stop motor 2
  analogWrite(motorPin3, motorOff);   // Stop motor 3
  analogWrite(motorPin4, motorOff);   // Stop motor 4
  analogWrite(motorPin5, motorOff);   // Stop motor 5
  analogWrite(motorPin7, motorOff);   // Stop motor 7
  analogWrite(motorPin8, motorOff);   // Stop motor 8
  analogWrite(motorPin9, motorOff);   // Stop motor 9
  analogWrite(motorPin10, motorOff);  // Stop motor 10
  analogWrite(motorPin11, motorOff);  // Stop motor 11
  analogWrite(motorPin12, motorOff);  // Stop motor Repeater
}

void MotorTiming() {
  delay(2000);  // Short pause for 2 secs
  activateHatchServos();
}

void MotorTiming2() {
  pwm.setPWM(drawerServos[4], 0, angleToPulse(60));  // raise the feeder 1 (servo motor)
  delay(1000);                                       // Short pause
  activateHatchServos2();
  delay(800);
  // pwm.setPWM(drawerServos[4], 0, angleToPulse(3));  // servo 1
}

void Feeder1() {
  analogWrite(motorPin1, feederMotorSpeed);  // ON motor 1
  analogWrite(motorPin2, feederMotorSpeed);  // ON motor 2
  analogWrite(motorPin3, motorOff);          // OFF motor 3
  analogWrite(motorPin4, motorOff);          // OFF motor 4
  analogWrite(motorPin5, motorOff);          // OFF motor 5
  analogWrite(motorPin7, motorOff);          // OFF motor 7
  analogWrite(motorPin8, motorOff);          // OFF motor 8
  analogWrite(motorPin9, motorOff);          // OFF motor 9
  analogWrite(motorPin10, motorOff);         // OFF motor 10
  analogWrite(motorPin11, motorOff);         // OFF motor 11
  delay(1000);
  pwm.setPWM(drawerServos[4], 0, angleToPulse(60));  // feeder servo 1
  delay(1500);
}

void Feeder2() {
  analogWrite(motorPin5, motorOff);          // OFF motor 5
  analogWrite(motorPin7, motorOff);          // OFF motor 7
  analogWrite(motorPin8, motorOff);          // OFF motor 8
  analogWrite(motorPin9, motorOff);          // OFF motor 9
  analogWrite(motorPin10, motorOff);         // OFF motor 10
  analogWrite(motorPin11, motorOff);         // OFF motor 11
  analogWrite(motorPin3, feederMotorSpeed);  // ON motor 3
  analogWrite(motorPin4, feederMotorSpeed);  // ON motor 4
  delay(1000);
  pwm.setPWM(drawerServos[3], 0, angleToPulse(60));  // feeder servo 2
  delay(1500);
  analogWrite(motorPin1, feederMotorSpeed);  // ON motor 1
  analogWrite(motorPin2, feederMotorSpeed);  // ON motor 2
  delay(1000);
  pwm.setPWM(drawerServos[4], 0, angleToPulse(60));  // feeder servo 1
}

void Feeder3() {
  analogWrite(motorPin8, motorOff);          // OFF motor 8
  analogWrite(motorPin9, motorOff);          // OFF motor 9
  analogWrite(motorPin10, motorOff);         // OFF motor 10
  analogWrite(motorPin11, motorOff);         // OFF motor 11
  analogWrite(motorPin5, feederMotorSpeed);  // ON motor 5
  analogWrite(motorPin7, feederMotorSpeed);  // ON motor 7
  delay(1000);
  pwm.setPWM(drawerServos[2], 0, angleToPulse(65));  // feeder servo 3
  delay(1500);
  analogWrite(motorPin3, feederMotorSpeed);  // ON motor 3
  analogWrite(motorPin4, feederMotorSpeed);  // ON motor 4
  delay(1000);
  pwm.setPWM(drawerServos[3], 0, angleToPulse(60));  // feeder servo 2
  delay(1500);
  analogWrite(motorPin1, feederMotorSpeed);  // ON motor 1
  analogWrite(motorPin2, feederMotorSpeed);  // ON motor 2
  delay(1000);
  pwm.setPWM(drawerServos[4], 0, angleToPulse(60));  // feeder servo 1
}

void Feeder4() {
  analogWrite(motorPin10, motorOff);         // OFF motor 10
  analogWrite(motorPin11, motorOff);         // OFF motor 11
  analogWrite(motorPin8, feederMotorSpeed);  // ON motor 8
  analogWrite(motorPin9, feederMotorSpeed);  // ON motor 9
  delay(1000);
  pwm.setPWM(drawerServos[1], 0, angleToPulse(60));  // feeder servo 4
  delay(1500);
  analogWrite(motorPin5, feederMotorSpeed);  // ON motor 5
  analogWrite(motorPin7, feederMotorSpeed);  // ON motor 7
  delay(1000);
  pwm.setPWM(drawerServos[2], 0, angleToPulse(65));  // feeder servo 3
  delay(1500);
  analogWrite(motorPin3, feederMotorSpeed);  // ON motor 3
  analogWrite(motorPin4, feederMotorSpeed);  // ON motor 4
  delay(1000);
  pwm.setPWM(drawerServos[3], 0, angleToPulse(60));  // feeder servo 2
  delay(1500);
  analogWrite(motorPin1, feederMotorSpeed);  // ON motor 1
  analogWrite(motorPin2, feederMotorSpeed);  // ON motor 2
  delay(1000);
  pwm.setPWM(drawerServos[4], 0, angleToPulse(60));  // feeder servo 1
}

void Feeder5() {
  analogWrite(motorPin10, feederMotorSpeed);  // ON motor 10
  analogWrite(motorPin11, feederMotorSpeed);  // ON motor 11
  delay(1000);
  pwm.setPWM(drawerServos[5], 0, angleToPulse(70));  // feeder servo 5
  delay(1500);
  analogWrite(motorPin8, feederMotorSpeed);  // ON motor 8
  analogWrite(motorPin9, feederMotorSpeed);  // ON motor 9
  delay(1000);
  pwm.setPWM(drawerServos[1], 0, angleToPulse(60));  // feeder servo 4
  delay(1500);
  analogWrite(motorPin5, feederMotorSpeed);  // ON motor 5
  analogWrite(motorPin7, feederMotorSpeed);  // ON motor 7
  delay(1000);
  pwm.setPWM(drawerServos[2], 0, angleToPulse(65));  // feeder servo 3
  delay(1500);
  analogWrite(motorPin3, feederMotorSpeed);  // ON motor 3
  analogWrite(motorPin4, feederMotorSpeed);  // ON motor 4
  delay(1000);
  pwm.setPWM(drawerServos[3], 0, angleToPulse(60));  // feeder servo 2
  delay(1500);
  analogWrite(motorPin1, feederMotorSpeed);  // ON motor 1
  analogWrite(motorPin2, feederMotorSpeed);  // ON motor 2
  delay(1000);
  pwm.setPWM(drawerServos[4], 0, angleToPulse(60));  // feeder servo 1
}

void FeederState() {
  invalidStateFlag = false;  // Reset flag at the start of function

  // Set motor states based on paperCount and run motor/s
  if (paperCount == 1) {
    if (digitalRead(irSensorPin1) == LOW) {
      while (digitalRead(irSensorPin1) == LOW) {
        Feeder1();
        MotorTiming();
      }
    } else {
      InvalidState(1);
      return;
    }
  } else if (paperCount == 2) {
    if (digitalRead(irSensorPin1) == LOW && digitalRead(irSensorPin2) == LOW) {
      while (digitalRead(irSensorPin1) == LOW || digitalRead(irSensorPin2) == LOW) {
        Feeder2();
        MotorTiming();
      }
    } else {
      InvalidState(2);
      return;
    }
  } else if (paperCount == 3) {
    if (digitalRead(irSensorPin1) == LOW && digitalRead(irSensorPin2) == LOW && digitalRead(irSensorPin3) == LOW) {
      while (digitalRead(irSensorPin1) == LOW || digitalRead(irSensorPin2) == LOW || digitalRead(irSensorPin3) == LOW) {
        Feeder3();
        MotorTiming();
      }
    } else {
      InvalidState(3);
      return;
    }
  } else if (paperCount == 4) {
    if (digitalRead(irSensorPin1) == LOW && digitalRead(irSensorPin2) == LOW && digitalRead(irSensorPin3) == LOW && digitalRead(irSensorPin4) == LOW) {
      while (digitalRead(irSensorPin1) == LOW || digitalRead(irSensorPin2) == LOW || digitalRead(irSensorPin3) == LOW || digitalRead(irSensorPin4) == LOW) {
        Feeder4();
        MotorTiming();
      }
    } else {
      InvalidState(4);
    }
  } else if (paperCount == 5) {
    if (digitalRead(irSensorPin1) == LOW && digitalRead(irSensorPin2) == LOW && digitalRead(irSensorPin3) == LOW && digitalRead(irSensorPin4) == LOW && digitalRead(irSensorPin5) == LOW) {
      while (digitalRead(irSensorPin1) == LOW || digitalRead(irSensorPin2) == LOW || digitalRead(irSensorPin3) == LOW || digitalRead(irSensorPin4) == LOW || digitalRead(irSensorPin5) == LOW) {
        Feeder5();
        MotorTiming();
      }
    } else {
      InvalidState(5);
    }
  }
}

void noPaper() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("No paper detected");
  lcd.setCursor(0, 1);
  lcd.print("from the slot/s");
  lcd.setCursor(0, 3);
  lcd.print("Process Aborted!");
  beepBuzzerRepeatedly(3, 200, 1500);
  updateLCD();
  initializeServos();
  delay(1000);
  analogWrite(motorPin12, motorOff);  // Run motor Repeater
}

void countPaper() {
  irCount++;
  pwm.setPWM(drawerServos[4], 0, angleToPulse(3));
  delay(3000);
  pwm.setPWM(drawerServos[4], 0, angleToPulse(60));
  lcd.setCursor(0, 3);
  lcd.print("IR Count is <");
  updateLCD();

  if (irCount < paperCount) {
    countPaper();
  }
}

void handleCollatedMode() {
  irCount = 0;
  analogWrite(motorPin1, feederMotorSpeed);  // ON motor 1
  analogWrite(motorPin2, feederMotorSpeed);  // ON motor 2
  if (digitalRead(irSensorPin6) == LOW) {
    for (int i = 0; i < paperCount; i++) {
      irCount++;
      pwm.setPWM(drawerServos[4], 0, angleToPulse(3));
      delay(1000);
      pwm.setPWM(drawerServos[4], 0, angleToPulse(60));
      lcd.setCursor(0, 3);
      lcd.print("IR Count is <");
      updateLCD();
      delay(3000);

      if (irCount == paperCount) {
        MotorTiming2();
      }
    }
  }
}

void setup() {
  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Initialize PCA9685 Servo Driver
  pwm.begin();
  pwm.setPWMFreq(50);  // Set frequency to 50Hz for servos

  // Initialize servos to 0 degrees
  initializeServos();
  hatchDoorClose();

  // Buzzer Setup
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);  // Initially turn off the buzzer

  // Initialize pins
  pinMode(buttonIncrement, INPUT);  // Using external resistors
  pinMode(buttonDecrement, INPUT);  // Using external resistors
  pinMode(buttonSetMode, INPUT);    // Using external resistors
  pinMode(buttonStart, INPUT);      // Using external resistors
  pinMode(irSensorPin1, INPUT);
  pinMode(irSensorPin2, INPUT);
  pinMode(irSensorPin3, INPUT);
  pinMode(irSensorPin4, INPUT);
  pinMode(irSensorPin5, INPUT);
  pinMode(irSensorPin6, INPUT);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(motorPin5, OUTPUT);
  pinMode(motorPin7, OUTPUT);
  pinMode(motorPin8, OUTPUT);
  pinMode(motorPin9, OUTPUT);
  pinMode(motorPin10, OUTPUT);
  pinMode(motorPin11, OUTPUT);
  pinMode(motorPin12, OUTPUT);

  // Initialize all motors to off
  MotorStop();

  // Initial LCD display
  updateLCD();
}

void loop() {
  // Read state of each IR sensor
  int irState1 = digitalRead(irSensorPin1);
  int irState2 = digitalRead(irSensorPin2);
  int irState3 = digitalRead(irSensorPin3);
  int irState4 = digitalRead(irSensorPin4);
  int irState5 = digitalRead(irSensorPin5);
  int irState6 = digitalRead(irSensorPin6);

  // Increment button
  if (digitalRead(buttonIncrement) == HIGH) {
    if (mode) {  // Collated mode
      if (paperCount < 5) {
        beepBuzzerRepeatedly(1, 50, 50);
        paperCount++;
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Warning:");
        lcd.setCursor(0, 1);
        lcd.print("Collated mode only");
        lcd.setCursor(0, 2);
        lcd.print("has 5 max slot");
        beepBuzzerRepeatedly(2, 200, 1000);
      }
    } else {  // Uncollated mode
      if (paperCount < 5) {
        beepBuzzerRepeatedly(1, 50, 50);
        paperCount++;
      } else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Warning:");
        lcd.setCursor(0, 1);
        lcd.print("Uncollated mode only");
        lcd.setCursor(0, 2);
        lcd.print("has 5 max slot");
        beepBuzzerRepeatedly(2, 200, 1000);
      }
    }
    updateLCD();
    delay(200);  // Debounce delay
  }

  // Decrement button
  if (digitalRead(buttonDecrement) == HIGH) {
    if (paperCount > 1) {
      beepBuzzerRepeatedly(1, 50, 50);
      paperCount--;
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Warning:");
      lcd.setCursor(0, 1);
      lcd.print("Cannot go below 1");
      beepBuzzerRepeatedly(2, 200, 1000);
    }
    updateLCD();
    delay(200);  // Debounce delay
  }

  // Set mode button
  if (digitalRead(buttonSetMode) == HIGH) {
    mode = !mode;    // Toggle mode
    paperCount = 1;  // Reset paper count
    beepBuzzerRepeatedly(1, 50, 50);
    updateLCD();
    delay(200);  // Debounce delay
  }

  // Start button
  if (digitalRead(buttonStart) == HIGH) {
    irCount = 0;
    activateFeederServos();
    delay(3500);
    analogWrite(motorPin12, feederMotorSpeed);  // Run motor Repeater
    if (digitalRead(irSensorPin1) == LOW) {     // Check if primary paper is detected
      // Process starts
      beepBuzzerRepeatedly(1, 100, 100);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Processing Papers...");
      if (mode) {  // Collated mode
        if (digitalRead(irSensorPin1) == LOW) {
          while (digitalRead(irSensorPin1) == LOW) {
            handleCollatedMode();
            if (digitalRead(irSensorPin1) == HIGH) {
              delay(3000);
              irCount = 0;
            }
          }
        } else {
          noPaper();                // No detected paper
          invalidStateFlag = true;  // Set flag to indicate an error state
        }
      } else {          // Uncollated mode
        FeederState();  // Update motor states based on paperCount and run
      }

      // Check if an invalid state was encountered
      if (invalidStateFlag) {
        invalidStateFlag = false;  // Reset flag at the start of function
        return;                    // Exit function early if invalid state occurred
      }

      // Process finished
      MotorStop();         // stop all motors
      initializeServos();  // Reset servo
      hatchDoorClose();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Process Complete!");
      beepBuzzerRepeatedly(2, 500, 10000);
      paperCount = 1;  // Reset paper count
      updateLCD();

    } else {
      noPaper();  // Error for no paper
    }
    updateLCD();
    delay(500);  // Debounce delay
  }
}