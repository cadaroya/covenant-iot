# Setting up the Arduino IDE and drivers
- Download https://github.com/e-Gizmo/Arduino-1.8.5-IDE-Windows/archive/master.zip
- Install the driver from `/drivers/PL2303 Drivers.zip`
- Run `arduino.exe`

# Setting up the Arduino/Gizduino, MQ-7 sensor, & fan
- Plug the ESP-12E WiFi shield to the board
- Plug the MQ7 sensor kit to the Arduino using male-to-female pins
  - `VIN` to the Arduino's `5V`
  - `GND` to `GND`
  - `OUT` to any analog pin in the Arduino
- Calibrate the MQ-7 by adjusting the potentiometer
- Arrange the fan's circuit using [this guide](https://electronics.stackexchange.com/questions/137753/arduino-transistor-to-switch-pc-fan-on-off)
