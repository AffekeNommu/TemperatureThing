# TemperatureThing
ESP8266 and NTC temperature website and logging
Using a LoLin NodeMCU ESP8266 module and a 10k NTC thermistor to display temperature readings in a website and email a summary at the end of the day.

Info links for libraries etc:
Setting up ESP8266 with Arduino info is here: http://arduino-esp8266.readthedocs.io/en/latest/installing.html
The AJAX website design is here: http://www.esp8266.com/viewtopic.php?f=8&t=4307
The mail library is here: https://github.com/gpepe/esp8266-sendemail
NTP time library is here: https://github.com/PaulStoffregen/Time
Voltage divider formula: https://en.wikipedia.org/wiki/Voltage_divider

The code is commented but a brief description is that it starts and sets up an AJAX website to display temperatures and time stamps. It displays current minimum and maximum.
Temperature is calculated by working out the reistance value for the NTC and looking up in a table to get the temperature in centigrade. The NTC I bought had a table to 4 decimal places for average resistance so I rounded it to 2 and used that.
Every 15 minutes it logs the temperature and time to arrays.
Just before midnight it emails out the days readings as a CSV in the text of the email.
It then resets the readings and starts logging again.

There is a limitation in the amount of data you can put in the email but the data type used for the message body will happily hold 100 readings.

First full system project in Arduino and also first sharing on github.
