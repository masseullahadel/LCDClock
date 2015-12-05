#include "RTClib.h"
#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
int alarmState = 0;
int alarmSnooze = 0;
int btnSet = 0;
int btnSel = 0;
int alarmValue = 0;
int alarmCounter = 0;
boolean turnOffAlarm = false;
boolean settingAlarm = false;
int counter = 0;
boolean initialCount = true;
boolean alarmSet = false;
int timeArray[6]; // 0=Hour, 1=Minutes, 2=day, 3=Day of Week, 4=Month, 5=Year;
int timeCounter = 0;
int alarmArray[2];
char* days[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
char* months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
RTC_DS1307 rtc;    // Create a RealTimeClock object
DateTime now; // Read data from the RTC Chip

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(10, OUTPUT);
  rtc.begin(); // Start the RTC library code
  rtc.adjust(DateTime(0, 0, 0, 0, 0, 0));
}
void loop()
{
  alarmState = digitalRead(6);
  alarmSnooze = digitalRead(7);
  btnSet = digitalRead(8);
  btnSel = digitalRead(9);
  
  if (timeCounter == 0)
  {
    if(initialCount == true){
      counter=0;
      initialCount = false;
    }
    setHour();
  }
  else if (timeCounter == 1)
  {
    if(initialCount == true){
      counter=0;
      initialCount =false;
    }
    setMinute();
  }
  else if (timeCounter == 2)
  {
    if(initialCount == true){
      counter= 1;
      initialCount=false;
    }
    setDay();
  }
  else if (timeCounter == 3)
  {
    if(initialCount == true){
      counter = 0;
      initialCount =false;
    }
    setDOW();
  }
  else if (timeCounter == 4)
  {
    if(initialCount==true){
      counter=0;
      initialCount=false;
    }
    setMonth();
  }
  else if (timeCounter == 5)
  {
    if(initialCount==true){
      counter=0;
      initialCount=false;
    }
    setYear();
  }
  else if (timeCounter == 6)
  {
    rtc.adjust(DateTime(2000+timeArray[5], timeArray[4], timeArray[2], timeArray[0], timeArray[1], 0));
    timeCounter++;
    delay(200);
  }
  else if (timeCounter > 6){
    if(alarmState == LOW&&settingAlarm==false){
      now = rtc.now();
      digitalClockDisplay();
    }
    else if(alarmState == HIGH||settingAlarm==true)
    {
      settingAlarm = true;
      if(alarmValue==0){
        if(initialCount==true)
        {
          lcd.clear();
          alarmCounter=0;
          initialCount=false;
        }
        setAlarmHour();
      }
      else if(alarmValue==1){
        if(initialCount==true)
        {
          alarmCounter=0;
          initialCount=false;
        }
        setAlarmMinute();
      }
      else if(alarmValue>1){
        alarmSet = true;
        settingAlarm = false;
      }
    }
    if(alarmArray[0]==now.hour()&&alarmArray[1]==now.minute()&&turnOffAlarm==false&&alarmSet==true&&alarmSnooze==LOW){
      tone(13,262);
      digitalWrite(10, HIGH);
    }
    else if(alarmArray[0]==now.hour() && alarmArray[1]==now.minute()&&turnOffAlarm==false&&alarmSet==true&&alarmSnooze==HIGH){
      noTone(13);
      digitalWrite(10, LOW);
      turnOffAlarm = true;
    } 
  }
}

void nextValue(){
  btnSel=LOW;
  // Update array value with counter
  timeArray[timeCounter] = counter;
  counter=0;
  initialCount = true;
  timeCounter++;
  delay(200);
  lcd.clear();
}
void nextAlarmValue(){
  btnSel=LOW;
  // Update array value with counter
  alarmArray[alarmValue] = alarmCounter;
  alarmCounter=0;
  initialCount = true;
  alarmValue++;
  delay(200);
  lcd.clear();
}

void setAlarmHour(){
   if(alarmCounter==24){
    alarmCounter=0;
    lcd.clear();
  }
  lcd.setCursor(0,0);
  lcd.print("Set alarm hour");
  lcd.setCursor(7,1);
  lcd.print(alarmCounter);
  if (btnSet==HIGH)
  {
    alarmCounter++;
    delay(200);
  }
 else if(btnSel==HIGH)
 {
  nextAlarmValue();
 }
}

void setAlarmMinute(){
  if(alarmCounter==59){
  alarmCounter=0;
  lcd.clear();
  }
  lcd.setCursor(0,0);
  lcd.print("Set alarm minute");
  lcd.setCursor(7,1);
  lcd.print(alarmCounter);
  if (btnSet==HIGH)
  {
    alarmCounter++;
    delay(200);
  }
 else if(btnSel==HIGH)
 {
  nextAlarmValue();
 }
}

void setDOW(){
  if(counter==7){
    counter=0;
    lcd.clear();
  }
  lcd.setCursor(0,0);
  lcd.print("Set day of week");
  lcd.setCursor(7,1);
  lcd.print(days[counter]);
  if (btnSet==HIGH)
  {
    counter++;
    delay(200);
  }
 else if(btnSel==HIGH)
 {
  nextValue();
 }
}
void setHour()
{
  if(counter==24){
    counter=0;
    lcd.clear();
  }
  lcd.setCursor(0,0);
  lcd.print("Set hour");
  lcd.setCursor(7,1);
  lcd.print(counter);
  if (btnSet==HIGH)
  {
    counter++;
    delay(200);
  }
 else if(btnSel==HIGH)
 {
  nextValue();
 }
}

// Function to set minutes
void setMinute()
{
  if(counter==60){
    counter=0;
    lcd.clear();
  }
  lcd.setCursor(0,0);
  lcd.print("Set minute");
  lcd.setCursor(7,1);
  lcd.print(counter);
  if (btnSet==HIGH)
  {
    counter++;
    delay(200);
  }
 else if(btnSel==HIGH)
 {
  nextValue();
 }
}

// Function to set date
void setDay()
{
  if(counter==32){
    counter=1;
    lcd.clear();
  }
  lcd.setCursor(0,0);
  lcd.print("Set day");
  lcd.setCursor(7,1);
  lcd.print(counter);
  if (btnSet==HIGH)
  {
    counter++;
    delay(200);
  }
 else if(btnSel==HIGH)
 {
  nextValue();
 }
}

// Function to set month
void setMonth()
{
  if(counter==12){
    counter=0;
    lcd.clear();
  }
  lcd.setCursor(0,0);
  lcd.print("Set month");
  lcd.setCursor(7,1);
  lcd.print(months[counter]);
  if (btnSet==HIGH)
  {
    counter++;
    delay(200);
  }
 else if(btnSel==HIGH)
 {
  nextValue();
 }
}

// Function to set year
void setYear()
{
  lcd.setCursor(0,0);
  lcd.print("Set year");
  lcd.setCursor(7,1);
  lcd.print(counter);
  if (btnSet==HIGH)
  {
    counter++;
    delay(200);
  }
 else if(btnSel==HIGH)
 {
  nextValue();
 }
}

void digitalClockDisplay(){
  // digital clock display of the time
  long hours;
  String amOrPm = "";
  if(now.hour() > 12){
    hours = now.hour() - 12;
    amOrPm = "PM";
  }
  else if(now.hour()==0){
    hours=12;
    amOrPm = "AM";
  }
  else{
    hours = now.hour();
    amOrPm = "AM";
  }
  lcd.setCursor(0,0);
  lcd.print(days[timeArray[3]]);
  lcd.print(" ");
  lcd.print(months[timeArray[4]]);
  lcd.print(" ");
  lcd.print(now.day());
  lcd.print(" ");
  lcd.print(now.year());
  lcd.setCursor(0,1);
  lcd.print(hours);
  printDigits(now.minute());
  printDigits(now.second());
  lcd.print(" ");
  lcd.print(amOrPm);
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  lcd.print(":");
  if(digits < 10)
    lcd.print('0');
  lcd.print(digits);
}
