#include <M5Core2.h>
#include <multi_channel_relay.h>

Multi_Channel_Relay relay;
int H2posx=4;
int H2posy=61;
int V2posx=4;
int V2posy=121;
int C2posx=4;
int C2posy=181;
int H7posx=162;
int H7posy=61;
int V7posx=162;
int V7posy=121;
int C7posx=162;
int C7posy=181;
char strC[]={"CIRC"};
char strV[]={"VERT"};
char strH[]={"HORI"};

void setup(){
  M5.begin();
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setCursor(0, 0, 2);
  M5.Lcd.setBrightness(0);
  M5.Lcd.setTextColor(TFT_YELLOW,TFT_BLACK);
  M5.Lcd.setCursor(28 ,90,1);
  M5.Lcd.setTextSize(4);
  M5.Lcd.setTextFont(2);
  M5.Lcd.println("F G 8 O J");
  delay(500);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.fillRect(158, 0, 3, 240,TFT_YELLOW);
  M5.Lcd.fillRect(0, 0, 320, 60,TFT_YELLOW);
  M5.Lcd.fillRect(0, 120, 320, 3,TFT_YELLOW);
  M5.Lcd.fillRect(0, 180, 320, 3,TFT_YELLOW);
  M5.Lcd.fillRect(0, 237, 320, 3,TFT_YELLOW);
  M5.Lcd.fillRect(0, 0, 3, 240,TFT_YELLOW);
  M5.Lcd.fillRect(317, 0, 3, 240,TFT_YELLOW); 
  M5.Lcd.setTextColor(TFT_BLACK,TFT_YELLOW);
  M5.Lcd.setTextSize(4);
  M5.Lcd.setTextFont(2);
  M5.Lcd.setCursor(65 ,14,1);
  M5.Lcd.println("2m");
  M5.Lcd.setCursor(190 ,14,1);
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
  M5.Lcd.fillRect(posx, posy, 155, 56,TFT_BLACK);
  M5.Lcd.setCursor(posx+35 ,posy+2,1);
  M5.Lcd.setTextFont(2);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setTextColor(TFT_GREEN);
  M5.Lcd.println(text_button);   
}

void activate_button(int posx, int posy, char text_button[]  ) {
  M5.Lcd.fillRect(posx, posy, 155, 56,TFT_GREEN);
  M5.Lcd.setCursor(posx+35 ,posy+2,1);
  M5.Lcd.setTextFont(2);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setTextColor(TFT_BLACK);
  M5.Lcd.println(text_button); 
}

void loop() {
  M5.update();
  Event& e = M5.Buttons.event;
  if (e & (E_TOUCH | E_MOVE)) {
    if (e.to.x>H2posx && e.to.x<H2posx+155 && e.to.y>H2posy && e.to.y<H2posy+56) {
      activate_button(H2posx,H2posy,strH);
      deactivate_button(V2posx,V2posy,strV);
      deactivate_button(C2posx,C2posy,strC);
      relay.turn_on_channel(1);
      relay.turn_off_channel(2);
    }
    if (e.to.x>V2posx && e.to.x<V2posx+155 && e.to.y>V2posy && e.to.y<V2posy+56) {
      deactivate_button(H2posx,H2posy,strH);
      activate_button(V2posx,V2posy,strV);
      deactivate_button(C2posx,C2posy,strC);
      relay.turn_off_channel(1);
      relay.turn_off_channel(2);
    }
    if (e.to.x>C2posx && e.to.x<C2posx+155 && e.to.y>C2posy && e.to.y<C2posy+56) {
      deactivate_button(H2posx,H2posy,strH);
      deactivate_button(V2posx,V2posy,strV);
      activate_button(C2posx,C2posy,strC);
      relay.turn_off_channel(1);
      relay.turn_on_channel(2);
    }
    if (e.to.x>H7posx && e.to.x<H7posx+155 && e.to.y>H7posy && e.to.y<H7posy+56) {
      activate_button(H7posx,H7posy,strH);
      deactivate_button(V7posx,V7posy,strV);
      deactivate_button(C7posx,C7posy,strC);
      relay.turn_on_channel(3);
      relay.turn_off_channel(4);
    }
    if (e.to.x>V7posx && e.to.x<V7posx+155 && e.to.y>V7posy && e.to.y<V7posy+56) {
      deactivate_button(H7posx,H7posy,strH);
      activate_button(V7posx,V7posy,strV);
      deactivate_button(C7posx,C7posy,strC);
      relay.turn_off_channel(3);
      relay.turn_off_channel(4);
    }
    if (e.to.x>C7posx && e.to.x<C7posx+155 && e.to.y>C7posy && e.to.y<C7posy+56) {
      deactivate_button(H7posx,H7posy,strH);
      deactivate_button(V7posx,V7posy,strV);
      activate_button(C7posx,C7posy,strC);
      relay.turn_off_channel(3);
      relay.turn_on_channel(4);
    }
  }
}



  
