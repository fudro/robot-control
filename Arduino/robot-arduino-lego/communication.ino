/*************************
 * This code provides wireless serial communication using an RN-42 bluetooth module.
 * 
 * **********************/


void checkCommand(){
  String t;
  while(Serial.available()) {
    t += (char)Serial.read();
  }
  if(t.length()) {
    if(t == "hello\r\n") {
      Serial.print("hi\n");
    }
    else if (t == "name\r\n") {
      Serial.print("Atom\n");
    }
    else {
      Serial.print("say wut\n");
    }
  }
  delay (20);
}
