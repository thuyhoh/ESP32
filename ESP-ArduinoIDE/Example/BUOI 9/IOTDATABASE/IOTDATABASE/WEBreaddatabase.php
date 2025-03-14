<?php
// khai báo kết nối mysql
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "iotdatabase1";

// hàm kết nối mysql
$conn = mysqli_connect($servername, $username, $password, $dbname);
mysqli_set_charset($conn, 'UTF8');

if ($conn->connect_error) {
	die("Không thể kết nối tới Database" . $conn->connect_error);
} else {
	//echo "Đã Kết nối Database";	
	// lấy dữ liệu => import mảng json => esp xử lý json cho dễ
	$sql = "SELECT id,nd, da, tb1, tb2, cai1, cai2  FROM dbsensor";

	$response = array(); // json
	$result = $conn->query($sql);

	if ($result->num_rows >= 0) {
		// output data of each row
		while ($row = $result->fetch_assoc()) {

			// echo "A" . $row["relay1"]. "B" . $row["relay2"]. "C" . $row["caidat1"]. "D" . $row["caidat2"]. "E" ;
			$response['ID'] = $row['id'];
			$response['ND'] = $row['nd'];
			$response['DA'] = $row['da'];
			$response['TB1'] = $row['tb1'];
			$response['TB1'] = $row['tb1'];
			$response['TB2'] = $row['tb2'];
			$response['C1'] = $row['cai1'];
			$response['C2'] = $row['cai2'];
		}
		echo json_encode($response);
		$result->free();
	} else {
		echo "0 results";
	}

	$conn->close();
}
