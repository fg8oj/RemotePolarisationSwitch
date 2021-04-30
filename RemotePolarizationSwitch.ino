#include <M5Core2.h>
#include <multi_channel_relay.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include "WebServer.h"
#include <Preferences.h>
#include "time.h"
#include "Free_Fonts.h"
#include "index.html.h"

char* ntpServer =  "fr.pool.ntp.org";
int timeZone = 0;
int tcount = 0;
RTC_TimeTypeDef RTC_TimeStruct;
RTC_DateTypeDef RTC_DateStruct;
uint32_t secs = 0;
int screensaver=60;
int timer=screensaver;
uint16_t prevTime = millis();
const IPAddress apIP(192, 168, 4, 1);
const char* apSSID = "M5STACK_SETUP";
boolean settingMode;
String ssidList;
String wifi_ssid;
String wifi_password;

// DNSServer dnsServer;
WebServer webServer(80);

// wifi config store
Preferences preferences;

Multi_Channel_Relay relay;
int H2posx=4;
int H2posy=91;
int V2posx=4;
int V2posy=142;
int C2posx=4;
int C2posy=191;
int H7posx=163;
int H7posy=91;
int V7posx=163;
int V7posy=142;
int C7posx=163;
int C7posy=191;
char strC[]={"CIRC"};
char strV[]={"VERT"};
char strH[]={"HORI"};
String Str2m="HORI";
String Str70cm="HORI";
void DingDong() {
  
}

void setup(){
  M5.begin();
  DingDong();
  Serial.begin(9600);
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setCursor(0, 0, 2);
  M5.Lcd.setBrightness(10);
  M5.Lcd.setTextColor(TFT_YELLOW,TFT_BLACK);
  M5.Lcd.setFreeFont(FSB12);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(0 ,40,1);
  M5.Lcd.println("          Remote Polarization");
  M5.Lcd.println("             Antenna Switch");
  M5.Lcd.setCursor(58 ,120,1);
  M5.Lcd.setFreeFont(FSB24);
  M5.Lcd.print("F G 8 O J");
  M5.Lcd.setFreeFont(FSB9);
  
  preferences.begin("wifi-config");
  if (restoreConfig()) {
    if (checkConnection()) {
      settingMode = false; 
      startWebServer();
      timeSync();
    }
    initscreen();  
  } else {
    settingMode = true;
    setupMode();
  }
  
}

void initscreen() {
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.fillRect(0, 0, 320, 46,TFT_NAVY);
  M5.Lcd.fillRect(158, 45, 3, 195,TFT_YELLOW);
  M5.Lcd.fillRect(0, 45, 320, 45,TFT_YELLOW);
  M5.Lcd.fillRect(0, 140, 320, 3,TFT_YELLOW);
  M5.Lcd.fillRect(0, 190, 320, 3,TFT_YELLOW);
  M5.Lcd.fillRect(0, 237, 320, 3,TFT_YELLOW);
  M5.Lcd.fillRect(0, 45, 3, 195,TFT_YELLOW);
  M5.Lcd.fillRect(317, 45, 3, 240,TFT_YELLOW); 
  M5.Lcd.setTextColor(TFT_BLACK,TFT_YELLOW);
  M5.Lcd.setFreeFont(FSB18);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(60 ,80,1);
  M5.Lcd.println("2 m");
  M5.Lcd.setCursor(210 ,80,1);
  M5.Lcd.println("70cm");
  
  activate_button(H2posx,H2posy,strH);
  deactivate_button(V2posx,V2posy,strV);
  deactivate_button(C2posx,C2posy,strC);
  activate_button(H7posx,H7posy,strH);
  deactivate_button(V7posx,V7posy,strV);
  deactivate_button(C7posx,C7posy,strC);

  relay.begin(0x11);
  relay.turn_on_channel(1);
  relay.turn_off_channel(2);
  relay.turn_on_channel(3);
  relay.turn_off_channel(4);
  
}
void deactivate_button(int posx, int posy, char text_button[]  ) {
  M5.Lcd.fillRect(posx, posy, 152, 47,TFT_LIGHTGREY);
  M5.Lcd.setCursor(posx+15 ,posy+40,1);
  M5.Lcd.setFreeFont(FSB24);
  M5.Lcd.setTextColor(TFT_BLACK);
  M5.Lcd.println(text_button);   
}

void activate_button(int posx, int posy, char text_button[]  ) {
  M5.Lcd.fillRect(posx, posy, 152, 47,TFT_GREEN);
  M5.Lcd.setCursor(posx+15 ,posy+40,1);
  M5.Lcd.setFreeFont(FSB24);
  M5.Lcd.setTextColor(TFT_BLACK);
  M5.Lcd.println(text_button); 
}
void doTime() {
  if (timeToDo(1000)) {
    M5.Lcd.fillRect(0, 0, 320, 46,TFT_NAVY);
    M5.Lcd.setTextColor(TFT_WHITE,TFT_NAVY);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setFreeFont(FSSB18);
    M5.Rtc.GetTime(&RTC_TimeStruct);
    M5.Rtc.GetData(&RTC_DateStruct);
    M5.Lcd.setCursor(3, 35);
    M5.Lcd.printf("%02d/%02d/%04d\n",RTC_DateStruct.Date , RTC_DateStruct.Month, RTC_DateStruct.Year);
    M5.Lcd.setCursor(182, 35);
    M5.Lcd.printf("%02d:%02d:%02d\n", RTC_TimeStruct.Hours, RTC_TimeStruct.Minutes, RTC_TimeStruct.Seconds);
  }
}


//delays stopping usualy everything using this workarround
bool timeToDo(int tbase) {
  /*tcount++;
  if (tcount == tbase) {
    tcount = 0;
    return true;    
  } else {
    return false;
  } 
  */
    if (millis() / tbase > secs) {
    secs = millis() / tbase;
    timer--;
    return true;   
} else {
    return false;
  } 

}

void timeSync() {
    // Set ntp time to local
    configTime(timeZone, 0, ntpServer);
    // Get local time
    struct tm timeInfo;
    if (getLocalTime(&timeInfo)) {
      // Set RTC time
      RTC_TimeTypeDef TimeStruct;
      TimeStruct.Hours   = timeInfo.tm_hour;
      TimeStruct.Minutes = timeInfo.tm_min;
      TimeStruct.Seconds = timeInfo.tm_sec;
      M5.Rtc.SetTime(&TimeStruct);

      RTC_DateTypeDef DateStruct;
      DateStruct.WeekDay = timeInfo.tm_wday;
      DateStruct.Month = timeInfo.tm_mon + 1;
      DateStruct.Date = timeInfo.tm_mday;
      DateStruct.Year = timeInfo.tm_year + 1900;
      M5.Rtc.SetData(&DateStruct);
    }
}



void loop() {
  M5.update();
  webServer.handleClient();
  Event& e = M5.Buttons.event;
  doTime();
  if(timer <= 0) {
    M5.Lcd.writecommand(ILI9341_DISPOFF);
  }
  if (settingMode) {
  }
  if (e & (E_TOUCH | E_MOVE)&& (timer <= 0)) {
    M5.Axp.SetLDOEnable(3,true);
    delay(100);
    M5.Axp.SetLDOEnable(3,false); 
    M5.Lcd.writecommand(ILI9341_DISPON);
    timer=screensaver;
    //delay(100);
    return;
  }
  
  if (e & (E_TOUCH | E_MOVE) && timer > 0) {
 // if (e & (E_TOUCH | E_MOVE) ) {
  
    timer=screensaver;
    M5.Axp.SetLDOEnable(3,true);
    delay(100);
    M5.Axp.SetLDOEnable(3,false); 
    
    if (e.to.x>H2posx && e.to.x<H2posx+155 && e.to.y>H2posy && e.to.y<H2posy+56) {
      activate_button(H2posx,H2posy,strH);
      deactivate_button(V2posx,V2posy,strV);
      deactivate_button(C2posx,C2posy,strC);
      relay.turn_on_channel(1);
      relay.turn_off_channel(2);
      Str2m="HORI";  
    }
    if (e.to.x>V2posx && e.to.x<V2posx+155 && e.to.y>V2posy && e.to.y<V2posy+56) {
      deactivate_button(H2posx,H2posy,strH);
      activate_button(V2posx,V2posy,strV);
      deactivate_button(C2posx,C2posy,strC);
      relay.turn_off_channel(1);
      relay.turn_off_channel(2);
      DingDong();
      Str2m="VERT"; 
    }
    if (e.to.x>C2posx && e.to.x<C2posx+155 && e.to.y>C2posy && e.to.y<C2posy+56) {
      deactivate_button(H2posx,H2posy,strH);
      deactivate_button(V2posx,V2posy,strV);
      activate_button(C2posx,C2posy,strC);
      relay.turn_off_channel(1);
      relay.turn_on_channel(2);
      DingDong();
      Str2m="CIRC"; 
    }
    if (e.to.x>H7posx && e.to.x<H7posx+155 && e.to.y>H7posy && e.to.y<H7posy+56) {
      activate_button(H7posx,H7posy,strH);
      deactivate_button(V7posx,V7posy,strV);
      deactivate_button(C7posx,C7posy,strC);
      relay.turn_on_channel(3);
      relay.turn_off_channel(4);
      DingDong();
      Str70cm="HORI";
    }
    if (e.to.x>V7posx && e.to.x<V7posx+155 && e.to.y>V7posy && e.to.y<V7posy+56) {
      deactivate_button(H7posx,H7posy,strH);
      activate_button(V7posx,V7posy,strV);
      deactivate_button(C7posx,C7posy,strC);
      relay.turn_off_channel(3);
      relay.turn_off_channel(4);
      DingDong();
      Str70cm="VERT";
    }
    if (e.to.x>C7posx && e.to.x<C7posx+155 && e.to.y>C7posy && e.to.y<C7posy+56) {
      deactivate_button(H7posx,H7posy,strH);
      deactivate_button(V7posx,V7posy,strV);
      activate_button(C7posx,C7posy,strC);
      relay.turn_off_channel(3);
      relay.turn_on_channel(4);
      DingDong();
      Str70cm="CIRC";
    }
  }
  
}

boolean restoreConfig() {
  wifi_ssid = preferences.getString("WIFI_SSID");
  wifi_password = preferences.getString("WIFI_PASSWD");
  M5.Lcd.println("");
  M5.Lcd.println("========== WIFI ==========");
  M5.Lcd.print("SSID: ");
  M5.Lcd.println(wifi_ssid);

  WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());

  if(wifi_ssid.length() > 0) {
    return true;
} else {
    return false;
  }
}

boolean checkConnection() {
  int count = 0;
  while ( count < 30 ) {
    if (WiFi.status() == WL_CONNECTED) {
      M5.Lcd.println("\nStarting Web Server at ");
      M5.Lcd.println(WiFi.localIP());
      delay(2000);
      return (true);
    }
    delay(500);
    M5.Lcd.print(". ");
    count++;
  }
  M5.Lcd.println("Timed out.");
  return false;
}

void startWebServer() {
  if (settingMode) {
    M5.Lcd.print("Starting Web Server at ");
    M5.Lcd.println(WiFi.softAPIP());
    webServer.on("/settings", []() {
      String s = "<h1>Wi-Fi Settings</h1><p>Please enter your password by selecting the SSID.</p>";
      s += "<form method=\"get\" action=\"setap\"><label>SSID: </label><select name=\"ssid\">";
      s += ssidList;
      s += "</select><br>Password: <input name=\"pass\" length=64 type=\"password\"><input type=\"submit\"></form>";
      webServer.send(200, "text/html", makePage("Wi-Fi Settings", s));
    });
    webServer.on("/setap", []() {
      String ssid = urlDecode(webServer.arg("ssid"));
      M5.Lcd.print("SSID: ");
      M5.Lcd.println(ssid);
      String pass = urlDecode(webServer.arg("pass"));
      M5.Lcd.print("Password: ");
      M5.Lcd.println(pass);
      M5.Lcd.println("Writing SSID to EEPROM...");

      // Store wifi config
      M5.Lcd.println("Writing Password to nvr...");
      preferences.putString("WIFI_SSID", ssid);
      preferences.putString("WIFI_PASSWD", pass);

      M5.Lcd.println("Write nvr done!");
      String s = "<h1>Setup complete.</h1><p>device will be connected to \"";
      s += ssid;
      s += "\" after the restart.";
      webServer.send(200, "text/html", makePage("Wi-Fi Settings", s));
      delay(3000);
      ESP.restart();
    });
    webServer.onNotFound([]() {
      String s = "<h1>AP mode</h1><p><a href=\"/settings\">Wi-Fi Settings</a></p>";
      webServer.send(200, "text/html", makePage("AP mode", s));
    });
  }
  else {
     webServer.on("/json", []() {
      if (webServer.hasArg("band") && webServer.hasArg("polar")) {
        String band=webServer.arg("band");
        String polar=webServer.arg("polar");
        M5.Lcd.writecommand(ILI9341_DISPON);
        timer=screensaver;
        if (band=="2m") { 
          if (polar=="HORI") {
            if (Str2m=="HORI") return;
            activate_button(H2posx,H2posy,strH);
            deactivate_button(V2posx,V2posy,strV);
            deactivate_button(C2posx,C2posy,strC);
            relay.turn_on_channel(1);
            relay.turn_off_channel(2);
          }
          if (polar=="VERT") {
            if (Str2m=="VERT") return;
            deactivate_button(H2posx,H2posy,strH);
            activate_button(V2posx,V2posy,strV);
            deactivate_button(C2posx,C2posy,strC);
            relay.turn_off_channel(1);
            relay.turn_off_channel(2);
          }
          if (polar=="CIRC") {
            if (Str2m=="CIRC") return;
            deactivate_button(H2posx,H2posy,strH);
            deactivate_button(V2posx,V2posy,strV);
            activate_button(C2posx,C2posy,strC);
            relay.turn_off_channel(1);
            relay.turn_on_channel(2);
          }
          Str2m=polar;
        }
        if (band=="70cm") {
          if (polar=="HORI") {
            if (Str70cm=="HORI") return;
            activate_button(H7posx,H7posy,strH);
            deactivate_button(V7posx,V7posy,strV);
            deactivate_button(C7posx,C7posy,strC);
            relay.turn_on_channel(3);
            relay.turn_off_channel(4);
            }
          if (polar=="VERT") {
            if (Str70cm=="VERT") return;
            deactivate_button(H7posx,H7posy,strH);
            activate_button(V7posx,V7posy,strV);
            deactivate_button(C7posx,C7posy,strC);
            relay.turn_off_channel(3);
            relay.turn_off_channel(4);
            }
          if (polar=="CIRC") {
            if (Str70cm=="CIRC") return;
            deactivate_button(H7posx,H7posy,strH);
            deactivate_button(V7posx,V7posy,strV);
            activate_button(C7posx,C7posy,strC);
            relay.turn_off_channel(3);
            relay.turn_on_channel(4);
          }
          Str70cm=polar;
        }
        DingDong();
          
      } else {
        String s = "{\"v2m\":\""+Str2m+"\",\"v70cm\":\""+Str70cm+"\"}";
        webServer.send(200, "application/json", s);
      }
    });
    webServer.on("/", []() {
     /* String s = "<h1>Remote Polarization Switch</h1><div id=\"one\"></div><table width='600' style='text-align:center;'><tr><td>2m</td><td>70cm</td></tr><tr><td>HORI</td><td>HORI</td></tr><tr><td>VERT</td><td>VERT</td></tr><tr><td>CIRC</td><td>CIRC</td></tr></table><p><a href=\"/reset\">Reset Wi-Fi Settings</a></p>";
      s+="<script type=\"text/javascript\">\n";
      s+=" setInterval(function(){\n\
        $.getJSON('/json', function(data) {\n\
            $('#one').empty();\n\
            console.debug(data);\n\
            for(var i in data) {\n\
                $('#one').append(data[i]);\n\
            }\n\
        });\n\
    }, 5000);\n\
</script><div id=\"one\"></div>\n";
    */
    
webServer.send(200, "text/html", indexHtml);
    });
    webServer.on("/reset", []() {
      // reset the wifi config
      preferences.remove("WIFI_SSID");
      preferences.remove("WIFI_PASSWD");
      String s = "<h1>Wi-Fi settings was reset.</h1><p>Please reset device.</p>";
      webServer.send(200, "text/html", makePage("Reset Wi-Fi Settings", s));
      delay(3000);
      ESP.restart();
    });
  }
  webServer.begin();
}

void setupMode() {
  WiFi.mode(WIFI_MODE_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  delay(100);
  M5.Lcd.println("");
  for (int i = 0; i < n; ++i) {
    ssidList += "<option value=\"";
    ssidList += WiFi.SSID(i);
    ssidList += "\">";
    ssidList += WiFi.SSID(i);
    ssidList += "</option>";
  }
  delay(100);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(apSSID);
  WiFi.mode(WIFI_MODE_AP);
  // WiFi.softAPConfig(IPAddress local_ip, IPAddress gateway, IPAddress subnet);
  // WiFi.softAP(const char* ssid, const char* passphrase = NULL, int channel = 1, int ssid_hidden = 0);
  // dnsServer.start(53, "*", apIP);
  startWebServer();
  M5.Lcd.print("Starting Access Point at \"");
  M5.Lcd.print(apSSID);
  M5.Lcd.println("\"");
}

String makePage(String title, String contents) {
  String s = "<!DOCTYPE html><html><head>";
  s += "<meta name=\"viewport\" content=\"width=device-width,user-scalable=0\">";
  s += "<script src=\"https://code.jquery.com/jquery-3.6.0.slim.min.js\" integrity=\"sha256-u7e5khyithlIdTpu22PHhENmPcRdFiHRjhAuHcs05RI=\" crossorigin=\"anonymous\"></script>";
  s += "\n<title>";
  s += title;
  s += "</title></head><body>\n";
  s += contents;
  s += "\n</body></html>";
  return s;
}

String urlDecode(String input) {
  String s = input;
  s.replace("%20", " ");
  s.replace("+", " ");
  s.replace("%21", "!");
  s.replace("%22", "\"");
  s.replace("%23", "#");
  s.replace("%24", "$");
  s.replace("%25", "%");
  s.replace("%26", "&");
  s.replace("%27", "\'");
  s.replace("%28", "(");
  s.replace("%29", ")");
  s.replace("%30", "*");
  s.replace("%31", "+");
  s.replace("%2C", ",");
  s.replace("%2E", ".");
  s.replace("%2F", "/");
  s.replace("%2C", ",");
  s.replace("%3A", ":");
  s.replace("%3A", ";");
  s.replace("%3C", "<");
  s.replace("%3D", "=");
  s.replace("%3E", ">");
  s.replace("%3F", "?");
  s.replace("%40", "@");
  s.replace("%5B", "[");
  s.replace("%5C", "\\");
  s.replace("%5D", "]");
  s.replace("%5E", "^");
  s.replace("%5F", "-");
  s.replace("%60", "`");
  return s;
}


  
