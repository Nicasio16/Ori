#include <Servo.h>
#include "Arduino_LED_Matrix.h"

ArduinoLEDMatrix matrix;
Servo left;
Servo leftBack;
Servo right;
Servo rightBack;
int redPin = 8;
int bluePin= 6;
int greenPin = 7;
int leftMic = A0;
int rightMic = A1;
int pos = 0;
int BalL=0;
int BalR=0;
int val = 0;
int val2 = 0;

const uint8_t heart[] = {
  0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
  0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
  0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0
};

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}
// this void is setting up a function that can be called later on

void drawHeart() {
  matrix.clear();  // Clear the matrix
  matrix.loadPixels((uint8_t*)heart, sizeof(heart));  // Load the heart pixel array to the matrix
}

void stepLeft() {
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      leftBack.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
    }
    for (pos = 0; pos <= 180; pos += 1) { // goes from 180 degrees to 0 degrees
      right.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15); 
    }

      for (pos = 180; pos >= 0; pos -= 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      right.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
    }
    for (pos = 0; pos <= 180; pos += 1) { // goes from 180 degrees to 0 degrees
      leftBack.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15); 
    }
}

void stepRight() {
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      rightBack.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      left.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15); 
    }

      for (pos = 180; pos >= 0; pos -= 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      left.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15 ms for the servo to reach the position
    }
    for (pos = 0; pos <= 180; pos += 1) { // goes from 180 degrees to 0 degrees
      rightBack.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15); 
    }
}

void activateTask() {
  Serial.println("Activation signal received!");
  for(int j = 0; j<=3; j +=1){
    BalL = abs(analogRead(leftMic)-33);
    BalR = abs(analogRead(rightMic)-33);
    int dir = BalL-BalR;

  if (dir>0){
    for (int i = 0; i<=3; i +=1){
      stepLeft();
    }
  }
    if (dir<0){
    for (int i = 0; i<=3; i +=1){
      stepRight();
    }
  }
  }
}

void setup() {
  // put your setup code here, to run once:
 // until the robot recieves the start up signal it should stay in standby
  //wifi socket
  Serial.begin(115200);
  //led and walking innitialization
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  left.attach(2);
  leftBack.attach(3);
  right.attach(4);
  rightBack.attach(5);
// until the robot recieves the start up signal it should stay in standby
  
  left.write(90);
  leftBack.write(90);
  right.write(90);
  rightBack.write(90);

  for (int i=0; i<=5; i++){
    setColor(255,0,0);
    delay(50);
    setColor(0,0,0);
    delay(50);
  }
  delay(1000);
  setColor(0,255,0);
  delay(10000);

  Serial.println("Send 'start' for assistance.");

}
  

void loop() {
if (Serial.available() > 0) {
  String command = Serial.readStringUntil('\n');  // Read input from the serial monitor

  // Check if the input command is "start"
  command.trim();  // Remove any extra spaces or newlines
  if (command.equalsIgnoreCase("start")) {
    Serial.println("Are you ok");
    activateTask();
  } else {
    // If the command is not recognized
    Serial.println("Unknown command. Send 'start' for assistance.");
  }
}
}