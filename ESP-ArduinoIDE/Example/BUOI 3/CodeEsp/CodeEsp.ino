#include <SoftwareSerial.h> // install esp phiên bản 2.5.0
const byte RX = D6;
const byte TX = D5;
SoftwareSerial mySerial = SoftwareSerial(RX, TX);

String inputString = "";
bool stringComplete = false;
String inputString1 = "";
bool stringComplete1 = false;



long last = 0;
void setup()
{
  Serial.begin(9600); //nạp code , check lỗi , nhận được dữ liệu mega gửi về không , hàm gì đó có xảy ra hay không
  while (!Serial);
  mySerial.begin(9600); // giao tiếp ESP - UNO
  while (!mySerial);

  Serial.println("start ESP");
  last = millis();

}

void loop()
{
  Read_Uart();
  Read_Uart_Monitor();
  // tự động yêu cầu arudino gửi dữ liệu về
    if(millis() - last >= 2000)
    {
    Serial.println("Đã Gửi Lệnh");
    mySerial.println("NM");
    mySerial.flush();
    last = millis();
    }
  
}

void  Read_Uart_Monitor()
{
  while (Serial.available())
  {
    char inChar1 = (char)Serial.read();

    inputString1 += inChar1;

    if (inChar1 == '\n')
    {
      stringComplete1 = true;
    }
    if (stringComplete1)
    {
      Serial.print("Data Gửi về Uno: ");
      Serial.println(inputString1);


     // gửi về UNO
      mySerial.print(inputString1);
      mySerial.flush();

      inputString1 = "";
      stringComplete1 = false;
    }
  }
}

void Read_Uart()
{
  while (mySerial.available())
  {
    char inChar = (char)mySerial.read();

    inputString += inChar;

    if (inChar == '\n')
    {
      stringComplete = true;
    }
    if (stringComplete)
    {
      Serial.print("Data nhận từ Uno: ");
      Serial.println(inputString);
      //=========================
      XulyData(String(inputString));
      //=========================
      inputString = "";
      stringComplete = false;
    }
  }
}

void XulyData(String inputString)
{

  //A 1182.00 B2068.48 C 0 D 0 E 0 F 0 G 0 H 0 J 1 K
  int TimA , TimB , TimC , TimD , TimE , TimF , TimG , TimH , TimJ , TimK;
  TimA = inputString.indexOf("A");
  TimB = inputString.indexOf("B");
  TimC = inputString.indexOf("C");
  TimD = inputString.indexOf("D");
  TimE = inputString.indexOf("E");
  TimF = inputString.indexOf("F");
  TimG = inputString.indexOf("G");
  TimH = inputString.indexOf("H");
  TimJ = inputString.indexOf("J");
  TimK = inputString.indexOf("K");


  if (TimA >= 0 && TimE >= 0 && TimK >= 0)
  {
    //  DataSendEsp = "A" + String(nhietdodht) + "B" + String(doamdht) + "C" + String(trangthaitb1) + "D" + String(trangthaitb2) + "E" + String(BienEF) + "F" + String(BienGH) + "G" + String(BienJK) + "H" + String(BienKL) + "J" + chedo + "K";

    String Datanhietdo , Datadoam , DataTTTB1 , DataTTTB2 , DataEF , DataGH , DataJK , DataKL , Datachedo = "";

    Datanhietdo = inputString.substring(TimA + 1 , TimB);
    Datadoam = inputString.substring(TimB + 1 , TimC);
    DataTTTB1 = inputString.substring(TimC + 1 , TimD);
    DataTTTB2 = inputString.substring(TimD + 1 , TimE);
    DataEF = inputString.substring(TimE + 1 , TimF);
    DataGH = inputString.substring(TimF + 1 , TimG);
    DataJK = inputString.substring(TimG + 1 , TimH);
    DataKL = inputString.substring(TimH + 1 , TimJ);
    Datachedo = inputString.substring(TimJ + 1 , TimK);


    Serial.print("Datanhietdo: ");
    Serial.println(Datanhietdo);

    Serial.print("Datadoam: ");
    Serial.println(Datadoam);


    Serial.print("DataTTTB1: ");
    Serial.println(DataTTTB1);
    Serial.print("DataTTTB2: ");
    Serial.println(DataTTTB2);
    Serial.print("DataEF: ");
    Serial.println(DataEF);
    Serial.print("DataGH: ");
    Serial.println(DataGH);
    Serial.print("DataJK: ");
    Serial.println(DataJK);
    Serial.print("DataKL: ");
    Serial.println(DataKL);
    Serial.print("Datachedo: ");
    Serial.println(Datachedo);


    DataJson(String(Datanhietdo), String(Datadoam), String(DataTTTB1), String(DataTTTB2), String(DataEF), String(DataGH), String(DataJK), String(DataKL), String(Datachedo));
  }


}

/*
  cấu trúc json {"nhietdo":"123","doam":"50"}
*/

void DataJson( String sensor1 ,  String sensor2 ,  String sensor3 , String sensor4 , String sensor5 , String sensor6, String sensor7, String sensor8, String sensor9)
{
  String ChuoiSendWebJson = "";
  ChuoiSendWebJson = "{\"ND\":\"" + String(sensor1) + "\"," +
                     "\"DA\":\"" + String(sensor2) + "\"," +
                     "\"TB1\":\"" + String(sensor3) + "\"," +
                     "\"TB2\":\"" + String(sensor4) + "\"," +
                     "\"EF\":\"" + String(sensor5) + "\"," +
                     "\"GH\":\"" + String(sensor6) + "\"," +
                     "\"JK\":\"" + String(sensor7) + "\"," +
                     "\"KL\":\"" + String(sensor8) + "\"," +
                     "\"Mode\":\"" + String(sensor9) + "\"}";

  Serial.print("ChuoiSendWebJson: ");
  Serial.println( ChuoiSendWebJson);

}
