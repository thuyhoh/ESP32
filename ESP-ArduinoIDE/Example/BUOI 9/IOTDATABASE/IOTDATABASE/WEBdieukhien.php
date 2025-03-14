<?php
$servername = "localhost"; // 
$username = "root";
$password = "";
$dbname = "iotdatabase1";

$RL1 = $_POST['RL1'];

$RL2 = $_POST['RL2'];

$C1 = $_POST['C1'];

$C2 = $_POST['C2'];

$conn = mysqli_connect($servername, $username, $password, $dbname);
mysqli_set_charset($conn, 'UTF8');

// Kiểm tra kết nối
if ($conn->connect_error) {
	die("Không thể kết nối tới Database" . $conn->connect_error);
} else {
	//echo "Đã Kết nối Database" ; 
	//$sql = "UPDATE dbdieukhien SET relay1= '$RL1'  WHERE id=1";
	if ($RL1 != null) {
		$sql = "UPDATE dbdieukhien SET tb1= '$RL1'  WHERE id=1";
	} else if ($RL2 != null) {
		$sql = "UPDATE dbdieukhien SET tb2= '$RL2'  WHERE id=1";
	} else if ($C1 != null) {
		$sql = "UPDATE dbdieukhien SET cai1= '$C1'  WHERE id=1";
	} else if ($C2 != null) {
		$sql = "UPDATE dbdieukhien SET cai2= '$C2'  WHERE id=1";
	}

	if ($conn->query($sql) === TRUE) {
	} else {
	}
}
$conn->close();
