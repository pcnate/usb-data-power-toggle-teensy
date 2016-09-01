// Simple LED blink

const int BUTTON_DEBOUNCE_DELAY = 50;
const int LED_CLOCK_DELAY = 49;

const int led = LED_BUILTIN;

const int led1 = 15;
const int led2 = 14;
const int led3 = 12;

unsigned long buttonClock = 0;
unsigned long ledClock = 0;
bool ledState = false;

void setup() {

  // buttons
  pinMode( 7, INPUT_PULLUP );
  pinMode( 8, INPUT_PULLUP );

  // led outputs
  pinMode( led1, OUTPUT );
  pinMode( led2, OUTPUT );
  pinMode( led3, OUTPUT );

  // built in LED
  pinMode(led, OUTPUT);

  Serial.begin( 9600 );

}

void loop() {

  if( ( millis() - ledClock ) >= LED_CLOCK_DELAY ) {
    digitalWrite(led, ledState ? HIGH : LOW );
    ledState = !ledState;
    ledClock = millis();
  }

  if( ( millis() - buttonClock ) >= BUTTON_DEBOUNCE_DELAY ) {
    if( digitalRead(7) == LOW ) {
      Serial.println("Button1 is pressed");
      digitalWrite( led1, LOW );
    } else {
      digitalWrite( led1, HIGH );
    }
    if( digitalRead(8) == LOW ) {
      Serial.println("Button2 is pressed");
      digitalWrite( led2, LOW );
      digitalWrite( led3, HIGH );
    } else {
      digitalWrite( led2, HIGH );
      digitalWrite( led3, LOW );
    }

    buttonClock = millis();
  }
  delay(1);
}
