/*
  Todo:
  - add config for TCP Server:Port
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include <ESP8266HTTPUpdateServer.h>

#include <Ticker.h>

#include <EEPROM.h>

#include <ESPiLight.h>

#include <Adafruit_NeoPixel.h>

#include "helpers.h"
#include "global.h"
/*
  Include the HTML, STYLE and Script "Pages"
*/
#include "Page_Admin.h"
#include "Page_Admin_STA.h"
#include "Page_Send.h"
#include "Page_Receive.h"
#include "Page_Script.js.h"
#include "Page_Style.css.h"
#include "Page_Information.h"
#include "PAGE_NetworkConfiguration.h"


void setup ( void ) {
  pinMode(BUTTON_PIN, INPUT);
  pixels.begin();
  EEPROM.begin(512);
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Starting ES8266");


  for (byte i = 0; i < 3; i++) {
    byte j = 0;
    for (j = 0; j < 255; j++) {
      setPixel(pixels.Color(j, 0, j));
      delay(1);
    }
    for (j = 255; j > 0; j--) {
      setPixel(pixels.Color(j, 0, j));
      delay(1);
    }
  }

  char charBuf[25];
  String AP_SSID = WiFi.softAPmacAddress();
  AP_SSID = "gw433-" + String(AP_SSID[12]) + String(AP_SSID[13]) + String(AP_SSID[15]) + String(AP_SSID[16]) + '.';
  AP_SSID.toCharArray(charBuf, AP_SSID.length());

  getButton();

  if (!ReadConfig())
  {
    // DEFAULT CONFIG
    config.ssid = "MYSSID";
    config.password = "MYPASSWORD";
    config.dhcp = true;
    config.IP[0] = 192; config.IP[1] = 168; config.IP[2] = 1; config.IP[3] = 100;
    config.Netmask[0] = 255; config.Netmask[1] = 255; config.Netmask[2] = 255; config.Netmask[3] = 0;
    config.Gateway[0] = 192; config.Gateway[1] = 168; config.Gateway[2] = 1; config.Gateway[3] = 1;
    config.AdminEnabled = true;
    WriteConfig();
    Serial.println("General config applied");
  }

  server.on ( "/config.html", []() {
    send_network_configuration_html();
  }  );

  server.on ( "/info.html", []() {
    server.send ( 200, "text/html", PAGE_Information );
  }  );

  server.on ( "/favicon.ico",   []() {
    server.send ( 200, "text/html", "" );
  }  );

  server.on ( "/style.css", []() {
    server.send ( 200, "text/plain", PAGE_Style_css );
    rf._enabledReceiver = false;
    setPixel(pixels.Color(0, 0, 255));
  } );
  server.on ( "/microajax.js", []() {
    server.send ( 200, "text/plain", PAGE_microajax_js );
  } );
  server.on ( "/admin/values", send_network_configuration_values_html );
  server.on ( "/admin/connectionstate", send_connection_state_values_html );
  server.on ( "/admin/infovalues", send_information_values_html );

  server.onNotFound ( []() {
    server.send ( 400, "text/html", "Page not Found" );
    rf._enabledReceiver = false;
    setPixel(pixels.Color(0, 0, 255));
  }  );

  if (config.AdminEnabled) {
    WiFi.softAP(charBuf, "");

    server.on ( "/", []() {
      server.send ( 200, "text/html", PAGE_AdminMainPage );
    }  );

    server.on ( "/adminoff.html", []() {
      server.send ( 200, "text/html", "Device will reboot and connect to your Network.");
      AdminTimeOutCounter = AdminTimeOut;
    }  );

    Serial.println("AP_SSID:" + AP_SSID);
    Serial.print("Server IP address: ");
    Serial.println(WiFi.softAPIP());

  } else {
    WiFi.mode(WIFI_STA);
    
    server.on ( "/", []() {
      server.send ( 200, "text/html", PAGE_AdminMainPageSTA );
    }  );

    server.on ( "/send.html", []() {
      processSend();
    }  );
    server.on ( "/fillsenddata", fillsenddata );

    server.on ( "/receive.html", []() {
      for ( byte i = 0; i < ARRAYSIZE;  ++i ) {
        results[i] = "-";
        resultsTime[i] = "0";
      }
      server.send ( 200, "text/html", PAGE_Receive );
    }  );
    server.on ( "/fillReceivedata", fillReceivedata );
    server.on ( "/startRec", []() {
      rf._enabledReceiver = true;
      setPixel(pixels.Color(0, 128, 0));
    }  );
    server.on ( "/stopRec", []() {
      rf._enabledReceiver = false;
      setPixel(pixels.Color(0, 0, 255));
    }  );

    //set callback funktion for raw messages
    rf.setPulseTrainCallBack(rfRawCallback);
    //inittilize receiver
    rf.initReceiver(RECEIVER_PIN);
    rf._enabledReceiver = false;
  }

  ConfigureWifi();

  httpUpdater.setup(&server);
  server.begin();
  Serial.println( "HTTP server started" );

  tkSecond.attach(1, Second_Tick);
}



void loop ( void ) {
  if (config.AdminEnabled)
  {
    if (AdminTimeOutCounter > AdminTimeOut)
    {
      config.AdminEnabled = false;
      WriteConfig();
      server.stop();
      Serial.println("Admin Mode disabled!");
      ESP.restart();
    }
  } else {
    if (WiFi.status() == WL_CONNECTED) {
      if (runOnce) {
        IPAddress ip = WiFi.localIP();
        Serial.println("");
        if (config.IP[0] != ip[0] || config.IP[1] != ip[1] || config.IP[2] != ip[2] || config.IP[3] != ip[3]) {
          config.IP[0] = ip[0];
          config.IP[1] = ip[1];
          config.IP[2] = ip[2];
          config.IP[3] = ip[3];
          WriteConfig();
        }
        Serial.print("Server IP address: ");
        Serial.println(WiFi.localIP());
        setPixel(pixels.Color(0, 0, 255));
        runOnce = false;
      }

      if (rfRawReady) {
        String rfRawRecRepl = rfRawRec;
        byte charCount = 0;
        char buffer[25];

        rfRawRecRepl.replace(": ", "");
        charCount = rfRawRec.length() - rfRawRecRepl.length();

        if (rfRawRec.indexOf('<') == -1 && rfRawRec.indexOf('>') == -1 && rfRawRec.indexOf('=') == -1 && charCount < 3 && rfRawRec.length() > 5) {
          float timeRX = millis();
          timeRX /= 1000;
          //Serial.println(rfRawRec);
          for (byte i = 0; i < ARRAYSIZE - 1; i++) {
            results[i] = results[i + 1];
            resultsTime[i] = resultsTime[i + 1];
          }
          resultsTime[ARRAYSIZE - 1] = dtostrf(timeRX, 5, 2, buffer);
          results[ARRAYSIZE - 1] = rfRawRec;

          if (pixelToggle)
            setPixel(pixels.Color(0, 64, 0));
          else
            setPixel(pixels.Color(0, 128, 0));
          pixelToggle = !pixelToggle;
        }
        rfRawRec = "";
        rfRawReady = false;
      }
      if (!rfRawReady)
        rf.loop();

    } else {

      rf._enabledReceiver = false;
      setPixel(pixels.Color(255, 0, 0));
      runOnce = true;

    }
  }

  server.handleClient();
}



