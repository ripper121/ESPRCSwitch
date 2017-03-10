#ifndef PAGE_RECEIVE_H
#define PAGE_RECEIVE_H
//
//   The HTML PAGE
//
const char PAGE_Receive[] PROGMEM = R"=====(
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />

  <link rel="stylesheet" href="style.css" type="text/css" />
  <script src="microajax.js"></script> 
  <a href="/"  class="btn btn--s"><</a>&nbsp;&nbsp;<strong>Receive 433Mhz</strong>
  <hr>
  <table border="1"  cellspacing="0" cellpadding="3" style="width:auto;border:1px;display: block;overflow-x: auto;" >
    <tr><td align="right">Time</td><td>RF Code</td></tr>
    <tr><td align="right"><div id="resultTime0">-</div></td><td><a href="/receive.html" target="_blank" id="resultLnk0"><div id="result0">-</div></a></td></tr>
    <tr><td align="right"><div id="resultTime1">-</div></td><td><a href="/receive.html" target="_blank" id="resultLnk1"><div id="result1">-</div></a></td></tr>
    <tr><td align="right"><div id="resultTime2">-</div></td><td><a href="/receive.html" target="_blank" id="resultLnk2"><div id="result2">-</div></a></td></tr>
    <tr><td align="right"><div id="resultTime3">-</div></td><td><a href="/receive.html" target="_blank" id="resultLnk3"><div id="result3">-</div></a></td></tr>
    <tr><td align="right"><div id="resultTime4">-</div></td><td><a href="/receive.html" target="_blank" id="resultLnk4"><div id="result4">-</div></a></td></tr>
    <tr><td align="right"><div id="resultTime5">-</div></td><td><a href="/receive.html" target="_blank" id="resultLnk5"><div id="result5">-</div></a></td></tr>
    <tr><td align="right"><div id="resultTime6">-</div></td><td><a href="/receive.html" target="_blank" id="resultLnk6"><div id="result6">-</div></a></td></tr>
    <tr><td align="right"><div id="resultTime7">-</div></td><td><a href="/receive.html" target="_blank" id="resultLnk7"><div id="result7">-</div></a></td></tr>
    <tr><td align="right"><div id="resultTime8">-</div></td><td><a href="/receive.html" target="_blank" id="resultLnk8"><div id="result8">-</div></a></td></tr>
    <tr><td align="right"><div id="resultTime9">-</div></td><td><a href="/receive.html" target="_blank" id="resultLnk9"><div id="result9">-</div></a></td></tr>    
  </table>
  <br>
  <div id="recStatus">Receiver is ...</div>
  <br>
  <table border="0"  cellspacing="0" cellpadding="3" style="width:auto" >
    <tr><td> <a href="javascript:startRec()" style="width:150px" class="btn btn--m btn--blue">Enable</a> </td></tr>
    <tr><td> <a href="javascript:stopRec()" style="width:150px" class="btn btn--m btn--blue">Disable</a> </td></tr>
  </table>
  <hr>   
   
  <script>
    var recEnabled = 0;
    
    function AutoRefresh()
    {
     setValues("/fillReceivedata");
     setTimeout(AutoRefresh, 1000);
    }

    function startRec()
    {
      setValues("/startRec");
      recEnabled = 1;
    }

    function stopRec()
    {
      setValues("/stopRec");
      recEnabled = 0;
    }

    window.onbeforeunload = function() {
        setValues("/stopRec");
    }
                
    window.onload = function ()
    { 
      load("style.css","css", function() 
      {
        load("microajax.js","js", function() 
        {  
            AutoRefresh();
        });
      });
    }
    function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}
  </script>

)=====" ;

void fillReceivedata()
{        
    String values ="";
    for (byte i = 0; i < ARRAYSIZE; i++) {
      values += "result" + (String) i + "|" + (String) results[i] + "|div\n";      
      values += "resultLnk" + (String) i + "|send.html?code=" + (String) results[i] + "|a\n"; 
      values += "resultTime" + (String) i + "|" + (String) resultsTime[i] + "|div\n"; 
    }
    
    if(rf._enabledReceiver){
      values += "recStatus|Receiver is enabled!|div\n"; 
    }else{
      values += "recStatus|Receiver is disabled!|div\n"; 
    }
    server.send ( 200, "text/plain", values);   
}

#endif
