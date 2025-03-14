// CỔNG GIAO TIẾP VỚI ESP
#include <SoftwareSerial.h>
const byte RX = 2;
const byte TX = 3;
SoftwareSerial mySerial = SoftwareSerial(RX, TX);


// khai báo biến UART
String inputString = "";  // lưu trữ dữ liệu đọc được
bool stringComplete = false;

// khai báo chân gắn relay 1 và relay 2
#define TB1 4
#define TB2 5

// khai báo biến trạng thái thiết bị để sau này gửi lên app và web
int trangthaitb1 = 0;
int trangthaitb2 = 0 ;

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

// biến lưu giá trị UART
long BienEF = 0;
long BienGH = 0;
long BienJK = 0;
long BienKL = 0;

/*
   3 NÚT NHẤN KHÔNG TỰ GIỮ , NUT1 CHUYỂN CHẾ ĐỘ , NUT2 ON OFF TB1 , NUT3 ON OFF TB2
   CÀI ĐẶT 4 THÔNG SỐ EF , GH , JKL
   CHẾ ĐỘ AUTO KHÔNG CHO PHÉP ĐIỀU KHIỂN
   CHẾ ĐỘ MAN THÌ CHO PHÉP ĐIỀU KHIỂN BẰNG NÚT NHẤN + WEB + APP ...
  Chuyển chế độ UART L1M = AUTO , L0M = MAN , Nuts1 chuyển chế độ
*/


void setup()
{

  Serial.begin(9600); // khởi tạo cổng tx0 rx0 baud 9600
  while (!Serial); // đợi cổng uart có mở dc hay không ?
  mySerial.begin(9600);  // Cổng giao tiếp UNO - ESP
  while (!mySerial); 

  // khai báo output TB1 TB2 để điều khiển relay
  pinMode(TB1, OUTPUT);
  pinMode(TB2, OUTPUT);


  //Read EEPROM và xử lý 
  
  // Mặc định mức ban đầu cho relay tắt
  digitalWrite(TB1, HIGH);
  digitalWrite(TB2, HIGH);

  // Khai báo nút nhấn không tự giữ
  pinMode(nut1, INPUT_PULLUP);
  pinMode(nut2, INPUT_PULLUP);
  pinMode(nut3, INPUT_PULLUP);

  last = millis();

  Serial.println("start Uno");
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
      Read_UART_ESP(); //Auto => chuyển chế độ
      // đọc nút nhấn có được nhấn hay không?
      NutnhanKhongTuGiu(); //Auto => chuyển chế độ
      if (millis() - last >= 1000)
      {
        Serial.println("BẠN ĐANG CHAY CHẾ ĐỘ AUTO");
        Read_UART_ESP();
        NutnhanKhongTuGiu();
        Read_DHT11();
        //hienthi();
        // Send dữ liệu về Esp
        //SendDataESP();
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
      Read_UART_ESP(); //MAN => chuyển chế độ , ON OFF , cài đặt thông số
      // Kiểm tra nút nhấn không tự giữ có được bấm hay không?
      NutnhanKhongTuGiu(); ////MAN => chuyển chế độ , ON OFF
      if (millis() - last >= 1000)
      {
        Serial.println("BẠN ĐANG CHAY CHẾ ĐỘ MANUAL");
        Read_UART_ESP();
        Read_DHT11();
        NutnhanKhongTuGiu();
        //hienthi();
        //SendDataESP();
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

void NutnhanKhongTuGiu()
{
  if (digitalRead(nut1) == 0)
  {
    delay(200);
    while (1)
    {
      // Kiểm tra cổng UART có gửi dữ liệu không?
      Read_UART_ESP();
      // Kiểm tra nút nhấn không tự giữ có được bấm hay không?
      NutnhanKhongTuGiu();
      if (millis() - last >= 1000)
      {
        Read_UART_ESP();
        Read_DHT11();
        //hienthi();
       // SendDataESP();
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
        //hienthi();
        //SendDataESP();
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
        //hienthi();
        //SendDataESP();
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
  // gửi ra cổng rx0 tx0
  Serial.print("DataSendEsp: ");
  Serial.println(DataSendEsp);
  // gửi về ESP  A1234B => println => A1234B\n , print => A1234B
  mySerial.println(DataSendEsp);
  mySerial.flush(); // đứng đợi truyền hết 
  
}
void Read_DHT11()
{

  //nhietdodht = 0;
  //doamdht = 0;
  //doamdht = dht.readHumidity();
  //nhietdodht = dht.readTemperature();
  nhietdodht = nhietdodht + 1.2;
  doamdht = doamdht + 2.1;

}

void Read_UART_ESP()
{
  while (mySerial.available()) // ABCD\n
  {
    char inChar = (char)mySerial.read(); // đọc từng byte
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

      if(inputString.indexOf("NM") >= 0)
      {
        Read_DHT11();
        hienthi();
        SendDataESP();
      }
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



/*
 * A998.41B1747.19C0D0E0F0G0H0J1K
 * nhietdo = 1=>100???
 * doam
 * anhsang
 * trangthaithietbi1
 * trangthaithietbi2
 * 
 * 
 * A nhietdo B doam C + ......
 * 

 */
