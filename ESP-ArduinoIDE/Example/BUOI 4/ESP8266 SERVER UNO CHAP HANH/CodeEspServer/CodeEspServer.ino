#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);

#include <SoftwareSerial.h> // install esp phiên bản 2.5.0
const byte RX = D6;
const byte TX = D5;
SoftwareSerial mySerial = SoftwareSerial(RX, TX);

String inputString = "";
bool stringComplete = false;

const char* ssid = "Bao Truc";
const char* pass = "24682468";

// PHAI KICK RELAY O MUC LOW
#define DEN D0  // RELAY GAN D0
#define QUAT D1 // RELAY GAN D1
#define LEDCHEDO 2 // LED CHE DO

#define NUT1 D2
#define NUT2 D3
#define NUT3 D5

String ChuoiSendWebJson = "";

float nhietdo;
float doam;
int TTTB1, TTTB2, BienEF , BienGH, BienJK, BienKL;

int Bienchedo = 1;

long last = 0;


// lưu trữ liệu => thực hiện chức luu => uno esp reset  , EEPROM ( code )  => cúp điện , reset uno esp => lưu biến giá 10

const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>

<head>
    <meta charset="utf-8"> 
</head>

<script>


function UpdateData(){
  var xhttp = new XMLHttpRequest();
  
  // kiểm tra XMLHttpRequest() có dữ liệu không
  xhttp.onreadystatechange = function (){
      // nếu có this.readyState == 4 && this.status == 200
     if(this.readyState == 4 && this.status == 200)
   {
   
      // dữ liệu ESP gửi về biến xhttp.responseText
       var DataVDK  = xhttp.responseText;
     
     // hiển thị dữ liệu ra F12
     
       console.log("Dữ liệu VDK:" + DataVDK);
           
    // cho hiển thị label  
      document.getElementById("datavdk").innerHTML = DataVDK; 
    
      // đưa dữ liệu vào JSON để xử lý cho dễ
    //{"ND":"844.79","DA":"921.61","TB1":"0","TB2":"0","EF":"3000","GH":"5000","JK":"100","KL":"200","Mode":"0"}
    
    
    // lây dữ liệu trong mảng json
    // DataJson.ND
    // DataJson.DA
    
      var DataJson = JSON.parse(DataVDK); 
    
    if(DataJson.Mode != null)
    {
      if(DataJson.Mode == 1)
      {
        document.getElementById("chedo").innerHTML = "AUTO";  
      }
      else if(DataJson.Mode == 0)
      {
        document.getElementById("chedo").innerHTML = "MAN";   
      }
    
    }
    
    if(DataJson.ND != null)
    {
    document.getElementById("nhietdo").innerHTML = DataJson.ND;   
    }
    
    if(DataJson.DA != null)
    {
    document.getElementById("doam").innerHTML = DataJson.DA;  
    }
    
    if(DataJson.EF != null)
    {
    document.getElementById("c1").innerHTML = DataJson.EF;  
    }
    
    if(DataJson.GH != null)
    {
    document.getElementById("c2").innerHTML = DataJson.GH;  
    }
    
    if(DataJson.JK != null)
    {
    document.getElementById("c3").innerHTML = DataJson.JK;  
    }
    
    if(DataJson.KL != null)
    {
    document.getElementById("c4").innerHTML = DataJson.KL;  
    }
    
    if(DataJson.TB1 != null)
    {
    if(DataJson.TB1 == 1)
    {
      document.getElementById("tttb1").innerHTML = "ON";    
    }
    else if(DataJson.TB1 == 0)
    {
      document.getElementById("tttb1").innerHTML = "OFF"; 
    }
    }
    
    
    if(DataJson.TB2 != null)
    {
    if(DataJson.TB2 == 1)
    {
      document.getElementById("tttb2").innerHTML = "ON";    
    }
    else if(DataJson.TB2 == 0)
    {
      document.getElementById("tttb2").innerHTML = "OFF"; 
    }
    }   
     }         
  }
  xhttp.open('GET','/Update',true);
  xhttp.send(); 
  // tự update webserver
 setTimeout(function(){ UpdateData() }, 2000);
}


function GetButtonData(data)
{
  switch (data)
  {
    
        case 1:
           // hiển thị F12
        console.log("OnClick Button Mode");  // hiển thị F12
        
         // lấy dữ liệu ở thuộc tính trong HTML có tên chedo
        var chedo = document.getElementById("chedo").innerHTML; // lấy dữ liệu ở thuộc tính trong HTML có tên chedo
        // hiển thị F12
        console.log(chedo);
        
        //chedo
        if(chedo == "AUTO")     
        {
            var tx_chedo = "L0M";
          
          // Gửi dữ liệu xuống ESp nó hiểu
          var xhttp = new XMLHttpRequest(); 
          xhttp.open("GET","/Button?Button="+tx_chedo,true);        
          xhttp.send();
        }
        else if(chedo == "MAN")
        {
          var tx = "L1M";
          
          // Gửi dữ liệu xuống ESp nó hiểu
          var xhttp = new XMLHttpRequest(); 
          xhttp.open("GET","/Button?Button="+tx,true);        
          xhttp.send();
        }
        
                break;
        case 2:
        console.log("OnClick Button TB 1"); 
        
        var trangthaitb1 = document.getElementById("tttb1").innerHTML;
        console.log(trangthaitb1);
        
    
        if(trangthaitb1 == "ON")      
        {
            var tx = "A0B";
          
          var xhttp = new XMLHttpRequest(); 
          xhttp.open("GET","/Button?Button="+tx,true);        
          xhttp.send();
        }
        else if(trangthaitb1 == "OFF")
        {
          var tx = "A1B";
          
          var xhttp = new XMLHttpRequest(); 
          xhttp.open("GET","/Button?Button="+tx,true);        
          xhttp.send();
        }
            
                break;
        case 3:
                console.log("Onclick Button TB 2");
        var trangthaitb2 = document.getElementById("tttb2").innerHTML;
        console.log(trangthaitb2);
        if(trangthaitb2 == "ON")      
        {
            var tx = "C0D";
          var xhttp = new XMLHttpRequest(); 
          xhttp.open("GET","/Button?Button="+tx,true);        
          xhttp.send();
        }
        else if(trangthaitb2 == "OFF")
        {
          var tx = "C1D";
          var xhttp = new XMLHttpRequest(); 
          xhttp.open("GET","/Button?Button="+tx,true);        
          xhttp.send();
        }
                break;


    case 4:
                console.log("Onclick Button AUTO");
            
        var tx = "L1M";
        var xhttp = new XMLHttpRequest(); 
        xhttp.open("GET","/Button?Button="+tx,true);        
        xhttp.send();
        
                break;

    case 5:
                console.log("Onclick Button MAN");
        
        var tx = "L0M";
        var xhttp = new XMLHttpRequest(); 
        xhttp.open("GET","/Button?Button="+tx,true);        
        xhttp.send();
        
                break;  
    }
}

function GetInPut(data)
{
  switch (data)
  {
    
        case 1:
        console.log("OnClick Button send text 1");      
        var C1 = "E" + document.getElementById("cai1").value + "F";
        console.log(C1);
        
        var xhttp = new XMLHttpRequest();
        //xhttp.open("GET","/Input?Input="+C1,true); 
        xhttp.open("GET","/Input?Input="+C1,true); 
        xhttp.send();
                break;
        case 2:
        console.log("OnClick Button send text 2");  
        var C2 = "G" + document.getElementById("cai2").value + "H";
        console.log(C2);
        
        var xhttp = new XMLHttpRequest();
        xhttp.open("GET","/Input?Input="+C2,true);        
        xhttp.send();
                break;
        case 3:
                console.log("Send Text 3");
        var C3 = "J" + document.getElementById("cai3").value + "K" + document.getElementById("cai4").value + "L";
        console.log(C3);
        
        var xhttp = new XMLHttpRequest();
        xhttp.open("GET","/Input?Input="+C3,true);  
        xhttp.send();
                break;        
    }
}

</script>

<body onload="UpdateData()">

<h1>WEBSERVER</h1>


<h1>
MODE
<button   onclick= GetButtonData(1)  >MODE </button>
</h1>

<h1>
MODE
<button   onclick= GetButtonData(4)  >AUTO</button>
<button   onclick= GetButtonData(5)  >MAN</button>
</h1>

<h1>
THIẾT BỊ 1: 
<button  onclick= GetButtonData(2)>THIẾT BỊ 1 </button>
</h1>


<h1>
THIẾT BỊ 2: 
<button onclick= GetButtonData(3)>THIẾT BỊ 2 </button>
</h1>

<h1>
CÀI ĐẶT 1: 
<input id="cai1">
<button  onclick= GetInPut(1)>Send 1</button>
</h1>

<h1>
CÀI ĐẶT 2: 
<input  id="cai2">
<button  onclick= GetInPut(2)>Send 2</button>
</h1>

<h1>
CÀI ĐẶT 3: 
<input  id="cai3">
<input  id="cai4">
<button  onclick= GetInPut(3)>Send 3</button>
</h1>

<h1>
Data VDK: 
<label id="datavdk">0</label>
</h1>



<h1>
CHẾ ĐỘ: 
<label id="chedo">AUTO</label>
</h1>

<h1>
NHIỆT ĐỘ: 
<label id="nhietdo">0</label>
</h1>

<h1>
ĐỘ ẨM: 
<label id="doam">0</label>
</h1>

<h1>
TRẠNG THÁI THIẾT BỊ 1: 
<label id="tttb1">OFF</label>
</h1>

<h1>
TRẠNG THÁI THIẾT BỊ 2: 
<label id="tttb2">OFF</label>
</h1>

<h1>
CÀI ĐẶT 1: 
<label id="c1">0</label>
</h1>

<h1>
CÀI ĐẶT 2: 
<label id="c2">0</label>
</h1>

<h1>
CÀI ĐẶT 3: 
<label id="c3">0</label>
</h1>

<h1>
CÀI ĐẶT 4: 
<label id="c4">0</label>
</h1>


</body>
</html>
)=====";


void SendWebPage()
{
  server.send(200, "text/html", webpage );
}


void Onclick_Button()
{
  Serial.println(">>>Onclick Button"); // hiển thị ra để hàm đã chạy => mới làm việc khác được


  // lấy dữ liệu web gửi xuống
  Serial.println(server.arg("Button"));
  String button = ""; 
  button = server.arg("Button");
   // nhận được dữ liệu từ web lưu trong biến button và gửi qua uno bằng lênh  mySerial.println(button);;   mySerial.flush();
  // gưi uno
  mySerial.println(button);
  mySerial.flush();
  // cập nhật web 200 send thành công => text/html
  server.send(200, "text/html", webpage );
}

void Send_Input()
{
  Serial.println(">>>Send InPut");
  Serial.println(server.arg("Input"));
  String input = "";
  input = server.arg("Input");
   // gưi uno
   // nhận được dữ liệu từ web lưu trong biến input và gửi qua uno bằng lênh mySerial.println(input);  mySerial.flush();
  mySerial.println(input);
  mySerial.flush();
  server.send(200, "text/html", webpage );
}

void Send_Update()
{
  DataJson(String(nhietdo), String(doam), String(TTTB1), String(TTTB2), String(BienEF), String(BienGH), String(BienJK), String(BienKL), String(Bienchedo));
  server.send(200, "text/html", String(ChuoiSendWebJson) );
}

void ConnectWiFi()
{
   WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  //=============================================================
  Serial.println();
  Serial.println("Connect WiFi");
  Serial.print("Address IP esp: ");
  Serial.println(WiFi.localIP());
}
void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  while (!Serial);
 
  // kết nối wifi
 ConnectWiFi();
  
  
  server.on("/", [] {
    SendWebPage();
  });

  server.on("/Button", [] {
    Onclick_Button();
  });

  server.on("/Input", [] {
    Send_Input();
  });

  server.on("/Update", [] {
    Send_Update();
  });

  server.begin();

  Serial.println("Server start");

  last = millis();

  
}

void loop() 
{
  server.handleClient(); // duy tri server cho mình

  Read_Uart();

  if(millis() - last >= 2000)
  {
    Serial.println("Đã Gửi Lệnh");
    mySerial.println("NM");
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



    nhietdo = Datanhietdo.toFloat();
    doam = Datadoam.toFloat();
    TTTB1 = DataTTTB1.toInt();
    TTTB2 = DataTTTB2.toInt();
     BienEF = DataEF.toInt();
      BienGH = DataGH.toInt();
       BienJK = DataJK.toInt();
       DataKL = DataKL.toInt();
       Bienchedo = Datachedo.toInt();
       
    
    
  }


}

void DataJson( String sensor1 ,  String sensor2 ,  String sensor3 , String sensor4 , String sensor5 , String sensor6, String sensor7, String sensor8, String sensor9)
{

  ChuoiSendWebJson = "{\"ND\":\"" + String(sensor1) + "\"," +
                     "\"DA\":\"" + String(sensor2) + "\"," +
                     "\"TB1\":\"" + String(sensor3) + "\"," +
                     "\"TB2\":\"" + String(sensor4) + "\"," +
                     "\"EF\":\"" + String(sensor5) + "\"," +
                     "\"GH\":\"" + String(sensor6) + "\"," +
                     "\"JK\":\"" + String(sensor7) + "\"," +
                     "\"KL\":\"" + String(sensor8) + "\"," +
                     "\"Mode\":\"" + String(sensor9) + "\"}";

}
