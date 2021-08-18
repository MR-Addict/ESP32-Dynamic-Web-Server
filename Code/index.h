const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>ESP32 Web Server</title>
    <style>
      h1 {
        margin: 2rem;
        font-size: 3rem;
      }
      p {
        font-size: 2rem;
      }
      button {
        width: 15rem;
        margin: 1rem;
        padding: 1rem 2rem;
        font-size: 2rem;
        border-radius: 1rem;
      }
      button:hover {
        cursor: pointer;
      }
    </style>
  </head>
  <body align="center">
    <h1>ESP32 Web Server</h1>
    <p id="LED">%LED_Status%</p>
    <p><button onclick="Send_Data('true');">Turn ON</button></p>
    <p><button onclick="Send_Data('false');">Turn OFF</button></p>
    <script>
      function Send_Data(command) {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/LED?LED=" + command, true);
        xhr.send();
        xhr.onreadystatechange = function () {
          if (this.readyState == 4 && this.status == 200)
            document.getElementById("LED").innerHTML = this.responseText;
        };
      }
    </script>
  </body>
</html>
)rawliteral";
