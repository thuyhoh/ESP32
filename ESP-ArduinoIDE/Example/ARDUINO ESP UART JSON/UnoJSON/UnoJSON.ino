#include <ArduinoJson.h>// 6.10.0
#include <SoftwareSerial.h>
const byte RX = 2;
const byte TX = 3;
SoftwareSerial mySerial = SoftwareSerial(RX, TX);

String inputString = "";
bool stringComplete = false;


String ChuoiSendEsp = "";

long nhietdo = 0;
long doam = 0;
long trangthaiTB1 = 0; // nút nhấn cơ
long trangthaiTB2 = 0; // nút nhấn cơ
long Cai1 = 100;
long Cai2 = 500;
long last = 0;
#define Relay1 4
#define Relay2 5

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  digitalWrite(Relay1, LOW);
  digitalWrite(Relay2, LOW);
  last = millis();
}

void loop()
{

  Read_UART_JSON();
  if (millis() - last >= 1000)
  {
    cambien();
    //DataJson(String(nhietdo) ,String(doam), String(trangthaiTB1), String(trangthaiTB2),String(Cai1),String(Cai2));
    last = millis();
  }
}


void cambien()
{
  //nhietdo++;
  nhietdo = nhietdo + 1;
  doam = doam + 2;
}

void DataJson(String nhietdo , String doam, String trangthaiTB1, String trangthaiTB2, String Cai1, String Cai2 )
{
  String a = "{\"ND\":\"giatrinhietdo\", \"DA\":\"giatridoam\"}";
  //          {\"ND\":\"giatrinhietdo\", \"DA\":\"giatridoam\"}
  //           {\"ND\":\"+String(nhietdo)+\", \"DA\":\" + String(doam) +\"}
  ChuoiSendEsp = "";
  ChuoiSendEsp = "{\"ND\":\"" + String(nhietdo) + "\"," +
                 "\"DA\":\"" + String(doam) + "\"," +
                 "\"TB1\":\"" + String(trangthaiTB1) + "\"," +
                 "\"TB2\":\"" + String(trangthaiTB2) + "\"," +
                 "\"C1\":\"" + String(Cai1) + "\"," +
                 "\"C2\":\"" + String(Cai2) + "\"}";


  // đưa dữ liệu vào thư viện ArduinoJSON
  StaticJsonDocument<200> doc;
  deserializeJson(doc, ChuoiSendEsp);
  //JsonObject obj = doc.as<JsonObject>();

  Serial.println("ChuoiSendEsp: ");
  serializeJsonPretty(doc, Serial);
  Serial.println();


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
      Serial.println("Data nhận được từ ESP: ");
      serializeJson(doc, Serial);
      Serial.println();

      //======================================
      // XỬ lý dữ liệu ESP gửi qua

      /*
         nhận dc lệnh ( do mình lập trình ra)  => gửi tất cả dữ liệu UNO đang có về ESP
         Điều khiển ON OFF
      */

      if (doc.containsKey("SEND") == 1)
      {
        DataJson(String(nhietdo) , String(doam), String(trangthaiTB1), String(trangthaiTB2), String(Cai1), String(Cai2));
      }
      else if (doc["RL1"] == "1")
      {
        Serial.println("ON TB 1");
        trangthaiTB1 = 1;
        digitalWrite(Relay1, HIGH);
        //digitalWrite();
      }
      else if (doc["RL1"] == "0")
      {
        Serial.println("OFF TB 1");
        trangthaiTB1 = 0;
        digitalWrite(Relay1, LOW);
      }

      else if (doc["RL2"] == "1")
      {
        Serial.println("ON TB 2");
        trangthaiTB2 = 1;
        //digitalWrite();
        digitalWrite(Relay2, HIGH);
      }
      else if (doc["RL2"] == "0")
      {
        Serial.println("OFF TB 2");
        trangthaiTB2 = 0;
        digitalWrite(Relay2, LOW);
      }
      // lấy dữ liệu gắn cho biến
      else
      {
        //{"C1":"123"}
        long bien4 ;
        long bien5 = "";
        if (doc.containsKey("C1") == 1)
        {
          bien4 = doc["C1"];
          Serial.println();
        }
        if (doc.containsKey("C2") == 1)
        {
          bien5 = doc["C2"];
        }
      }
      //======================================
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

  }
}
