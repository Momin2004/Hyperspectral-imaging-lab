#include "Motor.h"

// Initialiseer de statische instantie
Motor* Motor::instance = nullptr;

Motor::Motor(int dirPin, int stepPin, int enPin, HardwareSerial& serialPort, float rsense, uint8_t driverAddress)
    : dirPin(dirPin), stepPin(stepPin), enPin(enPin), serialPort(serialPort), 
      driver(&serialPort, rsense, driverAddress), speed(1000), running(false), stallGuardThreshold(50), Error(false), startTime(0) {}

void Motor::begin() {
    pinMode(dirPin, OUTPUT);
    pinMode(stepPin, OUTPUT);
    pinMode(enPin, OUTPUT);
    digitalWrite(enPin, HIGH); // Zet motor uit

    serialPort.begin(115200);
    driver.begin();
    driver.toff(10);
    driver.blank_time(24);
    driver.rms_current(2400);       // Stroom in mA
    driver.microsteps(16);         // Microstappen
    driver.TCOOLTHRS(0);
    //driver.TCOOLTHRS(0x10000); // Schakel naar SpreadCycle bij hogere snelheden
    //driver.TCOOLTHRS(0xFFFFF);     // Maximum snelheid
    driver.SGTHRS(60);            // Stel de StallGuard drempel in
    driver.semin(5);
    driver.semax(12);
    driver.en_spreadCycle();

    instance = this;               // Wijs dit object toe aan de statische instantie
    setupTimer();                  // Stel de timer in
}

void Motor::setSpeed(float Speed) {
    int SpeedInt = round((2000000/((((Speed/((PI*36)/1000))*200)*16)*2)));
    if (SpeedInt > 0) {
        speed = constrain(SpeedInt, 40, 10000); // Beperk de snelheid binnen het bereik
    }
    OCR1A = speed; // Update de timerwaarde
}

void Motor::start() {
    if ((!Error) && (!running)) {
        running = true;
        digitalWrite(enPin, LOW); // Zet motor aan
        startTime = millis();     // Sla het moment op wanneer de motor start
        TIMSK1 |= (1 << OCIE1A); // Schakel timer interrupt in
    }
}

void Motor::stop() {
    running = false;
    digitalWrite(enPin, HIGH); // Zet motor uit
    TIMSK1 &= ~(1 << OCIE1A); // Schakel timer interrupt uit
}

void Motor::setDirection(bool forward) {
    digitalWrite(dirPin, forward ? HIGH : LOW);
}

bool Motor::isRunning() {
    return running;
}

void Motor::setStallguardThreshold(int threshold) {
    stallGuardThreshold = threshold;
}

bool Motor::isError() {
    return Error; // Geen fout
}

int Motor::getSGResult() {
    return driver.SG_RESULT(); // Verkrijg de huidige SG_RESULT waarde
}

void Motor::setupTimer() {
    cli(); // Schakel interrupts tijdelijk uit
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    OCR1A = speed;
    TCCR1B |= (1 << WGM12); // CTC-modus
    TCCR1B |= (1 << CS11);  // Prescaler 8
    TIMSK1 |= (1 << OCIE1A); // Schakel compare-match interrupt in
    sei(); // Schakel interrupts weer in
}

void Motor::isrStep() {
    if (instance && instance->running) {
        digitalWrite(instance->stepPin, !digitalRead(instance->stepPin)); // Toggle STEP pin
    }
}



void Motor::update() {
    // Controleer of de motor al 5 seconden draait
    if (running && (millis() - startTime) >= 4000) {
        int sgResult = driver.SG_RESULT(); // Verkrijg de SG_RESULT waarde van de motor
        if ((sgResult < stallGuardThreshold)) {
            stop(); // Stop de motor als stallGuard onder de drempel ligt
            Error = true;
        }
    }
}
void Motor::resetError() {
    Error = false;  // Zet de errorstatus terug naar false
}


// ISR definitie
ISR(TIMER1_COMPA_vect) {
    Motor::isrStep(); // Roep de statische functie aan
}
