
/*
  created by Kyi Zaw Win 8/12/2020
  Lab no 4
  
*/

int soundPin = A2; // Analog sound sensor is to be attached to analog
int ledPin = 4; // Digital LED is to be attached to digital
int soundlevel = 700;

// the setup function runs once when you press reset or power the board
void setup() {
   pinMode(ledPin, OUTPUT);
   Serial.begin(9600); 
}

// the loop function runs over and over again forever
void loop() {
   int soundState = analogRead(soundPin); // Read sound sensor’s value
   Serial.println(soundState);
   // if the sound sensor’s value is greater than 200, the light will be on for 5 seconds.
   //Otherwise, the light will be turned off
   if (soundState > soundlevel) {
   digitalWrite(ledPin, HIGH);
   delay(100);
   }else{
   digitalWrite(ledPin, LOW);
   } 
}
