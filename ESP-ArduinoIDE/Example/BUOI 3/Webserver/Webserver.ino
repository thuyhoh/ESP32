#ifdef ESP32
  #include <WiFi.h>
  #include <WebServer.h>
  WebServer server(80);
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
  ESP8266WebServer server(80);
#endif




const char* ssid = "Bao Truc";
const char* pass = "24682468";

#define LED1 2
#define LED2 16

int trangthairelay1 = 0;
int trangthairelay2 = 0;

int bien = 0;
int bien1 = 0;
int bien2 = 0;
long last = 0;
int nhietdo = 0;
int doam = 0;
int anhsang = 0;
String ChuoiSendWebJson = "";


// lưu trữ liệu => thực hiện chức luu => uno esp reset  , EEPROM ( code )  => cúp điện , reset uno esp => lưu biến giá 10

const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>

<head>
    <meta charset="utf-8"> 
</head>

<script>

function Mode()
{
  console.log("OnClick Button MODE");
  var xhttp = new XMLHttpRequest();
  xhttp.open('GET','/SendMode',true);
  xhttp.send();
}

function TB1()
{
  console.log("OnClick Button TB1");
  var xhttp = new XMLHttpRequest();
  xhttp.open('GET','/TB1',true);
  xhttp.send();
}

function TB2()
{
  console.log("OnClick Button TB2");
  var xhttp = new XMLHttpRequest();
  xhttp.open('GET','/TB2',true);
  xhttp.send();
}

</script>

<body>

<h1>WEBSERVER</h1>


<h1>
MODE
<button type="button" onclick= "Mode()" >MODE</button>
</h1>


<h1>
THIẾT BỊ 1: 
<button type="button" onclick= "TB1()">THIẾT BỊ 1</button>
</h1>


<h1>
THIẾT BỊ 2: 
<button type="button" onclick= "TB2()">THIẾT BỊ 2</button>
</h1>


<h1>
NHIỆT ĐỘ: 
<label>0</label>
</h1>

<h1>
ĐỘ ẨM: 
<label>0</label>
</h1>

<h1>
TRẠNG THÁI THIẾT BỊ 1: 
<label>OFF</label>
</h1>

<h1>
TRẠNG THÁI THIẾT BỊ 2: 
<label>OFF</label>
</h1>

<h1>
CÀI ĐẶT 1: 
<label>0</label>
</h1>

<h1>
CÀI ĐẶT 2: 
<label>0</label>
</h1>


<h1>
CÀI ĐẶT 1: 
<input type="text" >
<button type="button" >SEND</button>
</h1>

<h1>
CÀI ĐẶT 2: 
<input type="text" >
<button type="button" >SEND</button>
</h1>




 
</body>
</html>
)=====";


void SendWebPage()
{
  server.send(200, "text/html", webpage );
}


void HamMode()
{
  Serial.println(">>>Nhận được lệnh hàm MODE WEB");
  // làm việc gì đó tại đây

 
  server.send(200, "text/html", webpage );
 
}

void DK_TB1()
{
  Serial.println(">>>Điều khiển thiết bị 1");
  server.send(200, "text/html", webpage );
}

void DK_TB2()
{
  Serial.println(">>>Điều khiển thiết bị 2");
  server.send(200, "text/html", webpage );
}
void setup()
{
  Serial.begin(9600);
  while (!Serial);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);


  // kết nối wifi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  //=============================================================
  Serial.println();
  Serial.println("Connect WiFi");
  Serial.print("Address IP esp: ");
  Serial.println(WiFi.localIP());
  
  

  server.on("/", [] {
    SendWebPage();
  });


  server.on("/SendMode", [] {
    HamMode();
  });

  server.on("/TB1", [] {
    DK_TB1();
  });

  server.on("/TB2", [] {
     DK_TB2();
  });

  
  server.begin();

  Serial.println("Server start");

  last = millis();

  
}

void loop() 
{
  server.handleClient(); // duy tri server cho mình
}
