#define A2Dpin 25
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("A2D TEST PROGRAM ");
  pinMode(A2Dpin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Analog value : "); 
  Serial.println(analogRead(A2Dpin));
  delay(1000);
}
