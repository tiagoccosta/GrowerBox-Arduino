

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>

#include <ArduinoJson.h>

#include <DHTesp.h>
//#include <DHT.h>

#include "MainPage.h" 

#include <ESP8266HTTPClient.h>


////////////////////////////////////////////////////////////////////////////////////////////////////
//============================= DEFINIÃ‡Ã•ES DE CONSTANTES do Sistema ==============================//
////////////////////////////////////////////////////////////////////////////////////////////////////
#define D0    16     //
#define D1    5      //
#define D2    4      //
#define D3    0      //
#define D4    2      //
#define D5    14     //
#define D6    12     //
#define D7    13     //
#define D8    15     //

#define DHTPIN D5     

#define INFOLED D0
#define DATALED D4



class GrowerBrain {

  //ID
  private: int id;
  public: int ID(){return id;}

  //Para uso geral
  long lastMillisTime = 0;
  long lastResetedTime = 0;
  long timeNow = 0;

  //Sensores
  private: 
    DHTesp dhtSensor; // DHT11 sensor pin
    int GasSensor_Pin; // MQ-135 sensor pin

  //Temperatura em graus
  public: 
    bool useTemperatureSystem = false;
    int tempRef = 24;
    int tempNow = 24;
    int tempToleranceValue = 1; //
    int tempToleranceTime = 15; //Tempo para mudar de estado novamente
    long tempToleranceTimeStart = 0;
    bool aquecedorRelay = false;
    bool refrigeradorRelay = false;
    bool usingTempTolerance = false;
    int tempState = 0;

  //Umidade em porcentagem
  public: 
    bool useHumidificationSystem = false;
    int humidRef = 85;
    int humidNow = 85;
    int humidToleranceValue = 2; //
    int humidToleranceTime = 15; //Tempo para mudar de estado novamente
    long humidToleranceTimeStart = 0;
    bool humidificatorRelay = false;
    bool usingHumidTolerance = false;
    int humidState = 0;

  //Oxigenação em porcentagem
  public: 
    bool useOxygenationSystem = false;
    int co2Ref = 700;
    float co2Now = 100;
    int co2ToleranceValue = 1; //
    int co2ToleranceTime = 15; //Tempo para mudar de estado novamente
    bool compressorRelay = true;

  //Iluminação em horas
  public: 
    bool useLightingSystem = false;
    int lightTimeRef = 12;
    int darkTimeRef = 12;
    int lightingTimeHour = 0;
    bool lightRelay = false;

  //Inicialização
  public: 
  GrowerBrain (int dhtPin){
    dhtSensor.setup(dhtPin, DHTesp::DHT11);
    
  }

  public: 
  void SetDirty(bool value){
    dirty = value;
  }
  bool IsDirty(){
    return dirty;
  }

  void Work(){

    if(lastMillisTime>millis()){
      lastResetedTime = millis();
    }
    timeNow = (lastResetedTime+millis()/1000);
    lastMillisTime = millis();
    

    //////////Temperature
    int tempNow_ = GetTemp();
    if(tempNow_!=tempNow){
      tempNow = tempNow_;
      SetDirty(true);
    }
    if(usingTempTolerance){
      int comparation = CompareValue(tempNow,tempRef);
      if(comparation!=tempState){
        if(timeNow-tempToleranceTimeStart>tempToleranceTime || comparation<0-tempToleranceValue || comparation>0+tempToleranceValue){
          if(comparation==0){
            aquecedorRelay = false;
            refrigeradorRelay = false;
          }
          if(comparation<0){
            aquecedorRelay = true;
            refrigeradorRelay = false;
          }
          if(comparation>0){
            aquecedorRelay = false;
            refrigeradorRelay = true;
          }
          tempState = comparation;
          usingTempTolerance = false;
          SetDirty(true);
        }
      }else{
        usingTempTolerance = false;
      }
    }else{
      int comparation = CompareValue(tempNow,tempRef);
      if(comparation!=tempState){
        usingTempTolerance = true;
        tempToleranceTimeStart = timeNow;
        
      }
    }



    //////////Humidity
    int humidNow_ = GetHumidity();
    if(humidNow_!=humidNow){
      humidNow = humidNow_;
      SetDirty(true);
    }
    if(usingHumidTolerance){
      int comparation = CompareValue(humidNow,humidRef);
      if(comparation!=humidState){
        if(timeNow-humidToleranceTimeStart>humidToleranceTime || comparation<0-humidToleranceValue || comparation>0+humidToleranceValue){
          if(comparation==0){
            humidificatorRelay = false;
          }
          if(comparation<0){
            humidificatorRelay = true;
          }
          if(comparation>0){
            humidificatorRelay = false;
          }
          humidState = comparation;
          usingHumidTolerance = false;
          SetDirty(true);
        }
      }else{
          usingHumidTolerance = false;
      }
    }else{
      int comparation = CompareValue(humidNow,humidRef);
      if(comparation!=humidState){
        usingHumidTolerance = true;
        humidToleranceTimeStart = timeNow;
      }
    }


    //Lighting
    int lightingTimeHour_ = (timeNow/60)/60;
    if(lightingTimeHour_>lightTimeRef+darkTimeRef){
      int val = lightingTimeHour_/(lightTimeRef+darkTimeRef);
      lightingTimeHour_ = lightingTimeHour_-((lightTimeRef+darkTimeRef)*val);
    }
    if(lightingTimeHour != lightingTimeHour_){
      lightingTimeHour = lightingTimeHour_;
      SetDirty(true);
    }
    bool isLighting = lightingTimeHour<lightTimeRef;
    if(lightRelay!=isLighting){
      lightRelay = isLighting;
      SetDirty(true);
    }
    
    
    

    
    
  }

  private:
  bool dirty = false;
  int CompareValue(double valueNow, double valueRef){
    return valueNow - valueRef;
  }
  
  int GetTemp() {
    int t = dhtSensor.getTemperature(); 
    if (isnan(t)) { t = tempRef; }
    return t;
  }

  int GetHumidity() {
    int h = dhtSensor.getHumidity(); 
    if (isnan(h)) { h = humidRef; }
    return h;
  }
  
};



char *ssid = "GrowerBrain";
char *password = "password";

String wifiCon_ssid = "";
String wifiCon_pass = "";


long updateRate = 60;
long lastUpdate = -60;

//============================================= Declaracao das instancias das bibliotecas =============================================///
ESP8266WebServer server(80);  

GrowerBrain gBrain (DHTPIN);



void setup() {
///================================================= PARTE DE CONFIGURACAO DO SISTEMA =================================================///
  
  Serial.begin(57600);     
  
  pinMode(INFOLED, OUTPUT);     
  digitalWrite(INFOLED, HIGH);
  pinMode(DATALED, OUTPUT);     
  digitalWrite(DATALED, HIGH);
  
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, password, 9); 


  IPAddress myIP; 


  myIP = WiFi.softAPIP();                       
  Serial.print("Acess point criado. Ip: ");     
  Serial.println(myIP);

///================================================= PARTE DE CONFIGURACAO DO SERVIDOR =================================================///

  server.on("/", MENU);                         
  server.on("/WifiSetup",WifiSetup);
  server.on("/WifiConnectSetup",WifiConnectSetup);
  server.on("/WifiDisconnect",WifiDisconnect);
  server.on("/GetWifiInfo",GetWifiInfo);
  server.on("/GetWifiScanData",GetWifiScanData);
  server.on("/GetData",GetData);
  server.on("/SetNewValue",SetNewValue);
  server.on("/CheckCredentials",CheckCredentials);
  
  server.begin();                               
}

///=================================================== LOOPING PRINCIPAL DO PROGRAMA ===================================================///
///=====================================================================================================================================///
///=====================================================================================================================================///

void loop() {
  gBrain.Work();
  yield();                                      
  
  server.handleClient();                       
  

  
  if(gBrain.timeNow>=lastUpdate+updateRate){
      Serial.print("Sending data for time...");
      SendDataToRemoteHost();
      lastUpdate = gBrain.timeNow;
  }                                    

  if(gBrain.IsDirty()){
      Serial.print("Sending dirty data...");
      SendDataToRemoteHost();
      gBrain.SetDirty(false);
  }
  
  delay(200);
}

void MENU(){
  server.send(200, "text/html", MainPage);
}

void WifiDisconnect(){
  digitalWrite(INFOLED, LOW);
  WiFi.disconnect();
  // Wait for disconnection
  int timeout = 50;
  while (WiFi.status() == WL_CONNECTED) {
    Serial.print(".");
    delay(200);
  }  
  WifiSetup();
  digitalWrite(INFOLED, HIGH);
}

void WifiSetup(){
  String html = Header("GrowerBrain - WifiSetup");
  html += "<h1><center>Wifi Setup</center></h1>";   
  
  if (WiFi.status() != WL_CONNECTED){
    
    int n = WiFi.scanNetworks();
    if (n == 0) {
      html += "<p><center><b><h1>Nenhuma rede wifi encontrada!</h1></b></center></p>";
    } else {
      Serial.print(n);
      Serial.println(" networks found");
      html += "<table class=\"editorDemoTable\"><thead><tr><td>Name of Wifi</td><td>Password</td><td>Connect</td></tr></thead><tbody>";
      for (int i = 0; i < n; ++i) {
        // Print SSID and RSSI for each network found
        html += "<form action='/WifiConnectSetup' method='GET'><tr>";
        html += "<td>"+WiFi.SSID(i)+"</td>";
        html += "<input type=\"hidden\" id=\"custId\" name=\"WifiName\" value="+WiFi.SSID(i)+">";
        html += "<td><input name=\"WifiPassword\" type=\"password\" placeholder=\"password\" /></td>";
        html += "<td><input type=\"submit\" value=\"Connect\"> </td>";
        html += "</tr></form>";
        
        
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(WiFi.SSID(i));
        Serial.print(" (");
        Serial.print(WiFi.RSSI(i));
        Serial.print(")");
        Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
        delay(10);
      }
      html += "</tbody></table>";
    }
  } else {
    html += "<p><a href=/WifiDisconnect><center><button style='background-color:#0000FF; height:100px; width:250px; type='button'><b><h1>Disconnect WIFI</h1></b></button></center></a></p>";
  }
  html += "<center><form method='GET' action='/'><p><input name=button2 type=submit value=Voltar /></p></form></center>";
  html = Footer(html);
  server.send(200, "text/html", html);
}

void GetData(){
  digitalWrite(DATALED, LOW);
  StaticJsonBuffer<600> jsonBuffer;
  JsonObject& cgJson = jsonBuffer.createObject();
  SetDataInJsonObject(cgJson);


  String json;
  cgJson.prettyPrintTo(json);
  server.send(200, "text/json", json);
  
  digitalWrite(DATALED, HIGH);
}

void SetDataInJsonObject(JsonObject& json){
  JsonObject& humidity = json.createNestedObject("humidity");
  humidity["now"] = gBrain.humidNow;
  humidity["ref"] = gBrain.humidRef;
  humidity["toleranceValue"] = gBrain.humidToleranceValue;
  humidity["toleranceTime"] = gBrain.humidToleranceTime;
  
  JsonObject& temperature = json.createNestedObject("temperature");
  temperature["now"] = gBrain.tempNow;
  temperature["ref"] = gBrain.tempRef;
  temperature["toleranceValue"] = gBrain.tempToleranceValue;
  temperature["toleranceTime"] = gBrain.tempToleranceTime;
  
  JsonObject& co2 = json.createNestedObject("co2");
  co2["now"] = gBrain.co2Now;
  co2["ref"] = gBrain.co2Ref;
  co2["toleranceValue"] = gBrain.co2ToleranceValue;
  co2["toleranceTime"] = gBrain.co2ToleranceTime;
  
  JsonObject& light = json.createNestedObject("light");
  light["now"] = gBrain.lightingTimeHour;
  light["lightTimeRef"] = gBrain.lightTimeRef;
  light["darkTimeRef"] = gBrain.darkTimeRef;
  
  JsonObject& equipment = json.createNestedObject("equipment");
  equipment["humidificatorRelay"] = gBrain.humidificatorRelay;
  equipment["aquecedorRelay"] = gBrain.aquecedorRelay;
  equipment["refrigeradorRelay"] = gBrain.refrigeradorRelay;
  equipment["compressorRelay"] = gBrain.compressorRelay;
  equipment["lightRelay"] = gBrain.lightRelay;
}

void GetWifiInfo(){
  StaticJsonBuffer<600> jsonBuffer;
  JsonObject& wifiJson = jsonBuffer.createObject();
  
  JsonObject& wifi = wifiJson.createNestedObject("wifi");
  wifi["Connected"] = WiFi.status() == WL_CONNECTED;
  wifi["ssid"] = WiFi.SSID();
  wifi["ip"] = ipToString(WiFi.localIP());
  
  String json;
  wifiJson.prettyPrintTo(json);
  server.send(200, "text/json", json);
}

void GetWifiScanData(){

  StaticJsonBuffer<600> jsonBuffer;
  JsonObject& wifiJson = jsonBuffer.createObject();
  int n = WiFi.scanNetworks();
  wifiJson["count"]=n;
  JsonArray& wifis = wifiJson.createNestedArray("networks");
  if (n == 0) {
  
  }else{
    for (int i = 0; i < n; ++i) {
      JsonObject& wifi = wifis.createNestedObject();
      wifi["ssid"] = WiFi.SSID(i);
      wifi["signal"] = WiFi.RSSI(i);
    }
  }
  String json;
  wifiJson.prettyPrintTo(json);
  server.send(200, "text/json", json);
}

void WifiConnectSetup(){
  digitalWrite(INFOLED, LOW);
  if (server.arg("WifiName")!= "" && server.arg("WifiPassword")!= ""){     
    String wifiName = server.arg("WifiName");
    String wifiPassword = server.arg("WifiPassword");

  Serial.print("Connecting to ");
  Serial.println(wifiName);
  Serial.print("WifiPassword: ");
  Serial.println(wifiPassword);

  WiFi.begin(wifiName.c_str(), wifiPassword.c_str());


  int timeout = 40;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
    timeout--;
    if(timeout==0){
      WifiSetup();
      digitalWrite(INFOLED, HIGH);
      return;
    }
  }  

  Serial.print("Connected to ");
  Serial.println(wifiName);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  wifiCon_ssid = wifiName;
  wifiCon_pass = wifiPassword;
  String html = Header("GrowerBrain - WifiConnect");
    String _ip = ipToString(WiFi.localIP());
    html += "<h1><center>"+wifiName+"</center></h1>";  
    html += "<h2><center>Ip: "+_ip+"</center></h2>";  
    html += "<p><a href=/WifiDisconnect><center><button style='background-color:#0000FF; height:100px; width:250px; type='button'><b><h1>Disconnect WIFI</h1></b></button></center></a></p>";
  html = Footer(html);
  server.send(200, "text/html", html);
  }else{
      WifiSetup();
    }
  
  
  
  digitalWrite(INFOLED, HIGH);
}

void SetNewValue(){
  digitalWrite(DATALED, LOW);

  StaticJsonBuffer<600> jsonBuffer;
  JsonObject& changeDataJson = jsonBuffer.createObject();
  JsonObject& change = changeDataJson.createNestedObject("Change");
  bool changeSuccess = false;
  String argValue = "";
  String argTypeClass = "";
  String argTypeAttr = "";
  if (server.arg("Value")!= "" && server.arg("TypeClass")!= "" && server.arg("TypeAttr")!= ""){     //Parameter not found
    argValue = server.arg("Value");
    argTypeClass = server.arg("TypeClass");
    argTypeAttr = server.arg("TypeAttr");

    if(argTypeClass=="Temperature"){
      if(argTypeAttr=="ReferenceValue"){
        gBrain.tempRef = argValue.toInt();
        changeSuccess = true;
      }
      if(argTypeAttr=="ToleranceValue"){
        gBrain.tempToleranceValue = argValue.toInt();
        changeSuccess = true;
      }
      if(argTypeAttr=="ToleranceTime"){
        gBrain.tempToleranceTime = argValue.toInt();
        changeSuccess = true;
      }
    }
    
    if(argTypeClass=="Humidity"){
      if(argTypeAttr=="ReferenceValue"){
        gBrain.humidRef = argValue.toInt();
        changeSuccess = true;
      }
      if(argTypeAttr=="ToleranceValue"){
        gBrain.humidToleranceValue = argValue.toInt();
        changeSuccess = true;
      }
      if(argTypeAttr=="ToleranceTime"){
        gBrain.humidToleranceTime = argValue.toInt();
        changeSuccess = true;
      }
    }
    
    if(argTypeClass=="CO2"){
      if(argTypeAttr=="ReferenceValue"){
        gBrain.co2Ref = argValue.toInt();
        changeSuccess = true;
      }
      if(argTypeAttr=="ToleranceValue"){
        gBrain.co2ToleranceValue = argValue.toInt();
        changeSuccess = true;
      }
      if(argTypeAttr=="ToleranceTime"){
        gBrain.co2ToleranceTime = argValue.toInt();
        changeSuccess = true;
      }
    }
  }
  if(changeSuccess){gBrain.SetDirty(false);}
  change["Success"] = changeSuccess;
  change["Value"] = argValue;
  change["TypeClass"] = argTypeClass;
  change["TypeAttr"] = argTypeAttr;
  changeDataJson.prettyPrintTo(Serial);
  String json;
  changeDataJson.prettyPrintTo(json);
  server.send(200, "text/json", json);
  
  digitalWrite(DATALED, HIGH);
}



void GetRemoteHostInfo(){
  StaticJsonBuffer<600> jsonBuffer;
  JsonObject& hostJson = jsonBuffer.createObject();
  JsonObject& host = hostJson.createNestedObject("wifi");
  host["address"] = "cogugrower.com.br";
  host["pass"] = "senha1";
  host["updateTime"] = 5; //In seconds
  
  String json;
  hostJson.prettyPrintTo(Serial);
  hostJson.prettyPrintTo(json);
  server.send(200, "text/json", json);
}


String GetSaveData(){
  StaticJsonBuffer<600> jsonBuffer;
  JsonObject& dataJson = jsonBuffer.createObject();
  JsonObject& data = dataJson.createNestedObject("data");
  SetDataInJsonObject(data);
  JsonObject& hostData = dataJson.createNestedObject("host");
  JsonObject& wifiInfo = dataJson.createNestedObject("wifi");
  
  String json;
  dataJson.prettyPrintTo(json);
  return json;
}

void CheckCredentials(){
  StaticJsonBuffer<600> jsonBuffer;
  JsonObject& hostJson = jsonBuffer.createObject();
  
  bool confirmed =false;
  String user = "";
  String pass = "";
  if (server.arg("user")!= "" && server.arg("pass")!= ""){     
    user = server.arg("user");
    pass = server.arg("pass");
  }
  
  JsonObject& admin = hostJson.createNestedObject("credentials");
  admin["user"] = user;
  admin["pass"] = pass;
  admin["confimed"] = confirmed;
  
  
  String json;
  hostJson.prettyPrintTo(json);
  server.send(200, "text/json", json);
}


void SendDataToRemoteHost(){
  
  if(WiFi.status() != WL_CONNECTED){
    return;
  }

  
  
  
    
  StaticJsonBuffer<600> jsonBuffer;
  JsonObject& cgJson = jsonBuffer.createObject();
  SetDataInJsonObject(cgJson);


  String json;
  cgJson.prettyPrintTo(json);
  
  HTTPClient http;    
 
  http.begin("http://dominio.com.br/send-data.php?user=admin&pass=password");      
  http.addHeader("Content-Type", "application/json");  
 
  int httpCode = http.POST(json); 
  String payload = http.getString();                                       
 
  Serial.println("Codigo de resposta: "+String(httpCode));  
  Serial.println("Resposta: "+payload);  

  http.end();  

  JsonObject& root = jsonBuffer.parseObject(payload);
  
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  if(root["Success"]){
    gBrain.SetDirty(false);
  }
    
}

String ipToString(IPAddress ip){
  String s="";
  for (int i=0; i<4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;
}
