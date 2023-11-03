# Remote Antenna Polarisation Switch

M5Stack touchscreen to remotly switch polarizations for XQuad or Crossed Yagi antennas.

![](https://raw.githubusercontent.com/fg8oj/RemotePolarisationSwitch/main/wimo-polarization-switch.jpg)
![](https://raw.githubusercontent.com/fg8oj/RemotePolarisationSwitch/main/M5Stack_relaybox.jpg)

Using :
- 2 x Wimo Remote Polarisation switch : https://int.wimo.com/en/catalogsearch/result/?q=Remote+Polarisation+switch (172.27€ to 205.88€)
- M5Stack Core2 ESP32 IoT Development Kit : https://shop.m5stack.com/products/m5stack-core2-esp32-iot-development-kit ($46.90)
- 4 relay Module Grove 103020133 https://www.gotronic.fr/art-module-4-relais-grove-103020133-28522.htm (10,50 €)

![](https://raw.githubusercontent.com/fg8oj/RemotePolarisationSwitch/main/m5polar.png)

Install : 
Import at least 2 files in your Arduino gui project : 
- RemotePolarizationSwitch.ino
- index.html.h

Needed libraries : 
- M5Core2-master  https://github.com/m5stack/M5Core2
- Multi_Channel_Relay_Arduino_Library-master  https://github.com/Seeed-Studio/Multi_Channel_Relay_Arduino_Library

To do list : 
- NTP sync at boot time, have to update it regularly

Version 1.0 :
- Wifi setup process with AP Mode
- Web remote service to switch remotely via a web navigator
- NTP Server Clock
- 60s screen saver

Version 0.3 :
- screensaver bug fixed
- remote web page added

Version 0.2 :
- wifi AP mode for SSID/password configuration
- NTP clock
- screensaver 
- web server created for remote access (in progress)
- freefont usage to enhance nicer fonts

Version 0.1 :
- basic design with remote I2C grove relay switch

