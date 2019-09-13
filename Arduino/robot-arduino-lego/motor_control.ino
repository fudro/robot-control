/*************************
 * This code provides motor functions.
 * 
 * NOTES
 * The motor control code uses interrupts to track motor rotation using opto-interrupter encoders.
 * The encoder disc and mounting structure are handmade.
 * 
 * References: 
 * https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
 * https://youtu.be/oQQpAACa3ac
 ************************/
//#define DEBUG

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
//Returns a rotation value based on the number of encoder ticks. Adjust values to calculate encoder output in different formats (e.g. RPM).
void ISR_timerone() {
  Timer1.detachInterrupt();   //Stop Timer1 to allow time for serial print out
  float rotation2 = (encoder2 / slots) * 1.00;   //Calculate RPM: Since Timer1 is set for 1 second, we get the number of slot ticks per second. Divide this by number of slots on the encoder disc to get number of revolutions per second. Then, multiply by 60 to get revolutions per minute.
  float rotation3 = (encoder3 / slots) * 1.00;
#ifdef DEBUG
  Serial.print("Encoder 2: ");
  Serial.print(encoder2);
  Serial.print(" ");
  Serial.print("Encoder 3: ");
  Serial.print(encoder3);
  Serial.println(" ");
#endif
  Timer1.attachInterrupt(ISR_timerone);
}

 /*************MOVEMENT FUNCTIONS*************/
void moveForward(int moveValue) {
  if (moved == false) {
    encoder2 = 0; //Reset encoders before any new movement
    encoder3 = 0;
    while(encoder2 < moveValue) {   //Robot travel distance is about half of the moveValue (adds approximately 1cm of additional travel).
      Motor2->run(FORWARD);
      Motor3->run(FORWARD);
    }
    if(encoder2 != 0) {             //Apply brakes by briefly reversing motor direction.
      Motor2->run(BACKWARD);
      Motor3->run(BACKWARD);
      delay(10);
      Motor2->run(RELEASE);
      Motor3->run(RELEASE);
      encoder2 = 0;
      encoder3 = 0;
    }
     moved = true;
  }
}

void spinLeft_Encoder(int spinValue) {
  if (moved == false) {
    encoder2 = 0; //Reset encoders before any new movement
    encoder3 = 0;
    while(encoder3 < spinValue) {   //spinvalue of 21 gives an approximate 90 degree turn on the test robot.
      Motor2->run(FORWARD);     //LEFT SPIN
      Motor3->run(BACKWARD);
    }
    if(encoder3 != 0) {
      Motor2->run(BACKWARD);
      Motor3->run(FORWARD);
      delay(10);
      Motor2->run(RELEASE);
      Motor3->run(RELEASE);
      encoder2 = 0;
      encoder3 = 0;
    }
    moved = true;
  }
}

void spinRight_Encoder(int spinValue) {   //spinvalue of 21 gives an approximate 90 degree turn on the test robot.
  if (moved == false) {
    encoder2 = 0; //Reset encoders before any new movement
    encoder3 = 0;
    while(encoder2 < spinValue) {   
      Motor2->run(BACKWARD);     //RIGHT SPIN
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
    moved = true;
  }
}
