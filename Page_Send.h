#ifndef PAGE_SEND_H
#define PAGE_SEND_H
//
//   The HTML PAGE
//
const char PAGE_Send[] PROGMEM = R"=====(
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />

  <link rel="stylesheet" href="style.css" type="text/css" />
  <script src="microajax.js"></script> 
  <a href="/"  class="btn btn--s"><</a>&nbsp;&nbsp;<strong>Send 433Mhz</strong>
  <hr>

  <form action="" method="get"> 
    <table border="0"  cellspacing="0" cellpadding="3" style="width:auto" >
      <tr><td><input type="text" id="code" name="code" style="width:auto"  value=""></td></tr>
      <tr><td>&nbsp;</td></tr>
      <tr><td><input type="submit" style="width:150px" class="btn btn--m btn--blue" value="Send"></td></tr>
    </table>  
  </form>
  <hr>
  
  <script>
    window.onload = function ()
    {
      load("style.css","css", function() 
      {
        load("microajax.js","js", function() 
        {
            setValues("/fillsenddata");  //-- this function calls the function on the ESP      
        });
      });
    }
    function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}
  </script>

)=====" ;

String txCodes="";
void processSend()
{        
    if (server.args() > 0 )  // Are there any POST/GET Fields ? 
    {
       for ( uint8_t i = 0; i < server.args(); i++ ) {  // Iterate through the fields
            if (server.argName(i) == "code") 
            {             
              // Your processing for the transmitted form-variable 
              int length = 0;
              uint16_t codes[MAXPULSESTREAMLENGTH];
              setPixel(pixels.Color(128, 128, 128));
              txCodes=server.arg(i);              
              // get pulse train from string (format see: pilight USB Nano)
              length = rf.stringToPulseTrain(txCodes, codes, MAXPULSESTREAMLENGTH);
              
              // transmit the pulse train
              rf._enabledReceiver=false;
              rf.sendPulseTrain(codes, length);
              rf._enabledReceiver=true;
              //Serial.print("Send:");
              //Serial.println(txCodes);
            }
        }
    }
    server.send ( 200, "text/html", PAGE_Send  ); 
}

void fillsenddata()
{        
    String values ="";
    values += "code|" + (String) txCodes + "|input\n";      
    server.send ( 200, "text/plain", values);   
}


#endif
