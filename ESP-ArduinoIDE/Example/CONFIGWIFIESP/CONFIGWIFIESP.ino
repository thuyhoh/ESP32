#ifdef ESP32
#include <WiFi.h>
#include <WebServer.h>
WebServer server(80);
#include <BlynkSimpleEsp32.h>
#define LED1 2
#define RELAY1 23
#define RELAY2 22
#define NUT1 21
#define NUT2 19
#define NUT3 18
#else
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <BlynkSimpleEsp8266.h>
ESP8266WebServer server(80);
#define LED1 2
#define RELAY1 D0
#define RELAY2 D1
#define NUT1 D2
#define NUT2 D3
#define NUT3 D5
#endif
#include <EEPROM.h>
#include <WiFiClient.h>
#define BLYNK_PRINT Serial
#define EEPROM_SIZE 512
#include <Ticker.h>
#include <time.h>
Ticker ticker;
WidgetLED LEDCONNECT(V0);
BlynkTimer timer;
bool Connected2Blynk = false;
long XoaEEPROM = 0;


/*
   V0 LED app biểu thị esp - app đang hoạt động
   V1 => Button thiết bị 1 APP
   V2 => button thiết bị 2 APP
   V3 => hiển thị nhiệt độ
   V4 => hiển thị Độ ẩm
*/
int trangthairelay1 = 0;
int trangthairelay2 = 0;


int dem = 0;
bool connectwifi = false;
bool connectkey = false;
bool connectwifiEEPROM = false;
int lengthssid = 20; // 1 đến 20
int lengthpass = 20; // 21 đến 40
int lengthkey = 40;  // 41 đế 80//
int addressSsid = 81;
int addressPass = 82;
int addressKey = 83;
bool checklengthwifikey = false;
const char* ssid1 = "";
const char* pass1 = "";
const char* key1 = "";
const char* ssid = "";
const char* pass = "";
const char* key = "";
const char* ssidapp = "ESPCONFIG";
const char* passapp = "123456789";
String ssid3 , pass3 , key3 = "";
String ssid2 , pass2 , key2  = "";
long nhietdo = 0;
long doam = 0;
long last = 0;


void blinkLedWidget()
{
  if (LEDCONNECT.getValue())
  {
    LEDCONNECT.off();

  }
  else
  {
    LEDCONNECT.on();
  }
}


//NUT BUTTON V1
BLYNK_WRITE(V1)
{
  last = millis();
  int btn1 = param.asInt();

  if (btn1 == 1)
  {
    Serial.println("ON THIẾT BỊ 1");
    digitalWrite(RELAY1, HIGH);
    trangthairelay1 = 1;
    Blynk.virtualWrite(V1, trangthairelay1);
  }
  if (btn1 == 0)
  {
    Serial.println("OFF THIẾT BỊ 1");
    digitalWrite(RELAY1, LOW);
    trangthairelay1 = 0;
    Blynk.virtualWrite(V1, trangthairelay1);
  }

}

//NUT BUTTON V2
BLYNK_WRITE(V2)
{
  last = millis();
  int btn2 = param.asInt();

  if (btn2 == 1)
  {
    Serial.println("ON THIẾT BỊ 2");
    digitalWrite(RELAY2, HIGH);
    trangthairelay2 = 1;
    Blynk.virtualWrite(V2, trangthairelay2);
  }
  if (btn2 == 0)
  {
    Serial.println("OFF THIẾT BỊ 2");
    digitalWrite(RELAY2, LOW);
    trangthairelay2 = 0;
    Blynk.virtualWrite(V2, trangthairelay2);
  }

}

const char webpage[] PROGMEM = R"=====(
 <!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8"> 
 <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<style>
body {font-family: Time New Roman;}
.tab {
    overflow: hidden;
    border: 1px solid #ccc;
    background-color: #FFCC00;
}

.button {
  display: inline-block;
  padding: 15px 25px;
  font-size: 24px;
  cursor: pointer;
  text-align: center;
  text-decoration: none;
  outline: none;
  color: #fff;
  background-color: #4CAF50;
  border: none;
  border-radius: 15px;
  box-shadow: 0 9px #999;
}
.button:hover {background-color: #3e8e41}
.button:active {
  background-color: #3e8e41;
  box-shadow: 0 5px #666;
  transform: translateY(4px);
}
</style>
<script type="text/javascript">




function SEND()
{
  var ssid =  document.getElementById("ssid").value ;
  var pass =  document.getElementById("pass").value ;
  var key =  document.getElementById("key").value ;
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET","/webconfigwifi?ssid="+ssid+"&pass="+pass+"&key="+key,true);      
  xhttp.send();
}
</script>
<body  style= "background-color: #000000;">       

<table width="500" height="400" border="4" cellpadding="1" align="center">
     <tr>
        <th  bgcolor="#66ff99" colspan="2"> <center> <font size="10"> <span style = "color: red">CONFIG WIFI AND CONNECT APP</span></font></center></th>
   </tr>  
  
  
   <tr>
        <td  bgcolor="#FFFF00" colspan="1"> <center> <span style = "color: Blue"> <b> Tên WiFi</b> </span> </center> </td>
        <td bgcolor="#FF00FF" colspan="1">
     <center>
        <input id="ssid" type="text" size="40" placeholder="Tên WiFi" >
      </center>

        </td>
     </tr> 
   
   <tr>
        <td  bgcolor="#FFFF00" colspan="1"> <center> <span style = "color: Blue"> <b> Mật Khẩu WiFi</b> </span> </center> </td>
        <td bgcolor="#FF00FF" colspan="1">
      <center>
        <input id="pass" type="text" size="40" placeholder="Mật Khẩu WiFi" >
      </center>
  
        </td>
     </tr>  
   <tr>
        <td  bgcolor="#FFFF00" colspan="1"> <center> <span style = "color: Blue"> <b> Mã Key App</b> </span> </center> </td>
        <td bgcolor="#FF00FF" colspan="1">
      <center>
        <input id="key" type="text" size="40" placeholder="Mã Key App"  >
  </center>
        </td>
     </tr> 
   <tr>
        <td  bgcolor="#FFFF00" colspan="1"> <center> <span style = "color: Blue"> <b> Gửi</b> </span> </center> </td>
        <td bgcolor="#FF00FF" colspan="1">
      <center>
         <button  type="button" class="button" onclick = "SEND()" style="height:50px; width:300px; ">Gửi</button>
      </center>
        </td>
     </tr>
  </table>

</body>
</html>  
)=====";



void webhtml()
{

  server.send(200, "text/html", webpage); 
}

void webconfigwifi()
{
  Serial.println("Write EEPROM Config");
  ssid3 = "", pass3 = "", key3 = "";
  ssid3 = server.arg("ssid");
  pass3 = server.arg("pass");
  key3 = server.arg("key");

  ssid1 = "";
  pass1 = "";
  ssid1 = ssid3.c_str();
  pass1 = pass3.c_str();
  key = key3.c_str();

  Serial.println();

  WiFi.begin(ssid1, pass1);

  while (WiFi.status() != WL_CONNECTED)
  {
    server.handleClient();
    delay(500);
    Serial.print(".");
    dem++;
    if(dem >= 30)
    {
      connectwifi = false;
      Serial.println("");
      Serial.println("Not Connect to WIFI WEBSERVER");
      dem = 0;
      break;
    } 
  }
  
  if(WiFi.status() == WL_CONNECTED)
  {
    Serial.println();
    Serial.print("Successfully connected to WIFI Webserver");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  
    // kết nối Blynk
    Blynk.begin(key, ssid1, pass1);
    // lưu ssid và pass   , key3 vào EEPROM
    WriteEEPROM(String(ssid3), String(pass3), String(key3)); 
    ticker.detach(); 
    Serial.println("Connect Blynk");
    
    connectwifi = true;
  }
  else 
  {
    connectwifi = false;  
  }
}

void ConnectWiFi()
{
  lengthssid = EEPROM.read(addressSsid);delay(5);
  lengthpass = EEPROM.read(addressPass);delay(5);
  lengthkey = EEPROM.read(addressKey); delay(5);
  ssid2 = "", pass2 = "", key2  = "";
  for (int i = 0 ; i < lengthssid ; i++)
  {
    ssid2 +=(char)EEPROM.read(i); delay(10);
  }
  for (int i = 21 ; i < lengthpass + 21; i++)
  {
    pass2 += (char)EEPROM.read(i);delay(10);
  }
  for (int i = 41 ; i < lengthkey + 41; i++)
  {
    key2 += (char)EEPROM.read(i);delay(10);
  }
  ssid = ssid2.c_str();
  pass = pass2.c_str();
  key = key2.c_str();
  
  WiFi.mode(WIFI_AP_STA);WiFi.disconnect(); WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    dem++;
    if(dem >= 15)
    {
      connectwifi = false;
      Serial.println("");
      Serial.println("Not Connect to wifi");
      dem = 0;
      break;
    } 
  }
  
  if(WiFi.status() == WL_CONNECTED)
  {
    connectwifi = true; 
  }
  else 
  {
    connectwifi = false;
  }
}

void configwifikeyblynk()
{
    delay(1000);
    Serial.println("");
    WiFi.softAP(ssidapp, passapp);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);
    server.on("/", webhtml);
    server.on("/webconfigwifi", webconfigwifi);
    server.begin();
    Serial.println("Server start wifi config");
    last = millis();
    // while(true) == void loop
    while(true)
    {
      server.handleClient();
     if(connectwifi)
     {
        WiFi.mode(WIFI_STA);
        break;
     }
   }
}
void tick()
{

  int state = digitalRead(LED1);  // get the current state of GPIO1 pin
  digitalWrite(LED1, !state);     // set pin to the opposite state
}


void setup(void)
{
  Serial.begin(9600);
  while (!Serial);
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, LOW);

  // khởi tạo GPIO ESP
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(NUT1, INPUT_PULLUP);
  pinMode(NUT2, INPUT_PULLUP);
  pinMode(NUT3, INPUT_PULLUP);
  // tắt con relay đi
  digitalWrite(RELAY1, LOW);
  digitalWrite(RELAY2, LOW);
  delay(1000);
  BeginEEPROM();
  //Xóa EEPROM
  //ClearEEPROM();
   
  ConnectWiFi();  // => trả về biến connectwifi giá trị true hoặc false

  if(!connectwifi)
  {
    ticker.attach(0.1, tick);  // chơp chớp con led cho biết để rơi vào chế độ configwifi
    configwifikeyblynk();     // hàm config
  }
  else if(connectwifi)
  {
    Serial.println();
    Serial.println("Successfully connected to WIFI EEPROM: ");
    Serial.print("ssid: ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Blynk.begin(key, ssid, pass);
  }
  timer.setInterval(1000L, blinkLedWidget);
  last = millis();
  
  
}

void loop(void) 
{
  
  Blynk.run();
  timer.run();
  button();
  if (millis() - last >= 1000)
  {

    Led_Blynk();
    Send_CamBien();
    last = millis();
  }
  
}
void BeginEEPROM()
{
   #ifdef ESP32
      if (!EEPROM.begin(EEPROM_SIZE)) 
      {
        Serial.println("failed to init EEPROM");
        delay(2000);
      }
  #else
      for (int i = 0; i <= EEPROM_SIZE; i++)
      {
       EEPROM.begin(i);
        delay(5);
      }
  #endif
}
void Led_Blynk()
{
  
    #ifdef ESP32
      if(Blynk.connected() == 1)
      {
        digitalWrite(LED1, HIGH); 
        Serial.println("Connect Blynk");
      }
      else if(Blynk.connected() == 0)
      {
   
       Serial.println("Not Connect Blynk");
       digitalWrite(LED1, LOW);
    }

  #else
     if(Blynk.connected() == 1)
     {
       digitalWrite(LED1, LOW); 
       Serial.println("Connect Blynk");
     }
      else if(Blynk.connected() == 0)
      {
     
       Serial.println("Not Connect Blynk");
      digitalWrite(LED1, HIGH);
      }

  #endif
}
void button()
{
  // nhấn nút xuống xong thả tay ra thì mới thực hiện lệnh
  if (digitalRead(NUT1) == 0)
  {
    delay(300);
    while (1)
    {
      Lap();
      // kiểm tra nút có thả tay ra chưa
      if (digitalRead(NUT1) == 1)
      {
        // thực hiện lệnh
        if (trangthairelay1 == 1)
        {
          // Tắt
          digitalWrite(RELAY1, LOW);
          trangthairelay1 = 0;
          Blynk.virtualWrite(V1, trangthairelay1);
        }
        else if (trangthairelay1 == 0)
        {
          digitalWrite(RELAY1, HIGH);
          trangthairelay1 = 1;
          Blynk.virtualWrite(V1, trangthairelay1);
        }
        // thoát vòng lặp while(1)
        last = millis();
        break;
      }
    }

  }



  if (digitalRead(NUT2) == 0)
  {
    delay(300);
    while (1)
    {
      Lap();
      // kiểm tra nút có thả tay ra chưa
      if (digitalRead(NUT2) == 1)
      {
        // thực hiện lệnh
        if (trangthairelay2 == 1)
        {
          // Tắt
          digitalWrite(RELAY2, LOW);
          trangthairelay2 = 0;
          Blynk.virtualWrite(V2, trangthairelay2);
        }
        else if (trangthairelay2 == 0)
        {
          digitalWrite(RELAY2, HIGH);
          trangthairelay2 = 1;
          Blynk.virtualWrite(V2, trangthairelay2);
        }
        // thoát vòng lặp while(1)
        last = millis();
        break;
      }
    }

  }



  if (digitalRead(NUT3) == 0)
  {
    delay(300);
    while (1)
    {
      Lap();
      // kiểm tra nút có thả tay ra chưa
      if (digitalRead(NUT3) == 1)
      {
        // thực hiện lệnh
        ClearEEPROM();
        XoaEEPROM = 0;
        Serial.println("Đã xóa EEPROM");
        // thoát vòng lặp while(1)
        last = millis();
        break;
      }
    }

  }
}

void Lap()
{
  Blynk.run(); // duy trì kết nối blynk
  timer.run();

  if (millis() - last >= 1000)
  {
    Led_Blynk();
    Send_CamBien();
    last = millis();
  }
}
void Send_CamBien()
{

  nhietdo++;
  doam = doam + 2;

  if (nhietdo >= 150)
  {
    nhietdo = 0;
  }

  if ( doam  >= 100)
  {
    doam = 0;
  }
  Blynk.virtualWrite(V1, trangthairelay1); // V1 button 1 app
  Blynk.virtualWrite(V2, trangthairelay2); // button 2
  Blynk.virtualWrite(V3, nhietdo); // nhiệt độ
  Blynk.virtualWrite(V4, doam); // độ âm
}
void  WriteEEPROM(String ssid3, String pass3, String key3)
{
  lengthssid = ssid3.length(); 
  lengthpass = pass3.length();
  lengthkey = key3.length();  
  // Lưu độ dài vào EEPROM
  EEPROM.write(addressSsid, lengthssid);delay(5);
  EEPROM.write(addressPass, lengthpass);delay(5);
  EEPROM.write(addressKey, lengthkey);delay(5);
  for (int i = 0 ; i < lengthssid ; i++)
  {
    EEPROM.write(i, ssid3.charAt(i));delay(5);
  }
   for (int i = 21 ; i < lengthpass + 21; i++)
  {
    EEPROM.write(i, pass3.charAt(i - 21));delay(5);
  }
  for (int i = 41 ; i < lengthkey + 41; i++)
  {
    EEPROM.write(i, key3.charAt(i - 41));delay(5);
  }
  EEPROM.commit();delay(5);
  
  lengthssid = EEPROM.read(addressSsid);delay(5);
  lengthpass = EEPROM.read(addressPass);delay(5);
  lengthkey = EEPROM.read(addressKey); delay(5);
  ssid2 = "", pass2 = "", key2  = "";
  for (int i = 0 ; i < lengthssid ; i++)
  {
    ssid2 +=(char)EEPROM.read(i); delay(10);
  }
  for (int i = 21 ; i < lengthpass + 21; i++)
  {
    pass2 += (char)EEPROM.read(i);delay(10);
  }
  for (int i = 41 ; i < lengthkey + 41; i++)
  {
    key2 += (char)EEPROM.read(i);delay(10);
  }
}

void ClearEEPROM()
{
  for (int i = 0 ; i < EEPROM_SIZE ; i++)
  {
    EEPROM.write(i, 0);delay(5);
  }
  EEPROM.commit();delay(5);
}
