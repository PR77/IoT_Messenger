#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <FS.h>

#include <OLEDDisplay.h>
#include <OLEDDisplayFonts.h>
#include <OLEDDisplayUi.h>
#include <Wire.h>
#include <SSD1306.h>
#include <SSD1306Wire.h>
#include <NeoPixelBus.h>
#include <NTPClient.h>
#include <DHTesp.h>

#include <clickButton.h>
#include <beeperControl.h>
#include <batteryHistogram.h>

#include "uiGlobal.h"
#include "uiOverlay.h"
#include "uiFrameStatus.h"
#include "uiFrameMessage.h"
#include "uiFrameTime.h"
#include "uiFrameSensor.h"
#include "uiFrameTetris.h"
#include "uiFrameDino.h"
#include "uiFrameTunnel.h"
#include "uiFrameStars.h"
#include "uiFrameBattery.h"
#include "gameDino.h"
#include "gameTetris.h"
#include "gameTunnel.h"
#include "gameStars.h"

//=============================================================================
// Types
//=============================================================================

//=============================================================================
// Hostname, SSID and Password for Wifi router
//=============================================================================
#define HOSTNAME "IoTMessenger"

String staSSID(32);
String staPassword(32);

IPAddress apIPAddress(192,168,1,2);
IPAddress apGatwayAddress(192,168,1,1);
IPAddress apNetmask(255,255,255,0);

//=============================================================================
// Application specific defines and globals
//=============================================================================
const uint32_t blankUXAfterBootUp = 10L * (1000L);  // 10 seconds
const uint32_t blankUXAfterMessage = 10L;           // 10 seconds

static uint32_t lastDhtUpdateTime = 0;
static uint32_t lastLedUpdateTime = 0;

static uiGlobalState_e uiGlobalState = uiGlobalUXActive;
static bool ledNowOff = false;

//=============================================================================
// Global constants for NTP Client
//=============================================================================
const uint32_t utcOffsetInSeconds = 3600;

//=============================================================================
// Global constants for WS2182 interface, buttons, beeper interface, DHT11
//=============================================================================
const uint16_t PixelCount = 1;
const uint8_t PixelPin = 2;

const uint8_t EnterPin = 15;
const uint8_t MenuPin = 14;
const uint8_t UpPin = 12;
const uint8_t DownPin = 13;

const uint8_t BeeperPin = 13;

const uint8_t dhtSensorPin = 0;

const uint8_t DeepSleepPin = 16;

//=============================================================================
// Global objects for Wifi and ESP specifics
//=============================================================================
ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;
File fsUploadFile;

//=============================================================================
// OLED global object (https://github.com/ThingPulse/esp8266-oled-ssd1306)
//=============================================================================
SSD1306 display(0x3C, 4, 5, GEOMETRY_128_32);
OLEDDisplayUi ui(&display);

OverlayCallback overlays[] = { uiOverlay };
FrameCallback frames[] = { uiFrameStatus, uiFrameBattery, uiFrameMessage, uiFrameTime, uiFrameSensor, uiFrameTetris, uiFrameDino, uiFrameTunnel, uiFrameStars };
int overlaysCount = 1;
int frameCount = 9;

messageObject_s uiMessageObject = {"#IoT ... IoT ... IoT", false, false};

//=============================================================================
// Declare DHTxx object (DHT sensor library for ESPx by Bernd Giesecke)
//=============================================================================
DHTesp dht;
TempAndHumidity dhtTempAndHumidity;

//=============================================================================
// Global objects for NTP Client
//=============================================================================
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

//=============================================================================
// Global objects for WS2812 NeoPixelBus object
//=============================================================================
NeoPixelBus<NeoRgbFeature, NeoEsp8266Uart1800KbpsMethod> rgbLed(PixelCount, PixelPin);

RgbColor red(255, 0, 0);
RgbColor green(0, 255, 0);
RgbColor blue(0, 0, 255);
RgbColor black(0, 0, 0);

//=============================================================================
// Global objects for ClickButton object
//=============================================================================
ClickButton enterButton(EnterPin, HIGH);
ClickButton menuButton(MenuPin, LOW, CLICKBTN_PULLUP);
ClickButton upButton(UpPin, LOW, CLICKBTN_PULLUP);
ClickButton downButton(DownPin, LOW, CLICKBTN_PULLUP);

//=============================================================================
// Global objects for beeper object
//=============================================================================
BeeperControl beeper(BeeperPin);

//=============================================================================
// Global objects for battery object
//=============================================================================
BatteryHistogram battery(true);

//=============================================================================
// Global objects for UX
//=============================================================================
uiGlobalObject_s uiGlobalObject = {&uiMessageObject, &dhtTempAndHumidity, &timeClient, &battery};

//=============================================================================
// Function prototypes
//=============================================================================
bool loadFromSpiffs(String path);
void handleRoot(void);
void handleFileList(void);
void handleFileUpload(void);
void handleWebRequests(void);
void handleSensor(void);
void handleMessage(void);
void handleLED(void);
void handleBeeper(void);

//=============================================================================
// Helper function
//=============================================================================

bool loadFromSpiffs(String path) {
    String dataType = "text/plain";
    bool fileTransferStatus = false;

    // If a folder is requested, send the default index.html
    if (path.endsWith("/")) {
        path += "index.htm";
    } else if (path.endsWith(".html")) dataType = "text/html";
    else if (path.endsWith(".htm")) dataType = "text/html";
    else if (path.endsWith(".css")) dataType = "text/css";
    else if (path.endsWith(".js")) dataType = "application/javascript";
    else if (path.endsWith(".png")) dataType = "image/png";
    else if (path.endsWith(".gif")) dataType = "image/gif";
    else if (path.endsWith(".jpg")) dataType = "image/jpeg";
    else if (path.endsWith(".ico")) dataType = "image/x-icon";
    else if (path.endsWith(".xml")) dataType = "text/xml";
    else if (path.endsWith(".pdf")) dataType = "application/pdf";
    else if (path.endsWith(".zip")) dataType = "application/zip";

    File dataFile = SPIFFS.open(path.c_str(), "r");

    if (!dataFile) {
        return fileTransferStatus;
    }
    else {
        if (httpServer.streamFile(dataFile, dataType) == dataFile.size()) {
            fileTransferStatus = true;
        }
    }

    dataFile.close();
    return fileTransferStatus;
}

//=============================================================================
// Webserver event handlers
//=============================================================================
void handleRoot(void) {
    httpServer.sendHeader("Location", "/index.html", true);

    // Redirect to our index.html web page
    httpServer.send(302, "text/plain", "");
}

void handleFileList(void) {
    // curl -X GET IoTMessenger.local/list

    String path = "/";
    String directoryList = "[";
    Dir directoryEntry = SPIFFS.openDir(path);

    while (directoryEntry.next()) {
        File fileElement = directoryEntry.openFile("r");

        if (directoryList != "[") {
            directoryList += ", ";
        }

        directoryList += String(fileElement.name()).substring(1);
        fileElement.close();
    }

    directoryList += "]";
    httpServer.send(200, "text/plain", directoryList);
}

void handleFileUpload(void) {
    // curl -X POST -F "file=@SomeFile.EXT" IoTMessenger.local/upload

    HTTPUpload& upload = httpServer.upload();

    if (upload.status == UPLOAD_FILE_START) {
        String filename = upload.filename;

        if (!filename.startsWith("/")) {
            filename = "/" + filename;
        }

        fsUploadFile = SPIFFS.open(filename, "w");

    } else if (upload.status == UPLOAD_FILE_WRITE) {

        if(fsUploadFile)
            fsUploadFile.write(upload.buf, upload.currentSize);

    } else if (upload.status == UPLOAD_FILE_END) {

        if(fsUploadFile) {
            fsUploadFile.close();
        }
    }
}

void handleWebRequests(void) {
    String message = "File Not Detected\n\n";

    if (loadFromSpiffs(httpServer.uri()))
        return;

    message += "URI: ";
    message += httpServer.uri();
    message += "\nMethod: ";
    message += (httpServer.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += httpServer.args();
    message += "\n";

    for (uint8_t i = 0; i < httpServer.args(); i++) {
        message += " NAME:" + httpServer.argName(i);
        message += "\n VALUE:" + httpServer.arg(i) + "\n";
    }

    httpServer.send(404, "text/plain", message);
}

void handleSensor() {
    // curl -X POST IoTMessenger.local/sensor"

    String dht11Data = String();

    dht11Data = "{\"temperature\":" + String(dhtTempAndHumidity.temperature) + "}";
    dht11Data += "{\"humidity\":" + String(dhtTempAndHumidity.humidity) + "}";

    httpServer.send(200, "text/plain", dht11Data);
}

void handleMessage() {
    // curl -X POST IoTMessenger.local/message?text={TEXT MSG}"

    String messageRequestResponse = String();

    if (httpServer.hasArg("text")) {

        uiMessageObject.receivedMessage = httpServer.arg("text");
        uiMessageObject.newMessageReceived = true;
        uiMessageObject.messageRead = false;

        messageRequestResponse = "{\"success\":1}";

    } else {
        messageRequestResponse = "{\"no message specified\":2}";
    }

    httpServer.send(200, "text/plain", messageRequestResponse);
}

void handleLED() {
    // curl -X POST IoTMessenger.local/led?colour=%23{RRGGBB}"

    String ledRequestResponse = String();

    if (httpServer.hasArg("colour")) {
        HtmlColor pixelColour = HtmlColor();
        pixelColour.Parse<HtmlColorNames>(httpServer.arg("colour"));

        rgbLed.SetPixelColor(0, pixelColour);
        rgbLed.Show();

        ledRequestResponse = "{\"success\":1}";
    } else {
        ledRequestResponse = "{\"no colour specified\":3}";
    }

    httpServer.send(200, "text/plain", ledRequestResponse);
}

void handleBeeper() {
    // curl -X POST IoTMessenger.local/beeper?count={BEEPS}"

    String beeperRequestResponse = String();

    if (beeper.GetBeeperState() == beepHandlerIdle) {
        if (httpServer.hasArg("count")) {
            beeper.RequestBeeper(httpServer.arg("count").toInt());

            beeperRequestResponse = "{\"success\":1}";
        } else {
            beeperRequestResponse = "{\"no count specified\":4}";
        }
    }
    else {
        beeperRequestResponse = "{\"busy\":5}";
    }

    httpServer.send(200, "text/plain", beeperRequestResponse);
}

//=============================================================================
// Setup function
//=============================================================================
void setup() {
    File fsWifiConfig;
    uint32_t apModeStartTime = 0;
    uint8_t connectionProgress = 0;
    bool apMode = false;

    // Initialise serial object
    Serial.begin(115200);

    // Initialize deep sleep pin to allow for wakeup
    pinMode(DeepSleepPin, OUTPUT);
    digitalWrite(DeepSleepPin, LOW);

    // Initialise battery charge state GetBatteryHistogram
    battery.Init();

    // Initialize File System
    SPIFFS.begin();

    // This resets all the neopixels to an off state
    rgbLed.Begin();
    rgbLed.SetPixelColor(0, black);
    rgbLed.Show();

    // Initialise OLED display driver
    display.init();
    display.resetDisplay();
    display.flipScreenVertically();

    // AP if no wifi.config file exists or if UP Button pressed for 1 second.
    if (!SPIFFS.exists("/wifi.conf")) {
        apMode = true;
    } else {
        apModeStartTime = millis();

        while (digitalRead(UpPin) == LOW) {
            if ((millis() - apModeStartTime) >= 1000) {
                apMode = true;
                break;
            }
        }
    }

    if (apMode == true) {
        WiFi.mode(WIFI_AP);
        WiFi.softAPConfig(apIPAddress, apGatwayAddress, apNetmask);
        WiFi.softAP("IoTMsgSetup");

        rgbLed.SetPixelColor(0, blue);
        rgbLed.Show();
    } else {
        fsWifiConfig = SPIFFS.open("/wifi.conf", "r");

        if (!fsWifiConfig) {
            rgbLed.SetPixelColor(0, red);
            rgbLed.Show();
            delay(5000);
            ESP.reset();
        }

        staSSID = fsWifiConfig.readStringUntil(',');
        staPassword = fsWifiConfig.readStringUntil(',');
        fsWifiConfig.close();

        WiFi.setSleepMode(WIFI_NONE_SLEEP);
        WiFi.mode(WIFI_STA);
        WiFi.begin(staSSID, staPassword);

        // Wait for connection
        while (WiFi.status() != WL_CONNECTED) {
            display.drawProgressBar(0, 0, 127, 31, connectionProgress);
            display.display();

            connectionProgress += 10;

            if (connectionProgress > 100) {
                display.clear();
                connectionProgress = 0;
            }

            delay(100);
        }

        rgbLed.SetPixelColor(0, green);
        rgbLed.Show();
    }

    WiFi.hostname(HOSTNAME);

    if (MDNS.begin(HOSTNAME)) {
        MDNS.addService("http", "tcp", 80);
    }

    // Start servers
    httpServer.begin();
    httpUpdater.setup(&httpServer);

    // Attached to NTP Client
    timeClient.begin();

    // Setup UI
    ui.setTargetFPS(10);
    ui.disableAllIndicators();
    ui.disableAutoTransition();
    ui.setOverlays(overlays, overlaysCount);
    ui.setFrames(frames, frameCount);
    ui.getUiState()->userData = (void *)&uiGlobalObject;
    ui.init();
    display.flipScreenVertically();

    // Setup DHT11 interface
    dht.setup(dhtSensorPin, DHTesp::DHT11);

    // Assign server helper functions
    httpServer.on("/", handleRoot);
    httpServer.on("/list", HTTP_GET, handleFileList);
    httpServer.on("/sensor", HTTP_GET, handleSensor);
    httpServer.on("/info", HTTP_GET, []() {
        String spiffsInfo = String();
        FSInfo fsInfo;

        SPIFFS.info(fsInfo);
        spiffsInfo = "TotalByte: " + String(fsInfo.totalBytes);
        spiffsInfo += " UsedBytes: " + String(fsInfo.usedBytes);
        spiffsInfo += " Flash Chip: " + String(ESP.getFlashChipRealSize());
        spiffsInfo += " CPU Freq: " + String(ESP.getCpuFreqMHz());
        httpServer.send(200, "text/plain", spiffsInfo);
    });

    httpServer.on("/upload", HTTP_POST, []() {
        httpServer.send(200, "text/plain", "{\"success\":1}");
    }, handleFileUpload);

    httpServer.on("/format", HTTP_POST, []() {
        SPIFFS.format();
        httpServer.send(200, "text/plain", "{\"success\":1}");
    });

    httpServer.on("/message", HTTP_POST, handleMessage);
    httpServer.on("/led", HTTP_POST, handleLED);
    httpServer.on("/beeper", HTTP_POST, handleBeeper);

    httpServer.onNotFound(handleWebRequests);

    lastDhtUpdateTime = millis();
    lastLedUpdateTime = millis();
}

//=============================================================================
// Loop function
//=============================================================================
void loop() {

    static beepHandler_t previousBeeperState = beepHandlerIdle;
    uint32_t currentTime = millis();
    uint16_t uiRemainingBudget = 0;

    menuButton.Update();
    enterButton.Update();
    upButton.Update();

    // Only debounce downButton if beeper is idle (not blocked or not active)
    if (beeper.GetBeeperState() == beepHandlerIdle) {
        if (previousBeeperState != beepHandlerIdle) {
            previousBeeperState = beepHandlerIdle;

            // Once beeper is finished, set pin to input again
            pinMode(DownPin, INPUT_PULLUP);
        }

        downButton.Update();
    } else {
        // Beeper blocked or active so save previous state for edge detection
        previousBeeperState = beeper.GetBeeperState();
    }

    switch (uiGlobalState) {
        case uiGlobalUXActive:
        {
            if (ui.getUiState()->currentFrame == 5) {
                if (enterButton.clicks > 0) {

                    // Block beeper to prevent clickButton detecting button
                    // pressed when beeper is active
                    beeper.RequestBlock();
                    uiGlobalState = uiGlobalUXTetris;
                }
            }

            if (ui.getUiState()->currentFrame == 6) {
                if (enterButton.clicks > 0) {
                    uiGlobalState = uiGlobalUXDino;
                }
            }

            if (ui.getUiState()->currentFrame == 7) {
                if (enterButton.clicks > 0) {
                    uiGlobalState = uiGlobalUXTunnel;
                }
            }

            if (ui.getUiState()->currentFrame == 8) {
                if (enterButton.clicks > 0) {
                    uiGlobalState = uiGlobalUXStars;
                }
            }

            uiRemainingBudget = ui.update();

            if (menuButton.clicks > 0) {
                ui.nextFrame();
            } else if (menuButton.clicks < 0) {
                ui.previousFrame();
            }
        }
        break;

        case uiGlobalUXTetris:
        {
            if (gameTetrisCyclic((OLEDDisplay *)&display, &upButton, &downButton, &enterButton, &menuButton, &uiRemainingBudget) == gameTetrisStateExit) {

                // Unblocker beeper will cause pinMode to be set back to input
                beeper.RequestUnblock();

                // Update last UI upadate timer with current time to avoid frame skipping
                ui.getUiState()->lastUpdate = currentTime;
                uiGlobalState = uiGlobalUXActive;
            }
        }
        break;

        case uiGlobalUXDino:
        {
            if (gameDinoCyclic((OLEDDisplay *)&display, &upButton, &menuButton, &uiRemainingBudget) == gameDinoStateExit) {

                // Update last UI upadate timer with current time to avoid frame skipping
                ui.getUiState()->lastUpdate = currentTime;
                uiGlobalState = uiGlobalUXActive;
            }
        }
        break;

        case uiGlobalUXTunnel:
        {
            if (gameTunnelCyclic((OLEDDisplay *)&display, &menuButton, &uiRemainingBudget) == gameTunnelStateExit) {

                // Update last UI upadate timer with current time to avoid frame skipping
                ui.getUiState()->lastUpdate = currentTime;
                uiGlobalState = uiGlobalUXActive;
            }
        }
        break;

        case uiGlobalUXStars:
        {
            if (gameStarsCyclic((OLEDDisplay *)&display, &menuButton, &uiRemainingBudget) == gameStarsStateExit) {

                // Update last UI upadate timer with current time to avoid frame skipping
                ui.getUiState()->lastUpdate = currentTime;
                uiGlobalState = uiGlobalUXActive;
            }
        }
        break;

        default:
            uiGlobalState = uiGlobalUXActive;
            break;
    }

    if (uiRemainingBudget > 0) {
        httpServer.handleClient();
        beeper.Update();
        battery.Update();
        MDNS.update();

        if ((currentTime - lastDhtUpdateTime) >=  2000) {
            dhtTempAndHumidity = dht.getTempAndHumidity();

            lastDhtUpdateTime = currentTime;
        }

        if (((currentTime - lastLedUpdateTime) >=  2000) && (ledNowOff == false)) {
            rgbLed.SetPixelColor(0, black);
            rgbLed.Show();

            ledNowOff = true;
            lastLedUpdateTime = currentTime;
        }

        if (downButton.clicks < 0) {
            // If DOWN button held for longer than 1 second go to sleep
            display.displayOff();
            digitalWrite(DeepSleepPin, HIGH);
            ESP.deepSleep(ESP.deepSleepMax(), WAKE_RF_DEFAULT);
        }
    }
}
