#include <WiFi.h>
#include <WebServer.h>
#include <string.h>

WebServer server(80);

#define LED_Pin 2
const char *ssid = "6 aesiunhan";
const char *pass = "cccccccc";
String dataJson = "";
float nd;
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<header>
    <title>Webserver</title>
</header>
<script>
    function LED_Ctrl() {
        console.log("button");
        var tmp = document.getElementById("LED_State").innerHTML;
        if (tmp == "OFF") {
            document.getElementById("LED_State").innerHTML = "ON";
        } else {
            document.getElementById("LED_State").innerHTML = "OFF";
        }
        var xhttp = new XMLHttpRequest();
        xhttp.open('GET', '/Button?Button=' + document.getElementById("LED_State").innerHTML, true);
        xhttp.send();
    }

    function Msg_Send() {
        var tmp = document.getElementById("input1").value;
        if(tmp != null)
        {
            console.log(tmp);
            var xhttp = new XMLHttpRequest();
            document.getElementById("input1").value = "";
            xhttp.open('GET', '/Input?Input=' + tmp, true);
            xhttp.send();
        }
    }
</script>

<body>
    <h1>ESP32 Webserver</h1>
    <div>
        <label>LED Status: </label>
        <label id="LED_State">OFF</label>
        <button id="bnt1" onclick=LED_Ctrl()>button</button>
    </div>
    
    <div>
        <br>Messenge: <input type="text" name="fname" id="input1">
        <button id="bnt2" onclick=Msg_Send()>Send</button>
    </div>
</body>
</html>
)=====";

void Wifi_Connect(const char *ssid, const char *pass);
void Send_WebPage(void);
void Send_Update(void);
void LED_Control(void);

void setup()
{
  Serial.begin(9600);
  /* Connect Wifi */
  Wifi_Connect(ssid, pass);

  // Turn on Server
  server.on("/", []
            { Send_WebPage(); });
  server.on("/LED", []
            { LED_Control(); });
  server.on("/Update", []
            { Send_Update(); });

  server.begin();

  Serial.println("Server start");
}

void loop()
{
  server.handleClient(); // duy tri server
}

void Wifi_Connect(const char *ssid, const char *pass)
{
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  // Connect succesfully
  Serial.println();
  Serial.println("Connect WiFi");
  Serial.print("Address IP esp: ");
  Serial.println(WiFi.localIP());
}

void SendWebPage(void)
{
  server.send(200, "text/html", MAIN_page);
}

void LED_Control(void)
{
  serial.println(">> xu ly LED ");
  char msg[3];
  strcpy(msg, server.arg("LED"));
  serial.print(msg);
  if (strcmp(msg, "ON") == 0)
  {
    digitalWrite(LED_Pin, HIGH);
  }
  else if (strcmp(msg, "OFF") == 0)
  {
    digitalWrite(LED_Pin, LOW);
  }
  else
  {
    Serial.println("error");
  }
}
void Send_Update(void)
{
  dataJson = dataJson + "{\"ND\":\"" + String(nd) + "\"}";
  server.send(200, "text/html", String(dataJson));
}
