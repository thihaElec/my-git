#define LEDpin 2 //on board blueLED
void setup() {
  // put your setup code here, to run once:
  pinMode(LEDpin,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LEDpin,HIGH);
  delay(1000);
  digitalWrite(LEDpin,LOW);
  delay(1000);
}
