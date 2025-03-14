// chọn board esp8266 / esp32
#ifdef ESP32
#include <WiFi.h>
#include <HTTPClient.h>
#define LED1 2
#define DEN 23
#define QUAT 22
#define NUT1 21
#define NUT2 19
#else
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#define LED1 2
#define DEN D0
#define QUAT D1
#define NUT1 D2
#define NUT2 D3
#endif

#include <ArduinoJson.h> //6.10.0

float nhietdo;
float doam;
long TTTB1, TTTB2, BienEF , BienGH, BienJK, BienKL;
long last = 0;
String ChuoiSendWebJson = "";
char ssid[] = "Bao Truc";
char pass[] = "24682468";
String Data = "";


void setup()
{
  Serial.begin(9600);
  while (!Serial);
  // khởi tạo GPIO ESP
  pinMode(LED1, OUTPUT);
  pinMode(DEN, OUTPUT);
  pinMode(QUAT, OUTPUT);
  pinMode(NUT1, INPUT_PULLUP);
  pinMode(NUT2, INPUT_PULLUP);
  // tắt con relay đi
  digitalWrite(DEN, LOW);
  digitalWrite(QUAT, LOW);
  Led_Blynk_OFF();
  // cần kết nối wifi
  ConnectWiFi();
  Led_Blynk_ON();
  last = millis();
}

void loop()
{
  Button();
  if (WiFi.status() == WL_CONNECTED)
  {
    if (millis() - last > 1000)
    {
      Read_DieuKhien_PHP();
      chuongtrinhcambien();
      Send_Data_PHP( String(nhietdo) , String(doam) , String(TTTB1) , String(TTTB2) , String(BienEF), String(BienGH));
      last = millis();
    }
  }
}

void Send_Data_PHP(String nhietdo , String doam  , String TTTB1 , String TTTB2 , String BienEF , String BienGH )
{
  HTTPClient http;
  http.begin("http://192.168.1.143:82/IOTDATABASE/ESPinsertdatabase.php");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String httpRequestData = "nhietdo="   + String(nhietdo)
                           + "&doam=" +  String(doam)
                           + "&relay1="  + String(TTTB1)
                           + "&relay2="  + String(TTTB2)
                           + "&cai1="  + String(BienEF)
                           + "&cai2="  + String(BienGH)
                           + "";
  int httpResponseCode = http.POST(httpRequestData);
  if (httpResponseCode == 200)
  {
    Serial.print("Send Data Thành Công:");
    Serial.println(httpRequestData);
    delay(200);
  }
  else
  {
    Serial.println("Không gửi dữ liệu được!!!");
  }
  http.end();
  //=================================================================================
}


void Read_DieuKhien_PHP()
{
  HTTPClient http;
  http.begin("http://192.168.1.143:82/IOTDATABASE/ESPdieukhien.php");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.GET();
  if (httpCode == 200)
  {
    String payload = http.getString();
    Serial.print("Data Read PHP:");
    Serial.println(payload);
    XuLyJSON(String(payload));
  }
  http.end();
}


void checkwifi()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    ConnectWiFi();
  }
}
// hàm gửi dữ liệu lên server


void Led_Blynk_OFF()
{
#ifdef ESP32
  digitalWrite(LED1, LOW);
#else
  digitalWrite(LED1, HIGH);
#endif
}
void Led_Blynk_ON()
{
#ifdef ESP32
  digitalWrite(LED1, HIGH);
#else
  digitalWrite(LED1, LOW);
#endif
}

void ConnectWiFi()
{
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connect WIFI!!!");
}
void Button()
{
  // chuong trình chuyển chế độ AUTO - MAN
  // Nhấn xuống => thả tay ra mới làm việc

  if (digitalRead(NUT1) == LOW)
  {
    delay(300);
    while (1)
    {
      Serial.println("Nút 1 đã dc nhấn");

      last = millis();
      if (digitalRead(NUT2) == HIGH)
      {
        // thực hiện lệnh ở đây
        // xử lý MAN AUTO

        if (TTTB1 == 0)
        {
          digitalWrite(DEN, HIGH);
          TTTB1 = 1;
        }
        else if (TTTB1 == 1)
        {
          digitalWrite(DEN, LOW);
          TTTB1 = 0;
        }
        break;
      }
    }
  }

  if (digitalRead(NUT2) == LOW)
  {
    delay(300);
    while (1)
    {
      Serial.println("Nút 2 đã dc nhấn");

      last = millis();
      if (digitalRead(NUT2) == HIGH)
      {
        // thực hiện lệnh ở đây
        // xử lý MAN AUTO

        if (TTTB2 == 0)
        {
          digitalWrite(QUAT, HIGH);
          TTTB2 = 1;
        }
        else if (TTTB2 == 1)
        {
          digitalWrite(QUAT, LOW);
          TTTB2 = 0;
        }
        break;
      }
    }
  }
}
void chuongtrinhcambien()
{
  nhietdo = nhietdo + 1.1;
  doam = doam + 1.2;
}

void XuLyJSON(String payload)
{
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, payload);
  
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
  else
  {
    Serial.println("Data JSON: ");
    serializeJsonPretty(doc, Serial);
    String RL1 = doc["TB1"];
    String RL2 = doc["TB2"];
    String  C1 = doc["C1"];
    String  C2 = doc["C2"];
    Serial.println();
    Serial.println(RL1);
    Serial.println(RL2);
    Serial.println(C1);
    Serial.println(C2);

    if (RL1 == "1")
    {
      digitalWrite(DEN, HIGH);
      TTTB1 = 1;
    }
    else if (RL1 == "0")
    {
      digitalWrite(DEN, LOW);
      TTTB1 = 0;
    }

    if (RL2 == "1")
    {
      digitalWrite(QUAT, HIGH);
      TTTB2 = 1;
    }
    else if (RL2 == "0")
    {
      digitalWrite(QUAT, LOW);
      TTTB2 = 0;
    }

    BienEF = C1.toInt();
    BienGH = C2.toInt();  
  }
}
