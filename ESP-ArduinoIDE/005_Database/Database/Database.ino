#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h> //6.10.0

/* WiFi */
const char* ssid = "6 aesiunhan";
const char* pass = "cccccccc";
void Wifi_Connect(const char *ssid, const char *pass);

const char* serverUrl = "http://localhost:80/IOTDATABASE/insertdatabase.php";
char* Http_Get(const char* serverUrl);
void Http_Post(const char* serverUrl);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wifi_Connect(ssid, pass);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (WiFi.status() == WL_CONNECTED) {
    Http_Get(serverUrl);
    Http_Post(serverUrl);
  }else {
    Serial.println("WiFi Disconnected");
  }
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

char* Http_Get(const char* serverUrl)
{
  string payload;
  HTTPClient http;

  http.begin(serverUrl); 
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");  
  int httpCode = http.GET();
  if (httpCode == 200)
  {
    payload = http.getString();
    Serial.print("Data Read PHP: ");
    Serial.println(payload);
  }
  // End the connection
  http.end();
  return payload.c_str();
}

void Http_Post(const char* serverUrl, const char* httpRequestData)
{
  HTTPClient http;
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpResponseCode = http.POST(httpRequestData);
  if (httpResponseCode == 200)
  {
    Serial.print("UPdate Thành Công:");
    Serial.println(httpRequestData);
    delay(200);
  }
  else
  {
    Serial.println("Không gửi dữ liệu được!!!");
  }
  // End the connection
  http.end();
}

