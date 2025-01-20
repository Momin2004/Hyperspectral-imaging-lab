#include <Wire.h>
#include <DHT.h>
#include "Display.h"
#include "SelectorSwitch.h"
#include "RGBcontrol.h"
#include "TempController.h"
#include "Motor.h"
#include "Enc.h"

// Define the settings and object for OLED-display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define OLED_I2C_ADDR 0x3C
DisplayController display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_I2C_ADDR, OLED_RESET);

// Define the pins and objects for push buttons with RGB lighting
#define StopButton 2
RGBController rgbController(5, 4, 6);

// Define the pins and object for encoder push button
#define EncButtonS1 23
#define EncButtonS2 24
#define EncButtonKEY 22
SelectorSwitch selector(EncButtonS1, EncButtonS2, EncButtonKEY);

// Define the pins and objects for the two temperature sensors
#define DHT21_PIN1 9  // Pin DHT21 sensor control box
#define DHT21_PIN2 10  // Pin DHT21 sensor LAB setup
TempController TempControlBox(DHT21_PIN1, 15000, 45000);
TempController TempLABsetup(DHT21_PIN1, 15000, 45000);
float temp1 = 0.0;
float temp2 = 0.0;

// Pinnen voor de motor
#define STEP_PIN 11
#define DIR_PIN 12
#define EN_PIN 13
#define SERIAL_PORT Serial3 // TMC2208/TMC2224 HardwareSerial port
#define DRIVER_ADDRESS 0b00 // TMC2209 Driver address according to MS1 and MS2
#define R_SENSE 0.11f
Motor motor(DIR_PIN, STEP_PIN, EN_PIN, SERIAL_PORT, R_SENSE, DRIVER_ADDRESS);

#define ENCODER_A 18
#define ENCODER_B 19
#define LINEAR_DISTANCE_PER_STEP 0.00005655
Enc encoder(ENCODER_A, ENCODER_B, LINEAR_DISTANCE_PER_STEP);

// Startup settings
unsigned long startTime;
unsigned long delayTime = 5000; // 5 sec

// General settings
float scanLength = 0.2;
float scanSpeed = 0.150; 
float totalDistance = 1.50; // Totale afstand in meters

// Defining the states
enum State {
  Waiting,
  Homing,
  Accelerating,
  Scanning,
  Decelerating,
  Stopped,
  SafeStop
};
State currentState = Waiting;
State lastStringCurrentState = currentState;
String StringCurrentState = String(currentState);
bool StartButton = false;
bool ScanStart = false;

// Motion profile settings
float totalTime = 0;        // Totale tijd voor de beweging
float accelerationTime = 0; // Versnellingstijd (van 0 naar scanSpeed)
float decelerationTime = 0;
float AccelDeccelDistance = 0; // Versnelling afstand
float accelerationDistance = 0; // Versnelling afstand
float deccelerationDistance = 0;
unsigned long lastTime = 0; // Voor de tijdsberekeningen
float elapsedTime = 0;

// Declaration of the functions
void handleStopButton();
void calculateMotorSpeed();
float updateMotorSpeed();
void updateScanStart();
String stateToString(State state);

void setup() {
    Serial.begin(9600);

    pinMode(StopButton, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(StopButton), handleStopButton, CHANGE); // Interrupt op Rising-edge

    selector.begin();
    rgbController.begin();
    encoder.begin();

    if (!digitalRead(StopButton)) {
        currentState = Waiting;
    } else currentState = SafeStop;
    StartButton = false;
    ScanStart = false;

    motor.begin();
    motor.setDirection(false);     // Max snelheid (stappen per seconde)
    motor.setSpeed(0.1);    // Versnelling (stappen per seconde per seconde)
    motor.setStallguardThreshold(200);
    motor.stop();

    TempControlBox.begin();
    TempControlBox.attachTemp(&temp1);
    // TempLABsetup.begin();
    // TempLABsetup.attachTemp(&temp2);

    // Initialiseer het display
    if (!display.begin()) {
        Serial.println("Display niet gevonden, controleer verbinding!");
        while (true); // Stop programma als display niet is gevonden
    }
    display.clear();
    display.attachLength(&scanLength);
    display.attachSpeed(&scanSpeed); 
    display.attachTemp(&temp1, &temp2);
    display.attachState(&StringCurrentState);
    display.attachStartButton(&StartButton);
    display.printText("Startup...", 15, 15);
    display.updateDisplay();

    // Startup display with delay
    rgbController.setState(Startup);
    startTime = millis();
    while (millis() - startTime < delayTime) {
        rgbController.update();
    }
    display.clear();
    display.updateDisplay();
    display.mainMenu();
}


void loop() {
    rgbController.update();
    // display.checkButtons(selector.moveUp(), selector.moveDown(), selector.isButtonPressed());
    updateScanStart();

    if (currentState != Waiting) {
        StartButton = false;
        if ((lastStringCurrentState != currentState) && (currentState != SafeStop)){
            StringCurrentState = stateToString(currentState);
            lastStringCurrentState = currentState;
            display.scanInfo();
        }
    }
    
    static unsigned long lastUpdateSpeed = 0;
    if (millis() - lastUpdateSpeed > 1000) { // Elke seconde printen
        encoder.updateSpeed();
        lastUpdateSpeed = millis();
    }

    static unsigned long lastPrint = 0;
    if (millis() - lastPrint > 500) { // Elke seconde printen
        lastPrint = millis();

        float tempPosition = encoder.getPosition();
        float tempSpeed = encoder.getSpeed();
        Serial.print("Error : ");
        Serial.print(motor.isError());
        Serial.print(" , SG_Result: ");
        Serial.print(motor.getSGResult());
        Serial.print(" , Huidige positie: ");
        Serial.print(tempPosition, 6); // Print positie in meters met 6 decimalen
        Serial.print(" m, Snelheid: ");
        Serial.print(tempSpeed, 6);    // Print snelheid in m/s met 6 decimalen
        Serial.println(" m/s");
    }


    switch (currentState) {
        case Waiting:
        TempControlBox.update();
        TempLABsetup.update();
        rgbController.setState(Wait);
        display.checkButtons(selector.moveUp(), selector.moveDown(), selector.isButtonPressed());
        selector.update();

        if (StartButton) {
            motor.setDirection(false);
            motor.setStallguardThreshold(40);
            currentState = Homing;
        }
        break;

        case Homing:
        rgbController.setState(Home);
        motor.update();
        motor.setSpeed(0.025);
        motor.start();

        if (motor.isError()) {
            motor.stop();
            motor.resetError();
            calculateMotorSpeed();
            motor.setDirection(true);
            delay(5000);
            motor.setStallguardThreshold(10);
            encoder.resetPosition();
            elapsedTime=0;
            lastTime = millis();
            currentState = Accelerating;
        }
        break;

        case Accelerating:
        rgbController.setState(Run);
        //motor.update();
        motor.start();
        motor.setSpeed(updateMotorSpeed());

        if (ScanStart) {
            motor.setDirection(true);
            motor.setStallguardThreshold(10);
            currentState = Scanning;
        }
        break;

        case Scanning:
        rgbController.setState(Run);
        //motor.update();
        motor.setSpeed(updateMotorSpeed());

        if (!ScanStart) {
            motor.setDirection(true);
            motor.setStallguardThreshold(10);
            currentState = Decelerating;
            display.scanInfo();
        }
        break;

        case Decelerating:
        rgbController.setState(Run);
        //motor.update();
        motor.setSpeed(updateMotorSpeed());

        if (encoder.getSpeed() < 0.01) {
            motor.setDirection(false);
            motor.setStallguardThreshold(5);
            currentState = Stopped;
        }
        break;

        case Stopped:
        rgbController.setState(Stop);
        motor.stop();
        elapsedTime=0;
        lastTime = millis();
        delay(1000);        
        currentState = Waiting;
        display.mainMenu();
        break;

        case SafeStop:
        rgbController.setState(Safe);
        motor.stop();


        if (!digitalRead(StopButton)) {
            motor.resetError();
            encoder.resetPosition();
            elapsedTime=0;
            lastTime = millis();
            currentState = Waiting; 
            display.mainMenu();
        }
        break;

        default:
        Serial.println("Onbekende state!");
        break;
    }


}

// Interruptservicefunctie (ISR)
void handleStopButton() {
    currentState = SafeStop; // Wijzig de safe stop status
}

// Functie om de snelheid van de motor te berekenen
void calculateMotorSpeed() {
    lastTime = millis();

    // Versnelling en deceleratieafstand
    AccelDeccelDistance = (totalDistance - scanLength);
    accelerationDistance = AccelDeccelDistance / 2.0;
    deccelerationDistance = accelerationDistance;
    //accelerationDistance AccelDeccelDistance * 0.75;
    //deccelerationDistance = AccelDeccelDistance *0.25;

    // Bereken de benodigde versnelling
    float acceleration = (scanSpeed * scanSpeed) / (2.0 * accelerationDistance);
    float deceleration = (scanSpeed * scanSpeed) / (2.0 * deccelerationDistance);

    // Bereken de tijd om de snelheid op te bouwen
    accelerationTime = sqrt(2 * accelerationDistance / acceleration);
    decelerationTime = sqrt(2 * deccelerationDistance / deceleration);

    // Totale tijd voor de volledige cyclus
    totalTime = accelerationTime + (scanLength / scanSpeed + decelerationTime);
}

// Functie om de snelheid van de motor bij te werken op basis van de verstreken tijd
float updateMotorSpeed() {
    unsigned long currentTime = millis(); // Verkrijg de verstreken tijd sinds het opstarten in milliseconden
    float motorSpeed = 0.0;               // Standaard snelheid op nul
    elapsedTime = (currentTime - lastTime) / 1000.0;  // Verstreken tijd in seconden

    // Versnelling
    if (elapsedTime < accelerationTime) {
        motorSpeed = (scanSpeed / accelerationTime) * elapsedTime; // Lineair versnellen
    }
    // Constante snelheid
    else if (elapsedTime < (accelerationTime + scanLength / scanSpeed)) {
        motorSpeed = scanSpeed; // Constante snelheid
    }
    // Deceleratie
    else if (elapsedTime < totalTime) {
        float timeSinceDecelStart = elapsedTime - (accelerationTime + scanLength / scanSpeed);
        motorSpeed = scanSpeed * (1 - timeSinceDecelStart / decelerationTime); // Lineair afremmen
    }
    // Stopconditie
    else {
        motorSpeed = 0.0; // Stop de motor als de totale tijd is verstreken
    }
    

    return motorSpeed;
}


void updateScanStart() {
    if ((encoder.getPosition() > accelerationDistance) && (encoder.getPosition() < (totalDistance-accelerationDistance))) {
        ScanStart = true;
    } else ScanStart = false;
}

String stateToString(State state) {
    switch (state) {
        case Waiting:    return "Waiting";
        case Homing: return "Homing";
        case Accelerating:  return "Accelerating";
        case Scanning: return "Scanning";
        case Decelerating: return "Decelerating";
        case Stopped: return "Stopped";
        case SafeStop: return "SafeStop";
        default:             return "Unknown";
    }
}