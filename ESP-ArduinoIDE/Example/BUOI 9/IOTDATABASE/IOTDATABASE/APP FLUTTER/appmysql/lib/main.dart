import 'package:fluttertoast/fluttertoast.dart';
import 'package:flutter/material.dart';
import 'dart:async';
import 'package:flutter/cupertino.dart';
import 'package:http/http.dart' as http;
import 'dart:convert';



//======================================================================
void main()
{
  runApp(MyApp());
}
class MyApp extends StatelessWidget
{
  @override
  Widget build(BuildContext context)
  {
    return new MaterialApp(
      debugShowCheckedModeBanner: false,
      home: HomeMQTT(),
    );
  }
}

class HomeMQTT extends StatefulWidget
{
  @override
  AppStateMQTT createState()=> AppStateMQTT();
}
//======================================================================


// viết code trong hàm này
class AppStateMQTT extends State<HomeMQTT>
{
  // khai báo thông số

  String nodejs_server = "http://192.168.1.143:3000";

  String topicsOn = "AppFlutter"; // nhan du lieu
  String topicpEmit = "APP";

  String nhietdo = "0";
  String doam = "0";
  String relay1 = "OFF";
  String relay2 = "OFF";
  String cai1 = "0";
  String cai2 = "0";


  TextEditingController text1 = new TextEditingController();
  TextEditingController text2 = new TextEditingController();

  double _currentSliderValue = 0;

  String DataSendSocket = "";
  String DataReceived = "";


  // CHẠY 1 LẦN DUY NHẤT hay dùng để kết nối server hoặc khởi tạo hàm timer để lấy dữ liệu, mở app thông báo
  @override
  void initState()
  {
    super.initState();
    ShowToask("WELCOME TO APP IOT");
    Timer  timer = Timer.periodic(new Duration(seconds: 1), (timer)  {
      LoadDataMysql();
    });

  }
  @override
  Widget build(BuildContext context)
  {
    // TẠO GIAO DIỆN trong Scaffold
    return Scaffold(
      // viết giao diện trong đây

      appBar: AppBar(
        title: Row(
          mainAxisAlignment: MainAxisAlignment.center,

          children: <Widget>[
            Image(image:AssetImage('assets/10.jpg',),),
            Text("IOT - NODEJS"),
            SizedBox(width: 8.0,),
            Image(image:AssetImage('assets/12.jpg',),),
          ],
        ),
      ),

      body: Container(
        padding: EdgeInsets.fromLTRB(30, 0, 30, 0),
        constraints: BoxConstraints.expand(),
        color: Colors.white,
        child: SingleChildScrollView(
          child: Column(
            children: <Widget>[

              Slider(
                value: _currentSliderValue,
                min: 0,
                max: 100,
                divisions: 5,
                label: _currentSliderValue.round().toString(),
                onChanged: (double value) {
                  setState(() {
                    _currentSliderValue = value;
                  });
                },
              ),

              RaisedButton(
                onPressed: onclick1,
                child: Text("Button 1"),
              ),


              RaisedButton(
                onPressed: onclick2,
                child: Text("Button 2"),
              ),


              // html input , C# textbox
              TextField(
                controller: text1,
              ),

              RaisedButton(
                onPressed: send1,
                child: Text("SEND 1"),
              ),


              TextField(
                controller: text2,
              ),

              RaisedButton(
                onPressed: send2,
                child: Text("SEND 2"),
              ),

              Text(
                "Nhiệt Độ",
                style: TextStyle(fontSize: 20, color: Colors.red,),
              ),
              Text(
                nhietdo,
                style: TextStyle(fontSize: 20, color: Colors.blue,),
              ),

              Text("Độ Ẩm",
                style: TextStyle(fontSize: 20, color: Colors.red,),
              ),
              Text(doam,
                style: TextStyle(fontSize: 20, color: Colors.blue,),
              ),

              Text("Cài 1",
                style: TextStyle(fontSize: 20, color: Colors.red,),
              ),
              Text(cai1,
                style: TextStyle(fontSize: 20, color: Colors.blue,),
              ),


              Text("Cài 2",
                style: TextStyle(fontSize: 20, color: Colors.red,),
              ),
              Text(cai2,
                style: TextStyle(fontSize: 20, color: Colors.blue,),
              ),

              Text("thiết bị 1",
                style: TextStyle(fontSize: 20, color: Colors.red,),
              ),
              Text(relay1,
                style: TextStyle(fontSize: 20, color: Colors.blue,),
              ),


              Text("thiết bị 2",
                style: TextStyle(fontSize: 20, color: Colors.red,),
              ),
              Text(relay2,
                style: TextStyle(fontSize: 20, color: Colors.blue,),
              ),


            ],
          ),
        ),

      ),


    );
  }


  void send2() async
  {
    print("Send 2");
    print(text2.text.toString());
    String tx  = text2.text.toString() ;
    await http.post('http://192.168.1.143:82/IOTDATABASE/APPdieukhien.php', body: {
      "C2":tx,
    });

  }

  void send1() async
  {
    print("Send 1");
    print(text1.text.toString());
    String tx =  text1.text.toString() ;
    await http.post('http://192.168.1.143:82/IOTDATABASE/APPdieukhien.php', body: {
      "C1":tx,
    });

  }


  void onclick1() async
  {
    print("Onclick button 1");
    if(relay1.toString() == "ON")
      {
        String tx = "0";
        await http.post('http://192.168.1.143:82/IOTDATABASE/APPdieukhien.php', body: {
        "RL1":tx,
        });

      }
    else if(relay1.toString() == "OFF")
      {
        String tx = "1";
        //http://toannv10291.000webhostapp.com/APPreaddatabase.php
        await http.post('http://192.168.1.143:82/IOTDATABASE/APPdieukhien.php', body: {
          "RL1":tx,
        });
      }


  }

  void onclick2() async
  {
    print("Onclick button 2");
    if(relay2.toString() == "ON")
    {
      String tx = "0";
      //http://toannv10291.000webhostapp.com/APPreaddatabase.php
      await http.post('http://192.168.1.143:82/IOTDATABASE/APPdieukhien.php', body: {
        "RL2":tx,

      });
    }
    else if(relay2.toString() == "OFF")
    {

      String tx = "1";
      //http://toannv10291.000webhostapp.com/APPreaddatabase.php
      await http.post('http://192.168.1.143:82/IOTDATABASE/APPdieukhien.php', body: {
        "RL2":tx,
      });
    }

  }



  void LoadDataMysql() async
  {
    //http://toannv10291.000webhostapp.com/APPreaddatabase.php
   // var host = 'http://192.168.1.143:82/IOTDATABASE/APPreaddatabase.php';
    var host = 'http://toannv10291.000webhostapp.com/APPreaddatabase.php';
    http.Response response = await http.get(host);
    var Dataphp = response.body;
    print("Dataphp: ${Dataphp}");

    var DataJsonObject = json.decode(Dataphp);

    print('nhietdo :${DataJsonObject['ND']}');
    print('doam :${DataJsonObject['DA']}');
    print('relay1:${DataJsonObject['TB1']}');
    print('relay2:${DataJsonObject['TB2']}');

    setState(() {
      nhietdo = DataJsonObject['ND'];
      doam = DataJsonObject['DA'];

      cai1 = DataJsonObject['C1'];
      cai2 = DataJsonObject['C2'];

      if(DataJsonObject['TB1'] == "0")
      {
        relay1 = "OFF";
      }
      else if(DataJsonObject['TB1'] == "1")
      {
        relay1 = "ON";
      }

      if(DataJsonObject['TB2'] == "0")
      {
        relay2 = "OFF";
      }
      else if(DataJsonObject['TB2'] == "1")
      {
        relay2 = "ON";
      }
    });



  }
}






void ShowToask(String message)
{
  Fluttertoast.showToast(
      msg: '$message',
      toastLength: Toast.LENGTH_SHORT,
      gravity: ToastGravity.CENTER,
      timeInSecForIosWeb: 1,
      backgroundColor: Colors.red,
      textColor: Colors.white,
      fontSize: 16.0
  );
}

/*
var DataJsonObject = json.decode(data);

    print('nhietdo :${DataJsonObject['ND']}');
    print('doam :${DataJsonObject['DA']}');
    print('relay1:${DataJsonObject['TB1']}');
    print('relay2:${DataJsonObject['TB2']}');

    setState(() {
      nhietdo = DataJsonObject['ND'];
      doam = DataJsonObject['DA'];

      cai1 = DataJsonObject['EF'];
      cai2 = DataJsonObject['GH'];

      if(DataJsonObject['TB1'] == "0")
      {
        relay1 = "OFF";
      }
      else if(DataJsonObject['TB1'] == "1")
      {
        relay1 = "ON";
      }

      if(DataJsonObject['TB2'] == "0")
      {
        relay2 = "OFF";
      }
      else if(DataJsonObject['TB2'] == "1")
      {
        relay2 = "ON";
      }
 */