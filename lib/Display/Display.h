#ifndef DISPLAY_H
#define DISPLAY_H

#include <Wire.h>
#include <Adafruit_SSD1306.h>

class DisplayController {
public:
    // Constructor
    DisplayController(int width, int height, int i2c_addr, int resetPin);
    
    // Initialiseer het display
    bool begin();
    
    // Hoofdmenu functie (met de mogelijkheid om te scrollen)
    void mainMenu();
    void attachTemp(float* T1, float* T2);
    // Instellingen functie
    void settingLength();
    void settingSpeed();
    void attachState(String* MainState); 
    void attachStartButton(bool* buttonStart);  
    // ScanInfo functie
    void scanInfo();
    
    // Knopcontrole
    void checkButtons(bool MoveUp, bool MoveDown, bool Select);
   
    // Tekst weergeven
    void printText(const char* text, int x, int y);
    
    // Scherm bijwerken
    void clear();
    void updateDisplay();
    void attachLength(float* length);  // Functie om de pointer in te stellen
    void attachSpeed(float* speed);
private:
    // Functies voor navigatie en selecteren
    void navigateMenu(bool up);
    void selectMenu();

    float* Length;  // Pointer naar een externe lengte-variabele
    float* Speed;
    float* Temp1;
    float* Temp2;
    String* State;
    bool* StartButton;
    Adafruit_SSD1306 display;  // OLED display object
    int i2cAddress;     // I2C-adres opslaan als een membervariabele
    int selectedItem;
    int numItems;
    bool AdjustmodeLength;
    bool AdjustmodeSpeed;

};

#endif  // DISPLAY_H
