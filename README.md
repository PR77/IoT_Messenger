# IoT_Messenger
IoT Messenger (Pager) For Simple 1 Way Communication

# Warning
This design has not been compliance tested and will not be. It is published purely for inspiration to others. I take no responsibility for damages caused directly or indirectly. I accept no responsibility for any damage to any equipment that results from the use of this design and its components. IT IS ENTIRELY AT YOUR OWN RISK!

# Overview
As a child growing up in the 80's I have fond memories of old skool pagers and analog mobile phones the size of bricks. I remember buying a bunch of old analog Motorola (R) Brick Phones just before the analog network was shut down and turning them into scanners. This project is in no way connected to that, but the fond memory of pagers is something I wanted to bing back with this project. In addition to this, my other motivation for the project was it's "semi" practical use as a real pager within a known WiFi network.

## Basic Concept
My IoT Messenger is essentially some common components connected to an ESP8266, ESP12-F to be specific. However, it is the form factor and user interface which draws parallels to a pager. It is my first revision of the project and has a lot fo fine tuning - should I or the community want to pursue it further. I would like to continue improving this as I see it would be fun for those who use it.

Interfacing with the device is via the Web using the simple HTTP Server class. The IoT Messenger sends out a webpage where the message and other functions can be accessed. As my HTML and JS skills are not up there, I have resorted to using CURL during development and debugging - HELP NEEDED! :)

Here is a nice 3d model of the PCB;

### Appearance
Nice 3D model:
![3DModel](/Images/IoTMessenger.png)

### Basic Interfaces
My IoT Messenger needs to be portable therefore battery power and a basic set of controls are essential. I use a 600 mA LiPo cell which is charged via USB. USB provides power only. No intelligent power management has been implemented yet, so when running only via battery it last 4 - 5 hours.

There are 4 buttons for user input which can also be used for additional functions (see how Tetris and Dino game use them).

### Power Supply
I use the MCP73831 LiPo battery charger with a P-Channel load balancing FET. This is to ensure the LiPo charger only charges the battery when connected to a USB power source. The necessary 3.3 volts is obtained via a basic LM1117-3.3.

### Sensors And Indicators
I wanted to have the PCB somewhat universal - I guess like an evaluation or child friendly development board - so I added a nice RGB LED (PL9823) and an environmental sensor (DHT11). In addition to these, as the device is inspired by pagers, it has a OLED display (128x32) and Piezo Beeper.

As mentioned, flexibility was important so with the OLED, it is essentially dual-footprinted where a module with necessary passives could just be solder on, or the raw OLED panel and necessary passives be soldered on the IoT Messenger board. For my prototype I used the latter so get some experience with this. Lastly, I added a SAO port, well for - Add Ons.

Nice fully soldered IoT Messenger:
![FullySoldered](/Images/FullySoldered.jpg)

### Server
All of the interfacing with the hardware features is via HTTP Server GET and POST requests. The following services have been implemented currently;

|Request |Type|Parameters|Comments                              |
|--------|----|----------|--------------------------------------|
|/list   |GET |none      |Lists files currently saved to SPIFFS |
|/upload |POST|filename  |Uploads a file to SPIFFS              |
|/format |POST|none      |Formats SPIFFS                        |
|/info   |GET |none      |Get SPIFFS and CPU MHz info           |
|/sensor |GET |none      |Read environmental sensor data (DHT11)|
|/message|POST|text      |Write a message to the buffer         |
|/led    |POST|colour    |Set RGB LED colour                    |
|/beeper |POST|count     |Beep piezo beeper                     |

### Fun Stuff
Of course. Check out my IoT Messenger playing the Tetris game theme music or Dino Jump. Videos are in the Images folder.

## Open Sources Used
PlatformIO is the main development environment. In addition to the Arduino framework for ESP8266, I used the following (either important as libraries into PIO or seperate);

1. OLED library (https://github.com/ThingPulse/esp8266-oled-ssd1306)
2. ClickButton library (https://github.com/pkourany/clickButton)
3. RGB LED library (https://github.com/Makuna/NeoPixelBus)
4. Tetris theme song (https://dragaosemchama.com/en/2019/02/songs-for-arduino/)
5. Environmental sensor library (https://github.com/beegee-tokyo/DHTesp)
