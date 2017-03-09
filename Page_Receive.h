#ifndef PAGE_RECEIVE_H
#define PAGE_RECEIVE_H
//
//   The HTML PAGE
//
const char PAGE_Receive[] PROGMEM = R"=====(
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <meta http-equiv="refresh" content="3; URL=/receive.html">
  <link rel="stylesheet" href="style.css" type="text/css" />
  <script src="microajax.js"></script> 
  <a href="/"  class="btn btn--s"><</a>&nbsp;&nbsp;<strong>Receive 433Mhz</strong>
  <hr>  
    
   <div id="result0">Waiting for RF codes...</div>
   <div id="result1">Waiting for RF codes...</div>
   <div id="result2">Waiting for RF codes...</div>
   <div id="result3">Waiting for RF codes...</div>
   <div id="result4">Waiting for RF codes...</div>
   <div id="result5">Waiting for RF codes...</div>
   <div id="result6">Waiting for RF codes...</div>
   <div id="result7">Waiting for RF codes...</div>
   <div id="result8">Waiting for RF codes...</div>
   <div id="result9">Waiting for RF codes...</div>
   
   
   <script>
    window.onload = function ()
    {
      load("style.css","css", function() 
      {
        load("microajax.js","js", function() 
        {
            setValues("/filldynamicdata");  //-- this function calls the function on the ESP      
        });
      });
    }
    function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}
   </script>

)=====" ;

void filldynamicdata()
{        
    String values ="";
    values += "result0|" + (String) results[0] + "|div\n"; 
    values += "result1|" + (String) results[1] + "|div\n";
    values += "result2|" + (String) results[2] + "|div\n";
    values += "result3|" + (String) results[3] + "|div\n";
    values += "result4|" + (String) results[4] + "|div\n";
    values += "result5|" + (String) results[5] + "|div\n";
    values += "result6|" + (String) results[6] + "|div\n";
    values += "result7|" + (String) results[7] + "|div\n";
    values += "result8|" + (String) results[8] + "|div\n";
    values += "result9|" + (String) results[9] + "|div\n";
    server.send ( 200, "text/plain", values);   
}

#endif
