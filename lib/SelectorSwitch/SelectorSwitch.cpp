#include "SelectorSwitch.h"

// Constructor
SelectorSwitch::SelectorSwitch(int pinA, int pinB, int buttonPin)
    : encoder(pinA, pinB), buttonPin(buttonPin), buttonPressed(false), bmoveUp(false), bmoveDown(false),
      lastPosition(0), currentPosition(0), lastDebounceTime(0), debounceDelay(50) {} 

// Initialiseer de encoder en knop
void SelectorSwitch::begin() {
    pinMode(buttonPin, INPUT_PULLUP);  // Zet de knop in als input met pull-up
}

// Retourneer een pointer naar de Move struct
void SelectorSwitch::update() {
    bmoveUp = false;
    bmoveDown = false;
    currentPosition = encoder.read();

    // Lees de encoder en bepaal de draairichting
    if (abs(currentPosition - lastPosition) >= 4) {
        // Draairichting bepalen
        if (currentPosition > lastPosition) {
            bmoveUp = true;
            bmoveDown = false;
        } else if (currentPosition < lastPosition) {
            bmoveDown = true;
            bmoveUp = false;
        }

        // Update de laatste positie
        lastPosition = currentPosition;
    }
    delay(10);
}

bool SelectorSwitch::isButtonPressed() {
    bool currentButtonState = digitalRead(buttonPin);  // Lees de huidige knopstatus
    static bool lastButtonState = HIGH;  // De vorige status van de knop (begin bij HIGH omdat de pin met een pull-up is ingesteld)
    buttonPressed = false;
    // Als genoeg tijd is verstreken sinds de laatste debounce, controleren we de knopstatus
    if ((millis() - lastDebounceTime) > debounceDelay) {
        // Controleer de overgang van HIGH naar LOW (drukken)
        if (currentButtonState == LOW && lastButtonState == HIGH) {
            buttonPressed = true;  // Knop is ingedrukt (rising edge)
            lastDebounceTime = millis();  // Update de tijd van de laatste debounce
            lastButtonState = false;
        }
        else {
            buttonPressed = false;  // Knop is niet ingedrukt
        }

        // Werk de laatste debounce tijd bij
        //lastDebounceTime = millis();  // Update de laatste debounce tijd
    }

    // Update de vorige knopstatus voor de volgende loop
    if (currentButtonState == HIGH && lastButtonState == LOW) {
        lastButtonState = currentButtonState;
    }
    

    return buttonPressed;  // Return de knopstatus
}

// Functie om de beweging omhoog te controleren
bool SelectorSwitch::moveUp() {
    return bmoveUp;  // Return de status van beweging omhoog
}

// Functie om de beweging omlaag te controleren
bool SelectorSwitch::moveDown() {
    return bmoveDown;  // Return de status van beweging omlaag
}

long SelectorSwitch::currentPos() {
    return currentPosition;  // Return de status van beweging omlaag
}