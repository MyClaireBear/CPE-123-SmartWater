#include <CPutil.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <CPLcd.h>

CPLcd lcd;

byte box[] = 
{
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

//Talenti dimensions 3.6 x 3.6 x 3.9
//  about 2oz/inch
//sensor limits: 0-723

int waterPin = A8;
int touchSensorPin = A5;

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial3.begin(9600);

  lcd.init();
  lcd.begin(16, 2);
  lcd.createChar(0, box);
  lcd.clear();
}

void loop() 
{
  controlSensors();
}

//**********Control************************

//for sensor arduino
void controlSensors()
{
  static int beforeWater = 0;
  static int afterWater = 0;
  static int changeWater = 0;
  static int totalIntake = 0;
  
  enum {WAIT, BEFORE, AFTER, CALC, LESS};
  static int state = WAIT;

  switch (state)
  {
    case (WAIT):
      Serial.println(" ");
      Serial.println("waiting...");
      while (touchSensor() == false)
      {}
      Serial.println("pass: touch");
      state = BEFORE;
    break;

    case (BEFORE):
      beforeWater = waterCalc();
      Serial.println("calculated water");
      state = AFTER;
    break;

    case (AFTER):
      if (touchSensor()== false)
      {
        afterWater = waterCalc();
        changeWater = beforeWater - afterWater;
        Serial.println("calculated change in water");
        state = CALC;
      }
    break;

    case (CALC):
      if (changeWater <= 0)
      {
        Serial.println("too little change");
        state = WAIT;
      }
      else if (changeWater > 0)
      {
        Serial.println("drank water");
        state = LESS;
      }
    break;

    case (LESS):
      totalIntake = totalWaterIntake(changeWater);
      Serial.println("calculated total water intake");
      state = WAIT;
    break;   
  }
  Serial3.print(totalIntake);
  Serial3.print(" ");
//  lcdBarDisplay();
//  lcdTotalWaterIntake(totalIntake);
}


//for output arduino
void controlOutput()
{  
  if (Serial3.available());
  {
    lcdTotalWaterIntake(Serial3.parseInt());
    lcdBarDisplay();
  }
  //Serial.print(Serial3.read());
  //Serial3.print
  //Serial3.parseInt
}


//**********Touch*Sensor*******************


int touchSensor()
{
  int pressure = analogRead(touchSensorPin);
  static int returnVal = false;
  
  if (pressure >= 400)
  {
    returnVal = true;
  }
  else if (pressure < 400)
  {
    returnVal = false;
  }
  return returnVal;
  //Serial.print(info);
}



//**********Water Calculations*************


float totalWaterIntake(int wLevel)
{
  static int totalWaterIntake = 0;
  totalWaterIntake = totalWaterIntake + wLevel;
  return totalWaterIntake;
}


float waterCalc()
{
  int wLevel = readWaterLevel();
  float ounces = wLevel * 0.002;
  return ounces;
}


int readWaterLevel()
{
  static int waterLevel = 0;
  waterLevel = analogRead(waterPin);
  return waterLevel;
}


//******************LCD*******************


void lcdBarDisplay()
{
  int numBars = numOfBars();
  int i = 0;
  lcd.setCursor(0, 1);
  lcd.print("                ");
  for (i=0; i<numBars; i++)
  {
    lcd.setCursor(i, 1);
    lcd.write(byte(0));
  }
}


void lcdTotalWaterIntake(int totalIntake)
{
  lcd.setCursor(0, 0);
  lcd.print("Intake: ");
  lcd.print(totalIntake);
}


int numOfBars()
{
  int wLevel = readWaterLevel();
  static int numBars = 0;
  
  if (wLevel <= 47)
  {
    numBars = 1;
  }
  else if (wLevel <= 100)
  {
    numBars = 2;
  }
  else if (wLevel <= 150)
  {
    numBars = 3;
  }
  else if (wLevel <= 200)
  {
    numBars = 4;
  }
  else if (wLevel <= 250)
  {
    numBars = 5;
  }
  else if (wLevel <= 300)
  {
    numBars = 6;
  }
  else if (wLevel <= 340)
  {
    numBars = 7;
  }
  else if (wLevel <= 390)
  {
    numBars = 8;
  }
  else if (wLevel <= 430)
  {
    numBars = 9;
  }
  else if (wLevel <= 470)
  {
    numBars = 10;
  }
  else if (wLevel <= 516)
  {
    numBars = 11;
  }
  else if (wLevel <= 563)
  {
    numBars = 12;
  }
  else if (wLevel <= 609)
  {
    numBars = 13;
  }
  else if (wLevel <= 656)
  {
    numBars = 14;
  }
  else if (wLevel <= 670)
  {
    numBars = 15;
  }
  else if (wLevel >= 700)
  {
    numBars = 16;
  }
  
  return numBars;
}


//***********Water*Sensor*Test*************


void waterSensorTest()
{
  Serial.println(readWaterLevel());
  delay (1000);
}
