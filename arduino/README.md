The project mainly uses the `covenant-wifi` code.


# Required hardware
- Arduino Duemilanove/Diecimila, or a similar clone
- MQ-7 sensor
- ESP-12E or ESP-07E WiFi shield
    - The SUART interface in assumed to be at pins 2 & 3
- Any computer fan
- 2N3904 transistor
- Any diode
- Resistor (refer to fan circuit below for required resistance)
- Male-to-female and male-to-male wires
- USB male-to-male type A-to-B cable (usual printer cable)


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
  - `GND` to the Arduino's `GND`
  - `OUT` to analog pin 0
- Calibrate the MQ-7: refer to **Calibrating the sensor**
- Arrange the fan's circuit like this:

  ![circuitimage](https://i.imgur.com/sNUUjAt.png)
  - `R` is the resistance, `W` is the fan's wattage, and `V` is the external voltage source
    - Make sure to connect the external ground to the Arduino's ground
  - `Pin A5` is plugged into the Arduino's analog pin 5
  - `R = (5 - 0.7) / ((W/V) / 10)`
    - **9V source voltage, 0.75 fan wattage**: R ~= 300 ohms
- Plug the following LEDs:
  - Green (safe) - pin 4
  - Yellow (warning) - pin 5
  - Orange (danger) - pin 6
  - Red (critical) - pin 7
  - `<any color>` (network connected) - pin 8


# Running and monitoring code
- Plug the Arduino to the computer with a USB male-to-male type A-to-B cable
- Open the Arduino IDE and select the board, processor, & port
- Press Ctrl-Shift-M to open the Serial Monitor
- In the Serial Monitor window, select `9600 baud` in the dropdown in the bottom toolbar


# Calibrating the sensor
- Open `arduino/mq7-calibrator/mq7-calibrator.ino` in the Arduino IDE
- Plug the Arduino (with the MQ-7 sensor) to the computer, then upload the code
    - Make sure the sensor is in an area with relatively clean air
- Press Ctrl-Shift-M to open the Serial Monitor
- In the Serial Monitor window, select `9600 baud` in the dropdown in the bottom toolbar
- Let the code run for some time (at least 30 minutes), then use the last output as the `R0` value for the main program
- Open `arduino/mq7-tester/mq7-tester.ino` in the Arduino IDE
- Plug the Arduino (with the MQ-7 sensor) to the computer, then upload the code
- Press Ctrl-Shift-L to open the Serial Plotter
- In the Serial Plotter window, select `9600 baud` in the dropdown in the bottom toolbar
- Test the sensor by exposing it to different levels of CO, while noting the value displayed in the plotter for each exposure
    - Note the values before running the main program: **the threshold changes depending on the temperature & humidity of the environment**
    - If the sensor value doesn't change much by blowing over it, check for some misplaced wiring and/or a broken MQ-7 sensor


# Setting up the code
- Open `arduino/covenant-wifi/covenant-wifi.ino` in the Arduino IDE
- Set the following variables with the actual applicable values:
    - `ssid` & `pass` - details of the WiFi network to connect to
    - `host` & `port` - hostname/IP & port number of the server running Rails
    - `readInterval` - how often the board will check the sensor value
        - This affects everything that depends on the sensor value
    - `reconnectTimeout` - how often the board will try to reconnect to WiFi when it loses the connection
    - `maxTries` - how many consecutive times to try to reconnect to WiFi when it loses the connection before waiting again
        - Lowering this decreases the delay caused by the reconnection attempts, but also decreases the chance of reconnecting successfully
    - `R0` - the clean-air base value for the sensor
        - Refer to **Calibrating the sensor** for setting the value
    - `threshold` - the critical sensor value (in ppm) for turning the fan on/off
        - Calibrated by testing the MQ-7 sensor with varying CO levels
        - Depends on the use case: this is set to `25` for this project
    - `usePing` - whether to use ping to check the Internet connection or not
    - `pingServer` - which server to ping when `usePing` is true
        - This is set to "1.1.1.1" by default
	- `debug_cov` - whether to send debugging serial messages or not
		- Set this to `false` on deployment, since extra serial messages can slow down the communication between the Arduino & the WiFi module
- Plug the Arduino (with the MQ-7 sensor) to the computer, then upload the code


v0.1.8
