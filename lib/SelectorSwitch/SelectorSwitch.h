#ifndef SELECTOR_SWITCH_H
#define SELECTOR_SWITCH_H

#include <Arduino.h>
#include <Encoder.h>  // Als je de Encoder bibliotheek gebruikt

class SelectorSwitch {
private:
    Encoder encoder;
    int buttonPin;
    bool buttonPressed;
    bool bmoveUp;
    bool bmoveDown;
    long lastPosition;
    long currentPosition;
    unsigned long lastDebounceTime;
    unsigned long debounceDelay;

public:
    // Constructor
    SelectorSwitch(int pinA, int pinB, int buttonPin);

    // Initialiseer de encoder en knop
    void begin();

    // Update de status van de encoder
    void update();

    // Functie om de beweging omhoog te controleren
    bool moveUp();

    // Functie om de beweging omlaag te controleren
    bool moveDown();

    long currentPos();

    // Functie om de knopdruk te controleren met debounce
    bool isButtonPressed();
};

#endif
