#include <LedControl.h>

const int sensorPin = A0;
const float voltsPerMeasurement = 5.0/1024.0;
// US датчик 
const int echoPin = 8;
const int trigPin = 9;

const int displaysCount = 1;
 
const int dataPin = 12;
const int clkPin = 10;
const int csPin = 11;

int vz = 60;
int nz = 20;
int new_vz = 7;

LedControl lc = LedControl(dataPin, clkPin, csPin, displaysCount);
int prevRow = 0;
int prevCol = 0;



void setup()
{
 
  Serial.begin(115200);
  lc.shutdown(0, false);
  lc.setIntensity(0, 16);
  lc.clearDisplay(0);

  pinMode(echoPin, INPUT);
  pinMode(sensorPin, INPUT); 
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
}

void loop() {
  // Получение значения с ИК датчкиа 
  float irDistance = readDistIR();
  irDistance = constrain(irDistance, nz, vz);
  // Конвертируем данные в диапазон 0-7
  int curRow = convertToPixel(irDistance);
  // Получаем значения УЗ 
  float usDistance = readDistUs();
  usDistance = constrain(usDistance, nz, vz);
   Serial.println(usDistance);
  
  // Конвертируем данные в диапазон 0-7
  int curCol = convertToPixel(usDistance);
  // Очистить экран
  lc.clearDisplay(0);

  for (int i = 0; i <= curRow ; i ++)
  {
    lc.setLed(0, i, curCol, true);
  }

 

  // Задержка основного цикла 
  delay(50);  
  
}

int convertToPixel(float dist)
{
  // Переводи значение с 20 до 70 в значения с 0 до 7 
  float constrained = constrain(dist, nz, vz);
  return (int) map(constrained, nz, vz, 0, new_vz);
}

float readDistUs()
{
  const float speedOfSoundMPerSec = 340.0;
  const float speedOfSoundCmPerUs = speedOfSoundMPerSec / 10000.0;
  return readPulseUs() * speedOfSoundCmPerUs / 2.0;    
}

float readPulseUs()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 30000);
  return duration;
}

float readDistIR(){
  float volts = readAnalogIR() * voltsPerMeasurement;
  return 65 * pow(volts, -1.10); 
}

float readAnalogIR()
{
  int sum = 0;
  int maxV = -5000;
  int minV = 5000;
  int n = 15;
  for (int i = 0;  i < n; i++)
  {
    int cur = analogRead(sensorPin);
    if (cur > maxV)
    {
      maxV = cur;
    }
    if (cur < minV)
    {
      minV = cur;
    }
    sum += cur;
    delay(6);
  }
  return (sum - maxV - minV) / (float)(n - 2);
}
