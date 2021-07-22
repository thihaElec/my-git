
/*
  created by Kyi Zaw Win 24/12/2020
  Lab no 10
  
*/
int lightPin = A6; // Analog sound sensor is to be attached to analog
int ledPin = 4; // Digital LED is to be attached to digital
int lightlevel = 300;

// the setup function runs once when you press reset or power the board
void setup() {
   pinMode(ledPin, OUTPUT);
   Serial.begin(9600); 
}
// the loop function runs over and over again forever
void loop() {
   int lightState = analogRead(lightPin); // Read sound sensor’s value
   Serial.println(lightState);
   if (lightState > lightlevel) {
   digitalWrite(ledPin, HIGH);
   delay(100);
   }else{
   digitalWrite(ledPin, LOW);
   } 
}
