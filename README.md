# TemperatureThing
ESP8266 and NTC temperature website and logging
Using a LoLin NodeMCU ESP8266 module and a 10k NTC thermistor to display temperature readings in a website and log readings every 15 minutes to a MySQL database in the cloud with a PHP website to display.

The ESP8266 uses AJAX style javascript to take advantage of client side processing and the main report website uses php to reduce overhead on any mobile client user. I feel this is the right way round to do things as I dislike client side scripting on mobile browsers.

Info links for libraries etc:
Setting up ESP8266 with Arduino info is here: http://arduino-esp8266.readthedocs.io/en/latest/installing.html

The AJAX website design is here: http://www.esp8266.com/viewtopic.php?f=8&t=4307

The mail library is here: https://github.com/gpepe/esp8266-sendemail

NTP time library is here: https://github.com/PaulStoffregen/Time

Voltage divider formula: https://en.wikipedia.org/wiki/Voltage_divider

The arduino code is commented but a brief description is that it starts and sets up an AJAX website to display temperatures and time stamps. It displays current minimum and maximum temperatures with their respective times.

Temperature is calculated by working out the reistance value for the NTC and looking up in a table to get the temperature in centigrade. The NTC I bought had a table to 4 decimal places for average resistance so I rounded it to 2 and used that.
At 25degC the NTC is 10kohm and the resistor in the voltage divider is 10kohm 1%. 
Connection is:

3.3v<br>
   !<br>
10k<br>
   !<br>
input<br>
   !<br>
NTC<br>
   !<br>
GND
              
Every 15 minutes it logs the temperature and time to a MySQL database.
Just before midnight it emails out the days instantaneous readings.
It then reboots as a housekeeping event and starts logging again.

Readings in temp allow for -10 to 125degC. 

The website files are:
index.php which is the start page. It displays today's readings from midnight in 15 minute intervals.
month.html which is the form for selecting the month you want to review.
month.php is called by the form action and displays the min and max each day for the selected month.
inserto.php is the page the arduino uses to load the MySQL with readings.

Assumption is that you have the MySQL installation on your webserver. Daylight savings requires arduino to be updated as well as index.php.

Database structure is:<br>
Source, varchar(25)<br>
Temperature, int(11)<br>
TimeStamp, datetime<br>
No primary key is set as Source can be used for another input ESP8266 sensor which may be using same time intervals.

First full system project in Arduino and also first sharing on github.
