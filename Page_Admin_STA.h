//
//  HTML PAGE
//

const char PAGE_AdminMainPageSTA[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<strong>433Mhz-Gateway</strong>
<hr>
<a href="send.html" style="width:250px" class="btn btn--m btn--blue" >Send</a><br>
<a href="receive.html" style="width:250px" class="btn btn--m btn--blue" >Receive</a><br>
<a href="config.html" style="width:250px" class="btn btn--m btn--blue" >Configuration</a><br>
<a href="info.html"   style="width:250px"  class="btn btn--m btn--blue" >Information</a><br>
<hr><br><br>
.by <a href="mailto:ripper121@gmail.com">Ripper121</a><br>
<a href="http://github.com/ripper121/ESPRCSwitch">Github</a>
<script>
window.onload = function ()
{
	load("style.css","css", function() 
	{
		load("microajax.js","js", function() 
		{
				// Do something after load...
		});
	});
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}
</script>

)=====";


