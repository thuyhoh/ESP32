
#include <DHT.h>
String inputString = "";
bool stringComplete = false;

long BienEF = 0;
long BienGH = 0;
long BienJK = 0;
long BienKL = 0;

#define TB1 4
#define TB2 5
int trangthaitb1 = 0;
int trangthaitb2 = 0 ;

const int DHTPIN = 7;
const int DHTTYPE = DHT11;
DHT dht(DHTPIN, DHTTYPE);
float nhietdodht = 0;
float doamdht = 0;
long last = 0;
String DataSendEsp = "";
int chedo = 1; // 1 = AUTO , 0 = MANUAL

void setup()
{
  // chỉ chạy đúng 1 lần khi arduino khởi động , khởi tạo gpio , setup tin cảm biến
  Serial.begin(9600); // khởi tạo cổng tx0 rx0 baud 9600

  // khởi tạo dht
  dht.begin();
  // khai báo output chân
  pinMode(TB1, OUTPUT);
  pinMode(TB2, OUTPUT);

  digitalWrite(TB1, HIGH);
  digitalWrite(TB2, HIGH);
  last = millis();
}

void loop()
{
  if (chedo == 1)
  {
    //while(1) => vòng lặp vô hạn muốn thoát thì dùng lệnh break; => phụ thuộc điều kiện mới cho thoát

    while (1)
    {
      //==================================================================
      Read_UART_ESP();
      if (millis() - last >= 1000)
      {
        Serial.println("BẠN ĐANG CHAY CHẾ ĐỘ AUTO");
        Read_UART_ESP();
        Read_DHT11();
        hienthi();
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

  if (chedo == 0)
  {
    while (1)
    {
      //==================================================================
      Read_UART_ESP();
      if (millis() - last >= 1000)
      {
        Serial.println("BẠN ĐANG CHAY CHẾ ĐỘ MANUAL");
        Read_UART_ESP();
        Read_DHT11();
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
void DimmerLed(int a)
{

}
void RGB(int b)
{

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
    // chedo = 1
    chedo = 1;
  }
  else if (inputString.indexOf("L0M") >= 0)
  {
    // chedo = 0
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
    Serial.print("BienJK: ");
    Serial.println(BienJK);
    Serial.print("BienKL: ");
    Serial.println(BienKL);
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
