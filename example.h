#ifndef PAGE_EXAMPLE_H
#define PAGE_EXAMPLE_H
//
//   The EXAMPLE PAGE
//
const char PAGE_example[] PROGMEM = R"=====(
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />   
   <a href="?switch0=on"   style="width:250px"  class="btn btn--full btn--blue" align="center">Switch 0 On</a><br>
   <a href="?switch0=off"   style="width:250px"  class="btn btn--full btn--blue" align="center">Switch 0 Off</a><br>   
   <a href="admin.html"   style="width:250px"  class="btn btn--full btn--blue" align="center">Admin Page</a><br>
   <script>
		window.onload = function ()
		{
			load("style.css","css", function() 
			{
			});
		}
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

