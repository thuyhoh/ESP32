// cài đặt thư viện  DHT.h => Sketch => include Library => Manager Libraries.. => đánh DHT => chọn DHT sensor library
#include <DHT.h>
// cài đặt thư viện Adafruit_NeoPixel => Sketch => include Library => Manager Libraries.. => đánh Adafruit NeoPixel và chon Adafruit NeoPixel cài đặt 
#include <Adafruit_NeoPixel.h>
// khai báo biến UART
String inputString = "";  // lưu trữ dữ liệu đọc được
bool stringComplete = false;

// khai báo chân gắn relay 1 và relay 2
#define TB1 4
#define TB2 5

// khai báo biến trạng thái thiết bị để sau này gửi lên app và web
int trangthaitb1 = 0;
int trangthaitb2 = 0 ;

// khai báo chân DHT11
const int DHTPIN = 6;
const int DHTTYPE = DHT11;
DHT dht(DHTPIN, DHTTYPE);

// Khai báo biến đọc cảm biến DHT
float nhietdodht = 0;
float doamdht = 0;

// khai báo biến để lưu dữ liệu gửi qua ESP
long last = 0;
String DataSendEsp = "";

// khai báo biến chế độ hoạt động của vdk , mặc định 1 = auto , 0 là manual
int chedo = 1; // 1 = AUTO , 0 = MANUAL

// khai báo chân nút nhấn không tự giữ
#define nut1 A0
#define nut2 A1
#define nut3 A2

// khai báo chân nút nhấn tự giữ
#define nut4 A3
#define nut5 A4

// khai báo chân biến trở
#define Bientro A5

// Khai báo chân remote433mhz
#define KenhA 10
#define KenhB 11
#define KenhC 12
#define KenhD 13

// khai báo chân xuất xung pwm để dimmer led DC
#define PWM 9

//khai báo trạng thái đầu tiên khi khởi động nguồn uno cho nút nhấn tự giữ
int trangthainut4 = 0;
int trangthainut5 = 0;


// Biến dimmer led
int biendimmerled = 0;
int giatribientro = 0;


// khai báo led RGB ws2812b
#define LED_PIN 7 // chân IN nối chân 7
#define NUMPIXELS 16  // gồm 16 con led RGB
Adafruit_NeoPixel strip(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
int ColorRGB = 1;


// biến lưu giá trị UART
long BienEF = 0;
long BienGH = 0;
long BienJK = 0;
long BienKL = 0;

/*
 Khái quát nội dung code và mạch điện tử
 Mạch điện tử gồm:
 1/ 2 mạch relay
 2/ 1 mạch dimmer led bằng mosfet
 3/ mạch led RGB 16 con ws2812
 4/ Mạch remote 433Mhz 4 kênh
 5/ cảm biến DHT11
 6/ 1 Biên trở
 7/ 3 nút nhấn không tự giữ
 8/ 2 nút nhấn tự giữ
 

 Chương trình vdk
 1/ gồm 2 chế độ Auto và MANUAL
 - chedo = 0 => MANUAL
 - chedo = 1 => AUTO
 - Chuyển chế độ Bằng UART => Data gửi là: L1M => AUTO, L0M => MANUAL
 - Chuyển chế độ bằng nút nhấn không tự giữ ở chân A0
 - Chuyển chế độ bằng remote 433mhz Nút A trên remote
 2/ Chương trình Auto gồm:
 - Chuyển chế độ bằng UART => gửi L1M => AUTO , L0M => MANUAL
 - Không được phép điều khiển relay qua nút nhấn A1 và A2 và remote 433Mhz, led rgb ws2812b 
 - Dimmer led qua biến trở bằng hàm  Dimmeled();
 - Nút nhấn chân A0 + remote nút A có chức năng chuyển chế độ bằng hàm  NutnhanKhongTuGiu(); Remote433MHZ()
 - Đọc cảm biến DHT11 bằng hàm Read_DHT11()
 - Gửi dữ liệu về Esp 1s 1 lần bằng hàm SendDataESP();
 3/ Chương trình MANUAL
 - Chuyển chế độ bằng UART => gửi L1M => AUTO , L0M => MANUAL
 - Nút nhấn chân A0 + remote nút A có chức năng chuyển chế độ bằng hàm NutnhanKhongTuGiu(); Remote433MHZ()
 - Đọc cảm biến DHT11 bằng hàm Read_DHT11()
 - Điều khiển relay ON OFF bằng Nút nhấn gắn ở chân A1 và A2 bằng hàm NutnhanKhongTuGiu();
 - Điều khiển Relay ON OFF bằng remote ở nút B và C bằng hàm  Remote433MHZ()
 - Đảo trạng thái relay bằng nút tự giữ chân A4 bằng hàm NutnhanTuGiu();
 - Dimmer led bằng remote Nút D hiệu ứng tăng dần bằng hàm Remote433MHZ()
 - Dimmer bằng UART => gửi E dulieu F , dulieu gửi từ 0 đến 255
 - Thay đổi màu led RGB bằng nút tự giữ chân A5 bằng hàm NutnhanTuGiu();
 - Thay đổi màu Led RGB bằng UART => GỬi RGB
 - Gửi dữ liệu về Esp 1s 1 lần bằng hàm SendDataESP();
 */

 /*
   Nếu không có nút nhấn tự giữ thì khóa chương trình NutnhanTuGiu();
   Nếu không có nút nhấn không tự giữ thì khóa chương trình NutnhanKhongTuGiu();
   Nếu không có remote 433MHZ thì khóa chương trình Remote433MHZ()
   Nếu không có Led Ws2812B thì khóa chương trình ShowColorWs2812B() strip.begin();
  */

void setup()
{

  Serial.begin(9600); // khởi tạo cổng tx0 rx0 baud 9600

  //Xuất chân PWM dimmer Led DC => để Led tắt
  analogWrite(PWM , 0);

  // khởi tạo LED RGB
  strip.begin();
  ShowColorWs2812B();

  // khởi tạo dht
  dht.begin();
  // khai báo output TB1 TB2 để điều khiển relay
  pinMode(TB1, OUTPUT);
  pinMode(TB2, OUTPUT);

  // Mặc định mức ban đầu cho relay tắt
  digitalWrite(TB1, HIGH);
  digitalWrite(TB2, HIGH);

  // Khai báo nút nhấn không tự giữ
  pinMode(nut1, INPUT_PULLUP);
  pinMode(nut2, INPUT_PULLUP);
  pinMode(nut3, INPUT_PULLUP);

  //Khai báo nút nhấn tự giữ
  pinMode(nut4, INPUT_PULLUP);
  pinMode(nut5, INPUT_PULLUP);

  // Khai báo pinmode Remote433Mhz
  pinMode(KenhA, INPUT_PULLUP);
  pinMode(KenhB, INPUT_PULLUP);
  pinMode(KenhC, INPUT_PULLUP);
  pinMode(KenhD, INPUT_PULLUP);


  //Cập nhật trạng thái nút nhấn tự giữ
  trangthainut4 = digitalRead(nut4);
  trangthainut5 = digitalRead(nut5);

  last = millis();
}

void loop()
{
  //==============================================================================================================
  // Nếu chế độ = 1 thì Auto
  if (chedo == 1)
  {
    //while(1) => vòng lặp vô hạn muốn thoát thì dùng lệnh break; => phụ thuộc điều kiện mới cho thoát

    while (1)
    {
      //==================================================================
      // đọc dữ liệu UART
      Read_UART_ESP();
      // Đọc giá trị biến trở để dimmer led
      Dimmeled();
      // đọc nút nhấn có được nhấn hay không?
      NutnhanKhongTuGiu();
      // đọc nút nhấn có được nhấn hay không?
      NutnhanTuGiu();
      // Đọc remote có được bấm hay không?
      Remote433MHZ();
      if (millis() - last >= 1000)
      {
        Serial.println("BẠN ĐANG CHAY CHẾ ĐỘ AUTO");
        Read_UART_ESP();
        NutnhanKhongTuGiu();
        NutnhanTuGiu();
        Remote433MHZ();
        Read_DHT11();
        hienthi();
        // Send dữ liệu về Esp
        SendDataESP();
        Serial.println("================================================================");
        last = millis();
      }
      // chedo thay đổi bằng UART , button , remote
      if (chedo == 0)
      {
        break;
      }

      //==================================================================
    }
  }
  //==============================================================================================================
  // Nếu chê độ = 0 thì manual
  if (chedo == 0)
  {
    while (1)
    {
      //==================================================================
      // Kiểm tra cổng UART có gửi dữ liệu không?
      Read_UART_ESP();
      // Kiểm tra nút nhấn không tự giữ có được bấm hay không?
      NutnhanKhongTuGiu();
      // Kiểm tra nút nhấn tự giữ có dc nhấn hay không?
      NutnhanTuGiu();
      // kiểm tra remote 433mhz có được nhấn hay không?
      Remote433MHZ();
      if (millis() - last >= 1000)
      {
        Serial.println("BẠN ĐANG CHAY CHẾ ĐỘ MANUAL");
        Read_UART_ESP();
        Read_DHT11();
        NutnhanKhongTuGiu();
        NutnhanTuGiu();
        Remote433MHZ();
        hienthi();
        SendDataESP();
        Serial.println("================================================================");
        last = millis();
      }
      // chedo thay đổi bằng UART , button , remote
      if (chedo == 1)
      {
        break;
      }
      //==================================================================
    }
  }


}

void ColorWrite(uint32_t color)
{

  for (int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

void NutnhanTuGiu()
{
  // yêu cầu nhấn rùi thả ra luôn
  if (digitalRead(nut4) != trangthainut4)
  {
    delay(200);
    if (chedo == 0)
    {
      DieuKhientatcaTB();
    }
    trangthainut4 = digitalRead(nut4);
  }


  // điều khiển màu led RGB
  if (digitalRead(nut5) != trangthainut5)
  {
    delay(200);
    if (chedo == 0)
    {
      ShowColorWs2812B();
    }

    trangthainut5 = digitalRead(nut5);
  }
}

void DieuKhientatcaTB()
{
 
  digitalRead(TB1);
  digitalWrite(TB1, !digitalRead(TB1));
  digitalWrite(TB2, !digitalRead(TB2));
  trangthaitb1 = !digitalRead(TB1);
  trangthaitb2 = !digitalRead(TB2);
}
void ShowColorWs2812B()
{
  ColorRGB++;
  if (ColorRGB >= 5)
  {
    ColorRGB = 1;
  }
  if (ColorRGB == 1)
  {
    ColorWrite(strip.Color(255,   0,  0));
  }
  else if (ColorRGB == 2)
  {
    ColorWrite(strip.Color(  0, 255,   0));
  }
  else if (ColorRGB == 3)
  {
    ColorWrite(strip.Color(  0,   0, 255));
  }
  else if (ColorRGB == 4)
  {
    ColorWrite(strip.Color(  255,   255, 255));
  }
}
void Dimmeled()
{
  giatribientro = analogRead(Bientro); // 0 đến 1023 => dimmer led analogWrite 0 đến 255

  // giatribientro = giatribientro / 4;

  giatribientro = map(giatribientro, 0 , 1023, 0, 255);

  if (giatribientro < 5)
  {
    giatribientro = 0;
  }

  analogWrite(PWM , giatribientro);
}
void NutnhanKhongTuGiu()
{
  if (digitalRead(nut1) == 0)
  {
    delay(200);
    while (1)
    {
      Serial.println("Nút Chế Độ Đang Được Bấm");
      if (millis() - last >= 1000)
      {
        Read_UART_ESP();
        Read_DHT11();
        hienthi();
        SendDataESP();
        Serial.println("================================================================");
        last = millis();
      }

      if (digitalRead(nut1) == 1)
      {
        delay(200);
        // làm việc ở thực hiện ở đây
        if (chedo == 0)
        {
          chedo = 1;
        }
        else if (chedo == 1)
        {
          chedo = 0;
        }
        break;
      }
    }
  }

  //=====================================================================
  if (digitalRead(nut2) == 0)
  {
    delay(200);
    while (1)
    {
      Serial.println("Nút Điều Khiển TB1 Đang Được Bấm");
      // cứ 1s đọc dữ liệu và gửi về ESP
      if (millis() - last >= 1000)
      {
        Read_UART_ESP();
        Read_DHT11();
        hienthi();
        SendDataESP();
        Serial.println("================================================================");
        last = millis();
      }


      if (digitalRead(nut2) == 1)
      {
        delay(200);

        // làm việc ở thực hiện ở đây
        // kiểm tra chedo , nếu chedo = 0 => MANUAL mới cho điều khiển
        if (chedo == 0)
        {
          DieuKhienTB1();
        }
        break;
      }

    }
  }

  //=====================================================================
  if (digitalRead(nut3) == 0)
  {
    delay(200);
    while (1)
    {
      Serial.println("Nút Điều Khiển TB2 Đang Được Bấm");
      if (millis() - last >= 1000)
      {
        Read_UART_ESP();
        Read_DHT11();
        hienthi();
        SendDataESP();
        Serial.println("================================================================");
        last = millis();
      }

      if (digitalRead(nut3) == 1)
      {
        delay(200);
        // làm việc ở thực hiện ở đây
        // kiểm tra chedo , nếu chedo = 0 => MANUAL mới cho điều khiển
        if (chedo == 0)
        {
          DieuKhienTB2();
        }
        break;
      }
    }
  }
}
void DieuKhienTB1()
{
  if (trangthaitb1 == 0)
  {
    digitalWrite(TB1, LOW);
    trangthaitb1 = 1;
  }
  else if (trangthaitb1 == 1)
  {
    digitalWrite(TB1, HIGH);
    trangthaitb1 = 0;
  }
}
void DieuKhienTB2()
{
  if (trangthaitb2 == 0)
  {
    digitalWrite(TB2, LOW);
    trangthaitb2 = 1;

  }
  else if (trangthaitb2 == 1)
  {
    digitalWrite(TB2, HIGH);
    trangthaitb2 = 0;
  }
}
void Remote433MHZ()
{
  //=============================================================================
  //KenhA

  if (digitalRead(KenhA) == 1)
  {
    delay(200);
    while (1)
    {
      Serial.println("REMOTE kênh A Đang Được Bấm");
      if (millis() - last >= 1000)
      {
        Read_UART_ESP();
        Read_DHT11();
        hienthi();
        SendDataESP();
        Serial.println("================================================================");
        last = millis();
      }

      if (digitalRead(KenhA) == 0)
      {
        delay(200);
        // làm việc ở thực hiện ở đây
        if (chedo == 0)
        {
          chedo = 1;
        }
        else if (chedo == 1)
        {
          chedo = 0;
        }
        break;
      }
    }
  }
  //======================================================

  //=============================================================================
  //KenhB

  if (digitalRead(KenhB) == 1)
  {
    delay(200);
    while (1)
    {
      Serial.println("REMOTE kênh B Đang Được Bấm");
      if (millis() - last >= 1000)
      {
        Read_UART_ESP();
        Read_DHT11();
        hienthi();
        SendDataESP();
        Serial.println("================================================================");
        last = millis();
      }

      if (digitalRead(KenhB) == 0)
      {
        delay(200);
        // làm việc ở thực hiện ở đây
        if (chedo == 0)
        {
          DieuKhienTB1();
        }
        break;
      }
    }
  }
  //======================================================


  //=============================================================================
  //KenhC

  if (digitalRead(KenhC) == 1)
  {
    delay(200);
    while (1)
    {
      Serial.println("REMOTE kênh C Đang Được Bấm");
      if (millis() - last >= 1000)
      {
        Read_UART_ESP();
        Read_DHT11();
        hienthi();
        SendDataESP();
        Serial.println("================================================================");
        last = millis();
      }

      if (digitalRead(KenhC) == 0)
      {
        delay(200);
        // làm việc ở thực hiện ở đây
        if (chedo == 0)
        {
          DieuKhienTB2();
        }
        break;
      }
    }
  }
  //======================================================

  //=============================================================================
  //KenhC

  if (digitalRead(KenhD) == 1)
  {
    delay(200);
    while (1)
    {
      Serial.println("REMOTE kênh D Đang Được Bấm");
      if (millis() - last >= 1000)
      {
        Read_UART_ESP();
        Read_DHT11();
        hienthi();
        SendDataESP();
        Serial.println("================================================================");
        last = millis();
      }

      if (digitalRead(KenhD) == 0)
      {
        delay(200);
        // làm việc ở thực hiện ở đây
        if (chedo == 0)
        {
          DimmerLedTangDan();
        }
        break;
      }
    }
  }
  //======================================================
}

void DimmerLedTangDan()
{
  giatribientro = giatribientro + 50;
  if (giatribientro >= 255)
  {
    giatribientro = 255;
    biendimmerled++;
  }
  if (biendimmerled > 2)
  {
    giatribientro = 0;
    biendimmerled = 0;
  }
  analogWrite(PWM , giatribientro);
}
void hienthi()
{
  Serial.print("chedo: ");
  Serial.println(chedo);

  Serial.print("nhietdodht:");
  Serial.println(nhietdodht);
  Serial.print("doamdht:");
  Serial.println(doamdht);
  Serial.print("trangthaitb1: ");
  Serial.println(trangthaitb1);
  Serial.print("trangthaitb2: ");
  Serial.println(trangthaitb2);

  Serial.print("BienEF: ");
  Serial.println(BienEF);
  Serial.print("BienGH: ");
  Serial.println(BienGH);
  Serial.print("BienJK: ");
  Serial.println(BienJK);
  Serial.print("BienKL: ");
  Serial.println(BienKL);
}
void SendDataESP()
{
  // nhietdo , doam , trangthaitb1 trangthaitb2 BienEF  BienGH BienJK  BienKL
  DataSendEsp = "";
  DataSendEsp = "A" + String(nhietdodht) + "B" + String(doamdht) + "C" + String(trangthaitb1) + "D" + String(trangthaitb2) + "E" + String(BienEF) + "F" + String(BienGH) + "G" + String(BienJK) + "H" + String(BienKL) + "J" + chedo + "K";
  Serial.print("DataSendEsp: ");
  Serial.println(DataSendEsp);
}
void Read_DHT11()
{

  nhietdodht = 0;
  doamdht = 0;
  doamdht = dht.readHumidity();
  nhietdodht = dht.readTemperature();
}
void XulyColorWs2812B(String inputString)
{
  if (inputString.indexOf("RGB") >= 0)
  {
    ShowColorWs2812B();
  }

}
void Read_UART_ESP()
{
  while (Serial.available()) // ABCD\n
  {
    char inChar = (char)Serial.read(); // đọc từng byte
    //  inChar = B
    inputString += inChar;
    //inputString = AB
    if (inChar == '\n')
    {
      stringComplete = true;
    }
    if (stringComplete == true)
    {
      Serial.print("Data nhận được: ");
      Serial.println(inputString);
      // xử lý dữ liệu nhân dc
      //=======================
      if (chedo == 0)
      {
        XulyOnOff(String(inputString));
        CaiDat(String(inputString));
        Xuly2data(String(inputString));
        XulyColorWs2812B(String(inputString));
      }

      Xulychedo(String(inputString));
      //====================
      inputString = "";
      stringComplete = false;
    }
  }
}
void  Xulychedo(String inputString)
{
  if (inputString.indexOf("L1M") >= 0)
  {
    chedo = 1;
  }
  else if (inputString.indexOf("L0M") >= 0)
  {
    chedo = 0;
  }
}
void  Xuly2data(String inputString)
{
  /*
     B1: tìm vị trí kí tự J K L trong chuỗi tổng
     B2: cắt chuỗi
     B3: gắn cho biến gì đó
  */
  // bước 1
  int TimJ, TimK, TimL  ;
  TimJ = inputString.indexOf("J");
  TimK = inputString.indexOf("K");
  TimL = inputString.indexOf("L");
  if (TimJ >= 0 && TimK >= 0 && TimL >= 0)
  {
    // cắt chuỗi substring(vị trí bắt đầu , kết thúc)
    String DataJK , DataKL = "";
    DataJK = inputString.substring(TimJ + 1 , TimK);
    DataKL = inputString.substring(TimK + 1 , TimL);
    BienJK = DataJK.toInt();
    BienKL = DataKL.toInt();
    //Serial.print("BienJK: ");
    //Serial.println(BienJK);
    //Serial.print("BienKL: ");
    //Serial.println(BienKL);
  }
}
void CaiDat(String inputString)
{
  /*
     B1: tìm vị trí kí tự E F G H trong chuỗi tổng
     B2: cắt chuỗi
     B3: gắn cho biến gì đó
  */
  // bước 1
  int TimE, TimF, TimG , TimH ;
  TimE = inputString.indexOf("E");
  TimF = inputString.indexOf("F");
  TimG = inputString.indexOf("G");
  TimH = inputString.indexOf("H");
  // bước 2

  if (TimE >= 0 && TimF >= 0)
  {
    // cắt chuỗi substring(vị trí bắt đầu , kết thúc)
    String DataEF = "";
    DataEF = inputString.substring(TimE + 1 , TimF);
    BienEF = DataEF.toInt();
    Serial.print("BienEF: ");
    Serial.println(BienEF);
    analogWrite(PWM , BienEF);
  }

  if (TimG >= 0 && TimH >= 0)
  {
    // cắt chuỗi substring(vị trí bắt đầu , kết thúc)
    String DataGH = "";
    DataGH = inputString.substring(TimG + 1 , TimH);
    BienGH = DataGH.toInt();
    Serial.print("BienGH: ");
    Serial.println(BienGH);
  }
}
void XulyOnOff(String inputString)
{
  if (inputString.indexOf("A1B") >= 0)
  {
    Serial.println("ON TB 1");
    digitalWrite(TB1, LOW);
    trangthaitb1 = 1;
  }
  else if (inputString.indexOf("A0B") >= 0)
  {
    Serial.println("OFF TB 1");
    digitalWrite(TB1, HIGH);
    trangthaitb1 = 0;
  }

  if (inputString.indexOf("C1D") >= 0)
  {
    Serial.println("ON TB 2");
    digitalWrite(TB2, LOW);
    trangthaitb2 = 1;
  }
  else if (inputString.indexOf("C0D") >= 0)
  {
    Serial.println("OFF TB 2");
    digitalWrite(TB2, HIGH);
    trangthaitb2 = 0;
  }
}
