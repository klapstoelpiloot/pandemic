const int PIN_LED1 = 3;
const int PIN_LED2 = 4;
const int PIN_LED3 = 5;
const int PIN_SENSOR1 = 20;
const int PIN_SENSOR2 = 21;
const unsigned long RESET_TIME = 2000;

volatile int detections = 0;
volatile unsigned long lasttime = 0;
volatile bool newdisk = false;

void setup()
{
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  pinMode(PIN_LED3, OUTPUT);
  pinMode(PIN_SENSOR1, INPUT_PULLUP);
  pinMode(PIN_SENSOR2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_SENSOR1), Sensor1Rising, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_SENSOR2), Sensor2Falling, FALLING);
}

void loop()
{
  // Reset the counter?
  if((lasttime > 0) && (lasttime + RESET_TIME < millis()))
  {
    detections = 0;
    lasttime = 0;
  }

  // Display count by using LEDs
  digitalWrite(PIN_LED1, (detections >= 1));
  digitalWrite(PIN_LED2, (detections >= 2));
  digitalWrite(PIN_LED3, (detections >= 3));
}

void Sensor1Rising()
{
  if(newdisk)
  {
    detections++;
    lasttime = millis();
    newdisk = false;
  }
}

void Sensor2Falling()
{
  newdisk = true;
}
