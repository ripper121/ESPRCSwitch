#ifndef GLOBAL_H
#define GLOBAL_H

#define RECEIVER_PIN          5
#define TRANSMITTER_PIN       4

#define BUTTON_PIN            12
#define LED_PIN               13
#define NUMPIXELS             1

#define AdminTimeOut 180  // Defines the Time in Seconds, when the Admin-Mode will be diabled

#define ARRAYSIZE 10

ESP8266WebServer server(80);                  // The Webserver
ESP8266HTTPUpdateServer httpUpdater;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
ESPiLight rf(TRANSMITTER_PIN);  //use -1 to disable transmitter
Ticker tkSecond;                        // Second - Timer for Updating Datetime Structure

String rfRawRec = "";
bool rfRawReady = false;

String results[ARRAYSIZE] = { "-", "-", "-", "-", "-", "-", "-", "-", "-", "-" };
String resultsTime[ARRAYSIZE] = { "0", "0", "0", "0", "0", "0", "0", "0", "0", "0" };
byte resultCounter = 0;

bool pixelToggle = false;

unsigned long reconnectPreviousMillis = 0;
const long reconnectInterval = 1000;

boolean runOnce = true;										// On firststart = true, NTP will try to get a valid time
int AdminTimeOutCounter = 0;									// Counter for Disabling the AdminMode

struct strConfig {
  String ssid;
  String password;
  byte  IP[4];
  byte  Netmask[4];
  byte  Gateway[4];
  boolean dhcp;
  boolean AdminEnabled;
}   config;


void setPixel(uint32_t pixelColor) {
  pixels.setPixelColor(0, pixelColor);
  pixels.show();
}

// callback function. It is called on successfully received and parsed rc signal
void rfRawCallback(const uint16_t* codes, int length) {
  if (length >= MINPULSESTREAMLENGTH && length <= MAXPULSESTREAMLENGTH && !rfRawReady) {
    String data = rf.pulseTrainToString(codes, length);
    rfRawRec = data;
    rfRawReady = true;
  }
}



/*
**
** CONFIGURATION HANDLING
**
*/
void ConfigureWifi()
{
  WiFi.begin (config.ssid.c_str(), config.password.c_str());
  if (!config.dhcp)
  {
    WiFi.config(IPAddress(config.IP[0], config.IP[1], config.IP[2], config.IP[3] ),  IPAddress(config.Gateway[0], config.Gateway[1], config.Gateway[2], config.Gateway[3] ) , IPAddress(config.Netmask[0], config.Netmask[1], config.Netmask[2], config.Netmask[3] ));
  }
}

void WriteConfig()
{

  Serial.println("Writing Config");
  EEPROM.write(0, 'C');
  EEPROM.write(1, 'F');
  EEPROM.write(2, 'G');

  EEPROM.write(3, config.dhcp);

  EEPROM.write(4, config.IP[0]);
  EEPROM.write(5, config.IP[1]);
  EEPROM.write(6, config.IP[2]);
  EEPROM.write(7, config.IP[3]);

  EEPROM.write(8, config.Netmask[0]);
  EEPROM.write(9, config.Netmask[1]);
  EEPROM.write(10, config.Netmask[2]);
  EEPROM.write(11, config.Netmask[3]);

  EEPROM.write(12, config.Gateway[0]);
  EEPROM.write(13, config.Gateway[1]);
  EEPROM.write(14, config.Gateway[2]);
  EEPROM.write(15, config.Gateway[3]);

  EEPROM.write(16, config.AdminEnabled);

  WriteStringToEEPROM(64, config.ssid);
  WriteStringToEEPROM(96, config.password);

  EEPROM.commit();
}

boolean ReadConfig()
{

  Serial.println("Reading Configuration");
  if (EEPROM.read(0) == 'C' && EEPROM.read(1) == 'F'  && EEPROM.read(2) == 'G' )
  {
    Serial.println("Configurarion Found!");
    config.dhcp = 	EEPROM.read(3);
    config.IP[0] = EEPROM.read(4);
    config.IP[1] = EEPROM.read(5);
    config.IP[2] = EEPROM.read(6);
    config.IP[3] = EEPROM.read(7);
    config.Netmask[0] = EEPROM.read(8);
    config.Netmask[1] = EEPROM.read(9);
    config.Netmask[2] = EEPROM.read(10);
    config.Netmask[3] = EEPROM.read(11);
    config.Gateway[0] = EEPROM.read(12);
    config.Gateway[1] = EEPROM.read(13);
    config.Gateway[2] = EEPROM.read(14);
    config.Gateway[3] = EEPROM.read(15);
    config.AdminEnabled = EEPROM.read(16);
    config.ssid = ReadStringFromEEPROM(64);
    config.password = ReadStringFromEEPROM(96);
    return true;

  }
  else
  {
    Serial.println("Configurarion NOT FOUND!!!!");
    return false;
  }
}

void Second_Tick()
{
  if (digitalRead(BUTTON_PIN) == LOW) {
    Serial.println("BUTTON");
    config.AdminEnabled = true;
    WriteConfig();
    setPixel(pixels.Color(255, 255, 255));
    delay(1000);
    ESP.restart();
  }
  if (config.AdminEnabled) {
    AdminTimeOutCounter++;
    if (pixelToggle)
      setPixel(pixels.Color(0, 0, 0));
    else
      setPixel(pixels.Color(255, 0, 255));
    pixelToggle = !pixelToggle;
  } else {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      if (pixelToggle)
        setPixel(pixels.Color(0, 0, 0));
      else
        setPixel(pixels.Color(0, 0, 255));
      pixelToggle = !pixelToggle;
    }
  }
}
#endif
