// chọn board esp8266 / esp32
#ifdef ESP32
#include <WiFi.h>
#define LED1 2
#define DEN 23
#define QUAT 22
#define NUT1 21
#define NUT2 19
#else
#include <ESP8266WiFi.h>
#define LED1 2
#define DEN D0
#define QUAT D1
#define NUT1 D2
#define NUT2 D3
#endif


#include <PubSubClient.h>



int address = 1;

float nhietdo;
float doam;
int TTTB1, TTTB2, BienEF , BienGH, BienJK, BienKL;
long last = 0;
String ChuoiSendWebJson = "";

char ssid[] = "Bao Truc";
char pass[] = "24682468";
const char* mqtt_server = "mqtt.ngoinhaiot.com";
const char* mqtt_user = "toannv10291";
const char* mqtt_pass = "toannv10291";
int mqtt_port = 1111; // esp kết nối mqtt => TCP
String topicsub = "toannv10291/quat"; // nhận dữ liệu ESP ở topic toannv10291/quat => APP WEB gửi toannv10291/quat
String topicpub = "toannv10291/maylanh"; // gửi dữ liệu


String Data = "";
WiFiClient espClient;
PubSubClient client(espClient); // khai báo ESP chính là client

void ConnectWiFi();
void Button();
void chuongtrinhcambien();
void DataJson( String sensor1 ,  String sensor2 ,  String sensor3 , String sensor4 , String sensor5 , String sensor6, String sensor7);
void ConnectMqttBroker();
void callback(char* topic, byte* payload, unsigned int length);
void reconnect();
void Led_Blynk_OFF();
void Led_Blynk_ON();
void checkwifi();
void XulyModeONOFF(String(Data));
void XulyData(String(Data));

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
  // cần kết nối wifi
  ConnectWiFi();
  // Khai báo kết nối Server MQTT
  ConnectMqttBroker();
  last = millis();

}

void loop()
{
  // duy trì kết nối MQTT broker
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  Button();
  if (millis() - last >= 1000)
  {
    chuongtrinhcambien();
    DataJson( String(nhietdo) ,  String(doam),  String(TTTB1) , String(TTTB2) , String(BienEF) , String(BienGH), String(BienJK));
    last = millis();
  }
  checkwifi();

}



// hàm kiểm tra và kết nối MQTT
void reconnect()
{
  
  while (!client.connected())
  {
    String clientId = String(random(0xffff), HEX); // các id client esp không trung nhau => không bị reset server
    if (client.connect(clientId.c_str(), mqtt_user, mqtt_pass))
    {
      Serial.println("Connected MQTT ngoinhaiot.com");
      client.subscribe(topicsub.c_str());
      Led_Blynk_ON();
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");

      Led_Blynk_OFF();
      delay(5000);
    }
  }
}
// hàm ESP nhận dữ liệu
void callback(char* a, byte* payload, unsigned int length)
{
  for (int i = 0; i < length; i++)
  {
    // ghép Data = thành chuỗi tổng để xử lý
    Data += (char)payload[i];
  }
  Serial.print("Data nhận: ");
  Serial.println(Data);
  // xử lý dữ liệu
  //===================================================================
  //mySerial.println(Data);
  //mySerial.flush();
  XulyModeONOFF(String(Data));
  XulyData(String(Data));
  //===================================================================
  Data = "";
}
void ConnectMqttBroker()
{
  client.setServer(mqtt_server, mqtt_port); // sét esp client kết nối MQTT broker
  delay(10);
  client.setCallback(callback); // => đọc dữ liệu mqtt broker mà esp subscribe
  delay(10);
}




void XulyData(String Data)
{
  int TimE, TimF , TimG , TimH , TimJ , TimK , TimL;
  TimE = Data.indexOf("E");
  TimF = Data.indexOf("F");
  TimG = Data.indexOf("G");
  TimH = Data.indexOf("H");
  TimJ = Data.indexOf("J");
  TimK = Data.indexOf("K");
  TimL = Data.indexOf("L");

  if (TimE >= 0 && TimF >= 0)
  {
    String DataEF = "";
    DataEF = Data.substring(TimE + 1 , TimF);
    BienEF = DataEF.toInt();
  }

  if (TimG >= 0 && TimH >= 0)
  {
    String DataGH = "";
    DataGH = Data.substring(TimG + 1 , TimH);
    BienGH = DataGH.toInt();
  }


  if (TimJ >= 0 && TimK >= 0 && TimL >= 0)
  {
    String DataJK , DataKL = "";
    DataJK = Data.substring(TimJ + 1 , TimK);
    BienJK = DataJK.toInt();

    DataKL = Data.substring(TimK + 1 , TimL);
    BienKL = DataKL.toInt();
  }
}
void XulyModeONOFF(String Data)
{


  if (Data.indexOf("A0B") >= 0)
  {
    TTTB1 = 0;
    digitalWrite(DEN, LOW);
    Serial.println("OFF DEN");
  }
  else if (Data.indexOf("A1B") >= 0)
  {
    TTTB1 = 1;
    digitalWrite(DEN, HIGH);
    Serial.println("ON DEN");
  }
  else if (Data.indexOf("C0D") >= 0)
  {
    TTTB2 = 0;
    digitalWrite(QUAT, LOW);
    Serial.println("OFF QUAT");
  }
  else if (Data.indexOf("C1D") >= 0)
  {
    TTTB2 = 1;
    digitalWrite(QUAT, HIGH);
    Serial.println("ON QUAT");
  }

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

void DataJson( String sensor1 ,  String sensor2 ,  String sensor3 , String sensor4 , String sensor5 , String sensor6, String sensor7)
{

  ChuoiSendWebJson = "{\"ND\":\"" + String(sensor1) + "\"," +
                     "\"DA\":\"" + String(sensor2) + "\"," +
                     "\"TB1\":\"" + String(sensor3) + "\"," +
                     "\"TB2\":\"" + String(sensor4) + "\"," +
                     "\"EF\":\"" + String(sensor5) + "\"," +
                     "\"GH\":\"" + String(sensor6) + "\"," +
                     "\"JK\":\"" + String(sensor7) + "\"}";

  client.publish(topicpub.c_str(), ChuoiSendWebJson.c_str());
}
