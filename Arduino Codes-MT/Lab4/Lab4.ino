
/*
  created by Kyi Zaw Win 8/12/2020
  Lab no 4
  
*/
const int buttonPin = 6; // the number of the pushbutton pin
const int ledPin = 4; // the number of the LED pin

// the setup function runs once when you press reset or power the board
void setup() {
 // initialize the LED pin as an output:
 pinMode(ledPin, OUTPUT);
 // initialize the pushbutton pin as an input:
 pinMode(buttonPin, INPUT); 
}

// the loop function runs over and over again forever
void loop() {

  if (digitalRead(buttonPin) == HIGH)
  {
    digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  else
  {
    digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
  }
  
}
