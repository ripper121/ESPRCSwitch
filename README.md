# ESPRCSwitch

RC Switch with Web Interface.

Tested with Arduino 1.6.8 (https://www.arduino.cc/en/Main/OldSoftwareReleases#previous) from the Arduino website and Arduino ESP8266 core 2.3.0 (https://github.com/esp8266/Arduino)

I use a modified version of https://github.com/puuu/ESPiLight


To send something you can do it directly on the Page or via GET.
```sh
http://gatewayIP/send.html?code=c:01001002000303403440000300004004440430000304000404000040404000000034040000400040440040000000000400555005000040050500678080004800000006080034000080800000000088000000004003380038000004800004000038808080008093:;p:213,4813,1920,543,359,679,849,1223,1012,1588,7631@
```

The Receive Page will show you the last 10 received codes.


| Screenshots |
| ------ |
|Index Page|
|![Alt text](/screenshots/index.png?raw=true "Index Page")|
|Send Page|
|![Alt text](/screenshots/send.png?raw=true "Send Page")|
|Receive Page|
|![Alt text](/screenshots/receive.png?raw=true "Receive Page")|
|Config Page|
|![Alt text](/screenshots/config.png?raw=true "Config Page")|
|Info Page|
|![Alt text](/screenshots/info.png?raw=true "Info Page")|
