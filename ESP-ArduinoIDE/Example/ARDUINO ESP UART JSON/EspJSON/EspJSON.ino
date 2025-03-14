#include <ArduinoJson.h>// 6.10.0
#include <SoftwareSerial.h>
const byte RX = D6;
const byte TX = D5;
SoftwareSerial mySerial = SoftwareSerial(RX, TX);

#define nut1 D2
#define nut2 D3

String inputString = "";
bool stringComplete = false;
String ChuoiSendEsp = "";

long nhietdo = 0;
long doam = 0;
long trangthaiTB1 = 0;
long trangthaiTB2 = 0;
long Cai1 = 100;
long Cai2 = 500;
long last = 0;

String DataSend = "";

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(nut1, INPUT_PULLUP);
  pinMode(nut2, INPUT_PULLUP);
  last = millis();

}

void loop()
{

  Read_UART_JSON();
  Button();
  if (millis() - last >= 2000)
  {
    RequestDataUno();
    last = millis();
  }

}
void Button()
{
  if (digitalRead(nut1) == 0)
  {
    delay(300);
    while (1)
    {
      
      if (digitalRead(nut1) == 1)
      {
        // gửi lệnh qua kia để điều khiển
        DataSend = "";
        DataSend = "{\"RL1\":\"1\"}";
        StaticJsonDocument<200> doc;
        deserializeJson(doc, DataSend);
        serializeJsonPretty(doc, mySerial);
        mySerial.flush();
        doc.clear();
        break;
      }
    }
  }


  if (digitalRead(nut2) == 0)
  {
    delay(300);
    while (1)
    {
  
      if (digitalRead(nut2) == 1)
      {
        // gửi lệnh qua kia để điều khiển
        DataSend = "";
        DataSend = "{\"RL1\":\"0\"}";
        StaticJsonDocument<200> doc;
        deserializeJson(doc, DataSend);
        serializeJsonPretty(doc, mySerial);
        mySerial.flush();
        doc.clear();
        break;
      }
    }
  }
}
void RequestDataUno()
{
  //{"SEND":"1"}
  DataSend = "";
  //DataSend = "{\"SEND\":\"" + String("1") + "\"}";
  DataSend = "{\"SEND\":\"1\"}";
  StaticJsonDocument<200> doc;
  deserializeJson(doc, DataSend);
  serializeJsonPretty(doc, mySerial);
  mySerial.flush();
  doc.clear();
}

void Read_UART_JSON()
{
  while (mySerial.available())
  {
    const size_t capacity = JSON_OBJECT_SIZE(2) + 256;
    DynamicJsonDocument doc(capacity);
    DeserializationError error = deserializeJson(doc, mySerial);
    if (error)
    {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }
    else
    {
      Serial.println("Data nhận Uno: ");
      serializeJson(doc, Serial);
      Serial.println();
      char buffer[500];
      size_t n = serializeJson(doc, buffer);
      //client.publish(topicpub.c_str(), buffer , n);

      for (int i = 0; i < 500 ; i++)
      {
        buffer[i] = 0;
      }

      doc.clear();
      Serial.println("-------------------------------------------------");
    }

  }
}


void a()
{
  String DataServer = "";
  DataServer  = "{\"RL1\":\"1\"}";
  const size_t capacity = JSON_OBJECT_SIZE(2) + 256;
  DynamicJsonDocument doc(capacity);
  DeserializationError error = deserializeJson(doc, DataServer);
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }
  else
  {
    Serial.println("Data nhận được từ ESP: ");
    serializeJson(doc, Serial);
    Serial.println();
    // xử lý ở đây
    if (doc["RL1"] == "1")
      {
        Serial.println("ON TB 1");
        trangthaiTB1 = 1;
  
        //digitalWrite();
      }
      else if (doc["RL1"] == "0")
      {
        Serial.println("OFF TB 1");
        trangthaiTB1 = 0;

      }

      else if (doc["RL2"] == "1")
      {
        Serial.println("ON TB 2");
        trangthaiTB2 = 1;
    
      }
  }
}
