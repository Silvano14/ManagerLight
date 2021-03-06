# LightManager
- - -
A simple project to manage lights by arduino with wifi module and [Blynk](https://www.blynk.cc/). You can set a countdown for a light (default is one minute) or 
receive a notification in the phone (by [Blynk](https://www.blynk.cc/) app) when the light is turned on.

## Prerequisites

This project works with the following libraries:

- [BlynkSimpleWiFiNINA](https://github.com/blynkkk/blynk-library/blob/master/src/BlynkSimpleWiFiNINA.h)
- [Rtczero](https://www.arduino.cc/reference/en/libraries/rtczero/)
- [SPI](https://www.arduino.cc/en/reference/SPI)

Some of these libraries are not mandatory, you can change it if you use a different hardware (For example rtczero is only for Arduino zero, MKR1000 and MKR1010).

Hardware components:

- [PIR](https://lastminuteengineers.com/pir-sensor-arduino-tutorial/)
- [Relay](https://arduinogetstarted.com/tutorials/arduino-relay) 

Your edits:

- Create a credentials.h file and put your credentials for wifi and blynk connection.

## Contributing

Nobody :'(

## Authors

Silvano Norberti - [LightManager](https://github.com/Silvano14/LightManager)
See also the list of contributors who participated in this project.

## License
This project is licensed under the GPL 3.0 License - see the [LICENSE](https://github.com/Silvano14/ManagerLight/blob/master/LICENSE) file for details