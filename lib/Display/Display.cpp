#include "Display.h"

// Constructor
DisplayController::DisplayController(int width, int height, int i2c_addr, int resetPin)
    : display(width, height, &Wire, resetPin), i2cAddress(i2c_addr), selectedItem(0), numItems(5), AdjustmodeLength(false), AdjustmodeSpeed(false) { // Pass correct i2cAddress
    selectedItem = 0;
    numItems = 5;
    AdjustmodeLength = false;
    AdjustmodeSpeed = false;
}

// Initialiseer het display
bool DisplayController::begin() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, i2cAddress)) {  // Use i2cAddress here
        Serial.println("OLED initialisatie mislukt!");
        return false;
    }

    // Standaard instellingen voor tekst en rotatie
    display.setRotation(2);  // Draai het scherm bijvoorbeeld 180 graden
    display.setTextSize(1);  // Standaard tekstgrootte
    display.setTextColor(SSD1306_WHITE);  // Standaard tekstkleur (wit)
    
    display.clearDisplay();
    display.display();
    return true;
}


// Hoofdmenu functie met scrollen en selecteren
void DisplayController::mainMenu() {
    clear();
    display.drawRect(0, 0, 128, 64, 1);
    display.setTextColor(1);
    display.setTextWrap(false);
    display.setCursor(63, 2);
    display.print("T2");
    display.setCursor(3, 2);
    display.print("T1");
    display.setCursor(17, 2);
    display.print(String(*this->Temp1, 1) + char(247) + "C");
    display.setCursor(78, 2);
    display.print(String(*this->Temp2, 1) + char(247) + "C");
    display.drawLine(1, 10, 126, 10, 1);
    
    // Menu-items tekenen
    String menuItems[numItems] = {"Info", "Start scan", "Scanlengte", "Scansnelheid", "Temperatuur"};
    
    for (int i = 0; i < numItems; i++) {
        if (i == selectedItem) {
            display.setCursor(3, 13 + (i * 10));  // Positie voor de indicator
            display.print("> ");  // Toon de pijl voor het geselecteerde item
        } else {
            display.setCursor(3, 13 + (i * 10));  // Positie zonder indicator
            display.print("  ");  // Lege ruimte voor niet-geselecteerde items
        }
        display.setCursor(15, 13 + (i * 10));  // Begin van de tekst (na "> ")
        display.print(menuItems[i]);  // Toon het menu-item
    }


    updateDisplay();  // Scherm bijwerken
}

// Functie voor navigeren door het menu (up/down)
void DisplayController::navigateMenu(bool up) {
    if (up) {
        if (!AdjustmodeLength && !AdjustmodeSpeed) {
            selectedItem--;
            if (selectedItem < 0) selectedItem = numItems - 1;  // Als we bovenaan zijn, ga naar beneden
        } else if (AdjustmodeLength) *Length = *Length + float(0.01);
        else if (AdjustmodeSpeed) *Speed = *Speed + float(0.001);
    } else {
        if (!AdjustmodeLength && !AdjustmodeSpeed) {
            selectedItem++;
            if (selectedItem >= numItems) selectedItem = 0;  // Als we onderaan zijn, ga naar boven
        } else if (AdjustmodeLength) *Length = *Length - float(0.01);
        else if (AdjustmodeSpeed) *Speed = *Speed - float(0.001);
    }
    if (!AdjustmodeLength && !AdjustmodeSpeed) {
            mainMenu();
        } else if (AdjustmodeLength) settingLength();
        else if (AdjustmodeSpeed) settingSpeed();
    
}

// Functie om te selecteren (bijvoorbeeld via een knop)
void DisplayController::selectMenu() {
    // Hier  de logica toevoegen voor wat er gebeurt als een menu-item wordt geselecteerd.
    // Dit kan bijvoorbeeld een andere functie aanroepen, zoals een instellingenmenu of scan-informatie.
    Serial.print("Geselecteerd item: ");
    Serial.println(selectedItem);
    if (AdjustmodeLength || AdjustmodeSpeed) {
        AdjustmodeLength = false;
        AdjustmodeSpeed = false;
        mainMenu();
    } else if (selectedItem == 0) {


    } else if (selectedItem == 1) {
        // Voorbeeldactie voor Item2
        scanInfo();
        *StartButton = true;
    } else if (selectedItem == 2) {
        // Voorbeeldactie voor Scanlengte
        settingLength();
        AdjustmodeLength = true;
    } else if (selectedItem == 3) {
        // Voorbeeldactie voor Scansnelheid
        settingSpeed();
        AdjustmodeSpeed = true;
    }
    // Voeg meer acties toe voor andere items...
}

// Knop controleren en de geselecteerde optie wijzigen
void DisplayController::checkButtons(bool MoveUp, bool MoveDown, bool Select) {
    
    if (MoveUp == HIGH) {
        navigateMenu(true);  // Beweeg omhoog als de up-knop ingedrukt is
    }
    
    if (MoveDown == HIGH) {
        navigateMenu(false);  // Beweeg omlaag als de down-knop ingedrukt is
    }
    
    if (Select == HIGH) {
        selectMenu();  // Selecteer het item als de select-knop wordt ingedrukt
    }
}

// Instellingenmenu functie
void DisplayController::settingSpeed() {
    clear();
    display.drawRect(0, 0, 128, 64, 1);
    display.setTextColor(1);
    display.setTextWrap(false);
    display.setCursor(63, 2);
    display.print("T2");
    display.setCursor(3, 2);
    display.print("T1");
    display.setCursor(17, 2);
    display.print(*this->Temp1, 1);
    display.setCursor(78, 2);
    display.print(*this->Temp2, 1);
    display.drawLine(1, 10, 126, 10, 1);
    display.setCursor(4, 19);
    display.print("Scansnelheid:");
    display.setCursor(85, 37);
    display.print("m/s");
    display.setCursor(45, 37);
    if (this->Speed) {  
        display.setCursor(45, 37);
        display.print(*this->Speed, 3);  // Gebruik de waarde waar de pointer naar wijst
    }
    updateDisplay();  // Scherm bijwerken
}

void DisplayController::settingLength() {
    clear();
    display.drawRect(0, 0, 128, 64, 1);
    display.setTextColor(1);
    display.setTextWrap(false);
    display.setCursor(63, 2);
    display.print("T2");
    display.setCursor(3, 2);
    display.print("T1");
    display.setCursor(17, 2);
    display.print(*this->Temp1, 1);
    display.setCursor(78, 2);
    display.print(*this->Temp2, 1);
    display.drawLine(1, 10, 126, 10, 1);
    display.setCursor(4, 19);
    display.print("Scanlengte:");
    display.setCursor(85, 37);
    display.print("m");
    display.setCursor(45, 37);
    if (this->Length) {  
        display.setCursor(45, 37);
        display.print(*this->Length, 2);  // Gebruik de waarde waar de pointer naar wijst
    }
    updateDisplay();  // Scherm bijwerken
}

// ScanInfo functie
void DisplayController::scanInfo() {
    clear();
    display.drawRect(0, 0, 128, 64, 1);
    display.setTextColor(1);
    display.setTextWrap(false);
    display.setCursor(63, 2);
    display.print("T2");
    display.setCursor(3, 2);
    display.print("T1");
    display.setCursor(17, 2);
    display.print(*this->Temp1, 1);
    display.setCursor(78, 2);
    display.print(*this->Temp2, 1);
    display.drawLine(1, 10, 126, 10, 1);
    display.setCursor(3, 17);
    display.print("Status:");
    display.setCursor(47, 17);
    display.print(*this->State);
    display.setCursor(3, 54);
    display.print("Snelheid:");
    display.setCursor(3, 43);
    display.print("Lengte:");
    display.setCursor(2, 31);
    display.print("Instellingen scan");
    display.setCursor(107, 54);
    display.print("m/s");
    display.setCursor(107, 43);
    display.print("m");
    display.setCursor(65, 54);
    display.print(*this->Speed, 3);
    display.setCursor(65, 43);
    display.print(*this->Length, 3);
    updateDisplay();
}

// Tekst weergeven op een specifieke regel
void DisplayController::printText(const char* text, int x, int y) {
    display.setCursor(x, y);
    display.print(text);
}

// Scherm wissen
void DisplayController::clear() {
    display.clearDisplay();
}

// Scherm bijwerken
void DisplayController::updateDisplay() {
    display.display(); // Update de inhoud op het scherm
}

void DisplayController::attachLength(float* length) {
    this->Length = length;  // Koppel de externe variabele
}

void DisplayController::attachSpeed(float* speed) {
    this->Speed = speed;  // Koppel de externe variabele
}

void DisplayController::attachTemp(float* T1, float* T2) {
    this->Temp1 = T1;
    this->Temp2 = T2;
}

void DisplayController::attachState(String* MainState) {
    this->State = MainState;
}

void DisplayController::attachStartButton(bool* buttonStart) {
    this->StartButton = buttonStart;
}