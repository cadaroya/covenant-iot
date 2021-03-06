# Required hardware
- Arduino Duemilanove/Diecimila, or a similar clone
- MQ-7 sensor kit
  - If the kit doesn't automatically vary the voltage, follow [this guide](http://www.instructables.com/id/Arduino-CO-Monitor-Using-MQ-7-Sensor/)
- ESP-12E or ESP-07E WiFi shield
- Any computer fan
- 2N3904 transistor
- Any diode
- Resistor (refer to fan circuit below for required resistance)
- Male-to-female wires
- USB male-to-male type A-to-B cable (printer cable)


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
- Open `<Sketchbook Location>/libraries/WiFiEsp-master/src/utility/EspDrv.cpp` in a text editor
- At lines **805**, **834**, & **874**, change `int idx = readUntil(1000` to `int idx = readUntil(8000`
  - This is needed to avoid (or at least minimize) [this error](https://github.com/bportaluri/WiFiEsp/issues/111)

# Setting up the hardware
- Plug the ESP-12E WiFi shield over the Arduino
- Plug the MQ7 sensor kit to the Arduino using male-to-female pins:
  - `VIN` to the Arduino's `5V`
  - `GND` to `GND`
  - `OUT` to analog pin 0
- Calibrate the MQ-7:
  - Leave the MQ-7 kit and the Arduino running for at least 24 hours in relatively clean air
- Arrange the fan's circuit like this:

  ![circuitimage](https://i.imgur.com/sNUUjAt.png)
  - `R` is the resistance, `W` is the fan's wattage, and `V` is the external voltage source
  - `Pin A5` is plugged into the Arduino's analog pin 5
  - `R = (5 - 0.7) / ((W/V) / 10)`
- Plug the following LEDs:
  - Green (safe) - pin 4
  - Yellow (warning) - pin 5
  - Orange (danger) - pin 6
  - Red (critical) - pin 7
  - `<any color>` (network connected) - pin 8

# Setting up the actual code
- Open `covenant-arduino/covenant-arduino.ino` in the Arduino IDE
- Set the following variables with the actual applicable values:
    - `ssid` & `pass` - details of the WiFi network to connect to
    - `host` & `port` - hostname/IP & port number of the server running Rails
    - `postingInterval` - how often the board will send data to the server
    - `readInterval` - how often the board will check the sensor value
      - this affects everything that depends on the sensor value
    - `reconnectTimeout` - how often the board will try to reconnect to WiFi when it loses the connection
    - `maxTries` - how many consecutive times to try to reconnect to WiFi before waiting again
        - lowering this decreases the delay caused by the reconnection attempts, but also decreases the chance of reconnecting successfully
    - `threshold` - the critical sensor value for turning the fan on/off
       - calibrated by testing the MQ-7 sensor with different CO levels
    - `usePing` - whether to use ping to check the server's status
        - disable this if the server doesn't respond to ping requests
	- `debug_cov` - whether to send debugging serial messages or not
		- set this to `false` on deployment, since extra serial messages can slow down the communication
between the Arduino & the WiFi module 

# Running (and monitoring) the code
- Plug the Arduino to the computer with a USB male-to-male type A-to-B cable
- Open the Arduino IDE and select the board, processor, & port
- Press Ctrl-Shift-M to open the Serial Monitor
- In the Serial Monitor window, select `9600 baud` in the dropdown in the bottom toolbar

v0.1.5
