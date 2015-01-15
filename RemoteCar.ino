/* 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
IR Remote Control Car v1.0
2014/10/04 Created by Terence
This program will work with
 - Arduino Nano v3
 - L298N DC Motor board (for 2 DC Motors)
 - IR receiver by Ken Shirriff's IR libarary
   http://www.righto.com/2009/08/multi-protocol-infrared-remote-library.html
 - LEDs
 - Apple IR remote controller
 
 Functions of Apple Remote Controller
 - Up: Go Forward 
 - Down: Go Backward
 - Center: Stop
 - Right: Turn Right for 0.5s
 - Left: Turn Right for 0.5s
 - Menu: Turn On/Off 2 LEDs
 - Play/Pause: No definition
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/


#include <IRremote.h>
int irReceiver = 12;
IRrecv irrecv(irReceiver);
decode_results results;        // create an instance for ir receiver

int motorIn1 = 5;              // Motor pin definition, all pins are PWM output
int motorIn2 = 6;
int motorIn3 = 9;
int motorIn4 = 10;
int pwmTurn = 180;             // PWM value for turn 180/255

int led1 = 2;
int led2 = 3;
boolean ledStatus = true;      // set a boolean for led light status

void setup() {
  Serial.begin(9600);
  
  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);
  pinMode(motorIn3, OUTPUT);
  pinMode(motorIn4, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  
  irrecv.enableIRIn();         // start IR receiver
}

// Apple Remoter IR signal translation
void translateIR() {
  switch( results.value ) {
    
    case 0x77E13AFE:                 // Center button - Apple Remote Controller
      Serial.println("Stop");
      carStop();
      break;
        
    case 0x77E150FE:                 // Up button
      Serial.println("Forward");
      forward(130);
      break;
      
    case 0x77E130FE:                 // Down button
      Serial.println("Backward");
      backward(120);
      break;

    case 0x77E160FE:                 // Right button
      Serial.println("RightTurn");
      turnRight();
      delay(500);
      keepCenter();
      break;
      
    case 0x77E190FE:                 // Left button
      Serial.println("LeftTurn");
      turnLeft();
      delay(500);
      keepCenter();
      break;

    case 0X77E1C0FE:                 // Menu button 
      Serial.println("LED On/Off");
      lightLED();
      break;
  }
}

void loop() {
  if (irrecv.decode(&results)) {          //have we received an IR signal?
    translateIR();
    Serial.print( results.value, HEX );
    Serial.println(" ");
    irrecv.resume();                      //receive the next value
  }
}


// Moving functions
void carStop() {
  analogWrite(motorIn1, 0);
  analogWrite(motorIn2, 0);
}

void forward(int pwmSpeed) {
    analogWrite(motorIn1, pwmSpeed);
    analogWrite(motorIn2, 0);
}

void backward(int pwmSpeed) {
    analogWrite(motorIn1, 0);
    analogWrite(motorIn2, pwmSpeed);
}

void keepCenter() {
  analogWrite(motorIn3, 0);
  analogWrite(motorIn4, 0);
}

void turnRight() {
    analogWrite(motorIn3, pwmTurn);
    analogWrite(motorIn4, 0);
}

void turnLeft() {
    analogWrite(motorIn3, 0);
    analogWrite(motorIn4, pwmTurn);
}

// Turm on or turn off 2 sets of LED
void lightLED() {
  if (ledStatus == true) {
    digitalWrite( led1, HIGH );
    digitalWrite( led2, HIGH );
    ledStatus = false;
  }
  else {
    digitalWrite( led1, LOW);
    digitalWrite( led2, LOW);
    ledStatus = true;
  }
}
