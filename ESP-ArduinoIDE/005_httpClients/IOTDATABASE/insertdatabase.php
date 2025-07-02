<?php
    // khai bao ket noi voi mysql
    $servername = "localhost"; // ip server
    $username = "root";// mac dinh la root
    $password = "";
    $dbname = "iotdatabase"; // ten databbase

    // ham ket noi voi mysql
    $conn = mysqli_connect($servername, $username, $password, $dbname);
    mysqli_set_charset($conn, "UTF9");
    
    if($conn->connect_error)
    {
        die("khong the ket noi". $conn->connect_error);
    }else
    {
        echo "da daket noi voi database";
        // dua du lieu vao mysql
        date_default_timezone_set("Asia/Jakarta");
        $date = date("Y-m-d");
        $time = date("H:i:s");

        $ND = "123";
        $DA = "70";
        $LED = "ON";

        $sql = "INSERT INTO dbsensor (ND, DA, date, time, LED) VALUES ('".$ND."', '".$DA."' ,'".$date."' ,'".$time."','".$LED."')"; 
        if($conn->query($sql) === TRUE)
        {
            echo "OK";
        }else{
            echo "Error :".$sql."<br>".$conn->error;
        }
    }
?>