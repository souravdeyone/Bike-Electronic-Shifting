
const int buttonPin = 2;    // the number of the pushbutton pin
const int ledPin = 13;      // the number of the LED pin

#define AIN1 8
#define AIN2 6
#define PWMA 3
#define STBY 10

const int offsetA = 1;

Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);

int ledState = HIGH;
int buttonState;
int lastButtonState = LOW;
int speeds = 10;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 20;
int doubleTap = 400;
unsigned long lastShift = 0;
int gear = 0;
bool up = true;
bool change = false;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  // set initial LED state
  digitalWrite(ledPin, ledState);
  Serial.begin(9600);
}

void loop() {
  // read the state of the switch into a local variable:

  int reading = digitalRead(buttonPin);
  

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
        ledState = !ledState;
        change = true;

        if ((millis() - lastShift) < doubleTap) {
          up = false;
        }
        lastShift = millis();
      }
    }
  }

  // set the LED:
  digitalWrite(ledPin, ledState);

  lastButtonState = reading;

  if ((up == true && (millis() - lastShift) > doubleTap) && change == true) {
    if (gear < speeds) {
      gear += 1;
      motorDrive(1);
    }
    change = false;
    Serial.println(gear);
  }

  else if (up == false && change == true) {
    if (gear > 1){
    gear -= 1;
    motorDrive(-1);

  }
  up = true;
  change = false;
  Serial.println(gear);
  }
}

void motorDrive(int direction){

   motor1.drive(255*direction*-1,350);
   motor1.brake();
     
}
