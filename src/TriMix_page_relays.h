/**********************************************************************

   TriMix - Oxygen and helium mixer controller
   https://hilitec.net - info@hilitec.net

   @file  TriMix_page_relays.h
   @brief Web page_relays

 **********************************************************************/

// https://github.com/Hieromon/AutoConnect/blob/master/examples/WebSocketServer/data/index.html

static const char PROGMEM page_relays[] = R"*lit(
<!DOCTYPE html>
<html lang="en">
<head>
  <link href="data:image/svg+xml,%0A%3Csvg id='favicon' data-name='favicon' xmlns='http://www.w3.org/2000/svg' viewBox='0 0 122.88 121.21'%3E%3Cdefs%3E%3Cstyle%3E.cls-1%7Bfill:%232020ff;fill-rule:evenodd;%7D%3C/style%3E%3C/defs%3E%3Ctitle%3Escuba-diving%3C/title%3E%3Cpath class='cls-1' d='M100.07,67.24c-.44-10.1-3-13.34-6.11-13.38A59.88,59.88,0,0,1,93.86,80c3.5.24,5.78-3.94,6.21-12.77Zm18.27-47.48a4.54,4.54,0,1,1-4.53,4.53,4.53,4.53,0,0,1,4.53-4.53ZM109,0a7.23,7.23,0,1,1-7.22,7.22A7.22,7.22,0,0,1,109,0ZM18.12,87.85Zm45,2.47A10.29,10.29,0,1,1,52.83,100.6,10.29,10.29,0,0,1,63.11,90.32ZM9.66,69.89a6.14,6.14,0,0,1,3.09-2v-.34a63.9,63.9,0,0,1,1-10.52,16.18,16.18,0,0,1,4-9.2l.48-.5-1.43-.46c-2-62.7,96.25-54.49,94.79.4,0,.65,0,1.28-.11,1.88a14.68,14.68,0,0,1,2.37,8,55.7,55.7,0,0,1,1.06,10.4,46.07,46.07,0,0,1-1.05,10c-.1,5-1.71,8.52-4.59,10.91a12.08,12.08,0,0,1-4.78,2.41,43,43,0,0,1-8.84,15.58A43.87,43.87,0,0,1,81,117.25a41.53,41.53,0,0,1-17.13,3.8,35.7,35.7,0,0,1-16.47-3.92,35,35,0,0,1-28.06,2.07A30.84,30.84,0,0,1,6.85,111,26.13,26.13,0,0,1,.29,97.17C-.92,89,1.58,79.58,9.66,69.89Zm8.89-.56a.86.86,0,0,1,0,.8,56.09,56.09,0,0,0,.66,6.47,1,1,0,0,1,.13.78l.1.52c.72,5.21,3.6,7.5,7.89,7.89H43.43c10.35-.58,11.64-6.27,16.12-8.67a9.92,9.92,0,0,1,9.5.29c4.06,2.6,5.4,8.1,14.82,8.1H98.39c6.11.65,9.72-1.74,9.65-8.6a44.4,44.4,0,0,0,0-19.17c0-6.14-3.67-8.55-9.23-9H27.22c-4.23,1.18-7.11,3.86-7.75,9a56.23,56.23,0,0,0-.92,11.59ZM15.12,83.64C12.74,88,12,92,12.52,95.38A13.88,13.88,0,0,0,16,102.69a18.54,18.54,0,0,0,7.48,4.91,22.36,22.36,0,0,0,12.51.69C31.55,104,28.23,100.14,25.9,95.8A31.57,31.57,0,0,1,23.8,91a13,13,0,0,1-8.68-7.36Zm48,1.06a15.91,15.91,0,1,1-15.9,15.9,15.9,15.9,0,0,1,15.9-15.9Z'/%3E%3C/svg%3E" rel="shortcut icon">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style type="text/css">
  body {
    -webkit-appearance:none;
    -moz-appearance:none;
    font-family:'Arial',sans-serif;
    text-align:center;
    background-color: #3f3f3f;
    color: #ffff00;
  }
  .menu > a:link {
    position: absolute;
    display: inline-block;
    right: 12px;
    padding: 0 6px;
    text-decoration: none;
  }
  .big, .inp {
    font-size: 2em;
    font-weight: bold;
  }
  table.c {
    margin-left:auto; 
    margin-right:auto;
  }
  .ip, .so {
    text-align: left;
    vertical-align: center;
    width: 50px;
    height: 50px;
  }
  .so {
    font-weight: bold;
  }
  .ru {
    text-align: center;
    vertical-align: center;
    width: 50px;
    height: 50px;
  }
  .da, .co {
    text-align: right;
    vertical-align: center;
    width: 50px;
    height: 50px;
  }
  .co {
    font-style: italic;
  }
  .mo {
    text-align: center;
    vertical-align: bottom;
  }
  .o2, .he, .ca, .fi {
    text-align: center;
    vertical-align: center;
    width: 250px;
    height: 150px;
    cursor: pointer;
  }
  .o2, .o2 .inp {
    background-color: #a0a0a0;
  }
  .he, .he .inp {
    background-color: #bb4400;
  }
  .ca {
    background-color: #3f3fff;
  }
  .fi {
    background-color: #ff0000;
  }
  .inter {
    font-size: 1.5em;
    font-weight: bold;
    padding-top: 0.8em;
    color: #ffffff;
  }
  .inp {
    font-size: 1em;
    text-align: right;
    color: #ffffff;
    border: none;
    width: 2em;
  }
  .hid {
    visibility: hidden;
  }
  /*
  #run, #comp, #hevalve, #o2valve {
    visibility: hidden;
  }
  */
  </style>
</head>
<body>
  <div class="menu">{{AUTOCONNECT_MENU}}</div>
  <br />
  <br />
  <br />
  <form id="form" onsubmit="event.preventDefault(); DoForm();">
  <table class="c">
  <tr>
  <td class="o2" onclick="document.getElementById('o2mix').focus(); document.getElementById('o2mix').select();">
  <div class="big">O<sub>2</sub> <span id="o2">--.-</span>%</div>
  <!-- <div>(<span id="o2mv">--.--</span>mV)</div> -->
  <div>&nbsp;<span id="o2onoff"></span>&nbsp;</div>
  <div class="inter">Setpoint: <input onBlur="DoForm();" onfocus="this.select();" size="4" type="text" class="inp" id="o2mix" value="{{O2_MIX}}" />%</div>
  </td>
  <td>
    <svg version="1.1" id="hevalve" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" x="0px" y="0px" viewBox="0 0 122.88 117.35" style="enable-background:new 0 0 122.88 117.35" xml:space="preserve"><style type="text/css">.st0{fill-rule:evenodd;clip-rule:evenodd;}</style><g fill="#2f2f2f"><path class="st0" d="M79.22,54.41c5.16,3.03,9.06,6.88,11.81,11.19h5.36v36.21h-5.73c-5.72,9.03-15.74,15.53-28.98,15.53 c-13.23,0-23.34-6.5-29.09-15.53h-5.96V65.6h5.91c2.98-4.48,7.25-8.48,12.96-11.58c3.04-1.64,4.88-3.69,4.9-5.53 c0.03-2.33-2.52-4.31-5.26-4.31h-1.36c-3.32,0-6.04-2.72-6.04-6.04l0,0c0-3.32,2.72-6.04,6.04-6.04h7.76l6.1-7.57 c-0.02-0.28-0.04-0.57-0.04-0.87v-9.99H32.87c-3.77,0-6.85-3.08-6.85-6.85l0,0c0-3.76,3.08-6.85,6.85-6.85h58.18 c3.76,0,6.85,3.08,6.85,6.85l0,0c0,3.77-3.08,6.85-6.85,6.85H67.51v9.99c0,0.41-0.03,0.82-0.07,1.21l5.48,7.23h6.67 c3.32,0,6.04,2.72,6.04,6.04l0,0c0,3.32-2.72,5.97-6.04,6.04c-4.57,0.1-6.25,2.06-6.04,4.34C73.91,52.73,76.83,53.01,79.22,54.41 L79.22,54.41z M101.34,65.6h4.2v-8.65c0-1.54,1.26-2.79,2.79-2.79h11.76c1.54,0,2.79,1.26,2.79,2.79v53.51 c0,1.54-1.26,2.79-2.79,2.79h-11.76c-1.54,0-2.79-1.26-2.79-2.79v-8.65h-4.2V65.6L101.34,65.6z M21.67,101.81h-4.33v8.65 c0,1.54-1.26,2.79-2.79,2.79H2.79C1.26,113.25,0,112,0,110.46V56.95c0-1.54,1.26-2.79,2.79-2.79h11.76c1.54,0,2.79,1.26,2.79,2.79 v8.65h4.33V101.81L21.67,101.81z"/></g></svg><!-- https://uxwing.com/valve-icon/ -->
    <br /><br />
    <svg version="1.1" id="o2valve" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" x="0px" y="0px" viewBox="0 0 122.88 117.35" style="enable-background:new 0 0 122.88 117.35" xml:space="preserve"><style type="text/css">.st0{fill-rule:evenodd;clip-rule:evenodd;}</style><g fill="#2f2f2f"><path class="st0" d="M79.22,54.41c5.16,3.03,9.06,6.88,11.81,11.19h5.36v36.21h-5.73c-5.72,9.03-15.74,15.53-28.98,15.53 c-13.23,0-23.34-6.5-29.09-15.53h-5.96V65.6h5.91c2.98-4.48,7.25-8.48,12.96-11.58c3.04-1.64,4.88-3.69,4.9-5.53 c0.03-2.33-2.52-4.31-5.26-4.31h-1.36c-3.32,0-6.04-2.72-6.04-6.04l0,0c0-3.32,2.72-6.04,6.04-6.04h7.76l6.1-7.57 c-0.02-0.28-0.04-0.57-0.04-0.87v-9.99H32.87c-3.77,0-6.85-3.08-6.85-6.85l0,0c0-3.76,3.08-6.85,6.85-6.85h58.18 c3.76,0,6.85,3.08,6.85,6.85l0,0c0,3.77-3.08,6.85-6.85,6.85H67.51v9.99c0,0.41-0.03,0.82-0.07,1.21l5.48,7.23h6.67 c3.32,0,6.04,2.72,6.04,6.04l0,0c0,3.32-2.72,5.97-6.04,6.04c-4.57,0.1-6.25,2.06-6.04,4.34C73.91,52.73,76.83,53.01,79.22,54.41 L79.22,54.41z M101.34,65.6h4.2v-8.65c0-1.54,1.26-2.79,2.79-2.79h11.76c1.54,0,2.79,1.26,2.79,2.79v53.51 c0,1.54-1.26,2.79-2.79,2.79h-11.76c-1.54,0-2.79-1.26-2.79-2.79v-8.65h-4.2V65.6L101.34,65.6z M21.67,101.81h-4.33v8.65 c0,1.54-1.26,2.79-2.79,2.79H2.79C1.26,113.25,0,112,0,110.46V56.95c0-1.54,1.26-2.79,2.79-2.79h11.76c1.54,0,2.79,1.26,2.79,2.79 v8.65h4.33V101.81L21.67,101.81z"/></g></svg><!-- https://uxwing.com/valve-icon/ -->
  </td>
  <td class="he" onclick="document.getElementById('hemix').focus(); document.getElementById('hemix').select();">
  <div class="big">He <span id="he">--.-</span>%</div>
  <!-- <div>(<span id="hemv">--.--</span>mV)</div> -->
  <div>&nbsp;<span id="heonoff"></span>&nbsp;</div>
  <div class="inter">Setpoint: <input onBlur="DoForm();" onfocus="this.select();" size="4" type="text" class="inp" id="hemix" value="{{HE_MIX}}" />%</div>
  </td>
  </tr>
  <tr>
  <td  class="ip" onclick="incDebugMode('ip');">
  IP: {{IP_ADDRESS}}
  </td>
  <td class="ru">
  <svg id="run" xmlns:svg="http://www.w3.org/2000/svg" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" version="1.0" width="48px" height="48px" viewBox="0 0 128 128" xml:space="preserve"><rect x="0" y="0" width="100%" height="100%" fill="transparent" /><g><path d="M26.43 44.95a45.68 45.68 0 0 0 18.6 36.82h-.3a36.83 36.83 0 1 1 0-73.65h.3a45.68 45.68 0 0 0-18.6 36.83zm44-22.55a45.68 45.68 0 0 0-29.28 29.07l-.1-.28a36.83 36.83 0 0 1 70.05-22.8l.1.3a45.68 45.68 0 0 0-40.77-6.32zm35.27 34.9A45.68 45.68 0 0 0 69 38.45c.1-.06.17-.13.25-.2a36.83 36.83 0 1 1 43.3 59.6c-.08.07-.17.12-.25.18a45.68 45.68 0 0 0-6.6-40.73zm-22.57 44.3a45.68 45.68 0 0 0 6.6-40.72c.08.05.16.1.24.17a36.83 36.83 0 0 1-43.3 59.6l-.24-.2a45.68 45.68 0 0 0 36.7-18.85zm-48.8-7.86a45.68 45.68 0 0 0 40.77-6.3l-.1.28A36.83 36.83 0 1 1 4.96 64.95l.1-.28a45.68 45.68 0 0 0 29.28 29.07z" fill="#2ec0ce"/><animateTransform attributeName="transform" type="rotate" from="0 64 64" to="72 64 64" dur="1080ms" repeatCount="indefinite"></animateTransform></g></svg><!-- https://icons8.com/preloaders/en/search/twirl -->
  </td>
  <td class="da" onclick="incDebugMode('da');">
  <span id="time">--.--.---- --:--:--</span>
  </td>
  </tr>
  <tr>
  <td class="ca" onclick="sendOption(2);">
  <div class="big"><span class="noblink" id="cab">Calibrate</span></div>
  <br />
  <div>D1 probe, near He: <span id="d1c">{{CALIBRATED_D1}}</span>mV</div>
  <div>D2 probe, near O2: <span id="d2c">{{CALIBRATED_D2}}</span>mV</div>
  <div>Calibrated: <span id="cd">{{CALIBRATED_DATE}}</span></div>
  </td>
  <td class="mo">
  <svg id="comp" xmlns="http://www.w3.org/2000/svg" shape-rendering="geometricPrecision" text-rendering="geometricPrecision" image-rendering="optimizeQuality" fill-rule="evenodd" clip-rule="evenodd" viewBox="0 0 512 461.169"><g fill="#2f2f2f"><path d="M164.327 88.809h232.902v308.103h-11.572l20.326 47.279a11.763 11.763 0 011.181 5.155c0 6.529-5.294 11.823-11.823 11.823H195.626v-.047c-1.318 0-2.658-.223-3.971-.693-6.122-2.192-9.305-8.934-7.113-15.055l17.363-48.462h-37.578c-10.333 0-19.732-4.231-26.547-11.045l-.026-.026c-6.814-6.815-11.046-16.214-11.046-26.548v-15.651H79.376c-5.759 0-11.008-2.358-14.8-6.151-3.785-3.785-6.148-9.022-6.148-14.797v-59.423H20.411C9.139 263.271 0 254.132 0 242.86c0-11.271 9.139-20.41 20.411-20.41h38.017v-59.423c0-5.773 2.351-11.011 6.144-14.803 3.793-3.793 9.03-6.144 14.804-6.144h47.332v-15.652c0-10.334 4.232-19.733 11.046-26.548l.026-.026c6.815-6.813 16.214-11.045 26.547-11.045zM247.214 0h105.097c17.73 0 32.233 14.536 32.233 32.233v.004c0 17.696-14.535 32.233-32.233 32.233H247.214c-17.697 0-32.233-14.505-32.233-32.233v-.004C214.981 14.503 229.486 0 247.214 0zM122.04 165.726H82.075v154.269h39.965V165.726zm298.836-76.238c23.617 2.73 44.866 13.533 60.888 29.557C500.421 137.7 512 163.44 512 191.757v109.537c0 26.302-10.757 50.211-28.083 67.535-16.333 16.334-38.515 26.828-63.041 27.976V89.488zm-58.625 312.798H224.994l-12.624 35.236h165.029l-15.148-35.236zm-161.583-218.22c-7.45 0-13.492-6.042-13.492-13.492 0-7.451 6.042-13.493 13.492-13.493h146.274c7.45 0 13.492 6.042 13.492 13.493 0 7.45-6.042 13.492-13.492 13.492H200.668zm0 147.512c-7.45 0-13.492-6.042-13.492-13.493 0-7.45 6.042-13.492 13.492-13.492h146.274c7.45 0 13.492 6.042 13.492 13.492 0 7.451-6.042 13.493-13.492 13.493H200.668zm0-73.756c-7.45 0-13.492-6.041-13.492-13.492 0-7.45 6.042-13.492 13.492-13.492h146.274c7.45 0 13.492 6.042 13.492 13.492 0 7.451-6.042 13.492-13.492 13.492H200.668z"/></g></svg><!-- https://uxwing.com/motor-pump-solid-icon/ -->
  <!-- <svg id="comp" viewBox="0 0 40 40" xmlns="http://www.w3.org/2000/svg"><g fill="#20ff20"><circle cx="20" cy="15" r="15" /></g></svg> -->
  </td>
  <td class="fi" onclick="sendOption(3);">
  <div class="big"><span class="noblink" id="fib">FILL</span></div>
  <br />
  <div><span id="tf"></span></div>
  </td>
  </tr>
  <tr>
  <td  class="so">
  {{APPLICATION_INFO}} <span id="debugInfo">!DEBUG!</span>
  </td>
  <td>
  <input class="hid" id="submit" type="submit" value="Update" />
  </td>
  <td class="co">
  {{APP_COPYRIGHT}}
  </td>
  </tr>
  </table>
  <input type="hidden" id="lastContact" value="0" />
  <input type="hidden" id="option" value="0" />
  <input type="hidden" id="selectOption" value="0" />
  <input type="hidden" id="debugMode" value="0" />
  </form>
  <script type="text/javascript">
  
  var lastContact = 0;
  var option = 0;
  var selectOption = 0;
  var ws;
  var incDebugModeDa = 0;
  var incDebugModeIp = 0;

  var heon = 0;
  var o2on = 0;
  var tf = 0;
  
  function incDebugMode(elem) {
    if ('da' == elem) {
      incDebugModeDa++;
    } else if ('ip' == elem) {
      incDebugModeIp++;
    }
    
    if ((incDebugModeDa%4 >= 2) && (incDebugModeIp%4 >= 2)) {
      if ((ws !== undefined) && (ws.readyState == 1)) {
        ws.send(JSON.stringify({ debugMode: "1" }));
      } else {
        document.getElementById("debugMode").value = '1';
        console.log(`Debug mode is now enabled (no WS)`);
      }
    }
  }

  function DoForm() {
    document.getElementById("o2mix").blur();
    document.getElementById("hemix").blur();
    o2mix = parseFloat(document.getElementById("o2mix").value);
    hemix = parseFloat(document.getElementById("hemix").value);
    if (isNaN(o2mix)) {
      o2mix = 0.0;
    } else if (o2mix < 0.0) {
      o2mix = 0.0;
    } else if (o2mix > 40.0) {
      o2mix = 40.0 - 0.1;
    }
    if (isNaN(hemix)) {
      hemix = 0.0;
    } else if (hemix < 0.0) {
      hemix = 0.0;
    }
    if (hemix > 99.9) {
      hemix = 99.9;
      }
    document.getElementById("o2mix").value = o2mix.toFixed(1);
    document.getElementById("hemix").value = hemix.toFixed(1);

    if ((ws !== undefined) && (ws.readyState == 1)) {
      ws.send(JSON.stringify({ o2mix: o2mix.toFixed(1), hemix: hemix.toFixed(1) }));
    }
  }
  
  function sendOption(newOption) {
    var newSelectOption = ((option != newOption) ? 1 : (1 - selectOption));
    if ((ws !== undefined) && (ws.readyState == 1)) {
      ws.send(JSON.stringify({ option: newOption.toString(), selectOption: newSelectOption.toString() }));
    } else {
      console.log("Unable to send (current option: " + option + " ): JSON.stringify({ option: " + newOption.toString() + ", selectOption: " + newSelectOption.toString() + " })");
    }
    document.getElementById("option").value = newOption;
    document.getElementById("selectOption").value = (newSelectOption ? 1 : 0);
  }

  setInterval(
    function () {
      option = parseInt(document.getElementById("option").value);
      selectOption = (1 == parseInt(document.getElementById("selectOption").value));
      debugMode = (1 == parseInt(document.getElementById("debugMode").value));

      var blink = 0;
      var steps = 1000;
      var step = Date.now() % steps;
      if (step < (steps/2)) {
        // blink = (step)/(steps/2);
        blink = 1;
      } else {
        // blink = (steps-step)/(steps/2);
        blink = 0;
      }

      if (selectOption && (2 == option)) {
       document.getElementById('cab').className = 'blink';
      } else {
       document.getElementById('cab').className = 'noblink';
      }

      if (selectOption && (3 == option)) {
       document.getElementById('fib').className = 'blink';
      } else {
       document.getElementById('fib').className = 'noblink';
      }

      let blinkElements = document.querySelectorAll(".blink");
      for (let i = 0; i < blinkElements.length; i++) {
        blinkElements[i].style.opacity = blink;
      }
      let noBlinkElements = document.querySelectorAll(".noblink");
      for (let i = 0; i < noBlinkElements.length; i++) {
        noBlinkElements[i].style.opacity = 1;
      }

      if (selectOption && (3 == option)) {
        if (tf > 0) {
          document.getElementById("tf").innerHTML = 'Total time: ' + parseInt(tf/1000) + ' seconds'
          document.getElementById("heonoff").innerHTML = parseInt(0.5 + 100.0 * heon/tf).toString() + "% ON, " + (100-parseInt(0.5 + 100.0 * heon/tf)).toString() + "% OFF";
          document.getElementById("o2onoff").innerHTML = parseInt(0.5 + 100.0 * o2on/tf).toString() + "% ON, " + (100-parseInt(0.5 + 100.0 * o2on/tf)).toString() + "% OFF";
        } else {
          document.getElementById("tf").innerHTML = '';
          document.getElementById("heonoff").innerHTML = '';
          document.getElementById("o2onoff").innerHTML = '';
        }
      }

      document.getElementById("run").style.visibility = (((Date.now() - document.getElementById("lastContact").value) < 2000) ? 'visible' : 'hidden');
      document.getElementById('debugInfo').style.visibility = (debugMode ? 'visible' : 'hidden');
    }, 100
  );

  function wsConnect() {
    ws = new WebSocket("ws://" + location.hostname + ":3000/");

    ws.onmessage = (e) => {
      const dataArray = JSON.parse(e.data);

      document.getElementById("lastContact").value = Date.now();
      
      /*
      if (dataArray.hemv !== undefined) {
        document.getElementById("hemv").innerHTML = dataArray.hemv;
      }
      if (dataArray.o2mv !== undefined) {
        document.getElementById("o2mv").innerHTML = dataArray.o2mv;
      }
      */
      if (dataArray.he !== undefined) {
        document.getElementById("he").innerHTML = dataArray.he;
      }
      if (dataArray.o2 !== undefined) {
        document.getElementById("o2").innerHTML = dataArray.o2;
      }
      if (dataArray.time !== undefined) {
        document.getElementById("time").innerHTML = dataArray.time;
      }
      if (dataArray.comp !== undefined) {
        var comp_color = '#2f2f2f';
        switch(parseInt(dataArray.comp)) {
          case 100: // must be off, is off
            comp_color = '#2f2f2f';
            break;
          case 111: // must be on, is on
            comp_color = '#20ff20';
            break;
          case 110: // must be on, is off
            comp_color = '#ffff20';
            break;
          case 101: // must be off, is on, or error
          default:
            comp_color = '#ff2020';
            break;
        }
        document.getElementById("comp").getElementsByTagName("g")[0].style.fill = comp_color;
      }
      if (dataArray.o2valve !== undefined) {
        document.getElementById("o2valve").getElementsByTagName("g")[0].style.fill = (('ON' == dataArray.o2valve) ? '#a0a0a0' : '#2f2f2f');
      }
      if (dataArray.hevalve !== undefined) {
        document.getElementById("hevalve").getElementsByTagName("g")[0].style.fill = (('ON' == dataArray.hevalve) ? '#bb4400' : '#2f2f2f');
      }
      if (dataArray.op !== undefined) {
        document.getElementById("option").value = dataArray.op;
      }
      if (dataArray.so !== undefined) {
        document.getElementById("selectOption").value = dataArray.so;
      }
      if (dataArray.o2mix !== undefined) {
        document.getElementById("o2mix").innerHTML = dataArray.o2mix.toFixed(1);
      }
      if (dataArray.hemix !== undefined) {
        document.getElementById("hemix").innerHTML = dataArray.hemix.toFixed(1);
      }
      if (dataArray.cd !== undefined) {
        document.getElementById("cd").innerHTML = dataArray.cd;
      }
      if (dataArray.d1c !== undefined) {
        document.getElementById("d1c").innerHTML = dataArray.d1c;
      }
      if (dataArray.d2c !== undefined) {
        document.getElementById("d2c").innerHTML = dataArray.d2c;
      }
      if (dataArray.debug !== undefined) {
        document.getElementById("debugMode").value = parseInt(dataArray.debug);
      }
      if (dataArray.heon !== undefined) {
        heon = parseInt(dataArray.heon);
      }
      if (dataArray.o2on !== undefined) {
        o2on = parseInt(dataArray.o2on);
      }
      if (dataArray.tf !== undefined) {
        tf = parseInt(dataArray.tf);
      }
    };

    ws.onclose = (e) => {
      const reconnectInterval = 1000;
      console.log(`ws is closed(${e.code}) ${e.reason}, reconnect after ${reconnectInterval} msec.`);
      setTimeout(() => {
        wsConnect();
      }, reconnectInterval);
    };

    ws.onerror = (e) => {
      console.log(e.message);
      ws.close();
    };
  }

  wsConnect();
  </script>
  </body>
<html>
)*lit";