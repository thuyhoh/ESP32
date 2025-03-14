import 'package:flutter/material.dart';
import 'dart:async';
import 'dart:convert';
import 'package:flutter/cupertino.dart';
import 'package:fluttertoast/fluttertoast.dart';
import 'package:mqtt_client/mqtt_client.dart' as mqtt;
import 'dart:math';

import 'package:mqtt_client/mqtt_client.dart';



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
  String clientId =  Random().toString();
  String mqtt_server = "192.168.1.143";
  String  mqtt_user = "";
  String  mqtt_pass = "";
  int mqtt_port = 1883; // APP => TCP
  String topicsub = "pubdata"; // nhan du lieu
  String topicpub = "subdata";

  String nhietdo = "0";
  String doam = "0";
  String relay1 = "OFF";
  String relay2 = "OFF";
  String cai1 = "0";
  String cai2 = "0";

  mqtt.MqttClient client;
  mqtt.MqttConnectionState connectionState;

  TextEditingController text1 = new TextEditingController();
  TextEditingController text2 = new TextEditingController();

  double _currentSliderValue = 0;


  // CHẠY 1 LẦN DUY NHẤT hay dùng để kết nối server hoặc khởi tạo hàm timer để lấy dữ liệu, mở app thông báo
  @override
  void initState()
  {
    super.initState();
    ShowToask("WELCOME TO APP IOT MQTT");
    ConnectMQTT();
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
            Text("IOT - MQTT"),
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


              RaisedButton(
                onPressed: onclick3,
                child: Text("Button 3"),
              ),


              RaisedButton(
                onPressed: onclick4,
                child: Text("Button 4"),
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


  void send2()
  {
    print("Send 2");
    print(text2.text.toString());
    String tx = "G" + text2.text.toString() + "H";

   // String tx = "{\"SEND2\":\"${text2.text.toString()}\"}";
    publish(tx);

  }

  void send1()
  {
    print("Send 1");
    print(text1.text.toString());
    String tx = "E" + text1.text.toString() + "F";

    //String tx = "{\"SEND1\":\"${text1.text.toString()}\"}";


    publish(tx);


  }


  void onclick1()
  {
    print("Onclick button 1");
    String tx = "A0B";

    // {"RL1":"0"}
    //String tx = "{\"RL1\":\"0\"}";
    publish(tx);
  }

  void onclick2()
  {
    print("Onclick button 2");
    String tx = "A1B";
    publish(tx);

  }

  void onclick3()
  {
    print("Onclick button 3");
    String tx = "C0D";
    publish(tx);
  }

  void onclick4()
  {
    print("Onclick button 4");
    String tx = "C1D";
    publish(tx);

  }

  void publish(String message)
  {
    if (connectionState == mqtt.MqttConnectionState.connected)
    {
      final MqttClientPayloadBuilder message1 = MqttClientPayloadBuilder();
      message1.addString(message);
      client.publishMessage(topicpub, MqttQos.exactlyOnce, message1.payload);
      print('Data send:  ${message}');
      ShowToask(message);
    }
  }
  void ConnectMQTT() async
  {
    client = mqtt.MqttClient(mqtt_server, '');
    client.port = mqtt_port;
    client.logging(on: true);
    client.keepAlivePeriod = 30;

    final mqtt.MqttConnectMessage connMess = mqtt.MqttConnectMessage()
        .withClientIdentifier(clientId)
        .keepAliveFor(30)
        .withWillQos(mqtt.MqttQos.atMostOnce);
    client.connectionMessage = connMess;
    try
    {
      await client.connect(mqtt_user, mqtt_pass);
    }
    catch (e)
    {
      print(e);
      _disconnect();

    }
    if (client.connectionState == mqtt.MqttConnectionState.connected) {
      setState(()
      {
        connectionState = client.connectionState;
        client.subscribe(topicsub, mqtt.MqttQos.exactlyOnce);
        print('CONNECT MQTT BROKER AND SUBSCRIBE TOPIC: $topicsub');
        ShowToask("'CONNECT MQTT BROKER");
      });
    }
    else
    {
      _disconnect();
      print('Connection failed , state is ${client.connectionState}');
      ShowToask("'NOT CONNECT MQTT BROKER");

    }
    client.updates.listen(_onMessage);
  }

  void _onMessage(List<mqtt.MqttReceivedMessage> event) {
    // hàm nhận dũ liệu từ server mqtt
    final mqtt.MqttPublishMessage recMess = event[0].payload as mqtt.MqttPublishMessage;
    final String message =  mqtt.MqttPublishPayload.bytesToStringAsString(recMess.payload.message);
    print('MQTT message:${message}');
    String data = message;
    print('Data :${data}');

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
   });

  }

  void _disconnect() {
    client.disconnect();
    print('Disconnect Broker MQTT');

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