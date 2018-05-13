/*
  reads raw analog data from the sensor
*/

/*
 * normal: 500-ish
 * raw smoke: 800-ish
 */

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  analogWrite(A1, 1023);
  analogWrite(8, 1023);
  for (int i = 0; i<600; i++) {
    Serial.println(analogRead(A0));
    delay(100);
  }
  analogWrite(A1, (1023.0/5.0)*1.4);
  analogWrite(8, 0);
  for (int i = 0; i<900; i++) {
    Serial.println(analogRead(A0));
    delay(100);
  }
}
