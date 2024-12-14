#include <Arduino.h>

float MbHSInN = A0;
float MbHSInP = A1;

float MaHSInN = A2;
float MaHSInP = A3;

// Motor1

int enA = 9;
int in1 = 8;
int in2 = 7;

// Motor 2

int enB = 3;
int in3 = 5;
int in4 = 4;

// put function declarations here:
void setup();
void loop();

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  pinMode(MbHSInN, INPUT);
  pinMode(MbHSInP, INPUT);

  pinMode(MaHSInN, INPUT);
  pinMode(MaHSInP, INPUT);

  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

}

void moveMotor(int m, int s, int t, int d) {

  delay(100);

  if (m=='b') {

    if (d==1) {
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
    } else if (d==0)
    {
      /* move in reverse */
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    }
    

    analogWrite(enB, s);

    Serial.print(analogRead(MbHSInN)); Serial.print(":");
    Serial.println(analogRead(MbHSInP));
    delay(t);

    analogWrite(enB, 0);  // stop motor

  } else if (m=='a')
  {
    if (d==1) {
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
    } else if (d==0)
    {
      /* move in reverse */
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
    }
    

    analogWrite(enA, s);

    Serial.print(analogRead(MaHSInN)); Serial.print(":");
    Serial.println(analogRead(MaHSInP));
    delay(t);

    analogWrite(enA, 0);  // stop motor
    }
  

}

int moveForward(int speed, int duration) {

  delay(100);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  analogWrite(enA, speed);
  analogWrite(enB, speed);

  // Serial.print(analogRead(M1HSInN)); Serial.print(":");
  // Serial.println(analogRead(M1HSInP));
  delay(duration);

  analogWrite(enA, 0);
  analogWrite(enB, 0);

  return 0;

}

int moveBackwards(int speed, int duration) {

  delay(100);

  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  analogWrite(enA, speed);
  analogWrite(enB, speed);

  // Serial.print(analogRead(M1HSInN)); Serial.print(":");
  // Serial.println(analogRead(M1HSInP));
  delay(duration);

  analogWrite(enA, 0);
  analogWrite(enB, 0);

  return 0;

}

void loop() {
  // put your main code here, to run repeatedly:

// moveMotor('a', 150,1000,1);
delay(3000);
moveForward(150,1000);
delay(2000);
moveBackwards(250,1000);
}

