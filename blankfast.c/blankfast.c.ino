// Simple LED blink

const int BUTTON_DEBOUNCE_DELAY = 2500;
const int LED_CLOCK_DELAY = 49;

const bool POWER_OFF = HIGH;

const int led = LED_BUILTIN;

const int outputPower = 15;
const int outputData = 14;
const int outputSerial = 12;

const int powerDetect = 7;
const int userButton1 = 8;

unsigned long buttonClock = 0;
unsigned long ledClock = 0;

bool serialStatus = false;
bool ledState = false;
bool powerState = false;

void setup() {

  // buttons
  pinMode( powerDetect, INPUT_PULLUP );
  pinMode( userButton1, INPUT_PULLUP );

  // led outputs
  pinMode( outputPower,  OUTPUT );
  pinMode( outputData,   OUTPUT );
  pinMode( outputSerial, OUTPUT );

  // built in LED
  pinMode( led, OUTPUT );

  Serial.begin( 9600 );

}

void powerOn() {

}

void dataOn() {

}

void powerOff() {

}

void loop() {

  if( !serialStatus && Serial.dtr() ) {
    serialStatus = true;
    digitalWrite( outputSerial, serialStatus );
    Serial.println( powerState ? "POWER_ON" : "POWER_OFF");
  }
  if( serialStatus && !Serial.dtr() ) {
    serialStatus = false;
    digitalWrite( outputSerial, serialStatus );
  }

  // flash the led
  if( ( millis() - ledClock ) >= LED_CLOCK_DELAY ) {
    digitalWrite(led, ledState ? HIGH : LOW );
    ledState = !ledState;
    ledClock = millis();
  }

  // check for power
  if( ( millis() - buttonClock ) >= BUTTON_DEBOUNCE_DELAY ) {
    if( digitalRead( powerDetect ) == POWER_OFF ) {
      digitalWrite( outputPower, LOW );
      if( powerState ) {
        powerState = false;
        Serial.println("POWER_OFF");
      }
    } else {
      digitalWrite( outputPower, HIGH );
      if( !powerState ) {
        powerState = true;
        Serial.println("POWER_ON");
      }
    }
    if( digitalRead( userButton1 ) == LOW ) {
      Serial.println("Button2 is pressed");
      digitalWrite( outputData, HIGH );
    } else {
      digitalWrite( outputData, LOW );
    }

    buttonClock = millis();
  }

}
