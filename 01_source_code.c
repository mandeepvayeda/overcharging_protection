#include <LiquidCrystal.h>

LiquidCrystal LCD(11, 10, 9, 8, 7, 6);

#define Current_Sensor A5
#define Enter 4
#define Select_Battery_Amp 5
#define MOSFET 3

uint16_t amp = 0;

uint8_t i = 0, data = 0;
uint8_t Flag = 0, flag1 = 0;
float Current = 0;
int Selected_Battery_Current;
uint16_t Minute;
int Second = 60;
float per = 0, temp;

void setup() {
  // put your setup code here, to run once:
  delay(100);
  //Serial.begin(9600);
  LCD.begin(16, 2);
  delay(100);
  LCD.setCursor(0, 1);
  //LCD.print("Hello World");
  pinMode(A5, INPUT);
  pinMode(Enter, INPUT);
  pinMode(Select_Battery_Amp, INPUT);
  pinMode(MOSFET, OUTPUT);
  digitalWrite(MOSFET, LOW);
}

void loop() {

  while (Flag == 0)
  {
    LCD.setCursor(0, 0);
    LCD.print("Max Battery Cur.");
    if (flag1 == 0)
    {
      amp = map(analogRead(A4), 0, 1023, 500, 5000);
      LCD.setCursor(0, 1);
      LCD.print(amp);

      //LCD.setCursor(5, 1);
      LCD.print(" mAh  ");
      Selected_Battery_Current = amp;
    }

    else if (flag1 == 1)
    {
      per = analogRead(A4) * 0.0979;
      LCD.setCursor(10, 1);
      LCD.print(per);
    }

    if (!digitalRead(Select_Battery_Amp) && flag1 == 0)
    {
      flag1 = 1;
      delay(500);
      while (!digitalRead(Select_Battery_Amp));
    }

    if (!digitalRead(Select_Battery_Amp) && flag1 == 1)
    {
      flag1 = 0;
      delay(500);
      while (!digitalRead(Select_Battery_Amp));
    }

    if (!digitalRead(Enter))
    {
      Flag = 1;
      delay(500);
      while (!digitalRead(Enter));
    }

  }
  LCD.clear();
  if (Flag == 1)
  {

    if (!digitalRead(Enter))
    {
      Flag = 0;
      while (!digitalRead(Enter));
    }


    /* outputValue = (((long)sensorValue * 5000 / 1024) - 500 ) * 1000 / 133;
      amps = (float) outputValue / 1000;
    */
    while (digitalRead(Enter))
    {
      digitalWrite(MOSFET, HIGH);
      for (char j = 0; j < 25 ; j++)
      {
        uint16_t i = (analogRead(A5) - 509);
        data = (data + i);
      }

      data = data / 25;

      Current = (data * 0.0391);// * 1000 / 125;
      Minute = (Selected_Battery_Current / (Current * 1000)) * 60;

      temp = (100 - per);

      Minute = ((Minute * temp) / 100);
      LCD.setCursor(0, 0);
      LCD.print("Current: ");

      //LCD.setCursor(7, 0);
      LCD.print(Current * 1000);

      LCD.setCursor(0, 1);
      LCD.print("Time: ");
      
      //LCD.setCursor(0, 1);
      LCD.print(Minute);

      // LCD.setCursor(7, 1);
      // LCD.print(temp);

      delay(500);
      LCD.clear();
    }

    LCD.clear();

    while (1)
    {
      LCD.setCursor(0, 0);
      LCD.print("Running:");

      LCD.setCursor(9, 0);
      LCD.print(Minute);

      //LCD.setCursor(10, 0);
      LCD.print(':');

      //LCD.setCursor(11, 0);
      LCD.print(Second);

      LCD.setCursor(0, 1);
      LCD.print("C:");

      for (char j = 0; j < 25 ; j++)
      {
        uint16_t i = (analogRead(A5) - 509);
        data = (data + i);
      }

      data = data / 25;
      Current = (data * 0.0391);

      LCD.print(Current);

      LCD.print("  V: ");

      LCD.print(analogRead(A3) * 0.00489);

      delay(998);

      digitalWrite(MOSFET, HIGH);

      Second--;
      if (Second == 0)
      {
        Minute--;
        Second = 60;
        if (Minute == 0)
        {
          break;
        }
      }

      if (!digitalRead(Enter))
      {
        while (1)
        {
          LCD.setCursor(0, 0);
          LCD.print("Stopped:");
          delay(2000);
          digitalWrite(MOSFET, LOW);
          if (!digitalRead(Enter))
          {
            break;
          }
        }
      }
      LCD.clear();
      Flag = 2;

    }
    LCD.clear();

    while (Flag == 2)
    {
      digitalWrite(MOSFET, LOW);
      LCD.setCursor(0, 0);
      LCD.print("Charging Done");
      LCD.print("Press Enter to Start");
      while (1)
      {
        if (!digitalRead(Enter))
        {
          Flag = 0;
          break;
        }
      }
    }
    // Serial.println(Minute);

    delay(100);
    LCD.clear();
    // put your main code here, to run repeatedly:
  }
}
