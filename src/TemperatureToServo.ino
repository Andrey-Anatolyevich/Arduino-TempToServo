#include <Servo.h>
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2
#define SERIAL_SPEED 9600

bool runServo = true;
int angleBelowMin = 0;
int angleMin = 60;
int angleMax = 120;
int angleAboveMax = 180;
int tempMin = 23;
int tempMax = 30;
float tempCurrent = 0;
int servoAngle = angleBelowMin;
int oneLoopLastDelaySeconds = 5;

Servo myservo;
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup()
{
  // Setup serial output
  Serial.begin(SERIAL_SPEED);

  // Onboard LED
  pinMode(LED_BUILTIN, OUTPUT);

  // Servo
  myservo.write(servoAngle);
  myservo.attach(3);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // DS18B20
  sensors.begin();

  // Print a message to the LCD.
  write("Hi", "there!");
}

void loop()
{
  Serial.println();
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  sensors.requestTemperatures();
  tempCurrent = sensors.getTempCByIndex(0);
  adjustServoToTemperature(tempCurrent);
  write("Temp: " + String(tempCurrent), "Angle: " + String(servoAngle));
  digitalWrite(LED_BUILTIN, LOW);
  delay(oneLoopLastDelaySeconds * 1000);
}

void adjustServoToTemperature(float tempC)
{
  if (!runServo)
    return;

  if (tempC < tempMin)
    servoAngle = angleBelowMin;
  else if (tempC > tempMax)
    servoAngle = angleAboveMax;
  else
    servoAngle = map(tempC * 10, tempMin * 10, tempMax * 10, angleMin * 10, angleMax * 10) / 10;

  myservo.write(servoAngle);
}

void write(String line1, String line2)
{
  Serial.println("Line1: " + line1);
  Serial.println("Line2: " + line2);

  lcd.clear();
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print(line1);

  lcd.setCursor(0, 1);
  lcd.print(line2);
}

void StreamVariables()
{
  Serial.println("tempCurrent: " + String(tempCurrent));
  Serial.println("tempMin: " + String(tempMin));
  Serial.println("tempMax: " + String(tempMax));
  Serial.println("angleMin: " + String(angleMin));
  Serial.println("angleMax: " + String(angleMax));
}