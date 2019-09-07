/*************************
 * This program provides motor control for a robot.
 * NOTES
 * The motor control code uses interrupts to track motor rotation using opto-interrupter encoders.
 * The encoder disc and mounting structure are handmade.
 * 
 * References: 
 * https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
 * https://youtu.be/oQQpAACa3ac
 ************************/
 
#include "TimerOne.h"     //TimerOne library is used in the ISRs for the endoders
#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Connect a DC motor to port M2
Adafruit_DCMotor *Motor2 = AFMS.getMotor(2);
// Connect a DC motor to port M3
Adafruit_DCMotor *Motor3 = AFMS.getMotor(3);

const byte MOTOR2 = 2;  //assign hardware interrupt pins to each motor. For the robot-arduino-lego robot, MOTOR2 is the Left Motor. MOTOR3 is the Right Motor.
const byte MOTOR3 = 3;
//IMPORTANT: Variables that need to be modified within an ISR MUST be declared as "volatile". See Arduino reference listed above.
volatile int encoder2 = 1; //count encoder disc "ticks" each time the encoder circuitry sends an interrupt pulse. 
volatile int encoder3 = 1;
float slots = 1.00; //Number of slots in encoder disc. Used to track rotation and calculate speed.

/**************INTERRUPT SERVICE ROUTINES******************/
//Motor2 pulse count ISR
void ISR_encoder2() {
    encoder2++;
}

//Motor3 pulse count ISR
void ISR_encoder3() {
  encoder3++;
}

//TimerOne ISR
void ISR_timerone() {
  Timer1.detachInterrupt();   //Stop Timer1 to allow time for serial print out
  Serial.print("Encoder 2: ");
  float rotation2 = (encoder2 / slots) * 1.00;   //Calculate RPM. Since Timer1 is set for 1 second, we get the number of slot ticks per second. Divide this by number of slots on the encoder disc to get number of revolutions per second. Then, multiply by 60 to get revolutions per minute.
  Serial.print(encoder2);
  Serial.print(" ");
    Serial.print("Encoder 3: ");
  float rotation3 = (encoder3 / slots) * 1.00;
  Serial.print(encoder3);
  Serial.println(" ");
//  encoder2 = 0;
//  encoder3 = 0;
  Timer1.attachInterrupt(ISR_timerone);
}
 /*************END INTERRUPT SERVICE ROUTINES*************/


void setup() {
  Serial.begin(9600);
  Serial.println("Encoder Test!");
  AFMS.begin();  // create with the default frequency 1.6KHz
    // turn on motor M2
  Motor2->setSpeed(255);    //Max motor speed is 255.
  Motor2->run(RELEASE);
    // turn on motor M3
  Motor3->setSpeed(255);
  Motor3->run(RELEASE);
  
  pinMode(MOTOR2, INPUT_PULLUP);  //Activate pullup resistors on the interrupt pins
  pinMode(MOTOR3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(MOTOR2), ISR_encoder2, RISING);   //Attach interrupt service routines to hardware interrupt pins and set trigger mode.
  attachInterrupt(digitalPinToInterrupt(MOTOR3), ISR_encoder3, RISING);
  Timer1.initialize(1000000);   //Timer1 accepts parameter in microseconds. Set to one million for 1 second.
  Timer1.attachInterrupt(ISR_timerone);   //Enable the timer
}

void loop() {
//  spinLeft_Encoder(24);
//  spinRight_Encoder(24);

//  moveForward(20);
}

void moveForward(int moveValue) {
  while(encoder2 < moveValue && encoder2 != 0) {   //Encoder value of 24 gives an approximate 90 degree turn.
    Motor2->run(FORWARD);     //LEFT TURN
    Motor3->run(FORWARD);
  }
  if(encoder2 != 0) {
    Motor2->run(BACKWARD);
    Motor3->run(BACKWARD);
    delay(10);
    Motor2->run(RELEASE);
    Motor3->run(RELEASE);
    encoder2 = 0;
    encoder3 = 0;
  }
}

void spinLeft_Encoder(int spinValue) {
  while(encoder2 < spinValue && encoder2 != 0) {   //Encoder value of 24 gives an approximate 90 degree turn.
    Motor2->run(FORWARD);     //LEFT TURN
    Motor3->run(BACKWARD);
  }
  if(encoder2 != 0) {
    Motor2->run(BACKWARD);
    Motor3->run(FORWARD);
    delay(10);
    Motor2->run(RELEASE);
    Motor3->run(RELEASE);
    encoder2 = 0;
    encoder3 = 0;
  }
}

void spinRight_Encoder(int spinValue) {
  while(encoder2 < spinValue && encoder2 != 0) {   //Encoder value of 24 gives an approximate 90 degree turn.
    Motor2->run(BACKWARD);     //LEFT TURN
    Motor3->run(FORWARD);
  }
  if(encoder2 != 0) {   
    Motor2->run(FORWARD);
    Motor3->run(BACKWARD);
    delay(10);
    Motor2->run(RELEASE);
    Motor3->run(RELEASE);
    encoder2 = 0;
    encoder3 = 0;
  }
}
