int target = 512;

struct Potmeter
{
  int inputPin;
  int outputPin;
  int target;
  int now;

  Potmeter(int inputPin, int outputPin, int target, int now) :
    inputPin(inputPin),
    outputPin(outputPin),
    target(target),
    now(now)
  {
    pinMode(inputPin, INPUT);
    pinMode(outputPin, OUTPUT);
  }
};

Potmeter potmeters[2] = {
  Potmeter(A0, 2, 0, 0),
  Potmeter(A1, 3, 0, 0),
};

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  for (int i = 0; i < sizeof(potmeters) / sizeof(Potmeter); i++)
  {
    Potmeter* pot = &potmeters[i];
    
    pot->now = analogRead(pot->inputPin);
    Serial.print(i);
    Serial.print(":");
    Serial.println(pot->now);
    
    if (pot->now < pot->target)
    {
      digitalWrite(pot->outputPin, HIGH);
      delay(10);
    }
    
    digitalWrite(pot->outputPin, LOW);
  }
  
  if (Serial.available() > 0)
  {
    String data = Serial.readStringUntil("\r\n");
    int index = data.substring(0, data.indexOf(":")).toInt();
    int value = data.substring(data.indexOf(":") + 1, data.length()).toInt();
    potmeters[index].target = value;
  }
}
