/*
 * https://github.com/R2D2-2017/R2D2-2017/wiki/MQ-7-gas-sensor
 */

// house:       #define R0 0.07
#define R0 0.07

double rsr0_ppm(double r) {
    return 50.0 * pow((1.667 / r), 1.0/0.6661);
}

void setup() {
    Serial.begin(9600);
}

void loop() {
    double sensor_volt = 0;
    double RS_gas = 0;
    double ratio = 0;
    double sensorValue = 0;

    sensorValue = analogRead(A0);
    sensor_volt = (sensorValue/1024)*5;
    RS_gas = (5.0-sensor_volt)/sensor_volt;
    ratio = RS_gas/R0 ;
    double ppm = rsr0_ppm(ratio);
    Serial.println(ppm);
    delay(50);
}
