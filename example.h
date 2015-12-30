#ifndef PAGE_EXAMPLE_H
#define PAGE_EXAMPLE_H
//
//   The EXAMPLE PAGE
//
const char PAGE_example[] PROGMEM = R"=====(
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/2.1.4/jquery.min.js"></script> 
  <script>
      $(document).bind('mobileinit',function(){
          $.mobile.pushStateEnabled = false;
      });
  </script>
  <link rel="stylesheet" href="https://ajax.googleapis.com/ajax/libs/jquerymobile/1.4.5/jquery.mobile.min.css">
  <script src="https://ajax.googleapis.com/ajax/libs/jquerymobile/1.4.5/jquery.mobile.min.js"></script> 
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />   
    <div data-role="page" id="page1" data-theme="b">
        <div data-role="header">
            <h1>Home Automation</h1>
        </div>
        <div data-role="content">
            <div class="ui-grid-a">
                <div class="ui-block-a">
                    <a data-role="button" onclick="window.location = '/?switch0=on';" >Switch-1 ON</a>
                    <a data-role="button" >Switch-2 ON</a>
                    <a data-role="button">Switch-3 ON</a>
                    <a data-role="button">Switch-4 ON</a>
                    <a data-role="button">Switch-5 ON</a>
                    <a data-role="button">Switch-6 ON</a>
                    <a data-role="button">Switch-7 ON</a>
                    <a data-role="button">Switch-8 ON</a>
                    <a data-role="button">Switch-9 ON</a>
                    <a data-role="button">Switch-10 ON</a>
                </div>
                <div class="ui-block-b">
                    <a data-role="button" onclick="window.location = '/?switch0=off';">Switch-1 OFF</a>
                    <a data-role="button">Switch-2 OFF</a>
                    <a data-role="button">Switch-3 OFF</a>
                    <a data-role="button">Switch-4 OFF</a>
                    <a data-role="button">Switch-5 OFF</a>
                    <a data-role="button">Switch-6 OFF</a>
                    <a data-role="button">Switch-7 OFF</a>
                    <a data-role="button">Switch-8 OFF</a>
                    <a data-role="button">Switch-9 OFF</a>
                    <a data-role="button">Switch-10 OFF</a>
                </div>
            </div>
        </div>  
        <a data-role="button" onclick="window.location = '/admin.html';" data-mini="true">Admin Page</a>
      </div>
      <div data-role="footer">
      </div>

   <script>
		function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}
   </script>

)=====";
#endif


void filldynamicdata()
{        

}

void processExample()
{        
    if (server.args() > 0 )  // Are there any POST/GET Fields ? 
    {
       for ( uint8_t i = 0; i < server.args(); i++ ) {
                if (server.argName(i) == "switch0" && server.arg(i)=="on") mySwitch.switchOn("11111", "00000");
                if (server.argName(i) == "switch0" && server.arg(i)=="off") mySwitch.switchOff("11111", "00000");                
        }
    }
    
    server.send ( 200, "text/html", PAGE_example  ); 
}

