/*************************
   This code provides the core features for a small test robot.
   Features include:
    Communication (Bluetooth)
    Motor Control (DC Motor x 2, servo motor x 2 - controlled via PixyCam)
    Sensing (sonar, camera - PixyCam)
    Feedback and Output (Speaker, LEDs)
    Navigation (9-axis IMU, custom wheel encoders)

   (Refer to individual tabs for the relevant references.)
 ************************/
#include "TimerOne.h"     //TimerOne library is used in the interrupt service routine (ISRs) for the encoders
#include <Adafruit_MotorShield.h>   //Using the Adafruit Motorshield V2.3


// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Connect a DC motor to port M2 (refer to the PCB screenprint to determine correct ports)
Adafruit_DCMotor *Motor2 = AFMS.getMotor(2);
// Connect a DC motor to port M3
Adafruit_DCMotor *Motor3 = AFMS.getMotor(3);

//Assign hardware interrupt pins to each encoder.
const byte    ENCODER2 = 2;  //Right Motor
const byte    ENCODER3 = 3;  //Left Motor
//Assign variables to track each encoder
//IMPORTANT: Variables that need to be modified within an ISR MUST be declared as "volatile". See Arduino reference: https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
volatile int  encoder2 = 1; //count encoder disc "ticks" each time the encoder circuitry sends an interrupt pulse.
volatile int  encoder3 = 1; //TODO: Add note about why encoders start at value of 1.
float         slots = 1.00; //Number of slots in encoder disc. Used to track RPM and calculate speed. Set to value of 1 if just counting raw "ticks" from the encoder.
bool          moved = false;  //Track whether the motor movement has completed.
int           left_speed = 255;

void setup() {
  Serial.begin(115200);
  Serial.println("Robot Start!");

  AFMS.begin();  // start motorshield object with the default frequency of 1.6KHz
  // turn on motor M2
  Motor2->setSpeed(255);    //Max motor speed is 255.
  Motor2->run(RELEASE);
  // turn on motor M3
  Motor3->setSpeed(255);
  Motor3->run(RELEASE);

  pinMode(ENCODER2, INPUT_PULLUP);  //Activate pullup resistors on the interrupt pins
  pinMode(ENCODER3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODER2), ISR_encoder2, RISING);   //Attach interrupt service routines to hardware interrupt pins and set trigger mode.
  attachInterrupt(digitalPinToInterrupt(ENCODER3), ISR_encoder3, RISING);
  Timer1.initialize(1000000);   //Timer1 accepts parameter in microseconds. Set to one million for 1 second.
  Timer1.attachInterrupt(ISR_timerone);   //Set the ISR to be triggered by the timer
}

void loop() {
  delay(3000);
  moveForward(100);
//  checkCommands();
}
