# Esp8266 Covid Ticker mit i2c SSD1306
# Bleibt zuhause

SCL D1
SDA D2
Display ID 0x3c


(Bilder folgen)

 1- Wlan Zugangsdaten in **WifiConnect.h** anpassen
  `char ssid[32] = "yourssid";`
  `char password[64] = "yourpass";`

 2- Land auswählen in **corona.ino** 
   `#define country_code "yourcountrycode"`
   
   GET (https://coronavirus-19-api.herokuapp.com/countries) -> all countries info
   
 3- Ich hoffe für alle das ich die Datenfelder viel zu gross gewählt habe.
    Die Uhr läuft mit deutscher Sommerzeit. Auch da hoffe ich das das reicht ...
    Bleibt alle gesund
 

Überarbeitete Version für das SSD1306 Display.
