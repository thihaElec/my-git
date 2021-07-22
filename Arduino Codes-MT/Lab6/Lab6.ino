/*
  created by Kyi Zaw Win 8/12/2020
  Lab no 7
  
*/
int BuzzerPin = 5;
int TONEdelay = 200;

// the setup function runs once when you press reset or power the board
void setup() {
  int ii;
  pinMode(BuzzerPin, OUTPUT); 

  for(ii=0;ii<200;ii++)
  {
    digitalWrite(BuzzerPin, HIGH);
    delayMicroseconds(TONEdelay);
    digitalWrite(BuzzerPin, LOW);
    delayMicroseconds(TONEdelay);
  }

}

// the loop function runs over and over again forever
void loop() {
}
