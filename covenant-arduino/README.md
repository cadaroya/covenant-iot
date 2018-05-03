# Required hardware
- Arduino Duemilanove/Diecimila, or a similar clone of one of those
- MQ-7 sensor kit (not just the sensor)
- ESP-12E or ESP-07E WiFi shield
- Any computer fan
- 2N3904 transistor
- Any diode
- Resistor (based on formula in linked guide below)
- Male-to-female wires
- Male-to-male wires


# Setting up the Arduino IDE and drivers
- Download the [Arduino IDE](https://github.com/e-Gizmo/Arduino-1.8.5-IDE-Windows/archive/master.zip)
- Extract the `.zip`
- Install the driver from `/drivers/PL2303 Drivers.zip`
- Run the Arduino IDE
  - For the ACEduino:
     - Select `Tools/Board/Arduino Duemilanove or Diecimila`
     - Select `Tools/Processor/ATmega328P`
     - Select `Tools/Port/<whichever port the ACEduino is plugged into>`
- Download the [WiFiEsp library](https://github.com/bportaluri/WiFiEsp/archive/master.zip)
- Go to `Sketch/Include Library/Add .ZIP Library`
- Select `WiFiEsp.zip`

# Setting up the hardware
- Plug the ESP-12E *WiFi shield* over the Arduino
- Plug the MQ7 sensor kit to the Arduino using male-to-female pins:
  - `VIN` to the Arduino's `5V`
  - `GND` to `GND`
  - `OUT` to analog pin 0
- Calibrate the MQ-7 by adjusting the potentiometer
- Arrange the fan's circuit using [this guide](https://electronics.stackexchange.com/questions/137753/arduino-transistor-to-switch-pc-fan-on-off)

# Setting up the actual code
- Open `covenant-arduino/covenant-arduino.ino` in the Arduino IDE
- Set the following variables with the actual applicable values:
    - `threshold` - calibrated by testing the MQ-7 sensor with & without CO
    - `ssid` & `pass` - depends on the WiFi network it'll connect to
    - `host` - hostname/IP of the server running Rails & Ionic
    - `postingInterval` - how often it'll send data to the server

v0.0.5
