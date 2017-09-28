#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <sendemail.h>

//global variables
static const char ntpServerName[] = "us.pool.ntp.org";  
const int timeZone = 10;  
ESP8266WebServer server(80);  
const char* ssid = "my SSID";  
const char* password = "my password";  
String webSite, javaScript, XML;  
WiFiUDP Udp;  
unsigned int localPort = 8888;  
time_t timelast = 0;  
time_t timetest, timenow, timemax, timemin;  
int templast, tempnow, tempmax, tempmin;  
time_t times[100];//96 is one reading every 15 minutes  
int temps[100];  
int arridx = 0;

//build the html component of the site
void buildWebsite() {  
  buildJavascript();
  webSite = "<!DOCTYPE HTML>\n";
  webSite += javaScript;
  webSite += "<BODY onload='process()'>\n";
  webSite += "<b>Temperature Thing</b></p>";
  webSite += "<table stype=\"width:100%\" border=\"1\">";
  webSite += "<tr>";
  webSite += "<th>Reading</th>";
  webSite += "<th>Temp</th>";
  webSite += "<th>Time</th>";
  webSite += "</tr>";
  webSite += "<tr>";
  webSite += "<td>Now</td>";
  webSite += "<td>";
  webSite += "<A id='tempnow'></A>\n";
  webSite += "degC";
  webSite += "<td>";
  webSite += "<A id='timenow'></A>\n";
  webSite += "</td>";
  webSite += "</tr>";
  webSite += "<tr>";
  webSite += "<td>Max</td>";
  webSite += "<td>";
  webSite += "<A id='tempmax'></A>\n";
  webSite += "degC";
  webSite += "<td>";
  webSite += "<A id='timemax'></A>\n";
  webSite += "</td>";
  webSite += "</tr>";
  webSite += "<tr>";
  webSite += "<td>Min</td>";
  webSite += "<td>";
  webSite += "<A id='tempmin'></A>\n";
  webSite += "degC";
  webSite += "</td>";
  webSite += "<td>";
  webSite += "<A id='timemin'></A>\n";
  webSite += "</td>";
  webSite += "</tr>";
  webSite += "</table>";
  webSite += "</BODY>\n";
  webSite += "</HTML>\n";
}

//build the javascript component of the website
void buildJavascript() {  
  javaScript = "<SCRIPT>\n";
  javaScript += "var xmlHttp=createXmlHttpObject();\n";

  javaScript += "function createXmlHttpObject(){\n";
  javaScript += " if(window.XMLHttpRequest){\n";
  javaScript += "    xmlHttp=new XMLHttpRequest();\n";
  javaScript += " }else{\n";
  javaScript += "    xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');\n";
  javaScript += " }\n";
  javaScript += " return xmlHttp;\n";
  javaScript += "}\n";

  javaScript += "function process(){\n";
  javaScript += " if(xmlHttp.readyState==0 || xmlHttp.readyState==4){\n";
  javaScript += "   xmlHttp.open('PUT','xml',true);\n";
  javaScript += "   xmlHttp.onreadystatechange=handleServerResponse;\n"; // no brackets?????
  javaScript += "   xmlHttp.send(null);\n";
  javaScript += " }\n";
  javaScript += " setTimeout('process()',1000);\n";
  javaScript += "}\n";

  javaScript += "function handleServerResponse(){\n";
  javaScript += " if(xmlHttp.readyState==4 && xmlHttp.status==200){\n";
  javaScript += "   xmlResponse=xmlHttp.responseXML;\n";

  javaScript += "   xmldoc = xmlResponse.getElementsByTagName('tempnow');\n";
  javaScript += "   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript += "   document.getElementById('tempnow').innerHTML=message;\n";

  javaScript += "   xmldoc = xmlResponse.getElementsByTagName('timenow');\n";
  javaScript += "   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript += "   document.getElementById('timenow').innerHTML=message;\n";

  javaScript += "   xmldoc = xmlResponse.getElementsByTagName('tempmax');\n";
  javaScript += "   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript += "   document.getElementById('tempmax').innerHTML=message;\n";

  javaScript += "   xmldoc = xmlResponse.getElementsByTagName('timemax');\n";
  javaScript += "   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript += "   document.getElementById('timemax').innerHTML=message;\n";

  javaScript += "   xmldoc = xmlResponse.getElementsByTagName('tempmin');\n";
  javaScript += "   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript += "   document.getElementById('tempmin').innerHTML=message;\n";

  javaScript += "   xmldoc = xmlResponse.getElementsByTagName('timemin');\n";
  javaScript += "   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript += "   document.getElementById('timemin').innerHTML=message;\n";

  javaScript += " }\n";
  javaScript += "}\n";
  javaScript += "</SCRIPT>\n";
}

//build the xml component of the website
void buildXML() {  
  XML = "<?xml version='1.0'?>";
  XML += "<Stats>";
  XML += "<tempnow>";
  XML += tempnow;
  XML += "</tempnow>";
  XML += "<timenow>";
  XML += formattime(timenow);
  XML += "</timenow>";
  XML += "<tempmax>";
  XML += tempmax;
  XML += "</tempmax>";
  XML += "<timemax>";
  XML += formattime(timemax);
  XML += "</timemax>";
  XML += "<tempmin>";
  XML += tempmin;
  XML += "</tempmin>";
  XML += "<timemin>";
  XML += formattime(timemin);
  XML += "</timemin>";
  XML += "</Stats>";
}

//website handler
void handleWebsite() {  
  buildWebsite();
  server.send(200, "text/html", webSite);
}

//xml handler
void handleXML() {  
  buildXML();
  server.send(200, "text/xml", XML);
}

//Match the index temp in an array of resistance values - these were supplied by the NTC thermistor manufacturer and the average column was used
int gettemp(float res) {  
  float temp[] = {
    51.82,//-10
    49.28,
    46.89,
    44.62,
    42.48,
    40.45,
    38.53,
    36.70,
    34.97,
    33.33,
    31.77, //0
    30.25,
    28.82,
    27.45,
    26.16,
    24.94,
    23.77,
    22.67,
    21.62,
    20.63,
    19.68,
    18.78,
    17.93,
    17.12,
    16.35,
    15.62,
    14.93,
    14.26,
    13.63,
    13.04,
    12.47,
    11.92,
    11.41,
    10.91,
    10.45,
    10.00, //25
    9.57,
    9.17,
    8.78,
    8.42,
    8.06,
    7.73,
    7.41,
    7.11,
    6.81,
    6.54,
    6.27,
    6.02,
    5.78,
    5.55,
    5.33,
    5.12,
    4.92,
    4.72,
    4.54,
    4.36,
    4.20,
    4.03,
    3.88,
    3.73,
    3.59, //50
    3.46,
    3.33,
    3.20,
    3.09,
    2.97,
    2.86,
    2.76,
    2.66,
    2.56,
    2.47,
    2.38,
    2.30,
    2.22,
    2.14,
    2.07,
    1.99,
    1.93,
    1.86,
    1.80,
    1.74,
    1.68,
    1.62,
    1.57,
    1.51,
    1.46, //75
    1.42,
    1.37,
    1.33,
    1.28,
    1.24,
    1.20,
    1.16,
    1.13,
    1.09,
    1.06,
    1.03,
    1.00,
    0.97,
    0.94,
    0.91,
    0.88,
    0.86,
    0.83,
    0.81,
    0.78,
    0.76,
    0.74,
    0.72,
    0.69,
    0.67, //100
    0.66,
    0.64,
    0.62,
    0.60,
    0.59,
    0.57,
    0.55,
    0.54,
    0.52,
    0.51,
    0.49,
    0.48,
    0.47,
    0.45,
    0.44,
    0.43,
    0.42,
    0.41,
    0.39,
    0.38,
    0.37,
    0.36,
    0.35,
    0.34,
    0.33 //125
  };
  int i;
  float test;
  //Float array is sizeof/4
  for (i = 0; i < sizeof(temp) / 4; i++) {
    test = temp[i];
    if (res >= test) { //We have just gone past the value
      break;//leave loop
    }
  }
  return i - 11;
  //return index as temp with offset as index went past by 1 and we start at -10
}

//Read the analog port 20 times and average it and get the ADC value
int getraw () {  
  int sum = 0;
  int i;
  for (i = 0; i < 20; i++) {
    sum += analogRead(A0);
    delay(10);
  }
  sum = sum / 20;
  return sum;
  //we use this ADC value as a voltage index
}

//Calculate r2 where
//              1
// r2= r1 *  --------
//          (vi/vo-1)
//
// vi is 1024, R1=10k, vo is read from ADC
float getresist(int raw) {  
  float r2;
  r2 = (float)(10 * (1 / ((1024 / (float)raw) - 1)));
  return r2;
}

//NTP code to get time
const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message  
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()  
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)  
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

//show the time value as a readable format
String formattime(time_t in)  
{
  String timestamp = "";
  // nice formatted display of the time
  timestamp += year(in);
  timestamp += "-";
  if (month() <= 9) {
    timestamp += "0";
  }
  timestamp += month(in);
  timestamp += "-";
  if (day(in) <= 9) {
    timestamp += "0";
  }
  timestamp += day(in);
  timestamp += " ";
  if (hour(in) <= 9) {
    timestamp += "0";
  }
  timestamp += hour(in);
  timestamp += ":";
  if (minute(in) <= 9) {
    timestamp += "0";
  }
  timestamp += minute(in);
  timestamp += ":";
  if (second(in) <= 9) {
    timestamp += "0";
  }
  timestamp += second(in);
  //Serial.println(timestamp);
  return timestamp;
}

//send an email with minimum and maximum values followed by csv 10 minute readings for the day
void sendmail() {  
  String sub = "";
  String msg = "";
  int i;
  sub = "Temperature readings for the day\n";
  msg += "Minimum temp ";
  msg += tempmin;
  msg += " degC at ";
  msg += formattime(timemin);
  msg += "\nMaximum temp ";
  msg += tempmax;
  msg += " degC at ";
  msg += formattime(timemin);
  msg += "\n\n";
  for (i = 0; i < arridx; i++) {
    msg += temps[i];
    msg += ",";
    msg += formattime(times[i]);
    msg += "\n";
  }//watch out as the mail library has a message character limit
  SendEmail e("my.mail.server", 25, "", "", 5000, false);
  e.send("me@my.mail.server", "recipient@mail.com", sub, msg);
}

//Initialise all the junk
void setup() {  
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)delay(500);
  WiFi.mode(WIFI_STA);
  Serial.println("\n\nBOOTING ESP8266 ...");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("Station IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", handleWebsite);
  server.on("/xml", handleXML);
  server.begin();
  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(3600);
  //reset the min and max values and timestamps
  tempnow = gettemp(getresist(getraw()));
  tempmax = tempnow;
  tempmin = tempnow;
  templast = tempnow;
  timenow = now();
  timemax = timenow;
  timemin = timenow;
  timetest = timenow;
}

//main program loop
void loop() {  
  server.handleClient();
  if (timeStatus() != timeNotSet) {
    //update the display only if time has changed
    if (now() != timelast) {
      timelast = now();
      timenow = now();
    }
  }

  //if we get a new max we record it
  tempnow = gettemp(getresist(getraw()));
  if (tempnow > tempmax) {
    timemax = timenow;
    tempmax = tempnow;
  }

  //If we get a new min we record it
  if (tempnow < tempmin) {
    timemin = timenow;
    tempmin = tempnow;
  }

  //Reset the min and max for midnight and email readings for the day
  if (formattime(timenow).substring(11) == "23:59:59") {
    sendmail();//email the temps for the day
    tempmin = tempnow;
    tempmax = tempnow;
    timemin = timenow;
    timemax = timenow;
    timetest = timenow;
    arridx = 0;
  }

  //save the temp to the array every 15 minutes
  if (timenow >= timetest) {
    temps[arridx] = tempnow;
    times[arridx] = timenow;
    arridx++;
    if (arridx == 100) { //in case you shorten reading length and need to reset
      arridx = 0;
    }
    timetest = timenow + 900; //15 minutes is 900 seconds
  }
}
