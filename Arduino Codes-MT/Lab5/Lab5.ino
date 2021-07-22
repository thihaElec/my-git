
/*
  created by Kyi Zaw Win 8/12/2020
  Lab no 5
  
*/
int rotaryPin = A0; // select the input pin for the rotary

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  
}

// the loop function runs over and over again forever
void loop() {
  Serial.println(analogRead(rotaryPin)); 
  delay(500);
}
