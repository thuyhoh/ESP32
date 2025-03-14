#include <SoftwareSerial.h> // install esp phiên bản 2.5.0
const byte RX = D7;
const byte TX = D8;
SoftwareSerial mySerial = SoftwareSerial(RX, TX);

String inputString = "";
bool  stringComplete = false;
long bien = 0;

long last = 0;
void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600); // UART chân D7 = RX , D8 = TX
  last = millis();
}

void loop()
{
  Read_Uart();
  if(millis() - last >= 1000)
  {
    bien++;
    String Data = "LM" + String(bien);
    Serial.print("Đã send lệnh: ");
    Serial.println(Data);
    mySerial.println(Data);
    mySerial.flush();
    last = millis();
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
      Serial.print("Data nhận từ STM32: ");
      Serial.println(inputString);

      // xử lý có hết rùi
 
      inputString = "";
      stringComplete = false;
    }
  }
}
