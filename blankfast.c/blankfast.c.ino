// Simple LED blink

const int BUTTON_DEBOUNCE_DELAY = 2500;
const int LED_CLOCK_DELAY = 49;
const int POWER_TO_DATA_DELAY = 100;

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

byte incomingByte;

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
  digitalWrite( outputPower, HIGH );
}

void dataOn() {
  powerOn();
  delay( POWER_TO_DATA_DELAY );
  digitalWrite( outputData, HIGH );
}

void powerOff() {
  digitalWrite( outputPower, LOW );
  digitalWrite( outputData, LOW );
}

void loop() {

  if( !serialStatus && Serial.dtr() ) {
    delay( 100 );
    serialStatus = true;
    Serial.println( powerState ? "POWER_ON" : "POWER_OFF");
  }
  if( serialStatus && !Serial.dtr() ) {
    serialStatus = false;
  }

  // flash the led
  if( ( millis() - ledClock ) >= LED_CLOCK_DELAY * ( serialStatus ? 1 : 4 ) ) {
    digitalWrite(led, ledState ? HIGH : LOW );
    ledState = !ledState;
    ledClock = millis();
  }

  // check for power
  if( ( millis() - buttonClock ) >= BUTTON_DEBOUNCE_DELAY ) {
    if( digitalRead( powerDetect ) == POWER_OFF ) {
      if( powerState ) {
        powerState = false;
        Serial.println("POWER_OFF");
      }
    } else {
      if( !powerState ) {
        powerState = true;
        Serial.println("POWER_ON");
      }
    }
    if( digitalRead( userButton1 ) == LOW ) {
      Serial.println("Button2 is pressed");
    }

    buttonClock = millis();
  }

  // read the serial and check for messages
  if( Serial.available() ) {
    Serial.println("serial data recieved");
    incomingByte = Serial.read();
  }

}
