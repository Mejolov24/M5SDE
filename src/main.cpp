#include <CSDFP.h>
CSDFP sdex;
using input = CSDFP::Input;
#include "M5Cardputer.h"
M5Canvas canvas(&M5.Lcd);


// --- SD PINS ---
#define SD_SPI_SCK_PIN  40
#define SD_SPI_MISO_PIN 39
#define SD_SPI_MOSI_PIN 14
#define SD_SPI_CS_PIN   12


void OnSelection(const char* path){
    canvas.clear();
    canvas.drawString("Selected file :", 0,0,&fonts::Font4);
    canvas.drawString(String(path),0,30,&fonts::Font0); // print to screen the absolute path of selected option
    canvas.pushSprite(0,0);
}

void setup(){
    auto cfg = M5.config();
    M5Cardputer.begin(cfg);
    canvas.createSprite(240, 135);
    bool sd_active = false;
    SPI.begin(SD_SPI_SCK_PIN, SD_SPI_MISO_PIN, SD_SPI_MOSI_PIN, SD_SPI_CS_PIN);

    while(!sd_active){
    if (SD.begin(SD_SPI_CS_PIN, SPI, 25000000)) sd_active = true;
    else{
        canvas.setTextColor(RED);
        canvas.drawString("SD FAIL",0,0,&fonts::FreeMonoBold24pt7b);
        canvas.pushSprite(0,0);
        delay(100);}
}


sdex.begin(&canvas,OnSelection);
sdex.open();
}

void loop(){
  M5Cardputer.update();
    if (M5Cardputer.Keyboard.isChange()) {
    auto keys = M5Cardputer.Keyboard.keysState();
    if (M5Cardputer.Keyboard.isKeyPressed(';')){sdex.process_input(input::up);}
    if (M5Cardputer.Keyboard.isKeyPressed('.')){sdex.process_input(input::down);}
    if (keys.enter){sdex.process_input(input::select);}
    if (keys.del){sdex.process_input(input::back);}
    }
}