int target = 512;

struct Potmeter
{
  int inputPin;
  int outputPin1;
  int outputPin2;
  int target;
  int now;

  Potmeter(int inputPin, int outputPin1, int outputPin2, int target = 0, int now = 0) :
    inputPin(inputPin),
    outputPin1(outputPin1),
    outputPin2(outputPin2),
    target(target),
    now(now)
  {
    pinMode(inputPin, INPUT);
    pinMode(outputPin1, OUTPUT);
    pinMode(outputPin2, OUTPUT);
  }
};

Potmeter potmeters[2] = {
  Potmeter(A0, 2, 3),
  Potmeter(A1, 4, 5),
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
      digitalWrite(pot->outputPin1, HIGH);
      delay(10);
    }
    else if (pot->now > pot->target)
    {
      digitalWrite(pot->outputPin2, HIGH);
      delay(10);
    }
    
    digitalWrite(pot->outputPin1, LOW);
    digitalWrite(pot->outputPin2, LOW);
  }
  
  if (Serial.available() > 0)
  {
    String data = Serial.readStringUntil("\r\n");
    int index = data.substring(0, data.indexOf(":")).toInt();
    int value = data.substring(data.indexOf(":") + 1, data.length()).toInt();
    potmeters[index].target = value;
  }
}
