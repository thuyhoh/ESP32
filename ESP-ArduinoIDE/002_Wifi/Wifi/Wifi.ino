#include <WiFi.h>

const char *ssid = "6 aesiunhan";
const char *pass = "cccccccc";

void Wifi_Connect(const char *ssid, const char *pass);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  void Wifi_Connect(ssid, pass);
}

void loop()
{
  // put your main code here, to run repeatedly:
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
