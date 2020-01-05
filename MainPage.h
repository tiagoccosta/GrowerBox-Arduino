const char MainPage[] PROGMEM = R"=====(
<!doctype html>
<html>
<head>
<meta charset="utf-8">
<title>CoguGrower</title>
<style type="text/css">
body,td,th {
  color: #14253D;
}
body {
  background-color: #AEDCD1;
}
</style>
</head>

<body>
<div  style="background-color: #000A31; width: 100%;"><center>
  <h1 style="color:#99C1B4"><strong>CoguGrower</strong></h1>
</center></div>
  
<hr />
  
  <div width="100%"  style="border-radius: 10px 10px 10px 10px; border-width: thick; background-color:#6C9DB4">
  <center><h3>Ambiente</h3></center>
  <div id="tabelaAmbienteResumo" style="display: block" >
  <table align="center" title="Status" width="100%" border="1" style="background-color: #99CDE7">
    <tbody>
      <tr>
        <td id="tempResID" >Temperatura: 24 (24) Cº</td>
        <td id="HumidResID" >Umidade: 85 (85) %</td>
        <td id="Co2ResID" >CO2: 70 (70) ppm</td>
        <td id="LightResID" >Tempo: 0 (12|12) h</td>
      </tr>
    </tbody>
  </table>
  <center><a href="javascript:ChangeDivStates('tabelaAmbiente','tabelaAmbienteResumo')">Ver Detalhes</a></center>
  </div >
  <div id="tabelaAmbiente" style="display: none">
  <table align="center" title="Status" width="100%" border="1" style="background-color: #99CDE7">
   <thead>
    <tr>
      <td>Característica</td>
      <td>Valor</td>
      <td>Referência</td>
      <td>Tolerância</td>
      <td>Tempo Min</td>
    </tr>
   </thead>
    <tbody>
      <tr>
        <td>Temperatura</td>
      <td >
        <div id="tempID">0 Cº</div>
      </td>
      <td> <a href="javascript:ChangeDivStates('tempRefID_edit','tempRefID');"><div id="tempRefID"  >0 Cº</div></a>
        <div id="tempRefID_edit" style="display: none"><label id="tempRefID_edit_value">24</label><label> Cº</label><input type="range" min="1" max="70" step="1" oninput="ShowRangeEditVal(this.value, 'tempRefID_edit_value')" value="24"><input type="button" onClick="SaveChangeValue('tempRefID_edit_value','tempRefID','tempRefID_edit','Temperature','ReferenceValue')" value="Salvar"> <a href="javascript:ChangeDivStates('tempRefID','tempRefID_edit');">Cancelar</a></div>
      </td>
      <td> <a href="javascript:ChangeDivStates('tempTolerID_edit','tempTolerID');"><div id="tempTolerID"  >0 Cº</div></a>
        <div id="tempTolerID_edit" style="display: none"><label id="tempTolerID_edit_value">2</label><label> Cº</label><input type="range" min="1" max="10" step="1" oninput="ShowRangeEditVal(this.value, 'tempTolerID_edit_value')" value="2"><input type="button" onClick="SaveChangeValue('tempTolerID_edit_value','tempTolerID','tempTolerID_edit','Temperature','ToleranceValue')" value="Salvar"> <a href="javascript:ChangeDivStates('tempTolerID','tempTolerID_edit');">Cancelar</a></div>
      </td>
      <td> <a href="javascript:ChangeDivStates('tempTolerTimeID_edit','tempTolerTimeID');"><div id="tempTolerTimeID"  >15 s</div></a>
        <div id="tempTolerTimeID_edit" style="display: none"><label id="tempTolerTimeID_edit_value">15</label><label> s</label><input type="range" min="1" max="60" step="1" oninput="ShowRangeEditVal(this.value, 'tempTolerTimeID_edit_value')" value="15"><input type="button" onClick="SaveChangeValue('tempTolerTimeID_edit_value','tempTolerTimeID','tempTolerTimeID_edit','Temperature','ToleranceTime')" value="Salvar"> <a href="javascript:ChangeDivStates('tempTolerTimeID','tempTolerTimeID_edit');">Cancelar</a></div>
      </td>
      </tr>
      <tr>
        <td>Umidade</td>
      <td id="HumidID" >70%</td>
      <td id="HumidRefID" >85%</td>
      <td id="HumidTolerID" >3%</td>
      <td id="HumidTolerTimeID" >15s</td>
      </tr>
      <tr>
        <td>CO2</td>
      <td id="Co2ID" >70 ppm</td>
      <td id="Co2RefID" >70 ppm</td>
      <td id="Co2TolerID" >10</td>
      <td id="Co2TolerTimeID" >15s</td>
      </tr>
      <tr>
        <td>Iluminação</td>
      <td id="LightID" >0.0h</td>
      <td id="LightRefID" >12h / 12h</td>
      <td id="LightTolerID" ></td>
      <td id="LightTolerTimeID" ></td>
      </tr>
    </tbody>
  </table>
  <center><a href="javascript:ChangeDivStates('tabelaAmbienteResumo','tabelaAmbiente')">Esconder Detalhes</a></center>
  </div>
  </div>
<hr />
  <div width="100%"  style="border-radius: 10px 10px 0 0; border-width: thick; background-color:#6C9DB4">
       <center>
        <h3>Equipamentos</h3>
        </center>
  <table align="center" title="Status" width="100%" border="1" style="background-color: #99CDE7">
    <tbody>
      <tr>
        <td id="AqueceID" >Aquecedor: Desligado</td>
        <td id="FreezerID" >Refrigerador: Desligado</td>
        <td id="HumidificatorID" >Umidificador: Desligado</td>
        <td id="CompressorID" >Compressor: Desligado</td>
        <td id="LampID" >Lampada: Desligado</td>
      </tr>
      <tr>
        <td id="AqueceForceID" >
          <a>Forçar</a>
        </td>
        <td id="FreezerForceID" >
          <a>Forçar</a>
        </td>
        <td id="HumidificatorForceID" >
          <a>Forçar</a>
        </td>
        <td id="CompressorForceID" >
          <a>Forçar</a>
        </td>
        <td id="LampForceID" >
          <a>Forçar</a>
        </td>
      </tr>
    </tbody>
  </table>
</div>

<hr />

  <div id="WifiConnectedContainer" width="100%"  style="border-radius: 10px 10px 0 0; border-width: thick; background-color:#6C9DB4; display: none;">
       <center>
        <h3>Conexão WIFI</h3>
        </center>
  <table align="center" title="Status" width="100%" border="1" style="background-color: #99CDE7">
    <tbody>
      <tr>
        <td id="WifiNameID" >Wifi Name</td>
        <td id="WifiIpID" >192.168.0.1</td>
        <td width="90px" ><a href="/WifiDisconnect">Desconectar</a></td>
      </tr>
    </tbody>
  </table>
</div>
  
  <div id="WifiDisconnectedContainer" width="100%"  style="border-radius: 10px 10px 0 0; border-width: thick; background-color:#6C9DB4; display: none;">
       <center>
        <h3>Conexão WIFI</h3>
        </center>
  <table align="center" title="Status" width="100%" border="1" style="background-color: #99CDE7">
    <tbody>
      <tr>
        <td width="90px" ><center><a href="/WifiSetup">Configurar</a></center></td>
      </tr>
    </tbody>
  </table>
</div>


<script>
  GetJson();
    setInterval(function() { GetJson(); }, 2500);
  GetWifiInfoJson();
    setInterval(function() { GetWifiInfoJson(); }, 2500);
  
  
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
  GetHttpResponse("/GetData", SetData)
}
function SetData(arr) {
  document.getElementById("tempResID").innerHTML = "Temperatura: "+ arr.temperature.now+" ("+arr.temperature.ref+") Cº";
  document.getElementById("HumidResID").innerHTML = "Umidade: "+ arr.humidity.now+" ("+arr.humidity.ref+") %";
  document.getElementById("Co2ResID").innerHTML = "CO2: "+ arr.co2.now+" ("+arr.co2.ref+") ppm";
  document.getElementById("LightResID").innerHTML = "Tempo: "+ arr.light.now+" ("+arr.light.lightTimeRef+"|"+arr.light.darkTimeRef+") h";
  
  
  document.getElementById("tempID").innerHTML = arr.temperature.now+" Cº";
  document.getElementById("tempRefID").innerHTML = arr.temperature.ref+" Cº";
  document.getElementById("tempTolerID").innerHTML = arr.temperature.toleranceValue+" Cº";
  document.getElementById("tempTolerTimeID").innerHTML = arr.temperature.toleranceTime+" s";
  
  document.getElementById("HumidID").innerHTML = arr.humidity.now+" %";
  document.getElementById("Co2ID").innerHTML = arr.co2.now+" ppm";
  document.getElementById("LightID").innerHTML = arr.light.now+" h";

  

  var aquecedor = "Desligado";
  if(arr.equipment.aquecedorRelay == true){aquecedor = "Ligado"}
  document.getElementById("AqueceID").innerHTML = "Aquecedor: "+ aquecedor;
  var refrigerador = "Desligado";
  if(arr.equipment.refrigeradorRelay == true){refrigerador = "Ligado"}
  document.getElementById("FreezerID").innerHTML = "Refrigerador: "+ refrigerador;
  var umidificador = "Desligado";
  if(arr.equipment.humidificatorRelay){umidificador = "Ligado"}
  document.getElementById("HumidificatorID").innerHTML = "Umidificador: "+ umidificador;
  var compressor = "Desligado";
  if(arr.equipment.compressorRelay){compressor = "Ligado"}
  document.getElementById("CompressorID").innerHTML = "Compressor: "+ compressor;
  var lampada = "Desligada";
  if(arr.equipment.lightRelay == true){lampada = "Ligada"}
  document.getElementById("LampID").innerHTML = "Lampada: "+ lampada;
}
  
  
  
function GetWifiInfoJson() {
  GetHttpResponse("/GetWifiInfo", SetWifiInfo)
}
  
function SetWifiInfo(arr){
  if(arr.wifi.Connected == true){
    document.getElementById("WifiConnectedContainer").style.display = 'block';
    document.getElementById("WifiDisconnectedContainer").style.display = 'none';
      document.getElementById("WifiNameID").innerHTML = arr.wifi.ssid;
      document.getElementById("WifiIpID").innerHTML = arr.wifi.ip;
  }else{
    ChangeDivStates("WifiDisconnectedContainer", "WifiConnectedContainer");
  }
}
  
function ShowRangeEditVal(val, id){     document.getElementById(id).innerHTML = val;
}
function ChangeDivStates(idShow, idHidde){  
  document.getElementById(idShow).style.display = 'block';
  document.getElementById(idHidde).style.display = 'none';
}
function SaveChangeValue(idValue, idShow, idHidde, typeClass, typeAttr){
  if (!confirm("Você tem certeza que deseja alterar o valor?")) {
      return;
  }
  ChangeDivStates(idShow, idHidde);
  var value = document.getElementById(idValue).innerHTML;
  SetNewValue(value, typeClass, typeAttr);
}

function SetNewValue(newValue, typeClass, typeAttr) {
  var url = "/SetNewValue?"+"Value="+newValue+"&TypeClass="+typeClass+"&TypeAttr="+typeAttr;
  GetHttpResponse(url, SetNewValueResponse)
}

function SetNewValueResponse(response){
  if(response.Change.Success){
    alert("Valor alterado com sucesso!");
  }else{
    alert("O valor não foi alterado!");
  }
}
</script>

</body>
</html>

)=====";
