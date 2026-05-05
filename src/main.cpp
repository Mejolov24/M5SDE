#include <CSDFP.h>
CSDFP sdfp;
using input = CSDFP::Input;
#include "M5Cardputer.h"
M5Canvas canvas(&M5.Lcd);


// --- SD PINS ---
#define SD_SPI_SCK_PIN  40
#define SD_SPI_MISO_PIN 39
#define SD_SPI_MOSI_PIN 14
#define SD_SPI_CS_PIN   12


void OnSelection(const char* path){
    canvas.drawString(String(path),0,0,&fonts::FreeMonoBold24pt7b); // print to screen the absolute path of selected option
    canvas.pushSprite(0,0);
}

void setup(){
    auto cfg = M5.config();
    M5Cardputer.begin(cfg);
    SPI.begin(SD_SPI_SCK_PIN, SD_SPI_MISO_PIN, SD_SPI_MOSI_PIN, SD_SPI_CS_PIN);
    if (!SD.begin(SD_SPI_CS_PIN, SPI, 25000000)) {
        Serial.println("SD failed!");
        return;
    }
canvas.createSprite(240, 135);

sdfp.begin(&canvas,OnSelection);
sdfp.open("/");
}

void loop(){
  M5Cardputer.update();
    if (M5Cardputer.Keyboard.isChange()) {
    auto keys = M5Cardputer.Keyboard.keysState();
    if (M5Cardputer.Keyboard.isKeyPressed(';')){sdfp.process_input(input::up);}
    if (M5Cardputer.Keyboard.isKeyPressed('.')){sdfp.process_input(input::down);}
    if (keys.enter){sdfp.process_input(input::select);}
    if (keys.del){sdfp.process_input(input::back);}
    }
}