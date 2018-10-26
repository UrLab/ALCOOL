const byte numLeds = 22;
const byte numPins = 5;
const byte witnessLedPin = 13; // integrated led pin
const byte ledPowerPins[numPins] = {4, 7, 8, 11, 12};
const byte ledTransistorPins[numPins] = {3, 5, 6, 9, 10}; // pwm enabled pins
const byte btnPin = 2;

const unsigned DELAY_MS = 50;
const unsigned BAUD_RATE = 9600;

const bool DEBUG = false;

boolean isIdle = true;
unsigned idleTimer = 0;

void setup(){
  /*
   * Setup all pins, begin serial communication and seed random.
   */
  pinMode(btnPin, INPUT);
  pinMode(ledPowerPins[0], OUTPUT);
  pinMode(ledPowerPins[1], OUTPUT);
  pinMode(ledPowerPins[2], OUTPUT);
  pinMode(ledPowerPins[3], OUTPUT);
  pinMode(ledPowerPins[4], OUTPUT);
  pinMode(ledTransistorPins[0], OUTPUT);
  pinMode(ledTransistorPins[1], OUTPUT);
  pinMode(ledTransistorPins[2], OUTPUT);
  pinMode(ledTransistorPins[3], OUTPUT);
  pinMode(ledTransistorPins[4], OUTPUT);
  pinMode(witnessLedPin, OUTPUT);
  if(DEBUG)Serial.begin(BAUD_RATE);
  randomSeed(analogRead(0));
  randomSeed(random(1024) + analogRead(1));
  for(byte i = 0; i < numPins; i++){
    digitalWrite(ledPowerPins[i], HIGH);
  }
}

void loop(){
  /*
   * Main control loop.
   */
  if(digitalRead(btnPin)){
    chooseRandom();
  }
  else{
    idleAnimationUpdate();
  }
  delay(DELAY_MS);
}

void chooseRandom(){
  /*
   * Choose a random led with roulette-like animation
   */
  delay(10);
  while(digitalRead(btnPin)){
    writeBin(random(numLeds));
    delay(10);
  }
  for(unsigned int i = 1; i <= 25; i++){
    writeBin(random(numLeds));
    delay(pow(i, 2));
  }
  for(byte i = 0; i < 10; i++){
    digitalWrite(witnessLedPin, HIGH);
    delay(500);
    if(i < 9){
      digitalWrite(witnessLedPin, LOW);
    }
    delay(500);
  }
  digitalWrite(witnessLedPin, LOW);
  for(byte i = 0; i < numPins; i++){
    digitalWrite(ledPowerPins[i], HIGH);
  }
  idleTimer = 0;
}

void idleAnimationUpdate(){
  /*
   * show a nice fade-in, fade-out animation.
   */
  unsigned ms = idleTimer;
  idleTimer++;
  ms = ms - ((ms / 256) * 2 * (ms % 256));
  ms = (long)(128.0 * (-2.0 * pow(sin(ms / 16.0 + PI / 2.0), 4.0) + 2.0));
  if(ms == 255)idleTimer = 30; // Empirically found value;
  if(DEBUG)Serial.println(ms);
  for(byte i = 0; i < numPins; i++){
    analogWrite(ledTransistorPins[i], ms);
  }
}

void writeBin(byte n){
  /*
   * Write n's coordinates on power and transistor pins.
   */
  digitalWrite(witnessLedPin, HIGH);
  if(DEBUG)Serial.println(n);
  for(byte i = 0; i < numPins; i++){
    byte powerPin = n % numPins == i?HIGH:LOW;
    if(DEBUG)Serial.print(powerPin);
    digitalWrite(ledPowerPins[i], powerPin);
    byte transistorPin = n / numPins == i?HIGH:LOW;
    if(DEBUG)Serial.print(transistorPin);
    digitalWrite(ledTransistorPins[i], transistorPin);
  }
  if(DEBUG)Serial.println();
  digitalWrite(witnessLedPin, LOW);
}
