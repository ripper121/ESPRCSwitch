#ifndef PAGE_EXAMPLE_H
#define PAGE_EXAMPLE_H
//
//   The EXAMPLE PAGE
//
const char PAGE_example[] PROGMEM = R"=====(
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" /> 
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/2.1.4/jquery.min.js"></script> 
  <script>
      $(document).bind('mobileinit',function(){
          $.mobile.pushStateEnabled = false;
      });
  </script>
  <link rel="stylesheet" href="https://ajax.googleapis.com/ajax/libs/jquerymobile/1.4.5/jquery.mobile.min.css">
  <script src="https://ajax.googleapis.com/ajax/libs/jquerymobile/1.4.5/jquery.mobile.min.js"></script>
  <style> 
  .ui-grid-a .ui-btn {
   width : 100px !important;
  }
  </style>
  </head>
  <body>  
    <div data-role="page" id="page1" data-theme="b">
        <div data-role="header">
            <h1>Home Automation</h1>
        </div>
        <div data-role="content">
            <div class="ui-grid-a">
                <div class="ui-block-a">
                    <center><h3>ON</h3>
                    <a data-role="button" onclick="window.location = '?switch0=on';">0</a>
                    <a data-role="button" onclick="window.location = '?switch1=on';">1</a>
                    <a data-role="button" onclick="window.location = '?switch2=on';">2</a>
                    <a data-role="button" onclick="window.location = '?switch3=on';">3</a>
                    <a data-role="button" onclick="window.location = '?switch4=on';">4</a>
                    <a data-role="button" onclick="window.location = '?switch5=on';">5</a>
                    <a data-role="button" onclick="window.location = '?switch6=on';">6</a>
                    <a data-role="button" onclick="window.location = '?switch7=on';">7</a>
                    <a data-role="button" onclick="window.location = '?switch8=on';">8</a>
                    <a data-role="button" onclick="window.location = '?switch9=on';">9</a>
                    </center>
                </div>
                <div class="ui-block-b">
                    <center><h3>OFF</h3>
                    <a data-role="button" onclick="window.location = '?switch0=off';">0</a>
                    <a data-role="button" onclick="window.location = '?switch1=off';">1</a>
                    <a data-role="button" onclick="window.location = '?switch2=off';">2</a>
                    <a data-role="button" onclick="window.location = '?switch3=off';">3</a>
                    <a data-role="button" onclick="window.location = '?switch4=off';">4</a>
                    <a data-role="button" onclick="window.location = '?switch5=off';">5</a>
                    <a data-role="button" onclick="window.location = '?switch6=off';">6</a>
                    <a data-role="button" onclick="window.location = '?switch7=off';">7</a>
                    <a data-role="button" onclick="window.location = '?switch8=off';">8</a>
                    <a data-role="button" onclick="window.location = '?switch9=off';">9</a>
                    </center>
                </div>
            </div>
        </div>
        <div data-role="collapsible" data-mini="true">
           <h3>Admin Page</h3>
           <a data-role="button" onclick="window.location = 'admin.html';" data-mini="true">Admin Page</a>
        </div>
        <div data-role="footer">      
               <h1>by Ripper121</h1>
        </div>
      </div>


   <script>
    function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}
   </script>
  </body>
</html>
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

