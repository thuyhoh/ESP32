import 'package:flutter/material.dart';
import 'dart:async';
import 'dart:convert';
import 'package:flutter/cupertino.dart';
import 'package:mqtt_client/mqtt_client.dart' as mqtt;
import 'package:fluttertoast/fluttertoast.dart';
import 'package:mqtt_client/mqtt_client.dart';
import 'dart:math';


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

class AppStateMQTT extends State<HomeMQTT>
{

  // CHẠY 1 LẦN DUY NHẤT hay dùng để kết nối server hoặc khởi tạo hàm timer để lấy dữ liệu
  @override
  void initState()
  {
    super.initState();
    ShowToask("WELCOME TO APP IOT MQTT");
  }
  @override
  Widget build(BuildContext context)
  {
    // TẠO GIAO DIỆN trong Scaffold
    return Scaffold(

    );
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