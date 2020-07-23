/*************************
 * This code provides motor functions.
 * 
 * NOTES
 * The motor control code uses interrupts to track motor rotation using opto-interrupter encoders.
 * The encoder disc and mounting structure are handmade.
 * 
 * References: 
 * https://youtu.be/oQQpAACa3ac
 ************************/
//#define DEBUG

/**************************
INTERRUPT SERVICE ROUTINES
**************************/
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
  float rotation2 = (encoder2 / slots) * 1.00;    //Calculate RPM: When Timer1 is set for 1 second, the encoder varible returns the number of slot ticks per second. Divide this value by the number of slots on the encoder disc to get number of revolutions per second. Then, multiply by 60 to get revolutions per minute.
  float rotation3 = (encoder3 / slots) * 1.00;    //To get raw encoder ticks, set "slots" to 1 and multiply by 1
#ifdef DEBUG
  Serial.print("Encoder 2: ");
  Serial.print(encoder2);
  Serial.print(" ");
  Serial.print("Encoder 3: ");
  Serial.print(encoder3);
  Serial.println(" ");
#endif
  Timer1.attachInterrupt(ISR_timerone);   //Reattach interrupt to restart timer
}

 /*************MOVEMENT FUNCTIONS*************/
void moveForward(int moveValue) {
  if (moved == false) {   //flag to only perform movement once (since function is repeatedly called in the main loop)
    int left_speed = 255;   //set both motors to max speed
    int right_speed = 255;
    encoder2 = 0;   //Reset encoders before any new movement
    encoder3 = 0;
    while(encoder2 < moveValue) {   //wait for encoder to reach desired travel distance
      Motor2->run(FORWARD);
      Motor3->run(FORWARD);
      Serial.print("Right: ");
      Serial.print(encoder2);
      Serial.print("\t");
      Serial.print("Left: ");
      Serial.println(encoder3);
      delay(100);
    }
    if(encoder2 != 0) {         //If robot has moved...
      Motor2->run(BACKWARD);    //Apply brakes by briefly reversing motor direction.
      Motor3->run(BACKWARD);
      delay(10);
      Motor2->run(RELEASE);
      Motor3->run(RELEASE);
      delay(100);   //pause to make sure robot has come to rest.
      encoder2 = 0;   //Reset encoders to show that robot is not moving
      encoder3 = 0;
    }
     moved = true;
  }
}

void spinLeft_Encoder(int spinValue) {
  if (moved == false) {
    encoder2 = 0; //Reset encoders before any new movement
    encoder3 = 0;
    while(encoder3 < spinValue) {   //spinvalue of 21 gives an approximate 90 degree turn.
      Motor2->run(FORWARD);     //LEFT SPIN
      Motor3->run(BACKWARD);
    }
    if(encoder3 != 0) {
      Motor2->run(BACKWARD);
      Motor3->run(FORWARD);
      delay(10);
      Motor2->run(RELEASE);
      Motor3->run(RELEASE);
      delay(100);   //pause to make sure robot has come to rest.
      encoder2 = 0;
      encoder3 = 0;
    }
    moved = true;
  }
}

void spinRight_Encoder(int spinValue) {   //spinvalue of 21 gives an approximate 90 degree turn.
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
      delay(100);   //pause to make sure robot has come to rest.
      encoder2 = 0;
      encoder3 = 0;
    }
    moved = true;
  }
}
