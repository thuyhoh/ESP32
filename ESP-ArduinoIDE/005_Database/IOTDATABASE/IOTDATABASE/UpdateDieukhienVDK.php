<?php
	$servername = "localhost"; // 
    $username = "root";
    $password = "";        
    $dbname = "iotdatabase1";

    $RL1 = $_POST['RL1'];
	
	$RL2 = $_POST['RL2'];
	

    $conn = mysqli_connect($servername, $username, $password, $dbname);
    mysqli_set_charset($conn, 'UTF8');

    // Kiểm tra kết nối
    if ($conn->connect_error) 
    {
        die("Không thể kết nối tới Database" . $conn->connect_error);
    }
	else
	{
	
		if($RL1 != null)
		{
			$sql = "UPDATE dbdieukhien SET tb1= '$RL1'  WHERE id=1";
		}
	 
		if($RL2 != null)
		{
			$sql = "UPDATE dbdieukhien SET tb1= '$RL1' ,tb2= '$RL2'  WHERE id=1";
		}
		
		

        if ($conn->query($sql) === TRUE) 
        {
				
        } 
        else 
        {
     
        }
	}
	$conn->close();
?>