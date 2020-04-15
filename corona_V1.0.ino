#include <SPI.h>
#include <Wire.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include "json_parser.h"
#include "WifiConnect.h"
#include <Adafruit_SSD1306.h>
#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 7200, 60000);
#define s2ms(second) (second*1000)
unsigned long long prev_millis(0);

#define country_code "Germany"

int interval = s2ms(60);
unsigned long long PreviousMillis = 0;
unsigned long long CurrentMillis = interval;
bool bFirstKickMillis = false;

extern bool bGotIpFlag;
static String build_url_from_country(String country)
{
  String url = "http://coronavirus-19-api.herokuapp.com/countries/";
  url = url + country;
  return url;
}


void setup(void)
{
  timeClient.begin();
#if defined JSON_DEBUG
  Serial.begin(9600);
#endif

  JSON_LOG("Connecting...");
  vConnWifiNetworkViaSdk();
}

void loop()
{
  if (bGotIpFlag) bGotIp();
  if (bFirstKickMillis) CurrentMillis = millis();
  if (!bGotIpFlag && CurrentMillis - PreviousMillis >= interval)
  {
    if (!bFirstKickMillis) CurrentMillis = 0;
    bFirstKickMillis = true;
    PreviousMillis = CurrentMillis;
    HTTPClient http;
    http.begin(build_url_from_country(country_code));
    int httpCode = http.GET();
    if (httpCode > 0)
    {
      String payload = http.getString();
      char* JsonArray = (char *)malloc(payload.length() + 1);
      if (!JsonArray) JSON_LOG("upssss fuck");
      payload.toCharArray(JsonArray, payload.length() + 1);

      JSON_LOG(JsonArray);
      timeClient.update();
      if (json_validate(JsonArray))
      {
        int cases = (int)get_json_value(JsonArray, "cases", INT);
        int todayCases = (int)get_json_value(JsonArray, "todayCases", INT);
        int deaths = (int)get_json_value(JsonArray, "deaths", INT);
        int todayDeaths = (int)get_json_value(JsonArray, "todayDeaths", INT);
        int recovered = (int)get_json_value(JsonArray, "recovered", INT);
        int active1 = (int)get_json_value(JsonArray, "active", INT);
        int critical = (int)get_json_value(JsonArray, "critical", INT);


//Display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  #define DRAW_DELAY 118
  #define D_NUM 47
  display.clearDisplay();
  display.setRotation(2); // rotate 90 degrees counter clockwise
  //Textgroesse setzen
  display.setTextSize(1);
  //Textfarbe setzen
  display.setTextColor(WHITE);

  
  display.setCursor(0,0);
  display.println("Alle");

  display.setCursor(30,0);
  display.println(cases);

  display.setCursor(70,0);
  display.println("+");

  display.setCursor(90,0);
  display.println(todayCases);

  display.setCursor(0,8);
  display.println("Tote");

  display.setCursor(30,8);
  display.println(deaths);

  display.setCursor(70,8);
  display.println("+");

  display.setCursor(90,8);
  display.println(todayDeaths);

  display.setCursor(0,16);
  display.println("Akut");

  display.setCursor(30,16);
  display.println(active1);

  display.setCursor(70,16);
  display.println("IC");

  display.setCursor(90,16);
  display.println(critical);

  display.setCursor(0,24);
  display.println("OK");

  display.setCursor(30,24);
  display.println(recovered);

   display.setCursor(100,24);
  display.println("V1.0");


  display.display();
  display.clearDisplay();


      }

      free(JsonArray);
    }

    http.end();
  }  
}
