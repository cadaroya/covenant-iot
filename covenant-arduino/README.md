# Required hardware
- Arduino Duemilanove/Diecimila, or a similar clone of one of those
- MQ-7 sensor **kit**
  - If the kit doesn't automatically vary the voltage, 
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
- Go to `Sketch/Include Library/Add .ZIP Library` in the IDE
- Select `WiFiEsp.zip`
- Go to `File/Preferences` and note the `Sketchbook Location`
- Open `<Sketchbook Location>/libraries/WiFiEsp-master/src/utility/debug.h` in a text editor
- Add `#define _ESPLOGLEVEL_ 2` immediately below `#include <stdio.h>`
  - This is needed to limit the number of debug serial messages sent to just the errors & warnings
  - `#define _ESPLOGLEVEL_ 0` may be used instead for actual deployment

# Setting up the hardware
- Plug the ESP-12E *WiFi shield* over the Arduino
- Plug the MQ7 sensor kit to the Arduino using male-to-female pins:
  - `VIN` to the Arduino's `5V`
  - `GND` to `GND`
  - `OUT` to analog pin 0
- Calibrate the MQ-7:
  - Leave the MQ-7 kit and the Arduino running for at least 24 hours in relatively clean air
- Arrange the fan's circuit using [this guide](https://electronics.stackexchange.com/questions/137753/arduino-transistor-to-switch-pc-fan-on-off)

# Setting up the actual code
- Open `covenant-arduino/covenant-arduino.ino` in the Arduino IDE
- Set the following variables with the actual applicable values:
    - `threshold` - the critical sensor value for turning the fan on/off
       - calibrated by testing the MQ-7 sensor with different CO levels
    - `ssid` & `pass` - details of the WiFi network to connect to
    - `host` & `port` - hostname/IP & port number of the server running Rails
    - `postingInterval` - how often the board will send data to the server
    - `threshTimeout` - how long the fan should stay in its current state (on/off) after the sensor value passes the above/below the threshold
	- `DEBUG_COV` - whether to send debugging serial messages or not
		- set this to `false` on deployment, since extra serial messages can slow down the communication
between the Arduino & the WiFi module 
    - *(debug only)* `readInterval` - how often the board will send the sensor value to the serial

# Running (and monitoring) the code
- Plug the Arduino to the computer with a USB male-to-male type A-to-B cable
- Open the Arduino IDE and select the board, processor, & port
- Press Ctrl-Shift-M to open the Serial Monitor
- In the Serial Monitor window, select `9600 baud` in the dropdown in the bottom toolbar

# Other Notes
- Green Pin is set to pin 4
- Yellow Pin is set to pin 5
- Orange Pin is set to pin 6
- Red Pin is set to pin 7
v0.0.10