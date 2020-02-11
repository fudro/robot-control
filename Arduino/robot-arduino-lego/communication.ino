/*************************
 * This code provides wireless serial communication using an RN-42 bluetooth module.
 * 
 * The robot can receive serial commands consisting of a function name and a 3 digit argument value passed as a single combined message string.
 * The format of the message string is ["functionName"_"argumentValue"] (e.g. "foobar_123"), where the name of the function and the passed argument value are separated by an underscore character ("_").
 * Relevant ASCII values: 95 = underscore ("_"), 13 = cariage return (CR), 10 = new line/line feed (LF).
 * **********************/


void checkCommands(){
  String  t;
  String  commandName;
  char    onesPlace = 0;
  char    tensPlace = 0;
  char    hundredthsPlace = 0;
  int     argument;
  
  while(Serial.available()) {
    t += (char)Serial.read();
  }
  if(t.length()) {
    Serial.print("Total String Length: ");
    Serial.println((int)t.length());          //This statement prints a value that is always 2 more than the number of characters entered. This accounts for the "\r\n" (carriage return and new line) characters automatically appended to each message.
    Serial.print("Command Message: ");
    int separatorIndex = 0;                   //Reset the index position of the separator character between the function name and the passed argument value.
    for (int i = 0; i < t.length(); i++ ) {   //Search the string for the separator character (underscore "_"). Sequentially print each character of the entire message up to the separator character (underscore "_")
      Serial.print(t[i]);                     //Print each read character (including underscore separator but NOT including carriage return and new line).
      if (t[i] != 95) {                       //If the read character is NOT the separator: 95 = underscore ("_").
        if (separatorIndex == 0) {            //Only record command characters up until the separator has been found.
          commandName += (char)t[i];          //Add the read character to the command string.
        }
      }
      else if (t[i] == 95) {                  //If the read character IS the separator...
        separatorIndex = i;                   //Store the index of the separator character. This value is zero-indexed (starts from zero).
      }
    }
    Serial.print("separatorIndex: ");
    Serial.println(separatorIndex);
    Serial.print("Command Name: ");
    Serial.println(commandName);
    
    if (separatorIndex > 0) {                 //Check if the separator character was found in the message. This means that the target function of the message requires a parameter.
      for (int j = separatorIndex + 1; j < t.length(); j++) {
        if (t[j] == 13) {                                           //Look for the end of the argument value by detecting the carriage return (13).
          Serial.print("CR Index: ");
          Serial.println(j);
          if (j - separatorIndex > 1 && j - separatorIndex < 5) {   //Check if the argument has 1-3 digits.
            if (j-1 != separatorIndex) {                            //Store the available place values of the argument
              onesPlace = t[j-1];
              if (j-2 != separatorIndex) {                          //Conditionals are nested so that you can only have a tens place digit if you had a ones place digit and so on.
                tensPlace = t[j-2];
                if (j-3 != separatorIndex) {
                  hundredthsPlace = t[j-3];
                }
              }
            }
            //TODO:
            //Once all of the place values have been assigned, convert them to integers and add together.
            //Store the calulated argument vlaue
          }
          else {
            Serial.println("Invalid Argument");   //Throws for too many arguments AND no arguments.
            Serial.println();
          }
          break;                                  //Stop the loop once the carriage return is found.
        }
      }
      if (onesPlace != 0) {                       //if the argument has at least one digit, print the combined value
        Serial.println("Argument Value: "); 
        if (hundredthsPlace != 0) {
          Serial.print(hundredthsPlace);
        }
        if (tensPlace != 0) {
          Serial.print(tensPlace);
        }
        Serial.println(onesPlace);
        Serial.println();
      }
      //TODO:
      //Compare command string to all available commands that accept a parameter
      //Check command string against known commands and run the matching case
      //Each command case has the function call with the argument variable as the passed value.
    }
    else {                                        //If no separator is found in the message, compare with the commands DO NOT accept parameters.
      //TODO: Check non parameter commands
//    if(t == "hello\r\n") {
//      Serial.print("hi\n");
//    }
//    else if (t == "name\r\n") {
//      Serial.print("Atom\n");
//    }
//    else {
//      Serial.print("say wut\n");
//    }
    }
  }
  delay (20);
}
