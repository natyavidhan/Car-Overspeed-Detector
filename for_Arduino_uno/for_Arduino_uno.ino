//libraries
#include <LiquidCrystal_PCF8574.h>

//in-built features
#define buzzer 7
#define red 5
#define green 6
LiquidCrystal_PCF8574 lcd(0x27);

//speed related
const int IRSENSOR_PIN = 2;
#define BLACK 0
#define WHITE 1
long newTime = millis();
long oldTime = millis();
bool lastRead = 0;
bool newRead = 0;
float Time;


void setup() {
  Serial.begin(9600);
  lcd.clear();
  lcd.begin(16, 2); //initialize LCD
  lcd.setBacklight(255);
  pinMode(IRSENSOR_PIN, INPUT);

}

void loop() {
  newRead = digitalRead(IRSENSOR_PIN);
  if (newRead == BLACK && newRead != lastRead){
    lastRead = newRead;
  }

  if (newRead == WHITE && newRead != lastRead){
    newTime = millis();
    lastRead = newRead;
    Time = (newTime-oldTime);
    oldTime = millis();
    lcd.setCursor(0, 0);
    lcd.print((28/ Time) * 36);
    lcd.print(" Km/H ");
    if ((28/Time)*36 > 25){
      lcd.setCursor(0,1);
      lcd.print("Overspeeding");
      Serial.print("Overspeed");
    }
    else{
      lcd.setCursor(0, 1);
      lcd.print("Normal Speed");
    }
  }
}
