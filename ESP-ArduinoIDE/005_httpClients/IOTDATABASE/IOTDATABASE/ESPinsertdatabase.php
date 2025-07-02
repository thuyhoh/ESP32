<?php
	$servername = "localhost"; // 
    $username = "root";
    $password = "";        
    $dbname = "iotdatabase1";

    // connect mysql php
    $conn = mysqli_connect($servername, $username, $password, $dbname);
    mysqli_set_charset($conn, 'UTF8');

    // Kiểm tra kết nối
    if ($conn->connect_error) 
	{
        die("Không thể kết nối tới Database" . $conn->connect_error);
    }
	else
	{
		
		
	
		 
		    date_default_timezone_set('Asia/Jakarta');
			$date = date("Y-m-d");
			$time = date("H:i:s");
			
			
			$nhietdo = $_POST['nhietdo'];
			
			$doam = $_POST['doam'];
			
			$trangthaitb1 = $_POST['relay1'];
			
			$trangthaitb2 = $_POST['relay2'];
			
			$cai1 = $_POST['cai1'];
			
			$cai2 = $_POST['cai2'];
			
		
		
			$sql = "INSERT INTO dbsensor (nd, da, tb1 , tb2 ,cai1 , cai2 , date, time ) VALUES ('".$nhietdo."', '".$doam."' , '".$trangthaitb1."' , '".$trangthaitb2."' , '".$cai1."' , '".$cai2."' ,'".$date."' ,'".$time."')"; 

			if ($conn->query($sql) === TRUE) 
			{
				echo "OK";
			} 
			else 
			{
				echo "Error: " . $sql . "<br>" . $conn->error;
			}
		
		   
			
	 }
	$conn->close();

?>




