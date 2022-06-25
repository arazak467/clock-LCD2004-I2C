/* Arduino 'Clock' with LCD (20x4) I2C.

   This I2C LCD backpack contains the PCF8574 port-expander
   IC. Beware that this sketch can work with backpacks that
   contains this IC, but may not work with variations.

   Components:
                - Arduino Uno
                - LCD (20x4)

   Libraries:
                - LiquidCrystal_I2C library

   Created on 25 June 2022 by arazak467
*/

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

const int a_sec = 900; // clock second hand

int Hrs, Mins;
int Secs = 0;
String AmPm;  // to store the string data to display 'AM' or 'PM'
int ampm;     // to store user input
int day, d, m, yr;
char *DAYS[] = {"Mon", "Tues", "Wed", "Thurs", "Fri", "Sat", "Sun"};
char *MONTHS[] = {"Jan", "Feb", "Mar", "Apr", "May", "June",
                  "July", "Aug", "Sep", "Oct", "Nov", "Dec"
                 };

void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  clkInit();
}
void loop()
{
  //...Display Time to LCD...
  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  printolcd(Hrs);
  lcd.print(":");
  printolcd(Mins);
  lcd.print(":");
  printolcd(Secs);
  lcd.print(AmPm);

  //...Clock starts...
  Secs++;
  if (Secs == 60) {
    Secs = 0;
    Mins++;
  }
  if (Mins == 60) {
    Mins = 0;
    Hrs++;
  }
  if (Hrs == 13) {
    Hrs = 1;
  }
  if (Hrs == 12 && Mins == 00 && Secs == 00) {
    if (AmPm == "AM") {
      AmPm = "PM";
    }
    else {
      AmPm = "AM";
      d++;
      reset_dom(m); // reset the day to 1 when the days of the month exceed it's limit

      //...Reset LCD row 0 and update date...
      lcd.setCursor(0, 0);
      lcd.print("                    ");
      lcd.setCursor(0, 0);
      lcd.print(DAYS[day]);   // mon, tues, wed... etc.
      lcd.print(", ");
      printolcd(d);
      lcd.print(" ");
      lcd.print(MONTHS[m - 1]);
      lcd.print(" ");
      lcd.print(yr);
      day++;
    }
  }
  delay(a_sec);  // delay acts as the clock second hand
}


///////////////////////////////////////
////Initialise the 'clock' settings////
///////////////////////////////////////
void clkInit() {

  Serial.println("What is the day today? (1 - 7)");
  Serial.println("\t(1) Monday\n\t(2) Tuesday\n\t(3) Wednesday\n\t(4) Thursday\n\t(5) Friday\n\t(6) Saturday\n\t(7) Sunday");
  while (Serial.available() == 0) {}; // wait for user input
  day = Serial.parseInt(); // store what day is today in integer variable 'day'

  delay(500);

  Serial.println("\nWhat is today's date?");
  Serial.print("Day (1 - 31): ");
  while (Serial.available() == 0) {}; // wait for user input
  d = Serial.parseInt();  // store day in integer variable 'd'
  printoserial(d);

  Serial.print("\nMonth (1 - 12): ");
  while (Serial.available() == 0) {}; // wait for user input
  m = Serial.parseInt();  // store month in integer variable 'm'
  Serial.print(MONTHS[m - 1]);
  Serial.print("\nYear (YYYY): ");
  while (Serial.available() == 0) {}; // wait for user input
  yr = Serial.parseInt(); // store year in integer variable 'yr'
  Serial.print(yr);

  delay(500);

  Serial.print("\n\n\tToday is ");
  Serial.print(DAYS[day - 1]);
  Serial.print(", ");
  printoserial(d);
  Serial.print(" ");
  Serial.print(MONTHS[m - 1]);
  Serial.print(" ");
  Serial.print(yr);
  Serial.print("\n\nIs it - (1)AM or (2)PM ?");
  while (Serial.available() == 0) {};
  ampm = Serial.parseInt();

  delay(500);

  if (ampm == 1) {
    AmPm = "AM";
    Serial.print("\nClock set to AM\n\n");
  }
  if (ampm == 2) {
    AmPm = "PM";
    Serial.print("\nClock set to PM\n\n");
  }

  delay(500);

  Serial.print("Input Hours (1 - 12) : ");
  while (Serial.available() == 0) {} // wait till the user inputs data
  Hrs = Serial.parseInt();
  printoserial(Hrs);

  delay(500);

  Serial.print("\nInput Minutes (0 - 59) : ");
  while (Serial.available() == 0) {} // wait till the user inputs data
  Mins = Serial.parseInt();
  printoserial(Mins);

  delay(500);

  Serial.print("\n\n\tClock set to :  ");

  //...Display time to Serial Monitor...
  printoserial(Hrs);
  Serial.print(":");
  printoserial(Mins);
  Serial.print(AmPm);

  //... Display Date to LCD...
  lcd.setCursor(0, 0);
  lcd.print(DAYS[day - 1]);
  lcd.print(", ");
  printolcd(d);
  lcd.print(" ");
  lcd.print(MONTHS[m - 1]);
  lcd.print(" ");
  lcd.print(yr);
}


//////////////////////////////////////////////////////////////////////////
////To put an extra '0' infront of the variables on the Serial Monitor////
//////////////////////////////////////////////////////////////////////////
void printoserial(int val)
{
  if (val < 10) {
    Serial.print("0");
    Serial.print(val);
  }
  else {
    Serial.print(val);
  }
}

///////////////////////////////////////////////////////////////
////To put an extra '0' infront of the variables on the LCD////
///////////////////////////////////////////////////////////////
void printolcd(int val)
{
  if (val < 10) {
    lcd.print("0");
    lcd.print(val);
  }
  else {
    lcd.print(val);
  }
}

void reset_dom(int var) {
  switch (var) {
    case 1:   // Jan - 31 days
      if (d == 32) {
        d = 1;
        m++;
      }
      break;
    case 2:   // Feb - 28 days
      if (d == 29) {
        d = 1;
        m++;
      }
      break;
    case 3:   // March - 31 days
      if (d == 32) {
        d = 1;
        m++;
      }
      break;
    case 4:   // April - 30 days
      if (d == 31) {
        d = 1;
        m++;
      }
      break;
    case 5:   // May - 31 days
      if (d == 32) {
        d = 1;
        m++;
      }
      break;
    case 6:   // June - 30 days
      if (d == 31) {
        d = 1;
        m++;
      }
      break;
    case 7:   // July - 31 days
      if (d == 32) {
        d = 1;
        m++;
      }
      break;
    case 8:   // August - 31 days
      if (d == 32) {
        d = 1;
        m++;
      }
      break;
    case 9:   // September - 30 days
      if (d == 31) {
        d = 1;
        m++;
      }
      break;
    case 10:  // October - 31 days
      if (d == 32) {
        d = 1;
        m++;
      }
      break;
    case 11:  // November - 30 days
      if (d == 31) {
        d = 1;
        m++;
      }
      break;
    case 12:  // December - 31 days
      if (d == 32) {
        d = 1;
        m++;
      }
      break;
  }
}
