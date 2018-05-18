typedef unsigned short ushort;
typedef unsigned int uint;

#define DIR_STP       0x00
#define DIR_FWD       0x01
#define DIR_FWD_SLOW  0x10
#define DIR_BWD       0x02
#define DIR_BWD_SLOW  0x20

struct Potmeter
{
  ushort inputPin;
  ushort forwardPin;
  ushort backwardPin;
  short target;
  short value;
  short previousValue;
  ushort dir;
  byte correct;
  byte unexpected;
  
  Potmeter(ushort inputPin, ushort forwardPin, ushort backwardPin, short target = -1, ushort value = 0) :
    inputPin(inputPin),
    forwardPin(forwardPin),
    backwardPin(backwardPin),
    target(target),
    value(value),
    previousValue(value),
    dir(DIR_STP),
    correct(0),
    unexpected(0)
  {
    pinMode(inputPin, INPUT);
    pinMode(forwardPin, OUTPUT);
    pinMode(backwardPin, OUTPUT);
  }

  void update()
  {
    value = analogRead(inputPin) / 4;

    if (((dir == DIR_FWD || dir == DIR_FWD_SLOW) && value < previousValue) ||
        ((dir == DIR_BWD || dir == DIR_BWD_SLOW) && value > previousValue))
    {
      if (unexpected >= 3)
      {
        stop();
      }
      else
      {
        unexpected++;
      }
    }
    else
    {
      if (unexpected > 0)
      {
        unexpected--;
      }
    }

    if (((dir == DIR_FWD || dir == DIR_FWD_SLOW) && value - previousValue >= 10) ||
        ((dir == DIR_BWD || dir == DIR_BWD_SLOW) && previousValue - value >= 10))
    {
      stop();
    }

    if (target >= 0)
    {
      if (value + 1 < target)
      {
        if (target - value > 1)
        {
          dir = DIR_FWD;
        }
        else
        {
          dir = DIR_FWD_SLOW;
        }
        
        correct = 0;
      }
      else if (value - 1 > target)
      {
        if (value - target > 1)
        {
          dir = DIR_BWD;
        }
        else
        {
          dir = DIR_BWD_SLOW;
        }
        
        correct = 0;
      }
      else
      {
        if (correct >= 5)
        {
          stop();
        }
        else
        {
          correct++;
        }
      }
      
      switch (dir)
      {
      case DIR_STP:
        digitalWrite(forwardPin, LOW);
        digitalWrite(backwardPin, LOW);
        break;
      case DIR_FWD:
        digitalWrite(forwardPin, HIGH);
        digitalWrite(backwardPin, LOW);
        break;
      case DIR_FWD_SLOW:
        digitalWrite(forwardPin, HIGH);
        digitalWrite(backwardPin, LOW);
        digitalWrite(forwardPin, LOW);
        break;
      case DIR_BWD:
        digitalWrite(forwardPin, LOW);
        digitalWrite(backwardPin, HIGH);
        break;
      case DIR_BWD_SLOW:
        digitalWrite(forwardPin, LOW);
        digitalWrite(backwardPin, HIGH);
        digitalWrite(backwardPin, LOW);
        break;
      }
    }

    previousValue = value;
  }

  void stop()
  {
    dir = DIR_STP;
    target = -2;
    correct = 0;
    unexpected = 0;
    
    digitalWrite(forwardPin, LOW);
    digitalWrite(backwardPin, LOW);
  }

  void sendMidi(int index)
  {
    //TODO: Convert to midi
    Serial.print(index);
    Serial.print(":");
    
    if (target >= -1)
    {
      Serial.println(value);
    }
    else
    {
      Serial.println(target);
      target = -1;
    }
  }
};

Potmeter potmeters[2] = {
  Potmeter(A0, 2, 3),
  Potmeter(A1, 4, 5),
};

void setup()
{
  Serial.begin(9600);

  pinMode(12, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
}

void loop()
{
  for (uint i = 0; i < sizeof(potmeters) / sizeof(Potmeter); i++)
  {
    (&potmeters[i])->update();
    (&potmeters[i])->sendMidi(i);
  }

  if (Serial.available() > 0)
  {
    String data = Serial.readStringUntil('\n');
    int index = data.substring(0, data.indexOf(":")).toInt();
    int value = data.substring(data.indexOf(":") + 1, data.length()).toInt();
    (&potmeters[index])->stop();
    (&potmeters[index])->target = value;
  }

  if (digitalRead(12) == HIGH)
  {
    Serial.println("-1:0");
  }
  else if (digitalRead(13) == HIGH)
  {
    Serial.println("-1:1");
  }
}


