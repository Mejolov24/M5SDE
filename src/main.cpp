#include <CSDFP.h>
CSDFP sdfp;
using input = CSDFP::Input;
#include <M5CADVKeyCB.h>
M5CADVKeyCB keyHandler;
#include "M5Cardputer.h"
M5Canvas canvas(&M5.Lcd);

// --- SD PINS ---
#define SD_SPI_SCK_PIN  40
#define SD_SPI_MISO_PIN 39
#define SD_SPI_MOSI_PIN 14
#define SD_SPI_CS_PIN   12

void OnKey(uint8_t key, bool pressed){
    if(!pressed) return;
    switch (key)
    {
    case 51:
        sdfp.process_input(input::up);
        break;

    case 55:
        sdfp.process_input(input::down);
        break;

    case 46:
        sdfp.process_input(input::back);
        break;
    
    default:
        break;
    }
}

void OnSelection(const char* path){}

void setup(){
    auto cfg = M5.config();
    M5Cardputer.begin(cfg);
    keyHandler.SetupKeyboardCallback(OnKey);
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
  keyHandler.KeyboardUpdate();
}