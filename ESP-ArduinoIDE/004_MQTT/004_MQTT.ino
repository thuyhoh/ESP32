#include <WiFi.h>
#include <PubSubClient.h>



/* WiFi */
const char* ssid = "";
const char* pass = "";

/* MQTT */
WiFiClient espClient;
PubSubClient client(espClient);
const char* mqtt_ip = "";
uint32_t mqtt_port = 1883; // 8883
const char* mqtt_topic = "esp/test";

String msg = "Hello Esp32"; 

void Wifi_Connect(const char *ssid, const char *pass);
void Callback(char* a, byte* payload, unsigned int length)
void ConnectMqttBroker(const char* mqtt_ip, uint32_t mqtt_port, const char* mqtt_topic);
void ReconnectMqttBroker();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wifi_Connect(ssid, pass);
  ConnectMqttBroker(mqtt_ip, mqtt_port);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!client.Connect())
  {
    ReconnectMqttBroker(mqtt_ip, mqtt_topic);
  }
  client.publish(mqtt_topic, msg);
  delay(100);
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

void Callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Messange from topic [");
  Serial.print(topic);
  Serial.print("]");
  String Data = "";
  for(int i = 0; i< lenght; i++)
  {
    Data += payload[i];
  }
  Serial.println(Data.c_str);
}

void ConnectMqttBroker(const char* mqtt_ip, uint32_t mqtt_port)
{
  client.setServer(mqtt_ip, mqtt_port);
  delay(10);
  client.setCallback(Callback);
  delay(10);
}

void ReconnectMqttBroker(const char* mqtt_ip, const char* mqtt_topic)
{
  while(!client.Connect())
  {
    Serial.println("Reconnect to MQTT Broker ...");
    if(client.Connect(mqtt_ip))
    {
      Serial.println("Connect to MQTT Broker...");
      client.subscribe(mqtt_topic);
    }else
    {
      Serial.println("Connect false. Try again after 5 seconds");
      delay(5000);
    }
  }
}

