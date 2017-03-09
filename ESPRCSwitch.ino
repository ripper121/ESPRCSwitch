/*
  Todo:
  - add config for TCP Server:Port
*/
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
  for (byte i = 0; i < 3; i++) {
    byte j = 0;
    for (j = 0; j < 255; j++) {
      setPixel(pixels.Color(j, 0, j));
      delay(5);
    }
    for (j = 255; j > 0; j--) {
      setPixel(pixels.Color(j, 0, j));
      delay(5);
    }
  }

  EEPROM.begin(512);
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Starting ES8266");

  if (!ReadConfig())
  {
    // DEFAULT CONFIG
    config.ssid = "MYSSID";
    config.password = "MYPASSWORD";
    config.dhcp = true;
    config.IP[0] = 192; config.IP[1] = 168; config.IP[2] = 1; config.IP[3] = 100;
    config.Netmask[0] = 255; config.Netmask[1] = 255; config.Netmask[2] = 255; config.Netmask[3] = 0;
    config.Gateway[0] = 192; config.Gateway[1] = 168; config.Gateway[2] = 1; config.Gateway[3] = 1;
    config.AdminEnabled = false;
    WriteConfig();
    Serial.println("General config applied");
  }

  char charBuf[25];
  String AP_SSID = WiFi.softAPmacAddress();
  AP_SSID = "gw433-" + String(AP_SSID[12]) + String(AP_SSID[13]) + String(AP_SSID[15]) + String(AP_SSID[16]) + '.';
  AP_SSID.toCharArray(charBuf, AP_SSID.length());

  if (config.AdminEnabled) {
    server.on ( "/favicon.ico",   []() {
      //Serial.println("favicon.ico");
      server.send ( 200, "text/html", "" );
    }  );


    server.on ( "/", []() {
      Serial.println("admin.html");
      server.send ( 200, "text/html", PAGE_AdminMainPage );
    }  );

    server.on ( "/adminoff.html", []() {
      Serial.println("adminoff.html");
      server.send ( 200, "text/html", "Device will reboot and connect to your Network.");
      AdminTimeOutCounter = AdminTimeOut;
    }  );

    server.on ( "/config.html", send_network_configuration_html );
    server.on ( "/info.html", []() {
      Serial.println("info.html");
      server.send ( 200, "text/html", PAGE_Information );
    }  );

    //  server.on ( "/example.html", []() { server.send ( 200, "text/html", PAGE_EXAMPLE );  } );
    server.on ( "/style.css", []() {
      Serial.println("style.css");
      server.send ( 200, "text/plain", PAGE_Style_css );
    } );
    server.on ( "/microajax.js", []() {
      Serial.println("microajax.js");
      server.send ( 200, "text/plain", PAGE_microajax_js );
    } );
    server.on ( "/admin/values", send_network_configuration_values_html );
    server.on ( "/admin/connectionstate", send_connection_state_values_html );
    server.on ( "/admin/infovalues", send_information_values_html );

    server.onNotFound ( []() {
      //Serial.println("Page Not Found");
      server.send ( 400, "text/html", "Page not Found" );
    }  );

    httpUpdater.setup(&server);

    WiFi.hostname(AP_SSID);
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(charBuf, "");

    server.begin();
    Serial.println("WIFI_AP_STA");
    Serial.println("AP_SSID:" + AP_SSID);
    Serial.println( "HTTP server started" );
    Serial.print("Server address: ");
    Serial.println(WiFi.softAPIP());
    ConfigureWifi();
  } else {
    server.on ( "/favicon.ico",   []() {
      //Serial.println("favicon.ico");
      server.send ( 200, "text/html", "" );
    }  );

    server.on ( "/", []() {
      Serial.println("admin.html");
      server.send ( 200, "text/html", PAGE_AdminMainPageSTA );
    }  );

    server.on ( "/config.html", send_network_configuration_html );
    server.on ( "/info.html", []() {
      Serial.println("info.html");
      server.send ( 200, "text/html", PAGE_Information );
    }  );

    server.on ( "/send.html", []() {
      Serial.println("send.html");
      processSend();
    }  );

    server.on ( "/receive.html", []() {
      Serial.println("receive.html");
      server.send ( 200, "text/html", PAGE_Receive );
    }  );
    server.on ( "/filldynamicdata", filldynamicdata );

    //  server.on ( "/example.html", []() { server.send ( 200, "text/html", PAGE_EXAMPLE );  } );
    server.on ( "/style.css", []() {
      Serial.println("style.css");
      server.send ( 200, "text/plain", PAGE_Style_css );
    } );
    server.on ( "/microajax.js", []() {
      Serial.println("microajax.js");
      server.send ( 200, "text/plain", PAGE_microajax_js );
    } );
    server.on ( "/admin/values", send_network_configuration_values_html );
    server.on ( "/admin/connectionstate", send_connection_state_values_html );
    server.on ( "/admin/infovalues", send_information_values_html );

    server.onNotFound ( []() {
      //Serial.println("Page Not Found");
      server.send ( 400, "text/html", "Page not Found" );
    }  );

    httpUpdater.setup(&server);
    server.begin();

    Serial.println("WIFI_STA");
    WiFi.hostname(AP_SSID);
    WiFi.mode(WIFI_STA);
    ConfigureWifi();
    //set callback funktion for raw messages
    rf.setPulseTrainCallBack(rfRawCallback);
    //inittilize receiver
    rf.initReceiver(RECEIVER_PIN);
    rf.disableReceiver();
  }
  tkSecond.attach(1, Second_Tick);
  Serial.println("Start Tick");
}

// callback function. It is called on successfully received and parsed rc signal
void rfRawCallback(const uint16_t* codes, int length) {
  if (length >= MINPULSESTREAMLENGTH && length <= MAXPULSESTREAMLENGTH && !rfRawReady) {
    String data = rf.pulseTrainToString(codes, length);
    rfRawRec = data;
    rfRawReady = true;
  }
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
        Serial.print("Connected, IP address: ");
        Serial.println(WiFi.localIP());
        setPixel(pixels.Color(0, 0, 255));
        rf.enableReceiver();
        runOnce = false;
      }
      if (rfRawReady) {
        String rfRawRecRepl = rfRawRec;
        byte charCount = 0;

        rfRawRecRepl.replace(": ", "");
        charCount = rfRawRec.length() - rfRawRecRepl.length();

        if (rfRawRec.indexOf('<') == -1 && rfRawRec.indexOf('>') == -1 && rfRawRec.indexOf('=') == -1 && charCount < 3 && rfRawRec.length() > 5) {
          Serial.println(rfRawRec);
          results[resultCounter] = rfRawRec;
          resultCounter++;
          if (resultCounter > 9) {
            resultCounter = 0;
          }
        }

        if (pixelToggle)
          setPixel(pixels.Color(0, 64, 0));
        else
          setPixel(pixels.Color(0, 255, 0));
        pixelToggle = !pixelToggle;

        rfRawRec = "";
        rfRawReady = false;
      }

      if (!rfRawReady)
        rf.loop();
    } else {
      rf.disableReceiver();
      setPixel(pixels.Color(255, 0, 0));
      runOnce = true;
    }
  }

  server.handleClient();
}



