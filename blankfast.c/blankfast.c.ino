// Simple LED blink

const int BUTTON_DEBOUNCE_DELAY = 2500;
const int LED_CLOCK_DELAY = 49;
const int POWER_TO_DATA_DELAY = 100;
const int MAX_MESSAGE_LENGTH = 10;

const bool POWER_OFF = HIGH;

const int led = LED_BUILTIN;

const int outputPower = 15;
const int outputData = 14;
const int outputSerial = 12;

const int powerDetect = 7;
const int userButton1 = 8;

unsigned long buttonClock = 0;
unsigned long ledClock = 0;
unsigned int byteCount = 0;

bool serialStatus = false;
bool ledState = false;
bool powerState = false;

char incomingByte;
char incomingCommand[MAX_MESSAGE_LENGTH];

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

boolean isPowerOn( char incomingCommand[MAX_MESSAGE_LENGTH] ) {
  int size = 8;
  char command[8] = { 'p', 'o', 'w', 'e', 'r', ' ', 'o', 'n' };
  boolean rtn = true;

  int sizeCount = 0;
  while( sizeCount < size ) {
    if( incomingCommand[sizeCount] != command[sizeCount] ) {
      rtn = false;
    }
    sizeCount++;
  }

  return rtn;
}

boolean isPowerOff( char incomingCommand[MAX_MESSAGE_LENGTH] ) {
  int size = 9;
  char command[9] = { 'p', 'o', 'w', 'e', 'r', ' ', 'o', 'f', 'f' };
  boolean rtn = true;

  int sizeCount = 0;
  while( sizeCount < size ) {
    if( incomingCommand[sizeCount] != command[sizeCount] ) {
      rtn = false;
    }
    sizeCount++;
  }

  return rtn;
}

boolean isDataOn( char incomingCommand[MAX_MESSAGE_LENGTH] ) {
  int size = 7;
  char command[7] = { 'd','a','t','a', ' ', 'o', 'n' };
  boolean rtn = true;

  int sizeCount = 0;
  while( sizeCount < size ) {
    if( incomingCommand[sizeCount] != command[sizeCount] ) {
      rtn = false;
    }
    sizeCount++;
  }

  return rtn;
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

  byteCount = 0;
  // read the serial and check for messages
  while( Serial.available() && byteCount < MAX_MESSAGE_LENGTH ) {
    incomingCommand[byteCount] = Serial.read();
    byteCount++;
  }

  // if a message was received, check for messages
  if( byteCount > 0 ) {

    if( isPowerOn( incomingCommand ) ) {
      Serial.println("power on command received");
      powerOn();
    } else

    if( isPowerOff( incomingCommand ) ) {
      Serial.println("power off command received");
      powerOff();
    } else

    if( isDataOn( incomingCommand ) ) {
      Serial.println("data on command received");
      dataOn();
    } else

    {
      Serial.println( "unknown command received" );
      Serial.println( incomingCommand );
    }

    // clear the incomingCommand
    byteCount = 0;
    while( byteCount < MAX_MESSAGE_LENGTH ) {
      incomingCommand[byteCount] = ' ';
      byteCount++;
    }

  }

}
