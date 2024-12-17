#include <Arduino.h>

// Hall Sensor Pins
const int MbHSInN = A0; // Motor B Hall Sensor Negative
const int MbHSInP = A1; // Motor B Hall Sensor Positive
const int MaHSInN = A2; // Motor A Hall Sensor Negative
const int MaHSInP = A3; // Motor A Hall Sensor Positive

// Motor A Pins
const int enA = 9;
const int in1 = 8;
const int in2 = 7;

// Motor B Pins
const int enB = 3;
const int in3 = 5;
const int in4 = 4;

// Encoder Ticks
volatile int encoderTicksA = 0;
volatile int encoderTicksB = 0;

// Function Prototypes
void handleEncoder(int pinN, int pinP, volatile int &ticks);
void moveMotor(int speed, int direction);
void stopMotors();
void moveForward(int speed);
void moveBackward(int speed);
void rotateVhcl(int speed, int direction);

void setup() {
  Serial.begin(9600);

  // Motor Pins Setup
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Hall Sensor Pins as Input
  pinMode(MaHSInN, INPUT);
  pinMode(MaHSInP, INPUT);

  pinMode(MbHSInN, INPUT);
  pinMode(MbHSInP, INPUT);

  // Attach Interrupts for Encoders
  attachInterrupt(digitalPinToInterrupt(MaHSInN), [] { handleEncoder(MaHSInN, MaHSInP, encoderTicksA); }, CHANGE);
  attachInterrupt(digitalPinToInterrupt(MbHSInN), [] { handleEncoder(MbHSInN, MbHSInP, encoderTicksB); }, CHANGE);

  Serial.println("Ready to receive commands.");
}

void loop() {
  // Serial Command Parsing Inline
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n'); // Read command until newline
    command.trim(); // Remove leading/trailing whitespace

    char cmd = command.charAt(0); // Command letter: F, B, or S
    int firstColon = command.indexOf(':');
    int secondColon = command.indexOf(':', firstColon + 1);
    int thirdColon = command.indexOf(':', secondColon + 1);


    if (cmd == 'F' || cmd == 'B' || cmd == 'S' || cmd == 'R') {
      if (firstColon > 0 && secondColon > firstColon) {
        int speed = command.substring(firstColon + 1, secondColon).toInt();
        int duration = command.substring(secondColon + 1).toInt();
        int rotationDirection = command.substring(thirdColon + 1).toInt();

        speed = constrain(speed, 0, 255); // Ensure speed is valid

        Serial.print("Command: ");
        Serial.print(cmd);
        Serial.print(" | Speed: ");
        Serial.print(speed);
        Serial.print(" | Duration: ");
        Serial.println(duration);
        Serial.print(" | Rotation Direction: ");
        Serial.println(rotationDirection);

        if (cmd == 'F') {
          moveForward(speed);
        } else if (cmd == 'B') {
          moveBackward(speed);
        } else if (cmd == 'R') {
          rotateVhcl(speed, rotationDirection);
        }

        Serial.print("Motor A ticks "); Serial.println(encoderTicksA);
        Serial.print("Motor B ticks "); Serial.println(encoderTicksB);

        delay(duration); // Run for specified duration
        stopMotors(); // Stop after the duration
      }
    } else if (cmd == 'S') {
      Serial.println("Stopping Motors");
      stopMotors();
      } else {
      Serial.println("Invalid Command. Use F, B, or S.");
    }
  }
}

// Unified Encoder Handling Function
void handleEncoder(int pinN, int pinP, volatile int &ticks) {
  if (digitalRead(pinN) == digitalRead(pinP)) {
    ticks++;
  } else {
    ticks--;
  }
}

// Function to Move Forward
void moveForward(int speed) {
  moveMotor(speed, 1); // 1 means forward direction
}

// Function to Move Backward
void moveBackward(int speed) {
  moveMotor(speed, 0); // 0 means backward direction
}

// Function to Move Motors
void moveMotor(int speed, int direction) {
  // Motor A Control
  if (direction == 1) { // Forward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  } else { // Backward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }
  analogWrite(enA, speed);

  // Motor B Control
  if (direction == 1) { // Forward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  } else { // Backward
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }
  analogWrite(enB, speed);
}

void rotateVhcl(int speed, int direction) {

  // Motor A Control
  if (direction == 1) { // Forward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  } else { // Backward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }
  analogWrite(enA, speed);

  // Motor B Control
  if (direction == 1) { // Forward
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  } else { // Backward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }
  analogWrite(enB, speed);

}


// Function to Stop Motors
void stopMotors() {
  // Stop Motor A
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enA, 0);

  // Stop Motor B
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enB, 0);
}
