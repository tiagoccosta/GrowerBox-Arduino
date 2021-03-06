<!doctype html>
<html>
<head>
<meta charset="utf-8">
<title>Untitled Document</title>
<style type="text/css">
	
body,td,th {
	color: #14253D;
}
body {
	background-color: #AEDCD1;
}
	td{
		height: 100%;
	}
	
 /* The Modal (background) */
.modal {
  display: none; /* Hidden by default */
  position: fixed; /* Stay in place */
  z-index: 1; /* Sit on top */
  left: 0;
  top: 0;
  width: 100%; /* Full width */
  height: 100%; /* Full height */
  overflow: auto; /* Enable scroll if needed */
  background-color: rgb(0,0,0); /* Fallback color */
  background-color: rgba(0,0,0,0.4); /* Black w/ opacity */
  padding-top: 60px;
}

/* Modal Content/Box */
.modal-content {
  background-color: #fefefe;
  margin: 5px auto; /* 15% from the top and centered */
  border: 1px solid #888;
  width: 80%; /* Could be more or less, depending on screen size */
}

/* The Close Button */
.close {
  /* Position it in the top right corner outside of the modal */
  position: absolute;
  right: 25px;
  top: 0;
  color: #000;
  font-size: 35px;
  font-weight: bold;
}

/* Close button on hover */
.close:hover,
.close:focus {
  color: red;
  cursor: pointer;
}

/* Add Zoom Animation */
.animate {
  -webkit-animation: animatezoom 0.6s;
  animation: animatezoom 0.6s
}

@-webkit-keyframes animatezoom {
  from {-webkit-transform: scale(0)}
  to {-webkit-transform: scale(1)}
}

@keyframes animatezoom {
  from {transform: scale(0)}
  to {transform: scale(1)}
} 
	
	 /* Bordered form */
form {
  border: 3px solid #f1f1f1;
}

/* Full-width inputs */
input[type=text], input[type=password] {
  width: 100%;
  padding: 12px 20px;
  margin: 8px 0;
  display: inline-block;
  border: 1px solid #ccc;
  box-sizing: border-box;
}

/* Set a style for all buttons */
button {
	background-color: #005376;
	color: white;
	padding: 14px 20px;
	margin: 8px 0;
	border: none;
	cursor: pointer;
	width: 100%;
}

/* Add a hover effect for buttons */
button:hover {
  opacity: 0.8;
}

/* Extra style for the cancel button (red) */
.cancelbtn {
  padding: 10px 18px;
  background-color: #f44336;
}

/* Center the avatar image inside this container */
.imgcontainer {
  text-align: center;
  margin: 24px 0 12px 0;
}

/* Avatar image */
img.avatar {
  width: 40%;
  border-radius: 50%;
}

/* Add padding to containers */
.container {
  padding: 16px;
}

/* The "Forgot password" text */
span.psw {
  float: right;
  padding-top: 16px;
}

/* Change styles for span and cancel button on extra small screens */
@media screen and (max-width: 300px) {
  span.psw {
    display: block;
    float: none;
  }
  .cancelbtn {
    width: 100%;
  }
} 
</style>
</head>

<body>

<div  style="background-color: #000A31; width: 100%;"><center>
  <h1 style="color:#99C1B4"><strong>CoguGrower</strong></h1>
</center>
</div>
	
<hr />


<div  style="background-color: #5C7D92; width: 100%;"><center>
  <img id="imageID" />
</center>
</div>
	
<hr />
	
	
 	<div width="100%"  style="border-radius: 10px 10px 10px 10px; border-width: thick; background-color:#6C9DB4">
  <center><h3>Ambiente</h3></center>
  <div id="tabelaAmbienteResumo" style="display: block" >
  <table align="center" title="Status" width="100%" border="1" style="background-color: #99CDE7; width: 100%">
    <tbody>
      <tr >
        <td >Temperatura:</td>
        <td id="tempResID" >24 (24) Cº</td>
      </tr>
      <tr>
        <td>Umidade:</td>
        <td id="HumidResID" >85 (85) %</td>
      </tr>
      <tr>
        <td>CO2:</td>
        <td id="Co2ResID" >70 (70) ppm</td>
      </tr>
      <tr>
        <td>Tempo:</td>
        <td id="LightResID" >0 (12|12) h</td>
      </tr>
    </tbody>
  </table>
  </div >
	</div>
	
	
 	<div width="100%"  style="border-radius: 10px 10px 0 0; border-width: thick; background-color:#6C9DB4">
       <center>
        <h3>Equipamentos</h3>
        </center>
  <table align="center" title="Status" width="100%" border="1" style="background-color: #99CDE7">
    <tbody>
      <tr>
        <td >Aquecedor:</td>
        <td id="AqueceID" style="background-color: #11FF8C" >Ligado</td>
      </tr>
      <tr>
        <td >Refrigerador:</td>
        <td id="FreezerID"  style="background-color: #D43535">Desligado</td>
      </tr>
      <tr>
        <td >Umidificador:</td>
        <td id="HumidificatorID"  style="background-color: #D43535">Desligado</td>
      </tr>
      <tr>
        <td >Compressor:</td>
        <td id="CompressorID"  style="background-color: #D43535">Desligado</td>
      </tr>
      <tr>
        <td >Lampada:</td>
        <td id="LampID"  style="background-color: #D43535">Desligado</td>
      </tr>
    </tbody>
  </table>
</div>

	
		
<script>
GetJson();
setInterval(function() { GetJson(); }, 2500);

	
setInterval(function() {UpdateImage(); }, 4000);
	
function GetHttpResponse(url, callback){
	var xmlhttp = new XMLHttpRequest();
	xmlhttp.onreadystatechange = function() {
  		if (this.readyState == 4 && this.status == 200) {
  			callback(JSON.parse(this.responseText)); // Another callback here
  		}
	};
	xmlhttp.open("GET", url, true);
	xmlhttp.send();
}
	
function GetJson() {
	GetHttpResponse("/get-data.php", SetData);
}
function SetData(arr) {
  document.getElementById("tempResID").innerHTML =  arr.temperature.now+" ("+arr.temperature.ref+") Cº";
  document.getElementById("HumidResID").innerHTML =  arr.humidity.now+" ("+arr.humidity.ref+") %";
  document.getElementById("Co2ResID").innerHTML =  arr.co2.now+" ("+arr.co2.ref+") ppm";
  document.getElementById("LightResID").innerHTML =  arr.light.now+" ("+arr.light.lightTimeRef+"|"+arr.light.darkTimeRef+") h";
	

	var corVerde = "#11FF8C";
	var corVermelha = "#D43535";
	
  var aquecedorColor = corVermelha;
  var aquecedor = "Desligado";
  if(arr.equipment.aquecedorRelay == true){aquecedor = "Ligado" ; aquecedorColor = corVerde;}
  document.getElementById("AqueceID").innerHTML = aquecedor;
  document.getElementById("AqueceID").style.backgroundColor =aquecedorColor;
	
  var refrigeradorColor = corVermelha;
  var refrigerador = "Desligado";
  if(arr.equipment.refrigeradorRelay == true){refrigerador = "Ligado" ; refrigeradorColor = corVerde;}
  document.getElementById("FreezerID").innerHTML = refrigerador;
  document.getElementById("FreezerID").style.backgroundColor =refrigeradorColor;

  var umidificadorColor = corVermelha;
  var umidificador = "Desligado";
  if(arr.equipment.humidificatorRelay){umidificador = "Ligado"; umidificadorColor = corVerde;}
  document.getElementById("HumidificatorID").innerHTML = umidificador;
  document.getElementById("HumidificatorID").style.backgroundColor = umidificadorColor;
	
  var compressorColor = corVermelha;
  var compressor = "Desligado";
  if(arr.equipment.compressorRelay){compressor = "Ligado"; compressorColor = corVerde;}
  document.getElementById("CompressorID").innerHTML = compressor;
  document.getElementById("CompressorID").style.backgroundColor = compressorColor;
	
  var lampadaColor = corVermelha;
  var lampada = "Desligada";
  if(arr.equipment.lightRelay == true){lampada = "Ligada" ; lampadaColor = corVerde;}
  document.getElementById("LampID").innerHTML = lampada;
  document.getElementById("LampID").style.backgroundColor = lampadaColor;
}


function UpdateImage()
{
    document.getElementById("imageID").src = "screenShot.png?rnd="+Math.random()+"&time=" + new Date();
}	
	
</script>
</body>
</html>
