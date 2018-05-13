/*
 * https://github.com/R2D2-2017/R2D2-2017/wiki/MQ-7-gas-sensor
 */


/* R0 values: probably 0.07
 */
 

double avg = 0;
int count = 0;

void setup() {
    Serial.begin(9600);
}

void loop() {
    double sensor_volt = 0;
    double RS_air = 0;
    double R0 = 0;
    double sensorValue = 0;

    for (int i = 0; i<600; i++) {
      sensorValue = sensorValue + analogRead(A0);
      delay(100);
    }
    sensorValue = sensorValue/100;
    
    sensor_volt = (sensorValue/1024.0) * 5.0;
    RS_air = (5.0 - sensor_volt)/sensor_volt;
    R0 = (RS_air/26.0);

    count += 1;
    avg = ((avg * (count - 1)) + R0) / count;

    Serial.println(avg);
}
