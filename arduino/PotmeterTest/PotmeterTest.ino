int target = 512;

void setup()
{
  Serial.begin(9600);
  
  pinMode(2, OUTPUT);
  pinMode(A0, INPUT);
}

void loop()
{
  int now = analogRead(A0);
  Serial.print(now);
  Serial.print("->");
  Serial.println(target);

  if (Serial.available() > 0)
  {
    target = Serial.readStringUntil("\r\n").toInt();
  }
  
  if (now > target)
  {
    digitalWrite(2, HIGH);
    delay(5);
  }

  digitalWrite(2, LOW);
}
