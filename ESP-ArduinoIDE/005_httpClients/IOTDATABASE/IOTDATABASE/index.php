<!DOCTYPE html>
<html>


<head>
    <meta charset="utf-8"> 
	<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
	
</head>

<script>




function MessageArrived(message) 
{  

   // dữ liệu của web nhận được trong cái biến message.payloadString ( được esp chuẩn hóa về JSON rùi)
	console.log(message.destinationName +" : " + message.payloadString);
	
	
	 var DataVDK  = message.payloadString;
	   
	   // hiển thị dữ liệu ra F12
	
       console.log("Dữ liệu VDK:" + DataVDK);
	         
      // đưa dữ liệu vào JSON để xử lý cho dễ
      var DataJson = JSON.parse(DataVDK); 
	  
	  //{"ND":"844.79","DA":"921.61","TB1":"0","TB2":"0","EF":"3000","GH":"5000","JK":"100","KL":"200","Mode":"0"}	
	  
	  // Muốn lấy ND thì DataJson.ND , DA thì DataJson.DA
	  // cho hiển thị đúng chỗ thì  document.getElementById("nhietdo").innerHTML = DataJson.ND
     //document.getElementById("doam").innerHTML = DataJson.DA; 	
	// tương tự còn lại
	 
	  if(DataJson.ND != null)
	  {
		// hiển thi nhiệt độ ESP ra web
		document.getElementById("nhietdo").innerHTML = DataJson.ND; 	
	  }
	  
	  if(DataJson.DA != null)
	  {
		// hiển thi Độ Ẩm ESP ra web
		document.getElementById("doam").innerHTML = DataJson.DA; 	
	  }
	  
	  if(DataJson.EF != null)
	  {
		
		document.getElementById("cai1").innerHTML = DataJson.EF; 	
	  }
	  
	  if(DataJson.GH != null)
	  {
		document.getElementById("cai2").innerHTML = DataJson.GH; 	
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


function TB1()
{

	
	var thietbi1 = document.getElementById("tttb1").innerHTML;
	 console.log(thietbi1);
	 
	 if(thietbi1 == "OFF")
	 {

		var DataSend = "1";
		$.ajax({
                type: "POST",
                url: 'WEBdieukhien.php',
                dataType: 'json',
                data: {
                    RL1: DataSend,
                },
            });
		
	 }
	 else if(thietbi1 == "ON")
	 {
		var DataSend = "0";
		$.ajax({
                type: "POST",
                url: 'WEBdieukhien.php',
                dataType: 'json',
                data: {
                    RL1: DataSend,
                },
            });
		
	 }
}


function TB2()
{
	console.log("OnClick Button TB22222222222");
	
	var thietbi1 = document.getElementById("tttb2").innerHTML;
	 console.log(thietbi1);
	 if(thietbi1 == "OFF")
	 {
		
		var DataSend = "1";
		$.ajax({
                type: "POST",
                url: 'WEBdieukhien.php',
                dataType: 'json',
                data: {
                    RL2: DataSend,
                },
            });
	 }
	 else if(thietbi1 == "ON")
	 {
		var DataSend = "0";
		$.ajax({
                type: "POST",
                url: 'WEBdieukhien.php',
                dataType: 'json',
                data: {
                    RL2: DataSend,
                },
            });
		
	 }
}

function SEND1()
{


	
	var DataSend = document.getElementById("text1").value; 
	
		$.ajax({
                type: "POST",
                url: 'WEBdieukhien.php',
                dataType: 'json',
                data: {
                    C1: DataSend,
                },
            });

	 
}


function SEND2()
{
	
	var DataSend = document.getElementById("text2").value; 
	
		$.ajax({
                type: "POST",
                url: 'WEBdieukhien.php',
                dataType: 'json',
                data: {
                    C2: DataSend,
                },
            });
	
   
}

function TB11()
{
		var DataSend = "A1B";
		
}

function TB12()
{
	var DataSend = "A0B";
		
}

function UpdateData(){
  var xhttp = new XMLHttpRequest();
  
  xhttp.onreadystatechange = function ()
  {
     if(this.readyState == 4 && this.status == 200)
	 {
		 
		var DataVDK  = xhttp.responseText;
		console.log("Database MYSQL:" + DataVDK);	 
		var DataJson = JSON.parse(DataVDK);		

		if(DataJson.ND != null)
			{
				document.getElementById("nhietdo").innerHTML = DataJson.ND;
			}
			
			if(DataJson.DA != null)
			{
				document.getElementById("doam").innerHTML = DataJson.DA;
			}
			
			if(DataJson.C1 != null)
			{
				document.getElementById("cai1").innerHTML = DataJson.C1;
			}
			if(DataJson.C2 != null)
			{
				document.getElementById("cai2").innerHTML = DataJson.C2;
			}
			
			if(DataJson.TB1 != null)
			{
			    if(DataJson.TB1 == 0)
				{
					document.getElementById("tttb1").innerHTML = "OFF";
				}
				else if(DataJson.TB1 == 1)
				{
					document.getElementById("tttb1").innerHTML = "ON";
				}
				
			}
			
			
			if(DataJson.TB2 != null)
			{
			    if(DataJson.TB2 == 0)
				{
					document.getElementById("tttb2").innerHTML = "OFF";
				}
				else if(DataJson.TB2 == 1)
				{
					document.getElementById("tttb2").innerHTML = "ON";
				}
				
			}
     }         
  }
  xhttp.open('GET','WEBreaddatabase.php',true);
  xhttp.send(); 
  setTimeout(function(){ UpdateData() }, 500);
}

</script>

<body onload="UpdateData()">

<h1>MYSQL</h1>




<h1>
THIẾT BỊ 1: 
<button type="button" onclick= "TB1()">THIẾT BỊ 1</button>
</h1>


<h1>
THIẾT BỊ 2: 
<button type="button" onclick= "TB2()">THIẾT BỊ 2</button>
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
THIẾT BỊ 1: 
<label id="tttb1">OFF</label>
</h1>

<h1>
THIẾT BỊ 2: 
<label id="tttb2">OFF</label>
</h1>

<h1>
CÀI ĐẶT 1: 
<label id="cai1">0</label>
</h1>

<h1>
CÀI ĐẶT 2: 
<label id="cai2">0</label>
</h1>


<h1>
CÀI ĐẶT 1: 
<input type="text" id="text1">
<button type="button"   onclick= "SEND1()">SEND</button>
</h1>

<h1>
CÀI ĐẶT 2: 
<input type="text" id="text2">
<button type="button"  onclick= "SEND2()">SEND</button>
</h1>




 
</body>
</html>