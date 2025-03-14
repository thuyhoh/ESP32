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

#include <SocketIoClient.h>
extern bool connect;

SocketIoClient socket;



float nhietdo;
float doam;
int TTTB1, TTTB2, BienEF , BienGH, BienJK, BienKL;
long last = 0;
String ChuoiSendWebJson = "";

char ssid[] = "Bao Truc";
char pass[] = "24682468";

const char* nodejs_server = "192.168.1.143";
int nodejs_port = 3000; // esp kết nối mqtt => TCP
String topicOn = "ESP"; // nhận dữ liệu ESP ở topic
String topicEmit = "JSON"; // gửi dữ liệu

String Data = "";

void ConnectWiFi();
void Button();
void chuongtrinhcambien();
void DataJson( String sensor1 ,  String sensor2 ,  String sensor3 , String sensor4 , String sensor5 , String sensor6, String sensor7);
void ConnectNodeJs();
void Message(const char* message , size_t length);
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
  // Khai báo kết nối Server nodejs
  ConnectNodeJs();
  last = millis();

}

void loop()
{
  // duy trì kết nối nodejs
  socket.loop();
  Button();
  if (millis() - last >= 1000)
  {
    if (connect)
    {
      chuongtrinhcambien();
      DataJson( String(nhietdo) ,  String(doam),  String(TTTB1) , String(TTTB2) , String(BienEF) , String(BienGH), String(BienJK));
    }
    last = millis();
  }
  checkwifi();
}



// hàm ESP nhận dữ liệu
void Message(const char* message , size_t length)
{
  last = millis();
  Serial.println(message);
  Data = message;
  // xử lý dữ liệu
  //===================================================================
  //mySerial.println(Data);
  //mySerial.flush();
  XulyModeONOFF(String(Data));
  XulyData(String(Data));
  //===================================================================
  Data = "";
  last = millis();
}
void ConnectNodeJs()
{
  // kết nối
  socket.begin(nodejs_server, nodejs_port);
  // lắng nghe sự kiện
  socket.on(topicOn.c_str(), Message); // hàm Message nhận dc dữ liệu server gửi xuống
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

  socket.emit(topicEmit.c_str(), ChuoiSendWebJson.c_str());
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
      socket.loop();
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
      socket.loop();
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
