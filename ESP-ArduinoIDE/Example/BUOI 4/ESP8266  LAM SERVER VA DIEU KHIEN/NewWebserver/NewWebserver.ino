#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);

#define address 2

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
  XulyModeONOFF(String(button));
  // cập nhật web 200 send thành công => text/html
  server.send(200, "text/html", webpage );
}

void Send_Input()
{
  Serial.println(">>>Send InPut");
  Serial.println(server.arg("Input"));
  String input = "";
  input = server.arg("Input");
  XulyData(String(input));
  server.send(200, "text/html", webpage );
}

void Send_Update()
{
  DataJson(String(nhietdo), String(doam), String(TTTB1), String(TTTB2), String(BienEF), String(BienGH), String(BienJK), String(BienKL), String(Bienchedo));

  //ChuoiSendWebJson => DataJson
  server.send(200, "text/html", String(ChuoiSendWebJson) );
}
void XulyModeONOFF(String button)
{
  if(button.indexOf("L0M") >= 0)
  {
    Bienchedo = 0;
    digitalWrite(LEDCHEDO, LOW);
  }
  else if(button.indexOf("L1M") >= 0)
  {
    Bienchedo = 1;
    digitalWrite(LEDCHEDO, HIGH);
  }

  if(Bienchedo == 0)
 {
     if(button.indexOf("A0B") >= 0)
    {
     TTTB1 = 0;
     digitalWrite(DEN, LOW);
     Serial.println("OFF DEN");
    }
    else if(button.indexOf("A1B") >= 0)
    {
     TTTB1 = 1;
     digitalWrite(DEN, HIGH);
     Serial.println("ON DEN");
   }
    else if(button.indexOf("C0D") >= 0)
    {
     TTTB2 = 0;
     digitalWrite(QUAT, LOW);
     Serial.println("OFF QUAT");
    }
    else if(button.indexOf("C1D") >= 0)
    {
      TTTB2 = 1;
      digitalWrite(QUAT, HIGH);
      Serial.println("ON QUAT");
    }
 }
   
}
void XulyData(String input)
{
  /*
   * EF , GH , JKL
   */

   if(Bienchedo == 0)
 {
   int TimE, TimF , TimG , TimH , TimJ , TimK , TimL;
   TimE = input.indexOf("E");
   TimF = input.indexOf("F");
   TimG = input.indexOf("G");
   TimH = input.indexOf("H");
   TimJ = input.indexOf("J");
   TimK = input.indexOf("K");
   TimL = input.indexOf("L");

   if(TimE >=0 && TimF >= 0)
   {
     String DataEF = "";
     DataEF = input.substring(TimE + 1 , TimF);
     BienEF = DataEF.toInt();     
   }

   if(TimG >=0 && TimH >= 0)
   {
     String DataGH = "";
     DataGH = input.substring(TimG + 1 , TimH);
     BienGH = DataGH.toInt();     
   }


   if(TimJ >=0 && TimK >= 0 && TimL >= 0)
   {
     String DataJK , DataKL = "";
     DataJK = input.substring(TimJ + 1 , TimK);
     BienJK = DataJK.toInt();

     DataKL = input.substring(TimK + 1 , TimL);
     BienKL = DataKL.toInt();
   }


 }
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
  while (!Serial);
  // KHAI BÁO OUTPUT ESP
  pinMode(DEN, OUTPUT);
  pinMode(QUAT, OUTPUT);
  pinMode(LEDCHEDO, OUTPUT);
  //LEDCHEDO

   pinMode(NUT1, INPUT_PULLUP);
   pinMode(NUT2, INPUT_PULLUP);
   pinMode(NUT3, INPUT_PULLUP);
 /*
   //input =A1B1C1D => slaver 1 điều khiển thiết 1 , trạng thái ON
   int TimA, TimB;
   TimA = input.indexOf("A");
   TimB = input.indexOf("B");
   if(TimA >= 0 && TimB >= 0)
   {
     String dulieu = "";
     dulieu = input.substring(TimA + 1 , TimB);
     if(dulieu.toInt() == address)
     {
       //thuc hiện lệnh
       if(input.indexOf("B1C") >= 0)
       {
         // đang muốn điều khiển thiết bị 1
         if(input.indexOf("C1D") >= 0)
         {
           ON thiết bị
         }
         else if(input.indexOf("C0D") >= 0)
         {
           OFF thiết bị
         }
       }
     }
   }

*/
   
  // MẶC ĐỊNH OFF RELAY
  digitalWrite(DEN, LOW);
  digitalWrite(QUAT, LOW);

  digitalWrite(LEDCHEDO, HIGH);


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
/*
 * Chê độ Auto MAN => Bienchedo == 1 AUTO , Bienchedo == 0 MANUAL
 * Nếu ESP hoạt động ở AUTO thì không cho điều khiển 2 thiết bị
 * 3 nút => 1 nút chuyển chế độ , 2 nút để điều khiển  ON OFF => AUTO không được điều khiển
 * , 
 */
void loop() 
{
  server.handleClient(); //duy tri server cho mình
 //Bienchedo == 1 => AUTO
 if(Bienchedo == 1)
 {
   while(1)
   {
     server.handleClient();
     Button();
      CapNhatSenSorHienThi();
     if(Bienchedo == 0)
     {
       break;
     }
   }
 }

 if(Bienchedo == 0)
 {
   while(1)
   {
     server.handleClient();
     Button();
     CapNhatSenSorHienThi();
     if(Bienchedo == 1)
     {
       break;
     }
   }
 } 
}

void Button()
{
  // chuong trình chuyển chế độ AUTO - MAN
  // Nhấn xuống => thả tay ra mới làm việc
  if(digitalRead(NUT1) == LOW)
  {
    delay(300);
    while(1)
    {
      Serial.println("Nút 1 đã dc nhấn");
      server.handleClient();
      CapNhatSenSorHienThi();
      if(digitalRead(NUT1) == HIGH)
      {
        // thực hiện lệnh ở đây
        // xử lý MAN AUTO
        if(Bienchedo == 1)
        {
          Bienchedo = 0;
          digitalWrite(LEDCHEDO, LOW);
        }
        else if(Bienchedo == 0)
        {
          Bienchedo = 1;
          digitalWrite(LEDCHEDO, HIGH);
        }
        break;
      }
    }
  }
  // xử lý 2 nút kia

  if(digitalRead(NUT2) == LOW)
  {
    delay(300);
    while(1)
    {
      Serial.println("Nút 2 đã dc nhấn");
      server.handleClient();
      CapNhatSenSorHienThi();
      if(digitalRead(NUT2) == HIGH)
      {
        // thực hiện lệnh ở đây
        // xử lý MAN AUTO
        if(Bienchedo == 0)
        {
           if(TTTB1 == 0)
          {
            digitalWrite(DEN,HIGH);
            TTTB1 = 1;
         }
          else if(TTTB1 == 1)
          {
            digitalWrite(DEN,LOW);
            TTTB1 = 0;
          }
        }
        
        break;
      }
    }
  }


  if(digitalRead(NUT3) == LOW)
  {
    delay(300);
    while(1)
    {
      Serial.println("Nút 3 đã dc nhấn");
      server.handleClient();
      CapNhatSenSorHienThi();
      if(digitalRead(NUT3) == HIGH)
      {
        // thực hiện lệnh ở đây
        // xử lý MAN AUTO
        if(Bienchedo == 0)
        {
           if(TTTB2 == 0)
          {
            digitalWrite(QUAT,HIGH);
            TTTB2 = 1;
          }
          else if(TTTB2 == 1)
          {
            digitalWrite(QUAT,LOW);
            TTTB2 = 0;
          }
        }
       
        break;
      }
    }
  }

  
}
void hienthi()
{
  if(Bienchedo == 1)
  {
    Serial.println("Ban dang chay che do AUTO");
  }
  else if(Bienchedo == 0)
  {
    Serial.println("Ban dang chay che do MAN");
  }
}
void CapNhatSenSorHienThi()
{
  if(millis() - last >= 1000)
  {
    chuongtrinhcambien();
     hienthi();
    last = millis();
  }
}
void chuongtrinhcambien()
{
  nhietdo = nhietdo + 1.1;
  doam = doam + 1.2;
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
