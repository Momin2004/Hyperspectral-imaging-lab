#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <TMCStepper.h>

class Motor {
public:
    Motor(int dirPin, int stepPin, int enPin, HardwareSerial& serialPort, float rsense, uint8_t driverAddress);
    void begin();                   // Initialiseer de motor
    void setSpeed(float speed);       // Stel de snelheid in (in microseconden per stap)
    void start();                   // Start de motor
    void stop();                    // Stop de motor
    void setDirection(bool forward); // Stel de draairichting in
    bool isRunning();               // Controleer of de motor draait
    void setStallguardThreshold(int threshold); // Stel de StallGuard drempel in
    bool isError();                 // Controleer of de motor een fout heeft (stall)
    int getSGResult();              // Verkrijg de huidige StallGuard waarde
    void update();
    static void isrStep();          // Maak deze functie publiek
    void resetError();
private:
    int dirPin;                     // Richtingspin
    int stepPin;                    // Steppin
    int enPin;                      // Enable pin
    HardwareSerial& serialPort;     // Seriële poort voor de driver
    TMC2209Stepper driver;          // Driverobject
    int speed;                      // Snelheid (microseconden per stap)
    bool running;                   // Of de motor momenteel draait
    int stallGuardThreshold;        // StallGuard drempelwaarde
    bool Error;
    void setupTimer();              // Stel de timer in voor stappen
    static Motor* instance;         // Statische instantie van de motor
    unsigned long startTime; // Tijd waarop de motor begon te draaien

};

#endif
