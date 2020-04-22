#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>


//WiFiServer server(80); //
ESP8266WebServer server(80);
char* ssid = "Braaap";
char* password = "gratteur";

const byte numChars = 64;
char receivedChars[numChars];
char tempChars[numChars];     // temporary array for use when parsing
char messageFromPC[numChars] = {0}; // variables to hold the parsed data
int integerFromPC = 0;
float floatFromPC = 0.0;
boolean newData = false;
int integr;
int ledstate;
int pompestate;

int tds;
float tair;
float teau;
float humi;
float tbox;

int Onhour;
int Onmin;
int Offhour;
int Offmin;
int valtds;
int maxteau;
int minteau;
int timear;

String maxtemp;
String mintemp;

String onhour;
String onmin;
String offhour;
String offmin;
String timearro = server.arg("arro");
String tdsvalue ;
int message;
int int1;
int int2;
int int3;
float float1;
float float2;
float float3;
float float4;

const char* www_username = "admin";
const char* www_password = "alex";

const char* www_realm = "Bienvenue Identification Requise";


String sendtds(int valtds) {
  String t = "<!DOCTYPE html>";
  t += "<html><head>";
  t += "<meta http-equiv='refresh' content='20' />";
  t += "<meta  name='viewport' content='width=device-width, initial-scale=1' />";
  t += "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css'>";
  t += "<style>";
  t += "html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #444444; background:black;}";
  t += "body {margin:0;font-family:Arial}";
  t += ".topnav {overflow: hidden;background-color: #333;}";
  t += ".topnav a {float: left;display: block;color: #f2f2f2;text-align: center;padding: 14px 16px;text-decoration: none;font-size: 17px;}";
  t += ".active {background-color: #4CAF50;color: white;}";
  t += ".topnav .icon {display: none;}";
  t += ".dropdown {float: left;overflow: hidden;}";
  t += ".dropdown .dropbtn {font-size: 17px;    border: none;outline: none;color: white;padding: 14px 16px;background-color: inherit;font-family: inherit;margin: 0;}";
  t += ".dropdown-content {display: none;position: absolute;background-color: #f9f9f9;min-width: 160px;box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);z-index: 1;}";
  t += ".dropdown-content a {float: none;color: black;padding: 12px 16px;text-decoration: none;display: block;text-align: left;}";
  t += ".topnav a:hover, .dropdown:hover .dropbtn {background-color: #555;color: white;}";
  t += ".dropdown-content a:hover {background-color: #ddd;color: black;}";
  t += ".dropdown:hover .dropdown-content {display: block;}";
  t += "@media screen and (max-width: 600px) {.topnav a:not(:first-child), .dropdown .dropbtn {display: none;}";
  t += ".topnav a.icon {float: right;display: block;}}";
  t += "@media screen and (max-width: 600px) {";
  t += ".topnav.responsive {position: relative;}";
  t += ".topnav.responsive .icon {position: absolute;right: 0;top: 0;}";
  t += ".topnav.responsive a {float: none;display: block;text-align: left;}";
  t += ".topnav.responsive .dropdown {float: none;}";
  t += ".topnav.responsive .dropdown-content {position: relative;}";
  t += ".topnav.responsive .dropdown .dropbtn {display: block;width: 100%;text-align: left;}}";
  t += "</style>";
  t += "</head><body>";
  t += "<div class='topnav' id='myTopnav'>";
  t += "<a href='../' >Home</a>";
  t += "<div class='dropdown'>";
  t += "<button class='dropbtn'> Panel V1 ";
  t += "<i class='fa fa-caret-down'></i>";
  t += "</button>";
  t += "<div class='dropdown-content'>";
  t += "<a href='../data'>Monitor</a>";
  t += "<a href='../ledtest'>Led state</a>";
  t += "<a href='../pompetest'>Pompe state</a>";
  t += "</div>";
  t += "</div>";
  t += "<a href='../formu'  >Set Water Temperature</a>";
  t += "<a href='../timer'>Set Timmer Light</a>";
  t += "<a href='/tds' class='active' >Set TDS</a>";
  t += "<a href='/ror'>Bonus</a>";
  t += "<a href='javascript:void(0);' style='font-size:15px;' class='icon' onclick='myFunction()'>&#9776;</a></div>";
  t += "<script>";
  t += "function myFunction() {";
  t += "var x = document.getElementById('myTopnav');";
  t += "if (x.className === 'topnav') {";
  t += "x.className += ' responsive';";
  t += "} else {";
  t += "x.className = 'topnav';";
  t += "}}</script></body>";
  t += "<div style='text-align:center;padding:1em 0;'><h2>Set TDS Value </h2>";
  t += "<center><body><form action='/tds' ;' method='POST' ><br>";
  t += "Total Dissolved Solid : <br><input  type='number' min='16' max='100' name='tdsvalue' placeholder=' 20 / 200 ' required type='number'><br> TDS actuel : ";
  t +=  valtds;
  t += "<br>";
  t += "<input type='submit' name='SUBMIT' value='Set TDS' onclick=\"return confirm(\'Are you sure ?\');\"></form>";
  t += "  <a href='/tds'>Refresh</a></body></center></html>";
  return t;
}

void tdsval() {
  if (!server.authenticate(www_username, www_password))
  {
    return server.requestAuthentication(DIGEST_AUTH, www_realm, errIMG());
  }
  tdsvalue = server.arg("tdsvalue");

  server.send(200, "text/html", sendtds(valtds));
  Serial.print("<");
  Serial.print("8");
  Serial.print(",");
  Serial.print(tdsvalue.toInt());
  Serial.print(">");

  Serial.println("<3>");

  server.sendHeader("Location", "/tds");
  server.send(301);
}
String sendwater(int timear, int maxteau, int minteau) {


  String t = "<!DOCTYPE html>";
  t += "<meta http-equiv='refresh' content='20' />";
  t += "<meta  name='viewport' content='width=device-width, initial-scale=1' />";
  t += "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css'>";
  t += "<style>";
  t += "html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #444444; background:black;}";
  t += "body {margin:0;font-family:Arial}";
  t += ".topnav {overflow: hidden;background-color: #333;}";
  t += ".topnav a {float: left;display: block;color: #f2f2f2;text-align: center;padding: 14px 16px;text-decoration: none;font-size: 17px;}";
  t += ".active {background-color: #4CAF50;color: white;}";
  t += ".topnav .icon {display: none;}";
  t += ".dropdown {float: left;overflow: hidden;}";
  t += ".dropdown .dropbtn {font-size: 17px;    border: none;outline: none;color: white;padding: 14px 16px;background-color: inherit;font-family: inherit;margin: 0;}";
  t += ".dropdown-content {display: none;position: absolute;background-color: #f9f9f9;min-width: 160px;box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);z-index: 1;}";
  t += ".dropdown-content a {float: none;color: black;padding: 12px 16px;text-decoration: none;display: block;text-align: left;}";
  t += ".topnav a:hover, .dropdown:hover .dropbtn {background-color: #555;color: white;}";
  t += ".dropdown-content a:hover {background-color: #ddd;color: black;}";
  t += ".dropdown:hover .dropdown-content {display: block;}";
  t += "@media screen and (max-width: 600px) {.topnav a:not(:first-child), .dropdown .dropbtn {display: none;}";
  t += ".topnav a.icon {float: right;display: block;}}";
  t += "@media screen and (max-width: 600px) {";
  t += ".topnav.responsive {position: relative;}";
  t += ".topnav.responsive .icon {position: absolute;right: 0;top: 0;}";
  t += ".topnav.responsive a {float: none;display: block;text-align: left;}";
  t += ".topnav.responsive .dropdown {float: none;}";
  t += ".topnav.responsive .dropdown-content {position: relative;}";
  t += ".topnav.responsive .dropdown .dropbtn {display: block;width: 100%;text-align: left;}}";
  t += "</style></head><body>";
  t += "<div class='topnav' id='myTopnav'>";
  t += "<a href='../'  >Home</a>";
  t += "<div class='dropdown'>";
  t += "<button class='dropbtn'> Panel V1 ";
  t += "<i class='fa fa-caret-down'></i>";
  t += "</button>";
  t += "<div class='dropdown-content'>";
  t += "<a href='../data'>Monitor</a>";
  t += "<a href='../ledtest'>Led state</a>";
  t += "<a href='../pompetest'>Pompe state</a>";
  t += "</div>";
  t += "</div>";
  t += "<a href='../formu'  class='active' >Set Water Temperature</a>";
  t += "<a href='../timer'>Set Timmer Light</a>";
  t += "<a href='/tds'>Set TDS</a>";
  t += "<a href='/ror'>Bonus</a>";
  t += "<a href='javascript:void(0);' style='font-size:15px;' class='icon' onclick='myFunction()'>&#9776;</a></div>";
  t += "<script>";
  t += "function myFunction() {";
  t += "var x = document.getElementById('myTopnav');";
  t += "if (x.className === 'topnav') {";
  t += "x.className += ' responsive';";
  t += "} else {";
  t += "x.className = 'topnav';";
  t += "}}</script></body>";
  t += "<div style='text-align:center;padding:1em 0;'><h2>Set Water Temperature </h2>";
  t += "<center><body><form action='/formu' method='POST'><br>";
  t += "Max T Eau : <input type='number' min='18' max='30' name='maxtemp' placeholder='20' required type='number'> Max actuel : ";
  t += maxteau;
  t += "<br>";
  t += "Min T Eau : <input type='number' min='18' max='30' name='mintemp' placeholder='18' required type='number' > Min actuel : ";
  t += minteau ;
  t += "<br>";
  t += "<input type='submit' name='SUBMIT' value='Set Temp' onclick=\"return confirm(\'Are you sure ?\');\"></form>";
  t += "<div style='text-align:center;padding:1em 0;'><h2>Set Water Timer </h2>";
  t += "<center><body><form action='/arro' method='POST'><br>";
  t += "Cycle On/Off Arrosage : <br><input type='number' min='1' max='15' name='arro' placeholder='1min' required type='number'><br> Min On/Off actuel : ";
  t += timear;
  t += "<br>";
  t += "<input type='submit' name='SUBMIT' value='Set Timer' onclick=\"return confirm(\'Are you sure ?\');\"></form><br>";
  t += "<a href='/formu'>Refresh</a><br></body></center></html>";
  return t;
}

void formular() {

  if (!server.authenticate(www_username, www_password))
  {
    return server.requestAuthentication(DIGEST_AUTH, www_realm, errIMG());
  }

  maxtemp = server.arg("maxtemp");
  mintemp = server.arg("mintemp");


  server.send(200, "text/html", sendwater(timear, maxteau, minteau));

  Serial.print("<");
  Serial.print("6");
  Serial.print(",");
  Serial.print(maxtemp.toInt());
  Serial.print(",");
  Serial.print(mintemp.toInt());
  Serial.print(">");

  Serial.println("<3>");

  server.sendHeader("Location", "/formu");
  server.send(301);
}
String sendtime(int Onhour, int Onmin, int Offhour, int Offmin) {

  String t = "<!DOCTYPE html>";
  t += "<meta http-equiv='refresh' content='20' />";
  t += "<meta  name='viewport' content='width=device-width, initial-scale=1' />";
  t += "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css'>";
  t += "<style>";
  t += "html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #444444; background:black;}";
  t += "body {margin:0;font-family:Arial}";
  t += ".topnav {overflow: hidden;background-color: #333;}";
  t += ".topnav a {float: left;display: block;color: #f2f2f2;text-align: center;padding: 14px 16px;text-decoration: none;font-size: 17px;}";
  t += ".active {background-color: #4CAF50;color: white;}";
  t += ".topnav .icon {display: none;}";
  t += ".dropdown {float: left;overflow: hidden;}";
  t += ".dropdown .dropbtn {font-size: 17px;    border: none;outline: none;color: white;padding: 14px 16px;background-color: inherit;font-family: inherit;margin: 0;}";
  t += ".dropdown-content {display: none;position: absolute;background-color: #f9f9f9;min-width: 160px;box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);z-index: 1;}";
  t += ".dropdown-content a {float: none;color: black;padding: 12px 16px;text-decoration: none;display: block;text-align: left;}";
  t += ".topnav a:hover, .dropdown:hover .dropbtn {background-color: #555;color: white;}";
  t += ".dropdown-content a:hover {background-color: #ddd;color: black;}";
  t += ".dropdown:hover .dropdown-content {display: block;}";
  t += "@media screen and (max-width: 600px) {.topnav a:not(:first-child), .dropdown .dropbtn {display: none;}";
  t += ".topnav a.icon {float: right;display: block;}}";
  t += "@media screen and (max-width: 600px) {";
  t += ".topnav.responsive {position: relative;}";
  t += ".topnav.responsive .icon {position: absolute;right: 0;top: 0;}";
  t += ".topnav.responsive a {float: none;display: block;text-align: left;}";
  t += ".topnav.responsive .dropdown {float: none;}";
  t += ".topnav.responsive .dropdown-content {position: relative;}";
  t += ".topnav.responsive .dropdown .dropbtn {display: block;width: 100%;text-align: left;}}";
  t += "</style></head><body>";
  t += "<div class='topnav' id='myTopnav'>";
  t += "<a href='../'  >Home</a>";
  t += "<div class='dropdown'>";
  t += "<button class='dropbtn'> Panel V1 ";
  t += "<i class='fa fa-caret-down'></i>";
  t += "</button>";
  t += "<div class='dropdown-content'>";
  t += "<a href='../data'>Monitor</a>";
  t += "<a href='../ledtest'>Led state</a>";
  t += "<a href='../pompetest'>Pompe state</a>";
  t += "</div>";
  t += "</div>";
  t += "<a href='../formu'  >Set Water Temperature</a>";
  t += "<a href='../timer' class='active'>Set Timmer Light</a>";
  t += "<a href='/tds'>Set TDS</a>";
  t += "<a href='/ror'>Bonus</a>";
  t += "<a href='javascript:void(0);' style='font-size:15px;' class='icon' onclick='myFunction()'>&#9776;</a></div>";
  t += "<script>";
  t += "function myFunction() {";
  t += "var x = document.getElementById('myTopnav');";
  t += "if (x.className === 'topnav') {";
  t += "x.className += ' responsive';";
  t += "} else {";
  t += "x.className = 'topnav';";
  t += "}}</script></body>";
  t += "<div style='text-align:center;padding:1em 0;'><h2>Set Water Temperature </h2>";
  t += "<body><form action='/timer' method='POST'><br>";
  t += "Hours : <input type='number' min='0' max='23' name='onhour' placeholder='07 heures' required type='number'> On actuel : ";
  t += Onhour;
  t += "<br>";
  t += "Minutes : <input type='number' min='0' max='59' name='onmin' placeholder='00 minutes' required type='number' > On actuel : ";
  t += Onmin ;
  t += "<br>";
  t += "Hours : <input type='number' min='0' max='23' name='offhour' placeholder='01 heures' required type='number'> Off actuel : ";
  t += Offhour;
  t += "<br>";
  t += "Minutes : <input type='number' min='0' max='59' name='offmin' placeholder='00 minutes' required type='number' > Off actuel : ";
  t += Offmin ;
  t += "<br>";
  t += "<input type='submit' name='SUBMIT' value='Set Time' onclick=\"return confirm(\'Are you sure ?\');\"></form>";
  t += "<a href='/timer'>Refresh</a><br></body></html>";
  return t;
}
void timmer() {
  if (!server.authenticate(www_username, www_password))
  {
    return server.requestAuthentication(DIGEST_AUTH, www_realm, errIMG());
  }
  onhour = server.arg("onhour");
  onmin = server.arg("onmin");
  offhour = server.arg("offhour");
  offmin = server.arg("offmin");


  Serial.print("<");
  Serial.print("7");
  Serial.print(",");
  Serial.print("0");
  Serial.print(",");
  Serial.print("0");
  Serial.print(",");
  Serial.print(onhour.toInt());
  Serial.print(",");
  Serial.print(onmin.toInt());
  Serial.print(",");
  Serial.print(offhour.toInt());
  Serial.print(",");
  Serial.print(offmin.toInt());
  Serial.print(">");

  server.send(200, "text/html", sendtime(Onhour, Onmin, Offhour, Offmin));
  Serial.println("<3>");

  server.sendHeader("Location", "/timer");
  server.send(301);

}


String SendHTML(float temperature, float humidity, float pressure, int altitude, float tbox) {

  String ptr = "<!DOCTYPE html>";
  ptr += "<html>";
  ptr += "<head>";
  ptr += "<title>Grow Box</title>";
  ptr += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  ptr += "<meta http-equiv='refresh' content='15' />";
  ptr += "<link href='https://fonts.googleapis.com/css?family=Open+Sans:300,400,600' rel='stylesheet'>";
  ptr += "<style>";
  ptr += "html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #444444; background:black;}";
  ptr += "body{margin: 0px;} ";
  ptr += "h1 {margin: 50px auto 30px;} ";
  ptr += ".side-by-side{display: table-cell;vertical-align: middle;position: relative;}";
  ptr += ".text{font-weight: 600;font-size: 19px;width: 200px;}";
  ptr += ".reading{font-weight: 300;font-size: 50px;padding-right: 25px;}";
  ptr += ".temperature .reading{color: #F29C1F;}";
  ptr += ".humidity .reading{color: #3B97D3;}";
  ptr += ".pressure .reading{color: #1abc9c ;}";
  ptr += ".altitude .reading{color: #955BA5;}";
  ptr += ".tbox .reading{color:   #FF0000;}";
  ptr += ".button { display: inline-block;";
  ptr += "padding: 8px 10px;";
  ptr += "font-size: 16px;";
  ptr += "cursor: pointer;";
  ptr += "text-align: center;";
  ptr += "text-decoration: none;";
  ptr += "outline: none;";
  ptr += "color: #f8fc00;";
  ptr += "background-color: #797E83;";
  ptr += "border: none;";
  ptr += "border-radius: 15px;";
  ptr += "box-shadow: 0 4px #ba0a0f;}";
  ptr += ".button:hover {background-color: #3e8e41}";
  ptr += ".button:active {background-color: #3e8e41; box-shadow: 0 3px #666; transform: translateY(4px);}";
  ptr += ".superscript{font-size: 17px;font-weight: 600;position: absolute;top: 10px;}";
  ptr += ".data{padding: 10px;}";
  ptr += ".container{display: table;margin: 0 auto;}";
  ptr += ".icon{width:65px}";
  ptr += ".topnav {overflow: hidden;background-color: #333;}";
  ptr += ".topnav a {float: left;display: block;color: #f2f2f2;text-align: center;padding: 14px 16px;text-decoration: none;font-size: 17px;}";
  ptr += ".active {background-color: #4CAF50;color: white;}";
  ptr += ".topnav .icon {display: none;}";
  ptr += ".dropdown {float: left;overflow: hidden;background-color: #4CAF50;}";
  ptr += ".dropdown .dropbtn {font-size: 17px;    border: none;outline: none;color: white;padding: 14px 16px;background-color: inherit;font-family: inherit;margin: 0;}";
  ptr += ".dropdown-content {display: none;position: absolute;background-color: #f9f9f9;min-width: 160px;box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);z-index: 1;}";
  ptr += ".dropdown-content a {float: none;color: black;padding: 12px 16px;text-decoration: none;display: block;text-align: left;}";
  ptr += ".topnav a:hover, .dropdown:hover .dropbtn {background-color: #555;color: white;}";
  ptr += ".dropdown-content a:hover {background-color: #ddd;color: black;}";
  ptr += ".dropdown:hover .dropdown-content {display: block;}";
  ptr += "@media screen and (max-width: 600px) {.topnav a:not(:first-child), .dropdown .dropbtn {display: none;}";
  ptr += ".topnav a.icon {float: right;display: block;}}";
  ptr += "@media screen and (max-width: 600px) {";
  ptr += ".topnav.responsive {position: relative;}";
  ptr += ".topnav.responsive .icon {position: absolute;right: 0;top: 0;}";
  ptr += ".topnav.responsive a {float: none;display: block;text-align: left;}";
  ptr += ".topnav.responsive .dropdown {float: none;}";
  ptr += ".topnav.responsive .dropdown-content {position: relative;}";
  ptr += ".topnav.responsive .dropdown .dropbtn {display: block;width: 100%;text-align: left;}}";
  ptr += "</style>";
  ptr += "</head>";
  ptr += "<body>";
  ptr += "<div class='topnav' id='myTopnav'>";
  ptr += "<a href='../'  >Home</a>";
  ptr += "<div class='dropdown'>";
  ptr += "<button class='dropbtn'> Panel V1 ";
  ptr += "<i class='fa fa-caret-down'></i>";
  ptr += "</button>";
  ptr += "<div class='dropdown-content'>";
  ptr += "<a href='../data' class='active'>Monitor</a>";
  ptr += "<a href='../ledtest'>Led state</a>";
  ptr += "<a href='../pompetest'>Pompe state</a>";
  ptr += "</div>";
  ptr += "</div>";
  ptr += "<a href='../formu'  >Set Water Temperature</a>";
  ptr += "<a href='../timer'>Set Timmer Light</a>";
  ptr += "<a href='/tds'>Set TDS</a>";
  ptr += "<a href='/ror'>Bonus</a>";
  ptr += "<a href='javascript:void(0);' style='font-size:15px;' class='icon' onclick='myFunction()'>&#9776;</a></div>";
  ptr += "<script>";
  ptr += "function myFunction() {";
  ptr += "var x = document.getElementById('myTopnav');";
  ptr += "if (x.className === 'topnav') {";
  ptr += "x.className += ' responsive';";
  ptr += "} else {";
  ptr += "x.className = 'topnav';";
  ptr += "}}</script>";
  ptr += "<h2>Control  V1</h2>";
  ptr += "<div class='container'>";
  ptr += "<div class='data tbox'>";
  ptr += "<div class='side-by-side icon'>";
  ptr += "<svg enable-background='new 0 0 19.438 54.003'height=54.003px id=Layer_1 version=1.1 viewBox='0 0 19.438 54.003'width=19.438px x=0px xml:space=preserve xmlns=http://www.w3.org/2000/svg xmlns:xlink=http://www.w3.org/1999/xlink y=0px><g><path d='M11.976,8.82v-2h4.084V6.063C16.06,2.715,13.345,0,9.996,0H9.313C5.965,0,3.252,2.715,3.252,6.063v30.982";
  ptr += "C1.261,38.825,0,41.403,0,44.286c0,5.367,4.351,9.718,9.719,9.718c5.368,0,9.719-4.351,9.719-9.718";
  ptr += "c0-2.943-1.312-5.574-3.378-7.355V18.436h-3.914v-2h3.914v-2.808h-4.084v-2h4.084V8.82H11.976z M15.302,44.833";
  ptr += "c0,3.083-2.5,5.583-5.583,5.583s-5.583-2.5-5.583-5.583c0-2.279,1.368-4.236,3.326-5.104V24.257C7.462,23.01,8.472,22,9.719,22";
  ptr += "s2.257,1.01,2.257,2.257V39.73C13.934,40.597,15.302,42.554,15.302,44.833z'fill=#FF0000 /></g></svg>";
  ptr += "</div>";
  ptr += "<div class='side-by-side text'>Temperature Box</div>";
  ptr += "<div class='side-by-side reading'>";
  ptr += tbox;
  ptr += "<span class='superscript'>&deg;C</span></div>";
  ptr += "</div>";
  ptr += "<div class='data temperature'>";
  ptr += "<div class='side-by-side icon'>";
  ptr += "<svg enable-background='new 0 0 19.438 54.003'height=54.003px id=Layer_1 version=1.1 viewBox='0 0 19.438 54.003'width=19.438px x=0px xml:space=preserve xmlns=http://www.w3.org/2000/svg xmlns:xlink=http://www.w3.org/1999/xlink y=0px><g><path d='M11.976,8.82v-2h4.084V6.063C16.06,2.715,13.345,0,9.996,0H9.313C5.965,0,3.252,2.715,3.252,6.063v30.982";
  ptr += "C1.261,38.825,0,41.403,0,44.286c0,5.367,4.351,9.718,9.719,9.718c5.368,0,9.719-4.351,9.719-9.718";
  ptr += "c0-2.943-1.312-5.574-3.378-7.355V18.436h-3.914v-2h3.914v-2.808h-4.084v-2h4.084V8.82H11.976z M15.302,44.833";
  ptr += "c0,3.083-2.5,5.583-5.583,5.583s-5.583-2.5-5.583-5.583c0-2.279,1.368-4.236,3.326-5.104V24.257C7.462,23.01,8.472,22,9.719,22";
  ptr += "s2.257,1.01,2.257,2.257V39.73C13.934,40.597,15.302,42.554,15.302,44.833z'fill=#F29C21 /></g></svg>";
  ptr += "</div>";
  ptr += "<div class='side-by-side text'>Temperature Air</div>";
  ptr += "<div class='side-by-side reading'>";
  ptr += temperature;
  ptr += "<span class='superscript'>&deg;C</span></div>";
  ptr += "</div>";
  ptr += "<div class='data pressure'>";
  ptr += "<div class='side-by-side icon'>";
  ptr += "<svg enable-background='new 0 0 19.438 54.003'height=54.003px id=Layer_1 version=1.1 viewBox='0 0 19.438 54.003'width=19.438px x=0px xml:space=preserve xmlns=http://www.w3.org/2000/svg xmlns:xlink=http://www.w3.org/1999/xlink y=0px><g><path d='M11.976,8.82v-2h4.084V6.063C16.06,2.715,13.345,0,9.996,0H9.313C5.965,0,3.252,2.715,3.252,6.063v30.982";
  ptr += "C1.261,38.825,0,41.403,0,44.286c0,5.367,4.351,9.718,9.719,9.718c5.368,0,9.719-4.351,9.719-9.718";
  ptr += "c0-2.943-1.312-5.574-3.378-7.355V18.436h-3.914v-2h3.914v-2.808h-4.084v-2h4.084V8.82H11.976z M15.302,44.833";
  ptr += "c0,3.083-2.5,5.583-5.583,5.583s-5.583-2.5-5.583-5.583c0-2.279,1.368-4.236,3.326-5.104V24.257C7.462,23.01,8.472,22,9.719,22";
  ptr += "s2.257,1.01,2.257,2.257V39.73C13.934,40.597,15.302,42.554,15.302,44.833z'fill=#1abc9c /></g></svg>";
  ptr += "</div>";
  ptr += "<div class='side-by-side text'>Temperature Eau</div>";
  ptr += "<div class='side-by-side reading'>";
  ptr += pressure;
  ptr += "<span class='superscript'>&deg;C</span></div>";
  ptr += "</div>";
  ptr += "<div class='data humidity'>";
  ptr += "<div class='side-by-side icon'>";
  ptr += "<svg enable-background='new 0 0 29.235 40.64'height=40.64px id=Layer_1 version=1.1 viewBox='0 0 29.235 40.64'width=29.235px x=0px xml:space=preserve xmlns=http://www.w3.org/2000/svg xmlns:xlink=http://www.w3.org/1999/xlink y=0px><path d='M14.618,0C14.618,0,0,17.95,0,26.022C0,34.096,6.544,40.64,14.618,40.64s14.617-6.544,14.617-14.617";
  ptr += "C29.235,17.95,14.618,0,14.618,0z M13.667,37.135c-5.604,0-10.162-4.56-10.162-10.162c0-0.787,0.638-1.426,1.426-1.426";
  ptr += "c0.787,0,1.425,0.639,1.425,1.426c0,4.031,3.28,7.312,7.311,7.312c0.787,0,1.425,0.638,1.425,1.425";
  ptr += "C15.093,36.497,14.455,37.135,13.667,37.135z'fill=#3C97D3 /></svg>";
  ptr += "</div>";
  ptr += "<div class='side-by-side text'>Humidity</div>";
  ptr += "<div class='side-by-side reading'>";
  ptr += humidity;
  ptr += "<span class='superscript'>%</span></div>";
  ptr += "</div>";
  ptr += "<div class='data altitude'>";
  ptr += "<div class='side-by-side icon'>";
  ptr += "<svg version='1.1' id='Capa_1' xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' x='0px' y='0px'";
  ptr += "viewBox='0 0 317.324 317.324' style='enable-background:new 0 0 317.324 317.324;' xml:space='preserve' fill='#955BA5 ' width='45px' height='45px'>";
  ptr += "<path d='M292.495,259.708L212.54,138.973V67.885h5.872c6.903,0,12.5-5.597,12.5-12.5V12.5c0-6.903-5.597-12.5-12.5-12.5h-119.5";
  ptr += "c-6.903,0-12.5,5.597-12.5,12.5v42.885c0,6.903,5.597,12.5,12.5,12.5h5.872v71.088L24.829,259.708";
  ptr += "c-7.56,11.416-8.231,26.001-1.752,38.061c6.479,12.062,19.009,19.555,32.701,19.555h205.767c13.692,0,26.223-7.493,32.701-19.554";
  ptr += "C300.727,285.709,300.056,271.124,292.495,259.708z M111.412,25h94.5v17.885c-8.475,0-84.209,0-94.5,0V25z M187.54,67.885v74.851";
  ptr += "c0,2.455,0.723,4.855,2.078,6.901l12.712,19.196h-57.336l-58.543,88.401c-3.811,5.754-11.564,7.333-17.323,3.52";
  ptr += "c-5.756-3.812-7.332-11.567-3.521-17.323l62.099-93.795c1.356-2.046,2.078-4.446,2.078-6.901V67.885H187.54z'/>";
  ptr += "</div>";
  ptr += "<div class='side-by-side text'>TDS</div>";
  ptr += "<div class='side-by-side reading'>";
  ptr += (int) altitude;
  ptr += "<span class='superscript'>ppm</span></div>";
  ptr += "</div>";
  ptr += "<div class='data tbox'>";
  ptr += "<div class='side-by-side icon'>";
  ptr += "<svg version='1.1' xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' x='0px' y='0px' width='55px' height='55px' viewBox='0 0 1000 1000' enable-background='new 0 0 1000 1000' xml:space='preserve'>";
  ptr += "<g><g transform='translate(0.000000,511.000000) scale(0.100000,-0.100000)'><path fill='#797E83' d='M3594.5,4973.6c-246.1-84.1-397.8-219.4-510.6-455.2c-57.4-121-63.6-151.7-63.6-338.3s6.1-217.4,63.6-338.4c141.5-295.3,393.7-469.6,703.4-488c178.4-10.3,342.5,26.7,486,110.7c96.4,57.4,262.5,225.6,299.4,305.5l22.5,51.3h387.6h385.5v-725.9v-725.9l-75.9-26.7c-176.3-57.4-299.4-168.1-393.7-356.8c-61.5-121-63.6-129.2-63.6-434.7v-313.7H3775c-1140.1,0-1203.7-6.2-1462.1-108.7c-194.8-80-342.5-176.4-508.5-332.2c-291.2-272.7-469.6-590.6-572.1-1015.1c-24.6-104.6-32.8-301.4-41-908.4l-10.3-773.1h709.5h709.5v543.4c0,498.3,4.1,551.6,43.1,656.2c80,213.3,305.5,379.4,629.5,463.4c149.7,36.9,254.3,39,2961.1,39h2805.3v717.7v717.7l-906.4,4.1l-908.4,6.2l-10.3,338.3c-10.2,297.3-16.4,350.7-59.5,434.7c-82,168.1-215.3,278.9-401.9,334.2l-82,24.6v715.7v715.7l399.9,6.2l399.9,4.1l39-71.8c59.5-108.7,244-272.7,375.3-334.2c104.6-47.2,143.5-55.4,336.3-55.4c209.2,0,221.5,2.1,371.2,77.9c186.6,90.2,299.4,209.2,393.7,404c57.4,121,63.6,151.7,63.6,338.4s-6.1,217.4-63.6,338.3c-94.3,194.8-207.1,313.7-393.7,404c-149.7,73.8-164.1,77.9-360.9,77.9c-166.1-2.1-229.7-10.3-326-49.2c-133.3-53.3-332.2-223.5-393.7-338.3l-41-73.8h-420.4l-420.4,2.1l-71.8,112.8c-75.9,121-221.5,233.8-356.8,274.8c-114.8,36.9-350.6,16.4-457.3-36.9c-121-61.5-223.5-157.9-285-262.5l-53.3-90.2h-403.9h-406l-22.5,51.3c-36.9,80-203,248.1-299.4,305.5C4078.5,5010.5,3793.4,5043.3,3594.5,4973.6z'/><path fill='#2FE5A8' d='M1584.9-2427.1c-252.2-342.5-465.5-756.7-578.3-1134c-43.1-139.4-53.3-219.4-55.4-399.9c-2.1-207.1,2-235.8,57.4-352.7c166.1-360.9,611.1-553.7,1027.4-447c346.6,90.2,572.1,342.5,615.2,689c47.2,385.5-231.7,1088.9-670.6,1685.6c-96.4,129.2-106.6,137.4-182.5,137.4C1720.2-2248.7,1712-2254.9,1584.9-2427.1z' /></g></g>";
  ptr += "</svg>";
  ptr += "</div>";
  ptr += "<div class='side-by-side text'>Pompes</div>";
  ptr += "<div class='side-by-side'>";
  ptr += "<a href=../pompeon><input  class='button' type=button value='On' onclick=\"return confirm(\'Are you sure ?\');\"></a>";
  ptr += "<a href=../pompeoff><input class='button' type=button value='Off' onclick=\"return confirm(\'Are you sure ?\');\"></a>";
  ptr += "</div>";
  ptr += "<div class='data tbox'>";
  ptr += "<div class='side-by-side icon'>";
  ptr += "<svg version='1.1' id='Layer_1' xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' x='-10px' y='0px'";
  ptr += "width='75px' height='75px' viewBox='0 0 100 100' enable-background='new 0 0 100 100' xml:space='preserve'><g><path fill='#FFE75A' d='M49.958,28.688c-9.278,0-16.804,7.525-16.804,16.81c0,3.056,0.857,5.898,2.24,8.391";
  ptr += "c4.126,7.438,5.221,11.613,5.896,15.654c0.564,3.387,1.344,4.174,3.902,4.174c1.091,0,2.869,0,4.766,0c1.901,0,3.683,0,4.767,0";
  ptr += "c2.563,0,3.344-0.787,3.907-4.174c0.677-4.043,1.771-8.219,5.896-15.654c1.388-2.492,2.24-5.334,2.24-8.391";
  ptr += "C66.771,36.212,59.245,28.688,49.958,28.688z'/><path fill='#FFE75A' d='M55.971,74.934H43.958c-0.5,0-0.903,0.402-0.903,0.9v0.602c0,0.496,0.403,0.9,0.903,0.9h12.013";
  ptr += "c0.493,0,0.897-0.404,0.897-0.9v-0.602C56.87,75.336,56.464,74.934,55.971,74.934z'/><path fill='#FFE75A' d='M55.971,78.537H43.958c-0.5,0-0.903,0.4-0.903,0.9v0.602c0,0.496,0.403,0.898,0.903,0.898h12.013";
  ptr += "c0.493,0,0.897-0.402,0.897-0.898v-0.602C56.87,78.938,56.464,78.537,55.971,78.537z'/><path fill='#FFE75A' d='M55.971,82.141H43.958c-0.5,0-0.903,0.402-0.903,0.9v0.602c0,0.498,0.403,0.9,0.903,0.9h12.013";
  ptr += "c0.493,0,0.897-0.402,0.897-0.9v-0.602C56.87,82.543,56.464,82.141,55.971,82.141z'/><path fill='#FFE75A' d='M45.458,85.744h9.013c0,1.801-2.406,3.002-4.513,3.002C47.861,88.746,45.458,87.545,45.458,85.744z'/><path fill='#008000' d='M69.264,16.854c1.532-2.683-2.605-5.092-4.15-2.414c-0.809,1.399-6.662,11.541-6.662,11.541l4.162,2.402";
  ptr += "c0,0,5.199-9.017,6.646-11.518c0.003-0.001,0.003-0.003,0.003-0.003L69.264,16.854L69.264,16.854z'/><path fill='#FF0000' d='M78.633,26.247L78.633,26.247L78.63,26.25c-0.008,0.001-0.008,0.001-0.008,0.001";
  ptr += "C76.51,27.472,67.456,32.7,67.456,32.7l2.397,4.16c0,0,9.076-5.239,11.188-6.458C83.727,28.854,81.316,24.713,78.633,26.247z'/><path fill='#FFE75A' d='M52.452,11.071c0,0.729,0,12.812,0,12.812h-4.806c0,0,0-12.084,0-12.812";
  ptr += "C47.646,7.973,52.452,7.973,52.452,11.071z'/><path fill='#FF0000' d='M30.737,16.854c-1.532-2.683,2.606-5.092,4.152-2.414c0.808,1.399,6.661,11.541,6.661,11.541l-4.158,2.402";
  ptr += "c0,0-5.208-9.017-6.652-11.518c0-0.001,0-0.003,0-0.003L30.737,16.854L30.737,16.854z'/><path fill='#008000' d='M21.371,26.247L21.371,26.247v0.003c0.007,0.001,0.007,0.001,0.007,0.001";
  ptr += "c2.116,1.221,11.167,6.449,11.167,6.449l-2.402,4.16c0,0-9.076-5.239-11.187-6.458C16.273,28.854,18.683,24.713,21.371,26.247/></g></svg>";
  ptr += "</div>";
  ptr += "<div class='side-by-side text'></div>";
  ptr += "<div class='side-by-side'>";
  ptr += "<a href=../ledon><input class='button' type=button value='On' onclick=\"return confirm(\'Are you sure ?\');\"></a>";
  ptr += "<a href=../ledoff><input class='button' type=button value='Off' onclick=\"return confirm(\'Are you sure ?\');\"></a>";
  ptr += "</div>";
  ptr += "</div>";
  ptr += "</body>";
  ptr += "</html>";
  return ptr;
}


String errIMG() {
  String IMG = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'><style>body, html {height: 100%;margin: 0;}";
  IMG += ".bg {";
  /* The image used */
  IMG += "background-image: url('https://i.ibb.co/mbVhMv0/ui-design-trends-13.jpg');";
  /* Full height */
  IMG += "height: 100%;";
  /* Center and scale the image nicely */
  IMG += "background-position: center;background-repeat: no-repeat;background-size: cover;}</style></head><body><div class='bg'></div></body></html>";
  return IMG;
}

String Send1() {
  String t = "<!DOCTYPE html>";
  t += "<html><head><meta name='viewport' content='width=device-width, initial-scale=1'>";
  t += "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css'>";
  t += "<style>";
  t += "html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #444444; background:black;}";
  t += "body {margin:0;font-family:Arial}";
  t += ".topnav {overflow: hidden;background-color: #333;}";
  t += ".topnav a {float: left;display: block;color: #f2f2f2;text-align: center;padding: 14px 16px;text-decoration: none;font-size: 17px;}";
  t += ".active {background-color: #4CAF50;color: white;}";
  t += ".topnav .icon {display: none;}";
  t += ".dropdown {float: left;overflow: hidden;}";
  t += ".dropdown .dropbtn {font-size: 17px;    border: none;outline: none;color: white;padding: 14px 16px;background-color: inherit;font-family: inherit;margin: 0;}";
  t += ".dropdown-content {display: none;position: absolute;background-color: #f9f9f9;min-width: 160px;box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);z-index: 1;}";
  t += ".dropdown-content a {float: none;color: black;padding: 12px 16px;text-decoration: none;display: block;text-align: left;}";
  t += ".topnav a:hover, .dropdown:hover .dropbtn {background-color: #555;color: white;}";
  t += ".dropdown-content a:hover {background-color: #ddd;color: black;}";
  t += ".dropdown:hover .dropdown-content {display: block;}";
  t += "@media screen and (max-width: 600px) {.topnav a:not(:first-child), .dropdown .dropbtn {display: none;}";
  t += ".topnav a.icon {float: right;display: block;}}";
  t += "@media screen and (max-width: 600px) {";
  t += ".topnav.responsive {position: relative;}";
  t += ".topnav.responsive .icon {position: absolute;right: 0;top: 0;}";
  t += ".topnav.responsive a {float: none;display: block;text-align: left;}";
  t += ".topnav.responsive .dropdown {float: none;}";
  t += ".topnav.responsive .dropdown-content {position: relative;}";
  t += ".topnav.responsive .dropdown .dropbtn {display: block;width: 100%;text-align: left;}}";
  t += "</style></head><body>";
  t += "<div class='topnav' id='myTopnav'>";
  t += "<a href='../' class='active' >Home</a>";
  t += "<div class='dropdown'>";
  t += "<button class='dropbtn'> Panel V1 ";
  t += "<i class='fa fa-caret-down'></i>";
  t += "</button>";
  t += "<div class='dropdown-content'>";
  t += "<a href='../data'>Monitor</a>";
  t += "<a href='../ledtest'>Led state</a>";
  t += "<a href='../pompetest'>Pompe state</a>";
  t += "</div>";
  t += "</div>";
  t += "<a href='../formu'  >Set Water Temperature</a>";
  t += "<a href='../timer'>Set Timmer Light</a>";
  t += "<a href='/tds'>Set TDS</a>";
  t += "<a href='/ror'>Bonus</a>";
  t += "<a href='javascript:void(0);' style='font-size:15px;' class='icon' onclick='myFunction()'>&#9776;</a></div>";
  t += "<script>";
  t += "function myFunction() {";
  t += "var x = document.getElementById('myTopnav');";
  t += "if (x.className === 'topnav') {";
  t += "x.className += ' responsive';";
  t += "} else {";
  t += "x.className = 'topnav';";
  t += "}}</script></body>";
  t += "<center><div style='text-align:center;padding:1em 0;'><h2>Bienvenue</h2>";
  t += " <h4><a style='text-decoration:none;' href='https://www.zeitverschiebung.net/fr/city/2660718'><span style='color:gray;'>Heure actuelle</span><br />Fribourg, Suisse</a></h4><br> <iframe src='https://www.zeitverschiebung.net/clock-widget-iframe-v2?language=fr&size=small&timezone=Europe%2FZurich' width='100%' height='90' frameborder='0' seamless></iframe> ";
  t += "<iframe src='https://www.meteoblue.com/fr/meteo/widget/daily?geoloc=detect&days=5&tempunit=CELSIUS&windunit=KILOMETER_PER_HOUR&precipunit=MILLIMETER&coloured=coloured&pictoicon=0&pictoicon=1&maxtemperature=0&maxtemperature=1&mintemperature=0&mintemperature=1&windspeed=0&windgust=0&winddirection=0&uv=0&humidity=0&humidity=1&precipitation=0&precipitation=1&precipitationprobability=0&precipitationprobability=1&spot=0&pressure=0&layout=light'  frameborder='0' scrolling='NO' allowtransparency='true' sandbox='allow-same-origin allow-scripts allow-popups allow-popups-to-escape-sandbox' style='width: 270px; height: 302px'></iframe>"; //t += "<a href=../><input type=button value='Index'></a>";
  t += "<a href='http://en.dnstools.ch/show-my-ip.html' target='_blank' title='What is my IP address'><img src='http://en.dnstools.ch/out/1.gif' alt='What is my IP address' style='border:0'/></a><br></div>";
  //t += "<a href=../data><input type=button value='Monitoring'></a>";
  t += "</html></center>";
  return t;
}


String ledpage1() {

  String t = "<!DOCTYPE html>";
  t += "<html><head><meta name='viewport' content='width=device-width, initial-scale=1'>";
  t += "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css'>";
  t += "<style>";
  t += "html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #444444; background:black;}";
  t += "body {margin:0;font-family:Arial}";
  t += ".topnav {overflow: hidden;background-color: #333;}";
  t += ".topnav a {float: left;display: block;color: #f2f2f2;text-align: center;padding: 14px 16px;text-decoration: none;font-size: 17px;}";
  t += ".active {background-color: #4CAF50;color: white;}";
  t += ".topnav .icon {display: none;}";
  t += ".dropdown {float: left;overflow: hidden;}";
  t += ".dropdown .dropbtn {font-size: 17px;    border: none;outline: none;color: white;padding: 14px 16px;background-color: inherit;font-family: inherit;margin: 0;}";
  t += ".dropdown-content {display: none;position: absolute;background-color: #f9f9f9;min-width: 160px;box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);z-index: 1;}";
  t += ".dropdown-content a {float: none;color: black;padding: 12px 16px;text-decoration: none;display: block;text-align: left;}";
  t += ".topnav a:hover, .dropdown:hover .dropbtn {background-color: #555;color: white;}";
  t += ".dropdown-content a:hover {background-color: #ddd;color: black;}";
  t += ".dropdown:hover .dropdown-content {display: block;}";
  t += "@media screen and (max-width: 600px) {.topnav a:not(:first-child), .dropdown .dropbtn {display: none;}";
  t += ".topnav a.icon {float: right;display: block;}}";
  t += "@media screen and (max-width: 600px) {";
  t += ".topnav.responsive {position: relative;}";
  t += ".topnav.responsive .icon {position: absolute;right: 0;top: 0;}";
  t += ".topnav.responsive a {float: none;display: block;text-align: left;}";
  t += ".topnav.responsive .dropdown {float: none;}";
  t += ".topnav.responsive .dropdown-content {position: relative;}";
  t += ".topnav.responsive .dropdown .dropbtn {display: block;width: 100%;text-align: left;}}";
  t += "</style></head><body>";
  t += "<div class='topnav' id='myTopnav'>";
  t += "<a href='../'  >Home</a>";
  t += "<div class='dropdown'>";
  t += "<button class='dropbtn'> Panel V1 ";
  t += "<i class='fa fa-caret-down'></i>";
  t += "</button>";
  t += "<div class='dropdown-content'>";
  t += "<a href='../data'>Monitor</a>";
  t += "<a href='../ledtest' class='active' >Led state</a>";
  t += "<a href='../pompetest'>Pompe state</a>";
  t += "</div>";
  t += "</div>";
  t += "<a href='../formu'  >Set Water Temperature</a>";
  t += "<a href='../timer'>Set Timmer Light</a>";
  t += "<a href='/tds'>Set TDS</a>";
  t += "<a href='/ror'>Bonus</a>";
  t += "<a href='javascript:void(0);' style='font-size:15px;' class='icon' onclick='myFunction()'>&#9776;</a></div>";
  t += "<script>";
  t += "function myFunction() {";
  t += "var x = document.getElementById('myTopnav');";
  t += "if (x.className === 'topnav') {";
  t += "x.className += ' responsive';";
  t += "} else {";
  t += "x.className = 'topnav';";
  t += "}}</script></body>";
  t += "<center><h2>Led State ON</h2><img src='https://i.ibb.co/TB4Nfwp/03-A59-E8-D-9-D50-411-A-A07-F-BC12-E185-CD38.jpg'  width='300' height='300'><br>";
  t += "<a style='vertical:align' href=../ledoff><input type=button  value='Off' onclick=\"return confirm(\'Are you sure ?\');\"></a><br></html></center>";
  t += "</html>";
  return t;
}
String ledpage2() {
  String t = "<!DOCTYPE html>";
  t += "<html><head><meta name='viewport' content='width=device-width, initial-scale=1'>";
  t += "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css'>";
  t += "<style>";
  t += "html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #444444; background:black;}";
  t += "body {margin:0;font-family:Arial}";
  t += ".topnav {overflow: hidden;background-color: #333;}";
  t += ".topnav a {float: left;display: block;color: #f2f2f2;text-align: center;padding: 14px 16px;text-decoration: none;font-size: 17px;}";
  t += ".active {background-color: #4CAF50;color: white;}";
  t += ".topnav .icon {display: none;}";
  t += ".dropdown {float: left;overflow: hidden;}";
  t += ".dropdown .dropbtn {font-size: 17px;    border: none;outline: none;color: white;padding: 14px 16px;background-color: inherit;font-family: inherit;margin: 0;}";
  t += ".dropdown-content {display: none;position: absolute;background-color: #f9f9f9;min-width: 160px;box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);z-index: 1;}";
  t += ".dropdown-content a {float: none;color: black;padding: 12px 16px;text-decoration: none;display: block;text-align: left;}";
  t += ".topnav a:hover, .dropdown:hover .dropbtn {background-color: #555;color: white;}";
  t += ".dropdown-content a:hover {background-color: #ddd;color: black;}";
  t += ".dropdown:hover .dropdown-content {display: block;}";
  t += "@media screen and (max-width: 600px) {.topnav a:not(:first-child), .dropdown .dropbtn {display: none;}";
  t += ".topnav a.icon {float: right;display: block;}}";
  t += "@media screen and (max-width: 600px) {";
  t += ".topnav.responsive {position: relative;}";
  t += ".topnav.responsive .icon {position: absolute;right: 0;top: 0;}";
  t += ".topnav.responsive a {float: none;display: block;text-align: left;}";
  t += ".topnav.responsive .dropdown {float: none;}";
  t += ".topnav.responsive .dropdown-content {position: relative;}";
  t += ".topnav.responsive .dropdown .dropbtn {display: block;width: 100%;text-align: left;}}";
  t += "</style></head><body>";
  t += "<div class='topnav' id='myTopnav'>";
  t += "<a href='../' class='active' >Home</a>";
  t += "<div class='dropdown'>";
  t += "<button class='dropbtn'> Panel V1 ";
  t += "<i class='fa fa-caret-down'></i>";
  t += "</button>";
  t += "<div class='dropdown-content'>";
  t += "<a href='../data'>Monitor</a>";
  t += "<a href='../ledtest' class='active' >Led state</a>";
  t += "<a href='../pompetest'>Pompe state</a>";
  t += "</div>";
  t += "</div>";
  t += "<a href='../formu'  >Set Water Temperature</a>";
  t += "<a href='../timer'>Set Timmer Light</a>";
  t += "<a href='/tds'>Set TDS</a>";
  t += "<a href='/ror'>Bonus</a>";
  t += "<a href='javascript:void(0);' style='font-size:15px;' class='icon' onclick='myFunction()'>&#9776;</a></div>";
  t += "<script>";
  t += "function myFunction() {";
  t += "var x = document.getElementById('myTopnav');";
  t += "if (x.className === 'topnav') {";
  t += "x.className += ' responsive';";
  t += "} else {";
  t += "x.className = 'topnav';";
  t += "}}</script></body>";
  t += "<center><h2>Led State Off</h2><img src='https://i.ibb.co/yyFXCcj/702490-DE-13-FF-47-B4-89-BF-821-CB0-CF5371.jpg' align;center width='300' height='300'><br>";
  t += "<a style='vertical:align' href=../ledon><input type=button value='On' onclick=\"return confirm(\'Are you sure ?\');\"><br></center></html>";
  t += "</html>";
  return t;
}

String pompepage1() {
  String t = "<!DOCTYPE html>";
  t += "<html><head><meta name='viewport' content='width=device-width, initial-scale=1'>";
  t += "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css'>";
  t += "<style>";
  t += "html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #444444; background:black;}";
  t += "body {margin:0;font-family:Arial}";
  t += ".topnav {overflow: hidden;background-color: #333;}";
  t += ".topnav a {float: left;display: block;color: #f2f2f2;text-align: center;padding: 14px 16px;text-decoration: none;font-size: 17px;}";
  t += ".active {background-color: #4CAF50;color: white;}";
  t += ".topnav .icon {display: none;}";
  t += ".dropdown {float: left;overflow: hidden;}";
  t += ".dropdown .dropbtn {font-size: 17px;    border: none;outline: none;color: white;padding: 14px 16px;background-color: inherit;font-family: inherit;margin: 0;}";
  t += ".dropdown-content {display: none;position: absolute;background-color: #f9f9f9;min-width: 160px;box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);z-index: 1;}";
  t += ".dropdown-content a {float: none;color: black;padding: 12px 16px;text-decoration: none;display: block;text-align: left;}";
  t += ".topnav a:hover, .dropdown:hover .dropbtn {background-color: #555;color: white;}";
  t += ".dropdown-content a:hover {background-color: #ddd;color: black;}";
  t += ".dropdown:hover .dropdown-content {display: block;}";
  t += "@media screen and (max-width: 600px) {.topnav a:not(:first-child), .dropdown .dropbtn {display: none;}";
  t += ".topnav a.icon {float: right;display: block;}}";
  t += "@media screen and (max-width: 600px) {";
  t += ".topnav.responsive {position: relative;}";
  t += ".topnav.responsive .icon {position: absolute;right: 0;top: 0;}";
  t += ".topnav.responsive a {float: none;display: block;text-align: left;}";
  t += ".topnav.responsive .dropdown {float: none;}";
  t += ".topnav.responsive .dropdown-content {position: relative;}";
  t += ".topnav.responsive .dropdown .dropbtn {display: block;width: 100%;text-align: left;}}";
  t += "</style></head><body>";
  t += "<div class='topnav' id='myTopnav'>";
  t += "<a href='../' class='active' >Home</a>";
  t += "<div class='dropdown'>";
  t += "<button class='dropbtn'> Panel V1 ";
  t += "<i class='fa fa-caret-down'></i>";
  t += "</button>";
  t += "<div class='dropdown-content'>";
  t += "<a href='../data'>Monitor</a>";
  t += "<a href='../ledtest'>Led state</a>";
  t += "<a href='../pompetest' class='active' >Pompe state</a>";
  t += "</div>";
  t += "</div>";
  t += "<a href='../formu'  >Set Water Temperature</a>";
  t += "<a href='../timer'>Set Timmer Light</a>";
  t += "<a href='/tds'>Set TDS</a>";
  t += "<a href='/ror'>Bonus</a>";
  t += "<a href='javascript:void(0);' style='font-size:15px;' class='icon' onclick='myFunction()'>&#9776;</a></div>";
  t += "<script>";
  t += "function myFunction() {";
  t += "var x = document.getElementById('myTopnav');";
  t += "if (x.className === 'topnav') {";
  t += "x.className += ' responsive';";
  t += "} else {";
  t += "x.className = 'topnav';";
  t += "}}</script></body>";
  t += "<center><h2>Pompe State ON</h2><img src='https://i.ibb.co/cTh1r3z/40813-E9-D-2278-4-B9-D-9593-E0181221-C103.jpg'  width='300' height='250'><br>";
  t += "<a style='vertical:align' href=../pompeoff><input type=button value='Off' onclick=\"return confirm(\'Are you sure ?\');\"></a></html></center><br>";
  t += "</html>";
  return t;
}
String pompepage2() {
  String t = "<!DOCTYPE html>";
  t += "<html><head><meta name='viewport' content='width=device-width, initial-scale=1'>";
  t += "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css'>";
  t += "<style>";
  t += "html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #444444; background:black;}";
  t += "body {margin:0;font-family:Arial}";
  t += ".topnav {overflow: hidden;background-color: #333;}";
  t += ".topnav a {float: left;display: block;color: #f2f2f2;text-align: center;padding: 14px 16px;text-decoration: none;font-size: 17px;}";
  t += ".active {background-color: #4CAF50;color: white;}";
  t += ".topnav .icon {display: none;}";
  t += ".dropdown {float: left;overflow: hidden;}";
  t += ".dropdown .dropbtn {font-size: 17px;    border: none;outline: none;color: white;padding: 14px 16px;background-color: inherit;font-family: inherit;margin: 0;}";
  t += ".dropdown-content {display: none;position: absolute;background-color: #f9f9f9;min-width: 160px;box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);z-index: 1;}";
  t += ".dropdown-content a {float: none;color: black;padding: 12px 16px;text-decoration: none;display: block;text-align: left;}";
  t += ".topnav a:hover, .dropdown:hover .dropbtn {background-color: #555;color: white;}";
  t += ".dropdown-content a:hover {background-color: #ddd;color: black;}";
  t += ".dropdown:hover .dropdown-content {display: block;}";
  t += "@media screen and (max-width: 600px) {.topnav a:not(:first-child), .dropdown .dropbtn {display: none;}";
  t += ".topnav a.icon {float: right;display: block;}}";
  t += "@media screen and (max-width: 600px) {";
  t += ".topnav.responsive {position: relative;}";
  t += ".topnav.responsive .icon {position: absolute;right: 0;top: 0;}";
  t += ".topnav.responsive a {float: none;display: block;text-align: left;}";
  t += ".topnav.responsive .dropdown {float: none;}";
  t += ".topnav.responsive .dropdown-content {position: relative;}";
  t += ".topnav.responsive .dropdown .dropbtn {display: block;width: 100%;text-align: left;}}";
  t += "</style></head><body>";
  t += "<div class='topnav' id='myTopnav'>";
  t += "<a href='../' class='active' >Home</a>";
  t += "<div class='dropdown'>";
  t += "<button class='dropbtn'> Panel V1 ";
  t += "<i class='fa fa-caret-down'></i>";
  t += "</button>";
  t += "<div class='dropdown-content'>";
  t += "<a href='../data'>Monitor</a>";
  t += "<a href='../ledtest'>Led state</a>";
  t += "<a href='../pompetest' class='active'>Pompe state</a>";
  t += "</div>";
  t += "</div>";
  t += "<a href='../formu'  >Set Water Temperature</a>";
  t += "<a href='../timer'>Set Timmer Light</a>";
  t += "<a href='/tds'>Set TDS</a>";
  t += "<a href='/ror'>Bonus</a>";
  t += "<a href='javascript:void(0);' style='font-size:15px;' class='icon' onclick='myFunction()'>&#9776;</a></div>";
  t += "<script>";
  t += "function myFunction() {";
  t += "var x = document.getElementById('myTopnav');";
  t += "if (x.className === 'topnav') {";
  t += "x.className += ' responsive';";
  t += "} else {";
  t += "x.className = 'topnav';";
  t += "}}</script></body>";
  t += "<center><h2>Pompe State Off</h2><img src='https://i.ibb.co/JzyF8RF/3-DA367-B6-0-AE4-47-FC-BCD2-54-F32-B5848-E5.jpg'  width='300' height='250'><br>";
  t += "<a style='vertical:align' href=../pompeon><input type=button value='On' onclick=\"return confirm(\'Are you sure ?\');\"></a></html></center><br>";
  t += "</html>";
  return t;
}

void arrosage() {
  String timearro = server.arg("arro");
  Serial.print("<");
  Serial.print("9");
  Serial.print(",");
  Serial.print(timearro.toInt());
  Serial.print(">");
  server.sendHeader("Location", "/formu");
  server.send(301);
}

void handle_NotFound() {
  server.send(404, "text/html", errIMG());
}

void ledoff() {
  if (!server.authenticate(www_username, www_password))
  {
    return server.requestAuthentication(DIGEST_AUTH, www_realm, errIMG());
  }
  Serial.println("<0>");
  ledstate = 0;
  server.sendHeader("Location", "/ledtest");
  server.send(303);

}
void ledon() {
  if (!server.authenticate(www_username, www_password))
  {
    return server.requestAuthentication(DIGEST_AUTH, www_realm, errIMG());
  }
  Serial.println("<1>");
  ledstate = 1;
  server.sendHeader("Location", "/ledtest");
  server.send(303);

}
void pompeon() {
  if (!server.authenticate(www_username, www_password))
  {
    return server.requestAuthentication(DIGEST_AUTH, www_realm, errIMG());
  }
  Serial.println("<4>");
  pompestate = 1;
  server.sendHeader("Location", "/pompetest");
  server.send(303);
}
void pompeoff() {
  if (!server.authenticate(www_username, www_password))
  {
    return server.requestAuthentication(DIGEST_AUTH, www_realm, errIMG());
  }
  Serial.println("<5>");
  pompestate = 0;
  server.sendHeader("Location", "/pompetest");
  server.send(303);
}

void ledtest() {
  if (!server.authenticate(www_username, www_password))
  {
    return server.requestAuthentication(DIGEST_AUTH, www_realm, errIMG());
  }
  if (ledstate == 1) {

    server.send(200, "text/html", ledpage1());
  }
  else if (ledstate == 0) {
    server.send(200, "text/html", ledpage2());
  }
  else {
    server.send(404, "text/html", errIMG());
  }

}

void pompetest() {
  if (!server.authenticate(www_username, www_password))
  {
    return server.requestAuthentication(DIGEST_AUTH, www_realm, errIMG());
  }
  if (pompestate == 1) {
    server.send(200, "text/html", pompepage1());
  }
  else if (pompestate == 0) {
    server.send(200, "text/html", pompepage2());
  }
  else {
    server.send(404, "text/html", errIMG());
  }
}

void monitoring() {
  if (!server.authenticate(www_username, www_password))
  {
    return server.requestAuthentication(DIGEST_AUTH, www_realm, errIMG());
  }
  Serial.println("<3>");
  delay(1);
  server.send(200, "text/html", SendHTML(tair, humi, teau, tds, tbox));
}



void handleIndex() {
  if (!server.authenticate(www_username, www_password))
  {
    return server.requestAuthentication(DIGEST_AUTH, www_realm, errIMG());
  }
  Serial.println("<3>");

  server.send(200, "text/html", Send1());
  //server.send(200, "text/html", sendtds(valtds));
  //server.send(200, "text/html", sendwater(timear, maxteau, minteau));
  //server.send(200, "text/html", sendtime(Onhour, Onmin, Offhour, Offmin));

}

void serialdata() {

  recvWithStartEndMarkers();
  if (newData == true) {
    strcpy(tempChars, receivedChars);
    // this temporary copy is necessary to protect the original data
    //   because strtok() used in parseData() replaces the commas with \0
    parseData();

    newData = false;
  }
}

//============

void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }
      else {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

//============


void parseData() {      // split the data into its parts

  char * strtokIndx; // this is used by strtok() as an index

  // strtokIndx = strtok(tempChars, ",");     // get the first part - the string
  //strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC

  strtokIndx = strtok(tempChars, ","); // this continues where the previous call left off
  ledstate = atoi(strtokIndx);     // convert this part to an integer

  strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  pompestate = atoi(strtokIndx);     // convert this part to an integer

  strtokIndx = strtok(NULL, ",");
  tds = atoi(strtokIndx);     // convert this part to a float

  strtokIndx = strtok(NULL, ",");
  tair = atof(strtokIndx);     // convert this part to a float

  strtokIndx = strtok(NULL, ",");
  teau = atof(strtokIndx);     // convert this part to a float

  strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  humi = atof(strtokIndx);     // convert this part to an integer

  strtokIndx = strtok(NULL, ",");
  tbox = atoi(strtokIndx);

  strtokIndx = strtok(NULL, ",");
  valtds = atoi(strtokIndx);

  strtokIndx = strtok(NULL, ",");
  Onhour = atoi(strtokIndx);

  strtokIndx = strtok(NULL, ",");
  Onmin = atoi(strtokIndx);

  strtokIndx = strtok(NULL, ",");
  Offhour = atoi(strtokIndx);

  strtokIndx = strtok(NULL, ",");
  Offmin = atoi(strtokIndx);

  strtokIndx = strtok(NULL, ",");
  maxteau = atoi(strtokIndx);

  strtokIndx = strtok(NULL, ",");
  minteau = atoi(strtokIndx);

  strtokIndx = strtok(NULL, ",");
  timear = atoi(strtokIndx);


}
void setup() {

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Connect Failed! Rebooting...");
    delay(1000);
    ESP.restart();
  }
  
  ArduinoOTA.begin();
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/data", monitoring);
  server.on("/ledon", ledon);
  server.on("/ledoff", ledoff);
  server.on("/pompeon", pompeon);
  server.on("/pompeoff", pompeoff);
  server.on("/ledtest", ledtest);
  server.on("/pompetest", pompetest);
  server.on("/formu", formular);
  server.on("/timer", timmer);
  server.on("/tds", tdsval);
  server.on("/arro", arrosage);
  server.on("/", handleIndex);
  server.onNotFound(handle_NotFound);
  server.begin();
}

void loop()
{
  ArduinoOTA.handle();
  server.handleClient();
  serialdata();
}
