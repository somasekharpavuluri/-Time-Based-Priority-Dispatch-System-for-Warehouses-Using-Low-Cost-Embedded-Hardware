#include <Servo.h>

// Motor driver pins
#define in1 7
#define in2 6
#define in3 5
#define in4 4

// Blade and buzzer
#define blade 3
#define buzz 12

// Servo motor
#define motor 8
#define spoint 103

// Mode selector pin (not needed anymore, but kept if you want to re-enable later)
const int DIN = 2;

// Servo object
Servo servo;

void setup() {
  Serial.begin(9600);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(blade, OUTPUT);
  pinMode(buzz, OUTPUT);
  pinMode(DIN, INPUT); // Not used anymore

  servo.attach(motor);
  servo.write(spoint); // Center the servo
}

void loop() {
  Bluetoothcontrol();  // Always in Bluetooth mode now
}

void Bluetoothcontrol() {
  if (Serial.available() > 0) {
    char inputvalue = char(Serial.read());

    switch (inputvalue) {
      case 'F': forward(); break;
      case 'B': backward(); break;
      case 'R': right(); break;
      case 'L': left(); break;
      case 'S': Stop(); break;
      case 'o': digitalWrite(blade, HIGH); break;
      case 'f': digitalWrite(blade, LOW); break;
    }
  }
}

void forward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void backward() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void right() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void left() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void Stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}